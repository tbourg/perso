#include "CFileBufT.h"

template<> wchar_t* CFileBufT<wchar_t>::LoadFromFile(const TCHAR* szFile, bool bTextFile)
{
  FILE* f;
  
  // initial values when LoadFromFile fails
  m_nLineNumber = 1;
  m_nLineStartPos = 0;
  m_nLineLength = -1;

  m_buf.Clear();

  f = openfile(szFile);
  if (f == NULL)
    return NULL;

  long nSize = getsize(f);
  if (!m_buf.Reserve(1 + nSize/sizeof(wchar_t)))
  {
    closefile(f);
    return NULL;
  }

  unsigned char* p = (unsigned char *) m_buf.GetData();
  if (!readfile(f, p, nSize))
  {
    closefile(f);
    return NULL;
  }

  closefile(f);

  m_buf.SetCountValue(nSize/sizeof(wchar_t));

  if ( bTextFile )
  {
    bool bIsUnicodeText = false;
    if ( (nSize >= 2) && ((nSize % 2) == 0) )
    {
      if ( m_buf[0] == 0xFEFF )
      {
        // Unicode LE
        bIsUnicodeText = true;
      }
      else if ( m_buf[0] == 0xFFFE )
      {
        // Unicode BE
        bIsUnicodeText = true;
        for ( int i = 0; i < m_buf.GetCount(); i++ )
        {
          const wchar_t wch = m_buf[i];
          m_buf[i] = ((wch >> 8) & 0x00FF) + (((wch & 0x00FF) << 8) & 0xFF00);
        }
      }
    }

    if ( !bIsUnicodeText )
    {
      CBufT<wchar_t> wbuf;

      if ( !wbuf.Reserve(nSize + 2) )
      {
        return NULL;
      }
      
      wbuf[0] = 0xFEFF;
      wbuf[1] = 0; // just in case :)

      int count;
      if ( (nSize >= 3) &&
           (p[0] == 0xEF && p[1] == 0xBB && p[2] == 0xBF) )
      {
        // UTF-8
        count = SysUniConv::UTF8ToUnicode(
                  (wchar_t *) wbuf.GetData() + 1, nSize + 1,
                  (const char *) p + 3, nSize - 3);
      }
      else
      {
        count = SysUniConv::MultiByteToUnicode( 
                  (wchar_t *) wbuf.GetData() + 1, nSize + 1,
                  (const char *) p, nSize);
      }
      wbuf.SetCountValue(count + 1);

      m_buf.Swap(wbuf);
    }
    
    m_nLineStartPos = 1;
  }

  if (bTextFile)
  {
    GoToFirstLine();
  }
  return m_buf.GetData();
}
