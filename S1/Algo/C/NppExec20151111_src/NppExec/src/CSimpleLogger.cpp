/*
This file is part of NppExec
Copyright (C) 2013 DV <dvv81 (at) ukr (dot) net>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "CSimpleLogger.h"
#include <stdio.h>


class LogFileWriter
{
    public:
        LogFileWriter() : m_f(0)
        {
            ::InitializeCriticalSection(&m_csFile);
        }

        ~LogFileWriter()
        {
            Close();
            ::DeleteCriticalSection(&m_csFile);
        }

        bool Open(const TCHAR* szFilePathName, bool bAppend = false)
        {
            bool bOpenOK = false;

            Close();

            ::EnterCriticalSection(&m_csFile);

            #ifdef UNICODE
                const wchar_t* szMode = bAppend ? L"a+b" : L"w+b";
                m_f = ::_wfopen(szFilePathName, szMode);
            #else
                const char* szMode = bAppend ? "a+b" : "w+b";
                m_f = ::fopen(szFilePathName, szMode);
            #endif

            if ( m_f )
            {
                m_filePathName = szFilePathName;
                bOpenOK = true;
            }

            ::LeaveCriticalSection(&m_csFile);

            return bOpenOK;
        }

        bool Close()
        {
            bool bCloseOK = false;
            if ( m_f )
            {
                ::EnterCriticalSection(&m_csFile);

                if ( ::fclose(m_f) == 0 )
                    bCloseOK = true;
                m_f = 0;
                m_filePathName.Clear();

                ::LeaveCriticalSection(&m_csFile);
            }
            return bCloseOK;
        }

        const TCHAR* GetFilePathName() const
        {
            return m_filePathName.c_str();
        }

        bool IsOpen() const
        {
            return (m_f ? true : false);
        }

        bool Write(const void* pBuf, size_t nBytes)
        {
            bool bWriteOK = false;

            if ( m_f )
            {
                ::EnterCriticalSection(&m_csFile);

                if ( ::fwrite(pBuf, 1, nBytes, m_f) == nBytes )
                {
                    if ( ::fflush(m_f) == 0 )
                        bWriteOK = true;
                }

                ::LeaveCriticalSection(&m_csFile);
            }

            return bWriteOK;
        }

    protected:
        FILE* m_f;
        CRITICAL_SECTION m_csFile;
        CStrT<TCHAR> m_filePathName;
};


static bool writefile(FILE* f, const void* pBuf, size_t nBytes)
{
  size_t count = fwrite(pBuf, 1, nBytes, f);
  return (count == nBytes);
}


CSimpleLogger::CSimpleLogger()
{
    m_bCanUpdate = true;
    m_nIndentLevel = 0;
    SetIndentStr( _T("   ") );
    m_pOutputStrFunc = NULL;
    m_pFileWriter = NULL;
}

CSimpleLogger::~CSimpleLogger()
{
    if ( m_pFileWriter )
    {
        writeFileLogEnd();
        delete m_pFileWriter;
    }
}

void CSimpleLogger::applyIndentLevel(int nIndentLevel)
{
    m_strIndentLevel.Clear();
    while ( nIndentLevel-- > 0 )
    {
        m_strIndentLevel.Append(m_IndentStr);
    }
}

void CSimpleLogger::writeFileLogStart()
{
    if ( m_pFileWriter && m_pFileWriter->IsOpen() )
    {
        bool bCanUpdate = m_bCanUpdate;
        m_bCanUpdate = true;

        #ifdef UNICODE
            Add( L"\xFEFFLog start >>" );
        #else
            Add( "Log start >>" );
        #endif

        m_bCanUpdate = bCanUpdate;
    }
}

void CSimpleLogger::writeFileLogEnd()
{
    if ( m_pFileWriter && m_pFileWriter->IsOpen() )
    {
        bool bCanUpdate = m_bCanUpdate;
        m_bCanUpdate = true;

        Add( _T("Log exit.") );

        m_bCanUpdate = bCanUpdate;
    }
}

void CSimpleLogger::Add(const TCHAR* str)
{
    if ( m_bCanUpdate )
    {
        tstr S = str;
        S.Insert(0, m_strIndentLevel);
        m_StrList.Add(S);
        if ( m_pOutputStrFunc )
        {
            m_pOutputStrFunc( S.c_str(), S.length() );
        }
        if ( m_pFileWriter && m_pFileWriter->IsOpen() )
        {
            S += _T("\r\n"); // new line
            m_pFileWriter->Write( S.c_str(), S.length()*sizeof(TCHAR) );
        }
    }
}

void CSimpleLogger::AddEx(const TCHAR* fmt, ...)
{
    if ( m_bCanUpdate && fmt )
    {
        TCHAR   str_fmt[2048];
        TCHAR   str[2048];
        va_list argList;
        int     i = 0, j = 0;

        while ( fmt[i] )
        {
            if ( (fmt[i] == _T('%')) && (fmt[i+1] == _T('s')) )
            {
                lstrcpy( str_fmt + j, _T("%.960s") );
                j += 6; // len of "%.960s"
                i += 2; // len of "%s"
            }
            else
            {
                str_fmt[j++] = fmt[i++];
            }
        }
        str_fmt[j] = 0;
    
        va_start(argList, fmt);
        ::wvsprintf(str, str_fmt, argList);
        va_end(argList);
        str[2048 - 1] = 0;
        Add(str);
    }
}

void CSimpleLogger::Clear()
{
    if ( m_bCanUpdate )
    {
        m_bCanUpdate = false;
        m_StrList.Clear();
        m_nIndentLevel = 0;
        applyIndentLevel( 0 );
        m_bCanUpdate = true;
    }
}

void CSimpleLogger::DecIndentLevel()
{
    if ( m_bCanUpdate )
    {
        if ( m_nIndentLevel > 0 )
            applyIndentLevel( --m_nIndentLevel );
    }
}

void CSimpleLogger::IncIndentLevel()
{
    if ( m_bCanUpdate )
    {
        applyIndentLevel( ++m_nIndentLevel );
    }
}

void CSimpleLogger::OutputAll(OUTPUTFUNC pOutputAllFunc)
{
    m_bCanUpdate = false;
    CListT<tstr>::CListItemPtr p = m_StrList.GetFirst();
    while ( p )
    {
        pOutputAllFunc( p->GetItem().c_str(), p->GetItem().length() );
        p = p->GetNext();
    }
    m_bCanUpdate = true;
}

void CSimpleLogger::SetIndentStr(const TCHAR* str)
{
    if ( m_bCanUpdate )
    {
        m_IndentStr = str;
    }
}

void CSimpleLogger::SetOutputMode(bool bDirectOutput, OUTPUTFUNC pOutputStrFunc )
{
    if ( bDirectOutput )
        m_pOutputStrFunc = pOutputStrFunc;
    else
        m_pOutputStrFunc = NULL;
}

const TCHAR* CSimpleLogger::GetLogFile() const 
{
    return ( m_pFileWriter ? m_pFileWriter->GetFilePathName() : _T("") );
}

bool CSimpleLogger::IsLogFileOpen() const
{
    return ( (m_pFileWriter && m_pFileWriter->IsOpen()) ? true : false );
}

bool CSimpleLogger::SetLogFile(const TCHAR* logFilePathName)
{
    if ( !m_pFileWriter )
    {
        m_pFileWriter = new LogFileWriter();
    }

    if ( m_pFileWriter->Open(logFilePathName) )
    {
        writeFileLogStart();
        return true;
    }

    return false;
}
