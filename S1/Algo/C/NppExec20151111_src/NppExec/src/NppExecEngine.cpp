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

#include "NppExecEngine.h"
//#include "NppExec.h"
#include "DlgConsole.h"
#include "DlgConsoleEncoding.h"
#include "DlgInputBox.h"
#include "c_base/MatchMask.h"
#include "c_base/int2str.h"
#include "c_base/str2int.h"
#include "c_base/str_func.h"
#include "c_base/HexStr.h"
#include "cpp/StrSplitT.h"
#include "CSimpleLogger.h"
#include "DirFileLister.h"
#include "fparser/fparser.hh"
#include "menuCmdID.h"
#include <stdio.h>
#include <shellapi.h>

#ifdef UNICODE
  #define _t_sprintf  swprintf
  #ifndef __MINGW32__
    #define _t_str2f(x) _wtof(x)
  #else
    #define _t_str2f(x) wcstof(x, 0)
  #endif
#else
  #define _t_sprintf  sprintf
  #define _t_str2f(x) atof(x)
#endif

#define MAX_VAR_LENGTH2SHOW 200

const TCHAR MACRO_FILE_FULLPATH[]       = _T("$(FULL_CURRENT_PATH)");
const TCHAR MACRO_FILE_DIRPATH[]        = _T("$(CURRENT_DIRECTORY)");
const TCHAR MACRO_FILE_FULLNAME[]       = _T("$(FILE_NAME)");
const TCHAR MACRO_FILE_NAMEONLY[]       = _T("$(NAME_PART)");
const TCHAR MACRO_FILE_EXTONLY[]        = _T("$(EXT_PART)");
const TCHAR MACRO_NPP_DIRECTORY[]       = _T("$(NPP_DIRECTORY)");
const TCHAR MACRO_CURRENT_WORD[]        = _T("$(CURRENT_WORD)");
const TCHAR MACRO_CURRENT_LINE[]        = _T("$(CURRENT_LINE)");
const TCHAR MACRO_CURRENT_COLUMN[]      = _T("$(CURRENT_COLUMN)");
const TCHAR MACRO_DOCNUMBER[]           = _T("$(#");
const TCHAR MACRO_SYSVAR[]              = _T("$(SYS.");
const TCHAR MACRO_LEFT_VIEW_FILE[]      = _T("$(LEFT_VIEW_FILE)");
const TCHAR MACRO_RIGHT_VIEW_FILE[]     = _T("$(RIGHT_VIEW_FILE)");
const TCHAR MACRO_PLUGINS_CONFIG_DIR[]  = _T("$(PLUGINS_CONFIG_DIR)");
const TCHAR MACRO_CURRENT_WORKING_DIR[] = _T("$(CWD)");
const TCHAR MACRO_ARGC[]                = _T("$(ARGC)");
const TCHAR MACRO_ARGV[]                = _T("$(ARGV");
const TCHAR MACRO_RARGV[]               = _T("$(RARGV");
const TCHAR MACRO_INPUT[]               = _T("$(INPUT)");
const TCHAR MACRO_INPUTFMT[]            = _T("$(INPUT[%d])");
const TCHAR MACRO_EXITCODE[]            = _T("$(EXITCODE)");
const TCHAR MACRO_OUTPUT[]              = _T("$(OUTPUT)");
const TCHAR MACRO_OUTPUT1[]             = _T("$(OUTPUT1)");
const TCHAR MACRO_OUTPUTL[]             = _T("$(OUTPUTL)");
const TCHAR MACRO_MSG_RESULT[]          = _T("$(MSG_RESULT)");
const TCHAR MACRO_MSG_WPARAM[]          = _T("$(MSG_WPARAM)");
const TCHAR MACRO_MSG_LPARAM[]          = _T("$(MSG_LPARAM)");
const TCHAR CMD_CLS[]                   = _T("CLS");
const TCHAR CMD_CD[]                    = _T("CD");
const TCHAR CMD_DIR[]                   = _T("DIR");
const TCHAR CMD_ECHO[]                  = _T("ECHO");
const TCHAR CMD_CONCOLOR[]              = _T("CON_COLOR");
const TCHAR CMD_CONCOLOUR[]             = _T("CON_COLOUR");
const TCHAR CMD_CONFILTER[]             = _T("CON_FILTER");
const TCHAR CMD_CONLOADFROM[]           = _T("CON_LOADFROM");
const TCHAR CMD_CONLOAD[]               = _T("CON_LOAD");
const TCHAR CMD_CONSAVETO[]             = _T("CON_SAVETO");
const TCHAR CMD_CONSAVE[]               = _T("CON_SAVE");
const TCHAR CMD_NPPCLOSE[]              = _T("NPP_CLOSE");
const TCHAR CMD_NPPCONSOLE[]            = _T("NPP_CONSOLE");
const TCHAR CMD_NPPEXEC[]               = _T("NPP_EXEC");
const TCHAR CMD_NPPSENDMSG[]            = _T("NPP_SENDMSG");
const TCHAR CMD_NPPSENDMSGEX[]          = _T("NPP_SENDMSGEX");
const TCHAR CMD_SCISENDMSG[]            = _T("SCI_SENDMSG");
const TCHAR CMD_NPPMENUCOMMAND[]        = _T("NPP_MENUCOMMAND");
const TCHAR CMD_NPPOPEN[]               = _T("NPP_OPEN");
const TCHAR CMD_NPPRUN[]                = _T("NPP_RUN");
const TCHAR CMD_NPPSAVE[]               = _T("NPP_SAVE");
const TCHAR CMD_NPPSAVEAS[]             = _T("NPP_SAVEAS");
const TCHAR CMD_NPPSAVEALL[]            = _T("NPP_SAVEALL");
const TCHAR CMD_NPPSWITCH[]             = _T("NPP_SWITCH");
const TCHAR CMD_NPECMDALIAS[]           = _T("NPE_CMDALIAS");
const TCHAR CMD_NPECONSOLE[]            = _T("NPE_CONSOLE");
const TCHAR CMD_NPEDEBUGLOG[]           = _T("NPE_DEBUGLOG");
const TCHAR CMD_NPEDEBUG[]              = _T("NPE_DEBUG");
const TCHAR CMD_NPENOEMPTYVARS[]        = _T("NPE_NOEMPTYVARS");
const TCHAR CMD_SET[]                   = _T("SET");
const TCHAR CMD_UNSET[]                 = _T("UNSET");
const TCHAR CMD_ENVSET[]                = _T("ENV_SET");
const TCHAR CMD_ENVUNSET[]              = _T("ENV_UNSET");
const TCHAR CMD_SETENV[]                = _T("SET_ENV");
const TCHAR CMD_UNSETENV[]              = _T("UNSET_ENV");
const TCHAR CMD_INPUTBOX[]              = _T("INPUTBOX");
const TCHAR CMD_SELLOADFROM[]           = _T("SEL_LOADFROM");
const TCHAR CMD_SELLOAD[]               = _T("SEL_LOAD");
const TCHAR CMD_SELSAVETO[]             = _T("SEL_SAVETO");
const TCHAR CMD_SELSAVE[]               = _T("SEL_SAVE");
const TCHAR CMD_SELSETTEXT[]            = _T("SEL_SETTEXT");
const TCHAR CMD_SELSETTEXTEX[]          = _T("SEL_SETTEXT+");
const TCHAR CMD_IF[]                    = _T("IF");
const TCHAR CMD_LABEL[]                 = _T("LABEL");
const TCHAR CMD_GOTO[]                  = _T("GOTO");
const TCHAR CMD_ELSE[]                  = _T("ELSE");
const TCHAR CMD_ENDIF[]                 = _T("ENDIF");

enum eParamType {
    PT_UNKNOWN = 0,
    PT_INT,
    PT_PINT,
    PT_STR,
    PT_PSTR,
    PT_HEXSTR,
    PT_PHEXSTR,

    PT_COUNT
};

const TCHAR* STR_PARAMTYPE[PT_COUNT] =
{
    _T(""),
    _T("int"),
    _T("@int"),
    _T("str"),
    _T("@str"),
    _T("hex"),
    _T("@hex")
};

extern FuncItem              g_funcItem[nbFunc + MAX_USERMENU_ITEMS + 1];

extern COLORREF              g_colorTextNorm;

extern CInputBoxDlg InputBoxDlg;

BOOL     g_bIsNppUnicode = FALSE;
WNDPROC  nppOriginalWndProc;

typedef std::map<tstr, tstr> tEnvVars;
tEnvVars g_GlobalEnvVars;
tEnvVars g_LocalEnvVarNames;

static double calc_precision = 0.000001;
static TCHAR  szCalcDefaultFmt[12] = _T("%.6f");
static TCHAR  szCalcSmallFmt[12]   = _T("%.6G");


static int  FileFilterPos(const TCHAR* szFilePath);
static void GetPathAndFilter(const TCHAR* szPathAndFilter, int nFilterPos, tstr& out_Path, tstr& out_Filter);
static void GetFilePathNamesList(const TCHAR* szPath, const TCHAR* szFilter, CListT<tstr>& FilesList);
static void PrintDirContent(CNppExec* pNppExec, const TCHAR* szPath, const TCHAR* szFilter);
static void runInputBox(CNppExec* pNppExec, const TCHAR* szMessage);


class PrintMacroVarFunc
{
    public:
        typedef const CNppExecMacroVars::tMacroVars container_type;
        typedef container_type::const_iterator iterator_type;

        PrintMacroVarFunc(CNppExec* pNppExec) : m_pNppExec(pNppExec)
        {
        }

        bool operator()(iterator_type itrVar, bool isLocalVar)
        {
            tstr S = isLocalVar ? _T("local ") : _T("");
            S += itrVar->first;
            S += _T(" = ");
            if ( itrVar->second.length() > MAX_VAR_LENGTH2SHOW )
            {
                S.Append( itrVar->second.c_str(), MAX_VAR_LENGTH2SHOW - 5 );
                S += _T("(...)");
            }
            else
            {
                S += itrVar->second;
            }
            m_pNppExec->GetConsole().PrintMessage( S.c_str(), false );

            return true;
        }

    protected:
        CNppExec* m_pNppExec;
};

class SubstituteMacroVarFunc
{
    public:
        typedef const CNppExecMacroVars::tMacroVars container_type;
        typedef container_type::const_iterator iterator_type;

        SubstituteMacroVarFunc(tstr& Value) : m_Value(Value)
        {
            m_ValueUpper = Value;
            if ( m_ValueUpper.length() > 0 )
                ::CharUpper( m_ValueUpper.c_str() );
        }

        bool operator()(iterator_type itrVar, bool isLocalVar)
        {
            int pos = m_ValueUpper.Find(_T("$("), 0);
            if ( pos < 0 )
                return false;

            const tstr& varName = itrVar->first;
            while ( (pos = m_ValueUpper.Find(varName, pos)) >= 0 )
            {
                const tstr& varValue = itrVar->second;
                m_ValueUpper.Replace( pos, varName.length(), varValue.c_str(), varValue.length() );
                m_Value.Replace( pos, varName.length(), varValue.c_str(), varValue.length() );
                pos += varValue.length();
            }

            return true;
        }

    protected:
        tstr& m_Value;
        tstr m_ValueUpper;
};

template<class MacroVarFunc> void IterateUserMacroVars(
    typename MacroVarFunc::container_type& userMacroVars,
    typename MacroVarFunc::container_type& userLocalMacroVars,
    MacroVarFunc func)
{
    bool isLocalVars = true;
    typename MacroVarFunc::iterator_type itrVar = userLocalMacroVars.begin();
    for ( ; ; ++itrVar )
    {
        if ( isLocalVars )
        {
            if ( itrVar == userLocalMacroVars.end() )
            {
                itrVar = userMacroVars.begin();
                if ( itrVar != userMacroVars.end() )
                    isLocalVars = false;
                else
                    break;
            }
        }
        else
        {
            if ( itrVar == userMacroVars.end() )
                break;
        }

        if ( isLocalVars ||
             (userLocalMacroVars.find(itrVar->first) == userLocalMacroVars.end()) )
        {
            if ( !func(itrVar, isLocalVars) )
                break;
        }
    }
}


#define abs_val(x) (((x) < 0) ? (-(x)) : (x))


static bool IsTabSpaceOrEmptyChar(const TCHAR ch)
{
  return ( (ch == 0 || ch == _T(' ') || ch == _T('\t')) ? true : false );
}
 /**/
void DelLeadingTabSpaces(tstr& S)
{
  TCHAR ch;
  int   i = 0;
  while ( ((ch = S.GetAt(i)) == _T(' ')) || (ch == _T('\t')) )
  {
    ++i;
  }
  S.Delete(0, i);
}
 /**/
void DelTrailingTabSpaces(tstr& S)
{
  TCHAR ch;
  int   i = S.length() - 1;
  while ( ((ch = S.GetAt(i)) == _T(' ')) || (ch == _T('\t')) )
  {
    --i;
  }
  S.Delete(i + 1, -1);
}

/**/
static bool isHexNumChar(const TCHAR ch)
{
    return ( ( (ch >= _T('0') && ch <= _T('9')) ||
               (ch >= _T('A') && ch <= _T('F')) ||
               (ch >= _T('a') && ch <= _T('f')) ) ? true : false );
}
/**/
static bool isDecNumChar(const TCHAR ch)
{
    return ( (ch >= _T('0') && ch <= _T('9')) ? true : false );
}

/**/
#define  SEP_TABSPACE  0

const TCHAR* get_param(const TCHAR* s, tstr& param, const TCHAR sep = SEP_TABSPACE, 
                       bool* pDblQuote = 0, bool* pBracket = 0)
{
    param.Clear();

    if ( pDblQuote )  *pDblQuote = false;
    if ( pBracket )   *pBracket = false;

    while ( *s == _T(' ') || *s == _T('\t') )  ++s;  // skip leading tabs/spaces

    int i = 0;
    int n = 0;
    bool isComplete = false;
    bool isDblQuote = false;
    bool isEnvVar = false;
    bool isBracket = false;

    if ( pBracket )
    {
        if ( *s == _T('@') )
        {
            param += _T('@');
            ++s;
        }
        if ( *s == _T('[') )
        {
            isBracket = true;
            *pBracket = true;
            param += _T('[');
            ++s;
        }
    }

    while ( !isComplete )
    {
        const TCHAR ch = *s;
        
        switch ( ch )
        {
            case _T('\"'):
                if ( !isEnvVar )
                {
                    isDblQuote = !isDblQuote;
                    if ( isDblQuote && pDblQuote )
                    {
                        *pDblQuote = true;
                    }
                    if ( isBracket )
                        param += ch;
                }
                else
                {
                    param += ch;
                }
                ++s; // to next character
                break;

            case _T(' '):
            case _T('\t'):
                if ( sep == SEP_TABSPACE && !isDblQuote && !isBracket && !isEnvVar )
                {
                    isComplete = true;
                }
                else
                {
                    param += ch;
                    if ( isDblQuote || isBracket )
                        n = param.length();
                }
                ++s; // to next character
                break;

            case _T('$'):
                if ( !isEnvVar )
                {
                    if ( *(s + 1) == _T('(') )
                    {
                        isEnvVar = true;
                    }
                }
                param += ch;
                ++s; // to next character
                break;

            case _T(')'):
                if ( isEnvVar )
                {
                    isEnvVar = false;
                }
                param += ch;
                ++s; // to next character
                break;

            case _T(']'):
                if ( isBracket && !isEnvVar && !isDblQuote )
                {
                    isBracket = false;
                }
                param += ch;
                ++s; // to next character
                break;

            case 0:
                isComplete = true;
                break;

            default:
                if ( ch == sep && !isDblQuote && !isEnvVar )
                {
                    isComplete = true;
                }
                else
                {
                    param += ch;
                }
                ++s; // to next character
                break;
        }
    }
    
    for ( i = param.length() - 1; i >= n; i-- )
    {
        const TCHAR ch = param[i];
        if ( ch != _T(' ') && ch != _T('\t') )
            break;
    }
    n = param.length() - 1;
    if ( i < n )
    {
        param.Delete(i + 1, n - i); // remove trailing tabs/spaces
    }

    while ( *s == _T(' ') || *s == _T('\t') )  ++s;  // skip trailing tabs/spaces

    return s;
}

/**/
bool calc2(tstr& func, tstr& calcError, tstr& ret);
/**/
void DelLeadingTabSpaces1(CStrT<char>& S)
{
    char  ch;
    int   i = 0;
    while ( ((ch = S.GetAt(i)) == ' ') || (ch == '\t') )
    {
        ++i;
    }
    S.Delete(0, i);
}
/**/
void DelTrailingTabSpaces1(CStrT<char>& S)
{
    char  ch;
    int   i = S.length() - 1;
    while ( ((ch = S.GetAt(i)) == ' ') || (ch == '\t') )
    {
        --i;
    }
    S.Delete(i + 1, -1);
}
/**/
typedef struct sUserConst {
    CStrT<char> constName;
    tstr        constValue;
    int         nLine;
} tUserConst;
/**/
void initFParserConsts(FunctionParser& fp)
{
    
    Runtime::GetLogger().Add(   _T("initFParserConsts()") );
    Runtime::GetLogger().Add(   _T("{") );
    Runtime::GetLogger().IncIndentLevel();
    
    fp.AddConstant("WM_COMMAND", WM_COMMAND);
    fp.AddConstant("WM_USER", WM_USER);
    fp.AddConstant("NPPMSG", NPPMSG);

    CDirFileLister FileLst;
    tstr           path;

    path = Runtime::GetNppExec().getPluginPath();
    path += _T("\\NppExec\\*.h");
    if ( FileLst.FindNext(path.c_str(), CDirFileLister::ESF_FILES | CDirFileLister::ESF_SORTED) )
    {
        CFileBufT<char>    fbuf;
        CStrT<char>        line;
        tUserConst         userConst;
        CListT<tUserConst> unparsedUserConstsList;
        
        do {
            path = Runtime::GetNppExec().getPluginPath();
            path += _T("\\NppExec\\");
            path += FileLst.GetItem();
            
            if ( fbuf.LoadFromFile(path.c_str()) )
            {
                int nItemsOK = 0;
                
                Runtime::GetLogger().Add(   FileLst.GetItem() );
                Runtime::GetLogger().Add(   _T("(") );
                Runtime::GetLogger().IncIndentLevel();

                unparsedUserConstsList.DeleteAll();
                
                while ( fbuf.GetLine(line) >= 0 )
                {
                    if ( line.length() > 0 )
                    {
                        int i = line.Find("//");
                        if ( i >= 0 )
                            line.Delete(i, -1);
                        
                        DelLeadingTabSpaces1(line);
                        
                        if ( line.length() > 0 && line.StartsWith("#define") )
                        {
                            line.Delete(0, 7); // delete "#define"
                            DelLeadingTabSpaces1(line);
                            if ( line.length() > 0 )
                            {
                                userConst.constName.Clear();
                                userConst.constValue.Clear();
                                userConst.nLine = fbuf.GetLineNumber() - 1;

                                i = 0;
                                while ( i < line.length() && line[i] != ' ' && line[i] != '\t' )
                                {
                                    userConst.constName += line[i];
                                    ++i;
                                }

                                line.Delete(0, i); // delete const name
                                DelLeadingTabSpaces1(line);
                                DelTrailingTabSpaces1(line);
                                if ( line.length() > 0 )
                                {
                                    double val = 0;
                                    bool isVal = false;

                                    #ifdef UNICODE
                                        wchar_t* p = SysUniConv::newMultiByteToUnicode(line.c_str());
                                        if ( p )
                                        {
                                            userConst.constValue = p;
                                            delete [] p;
                                        }
                                    #else
                                        userConst.constValue = line;
                                    #endif

                                    /*if ( isDecNumChar(constValue.GetAt(0)) )
                                    {
                                        val = c_base::_tstr2uint(constValue.c_str());
                                        isVal = true;
                                    }
                                    else*/
                                    {
                                        tstr calcErr;

                                        Runtime::GetLogger().Activate(false); // temporary disable

                                        isVal = calc2(userConst.constValue, calcErr, userConst.constValue);

                                        Runtime::GetLogger().Activate(true);  // enable again

                                        if ( isVal )
                                        {
                                            if ( userConst.constValue.Find(_T('.')) >= 0 )
                                                val = _t_str2f(userConst.constValue.c_str());
                                            else
                                                val = c_base::_tstr2uint(userConst.constValue.c_str());
                                        }
                                        else
                                        {
                                            unparsedUserConstsList.Add(userConst);
                                        }
                                    }
                                    
                                    if ( isVal )
                                    {
                                        fp.AddConstant(userConst.constName.c_str(), val);
                                        ++nItemsOK;
                                    }
                                }
                            }
                        }
                    }
                }

                if ( !unparsedUserConstsList.IsEmpty() )
                {
                    tstr calcErr;

                    CListItemT<tUserConst>* pItem = unparsedUserConstsList.GetFirst();
                    while ( pItem )
                    {
                        tUserConst& uc = pItem->GetItem();
                        bool isVal = false;
                        
                        Runtime::GetLogger().Activate(false); // temporary disable

                        isVal = calc2(uc.constValue, calcErr, uc.constValue);

                        Runtime::GetLogger().Activate(true);  // enable again

                        if ( isVal )
                        {
                            double val = 0;
                            if ( uc.constValue.Find(_T('.')) >= 0 )
                                val = _t_str2f(uc.constValue.c_str());
                            else
                                val = c_base::_tstr2uint(uc.constValue.c_str());

                            fp.AddConstant(uc.constName.c_str(), val);
                            ++nItemsOK;
                        }
                        else
                        {

                            Runtime::GetLogger().AddEx( _T("error at line %d: \"%s\""), uc.nLine, uc.constValue.c_str() );
                            Runtime::GetLogger().AddEx( _T(" - %s"), calcErr.c_str() );

                        }

                        pItem = pItem->GetNext();
                    }

                    unparsedUserConstsList.DeleteAll();
                }

                Runtime::GetLogger().AddEx( _T("%d definitions added."), nItemsOK );
                Runtime::GetLogger().DecIndentLevel();
                Runtime::GetLogger().Add(   _T(")") );

            }
        } 
        while ( FileLst.GetNext() );
    }
    else
    {
        
        path = Runtime::GetNppExec().getPluginPath();
        path += _T("\\NppExec");

        Runtime::GetLogger().AddEx( _T("; no *.h files found in \"%s\""), path.c_str() );

    }

    Runtime::GetLogger().DecIndentLevel();
    Runtime::GetLogger().Add(   _T("}") );

}

/**/
double calc(tstr& func, tstr& calcError)
{
    TCHAR szNum[50];
    
    /*// pre-parse for '0x...' numbers 
    int pos = func.Find( _T("0x") );
    while ( pos >= 0 )
    {
        const TCHAR ch = func.GetAt(pos + 2);
        if ( isHexNumChar(ch) )
        {
            const int nn = c_base::_tstr2int( func.c_str() + pos );
            int hexLen = 2;
            while ( isHexNumChar(func.GetAt(pos + hexLen)) )  ++hexLen;
            int decLen = c_base::_tint2str(nn, szNum);
            func.Replace(pos, hexLen, szNum, decLen);
            pos += decLen;
        }
        pos = func.Find( _T("0x"), pos );
    }*/

    #ifdef UNICODE
        char* pFunc = SysUniConv::newUnicodeToMultiByte( func.c_str() );
    #else
        const char* pFunc = func.c_str();
    #endif

    if ( (!pFunc) || (!pFunc[0]) )
    {
        #ifdef UNICODE
            if ( pFunc )
                delete [] pFunc;
        #endif

        calcError = _T("Input function is empty");
        return 0;
    }

    static bool hasConsts = false;
    static FunctionParser fp;
    
    if ( !hasConsts )
    {
        int len = 0;
        const TCHAR* pPrecision = Runtime::GetOptions().GetStr(OPTS_CALC_PRECISION, &len);
        if ( len > 0 )
        {
            double precision = _t_str2f(pPrecision);
            precision = abs_val(precision);
            if ( precision > 0.0 )
            {
                int d = 0;
                
                calc_precision = precision;

                while ( precision < 0.99 )
                {
                    precision *= 10;
                    ++d;
                }
                wsprintf(szCalcDefaultFmt, _T("%%.%df"), d);
                wsprintf(szCalcSmallFmt,   _T("%%.%dG"), d);
            }
        }
        
        hasConsts = true;
        initFParserConsts(fp);
    }

    int errPos;
    double ret = 0;

    calcError.Clear();

    if ( (errPos = fp.Parse(pFunc, "")) == -1 )
    {
        double var = 0;
        
        var = fp.Eval( &var );
        if ( fp.EvalError() == 0 )
            ret = var;
        else
            calcError = _T("Eval error");
    }
    else
    {
        #ifdef UNICODE
            TCHAR* pErr = SysUniConv::newMultiByteToUnicode( fp.ErrorMsg() );
        #else
            const char* pErr = fp.ErrorMsg();
        #endif

        calcError = pErr;
      
        #ifdef UNICODE
            delete [] pErr;
        #endif

        calcError += _T(" at pos ");
        c_base::_tint2str(errPos, szNum);
        calcError += szNum;
    }

    #ifdef UNICODE
        delete [] pFunc;
    #endif

    return ret;
}
/**/
bool calc2(tstr& func, tstr& calcError, tstr& ret)
{
    double fret = calc(func, calcError);

    if ( calcError.IsEmpty() )
    {
        TCHAR szNum[80];

        szNum[0] = 0;

        if ( abs_val(fret) > calc_precision*100 )
        {
            __int64 n = (__int64) fret;
            double  diff = fret - n;

            if ( abs_val(diff) < calc_precision )
            {
                // result can be rounded
                _t_sprintf( szNum, _T("%I64d"), n ); 
                // ha-ha, wsprintf does not support neither "%I64d" nor "%f"
            }
            else
            {
                int nn = _t_sprintf( szNum, szCalcDefaultFmt, fret );
                if ( nn > 0 )
                {
                    while ( nn > 0 && szNum[--nn] == _T('0') ) ;
                    if ( szNum[nn] == _T('.') )  ++nn; // keep one '0' after '.'
                    szNum[nn + 1] = 0; // exclude all trailing '0' after '.'
                }
            }   
        }
        else
        {
            // result is too small
            _t_sprintf( szNum, szCalcSmallFmt, fret );
            // ha-ha, wsprintf does not support neither "%f" nor "%G"
        }

        ret = szNum;
    }

    return calcError.IsEmpty();
}


/*
 * CScriptEngine
 *
 *
 * Internal Commands:
 * ------------------
 * cls
 *   - clear Console screen
 * cd
 *   - shows current path
 * cd <path>
 *   - changes current directory (absolute or relative)
 * cd <drive:\path>
 *   - changes current drive and directory
 * dir
 *   - lists subdirs and files
 * dir <mask>
 *   - lists subdirs and files matched the mask
 * dir <path\mask>
 *   - lists subdirs and files matched the mask
 * echo <text>
 *   - prints a text in the Console
 * if <condition> goto <label>
 *   - jumps to the label if the condition is true
 * goto <label>
 *   - jumps to the label
 * set 
 *   - shows all user's variables
 * set <var> 
 *   - shows the value of user's variable <var>
 * set <var> = <value> 
 *   - sets the value of user's variable <var>
 * set <var> ~ <math expression>
 *   - calculates the math expression
 * set <var> ~ strlen <string>
 *   - calculates the string length
 * set <var> ~ strlenutf8 <string>
 *   - calculates the UTF-8 string length
 * set <var> ~ strupper <string>
 *   - returns the string in upper case
 * set <var> ~ strlower <string>
 *   - returns the string in lower case
 * set <var> ~ substr <pos> <len> <string>
 *   - returns the substring
 * set <var> ~ strfind <s> <t>
 *   - returns the first position of <t> in <s>
 * set <var> ~ strrfind <s> <t>
 *   - returns the last position of <t> in <s>
 * set <var> ~ strreplace <s> <t0> <t1>
 *   - replaces all <t0> with <t1>
 * set local
 *   - shows all user's local variables
 * set local <var>
 *   - shows the value of user's local variable <var>
 * set local <var> = ...
 *   - sets the value of user's local variable <var>
 * set local <var> ~ ...
 *   - calculates the value of user's local variable <var>
 * unset <var>
 *   - removes user's variable <var>
 * unset local <var>
 *   - removes user's local variable <var>
 * env_set <var>
 *   - shows the value of environment variable <var>
 * env_set <var> = <value>
 *   - sets the value of environment variable <var>
 * env_unset <var>
 *   - removes/restores the environment variable <var>
 * inputbox "message"
 *   - shows InputBox, sets $(INPUT)
 * inputbox "message" : initial_value
 *   - shows InputBox with specified initial value, sets $(INPUT)
 * inputbox "message" : "value_name" : initial_value
 *   - InputBox customization
 * con_colour <colours>
 *   - sets the Console's colours
 * con_filter <filters>
 *   - enables/disables the Console's output filters
 * con_loadfrom <file> 
 *   - loads a file's content to the Console
 * con_load <file>
 *   - see "con_loadfrom"
 * con_saveto <file>
 *   - saves the Console's content to a file
 * con_save <file>
 *   - see "con_saveto"
 * sel_loadfrom <file> 
 *   - replace current selection with a file's content
 * sel_load <file> 
 *   - see "sel_loadfrom"
 * sel_saveto <file>
 *   - save the selected text (in current encoding) to a file
 * sel_saveto <file> : <encoding>
 *   - save the selected text (in specified encoding) to a file
 * sel_save <file> : <encoding>
 *   - see "sel_saveto"
 * sel_settext <text>
 *   - replace current selection with the text specified
 * sel_settext+ <text>
 *   - replace current selection with the text specified
 * npp_exec <script>
 *   - executes commands from specified script
 * npp_exec <file>
 *   - executes commands from specified file
 *   - works with a partial file path/name
 * npp_close
 *   - closes current file in Notepad++
 * npp_close <file>
 *   - closes specified file opened in Notepad++
 *   - works with a partial file path/name
 * npp_console <on/off/keep>
 *   - show/hide the Console window
 * npp_console <enable/disable>
 *   - enable/disable output to the Console
 * npp_console <1/0/?> 
 *   - show/hide the Console window
 * npp_console <+/->
 *   - enable/disable output to the Console
 * npp_menucommand <menu\item\name>
 *   - executes (invokes) a menu item
 * npp_open <file>
 *   - opens specified file in Notepad++
 * npp_open <mask>
 *   - opens file(s) matched the mask
 * npp_open <path\mask>
 *   - opens file(s) matched the mask
 * npp_run <command> 
 *   - the same as Notepad++'s Run command
 *   - executes command (runs a child process) w/o waiting until it returns
 * npp_save 
 *   - saves current file in Notepad++
 * npp_save <file>
 *   - saves specified file in Notepad++ (if it's opened in Notepad++)
 *   - works with a partial file path/name
 * npp_saveas <file>
 *   - saves current file with a new (path)name
 * npp_saveall
 *   - saves all modified files
 * npp_switch <file> 
 *   - switches to specified file (if it's opened in Notepad++)
 *   - works with a partial file path/name
 * npp_sendmsg <msg>
 *   - sends a message (msg) to Notepad++
 * npp_sendmsg <msg> <wparam>
 *   - sends a message (msg) with parameter (wparam) to Notepad++
 * npp_sendmsg <msg> <wparam> <lparam>
 *   - sends a message (msg) with parameters (wparam, lparam) to Notepad++
 * npp_sendmsgex <hwnd> <msg> <wparam> <lparam>
 *   - sends a message (msg) with parameters (wparam, lparam) to hwnd
 * sci_sendmsg <msg>
 *   - sends a message (msg) to current Scintilla
 * sci_sendmsg <msg> <wparam>
 *   - sends a message (msg) with parameter (wparam) to current Scintilla
 * sci_sendmsg <msg> <wparam> <lparam>
 *   - sends a message (msg) with parameters (wparam, lparam) to current Scintilla
 * npe_cmdalias
 *   - show all command aliases
 * npe_cmdalias <alias>
 *   - shows the value of command alias
 * npe_cmdalias <alias> =
 *   - removes the command alias
 * npe_cmdalias <alias> = <command>
 *   - sets the command alias
 * npe_console <options>
 *   - set/modify Console options/mode
 * npe_debuglog <on/off>
 *   - enable/disable Debug Log
 * npe_noemptyvars <1/0>
 *   - enable/disable replacement of empty vars
 *
 * Internal Macros (environment variables):
 * ----------------------------------------
 * The same as here: http://notepad-plus.sourceforge.net/uk/run-HOWTO.php
 *
 * $(FULL_CURRENT_PATH)  : E:\my Web\main\welcome.html
 * $(CURRENT_DIRECTORY)  : E:\my Web\main
 * $(FILE_NAME)          : welcome.html
 * $(NAME_PART)          : welcome
 * $(EXT_PART)           : .html
 * $(NPP_DIRECTORY)      : the full path of directory with notepad++.exe
 * $(CURRENT_WORD)       : word(s) you selected in Notepad++
 * $(CURRENT_LINE)       : current line number
 * $(CURRENT_COLUMN)     : current column number
 *
 * Additional environment variables:
 * ---------------------------------
 * $(#0)                 : C:\Program Files\Notepad++\notepad++.exe
 * $(#N), N=1,2,3...     : full path of the Nth opened document
 * $(LEFT_VIEW_FILE)     : current file path-name in primary (left) view
 * $(RIGHT_VIEW_FILE)    : current file path-name in second (right) view
 * $(PLUGINS_CONFIG_DIR) : full path of the plugins configuration directory
 * $(CWD)                : current working directory of NppExec (use "cd" to change it)
 * $(ARGC)               : number of arguments passed to the NPP_EXEC command
 * $(ARGV)               : all arguments passed to the NPP_EXEC command after the script name
 * $(ARGV[0])            : script name - first parameter of the NPP_EXEC command
 * $(ARGV[N])            : Nth argument (N=1,2,3...)
 * $(RARGV)              : all arguments in reverse order (except the script name)
 * $(RARGV[N])           : Nth argument in reverse order (N=1,2,3...)
 * $(INPUT)              : this value is set by the 'inputbox' command
 * $(INPUT[N])           : Nth field of the $(INPUT) value (N=1,2,3...)
 * $(OUTPUT)             : this value can be set by the child process, see npe_console v+
 * $(OUTPUT1)            : first line in $(OUTPUT)
 * $(OUTPUTL)            : last line in $(OUTPUT)
 * $(EXITCODE)           : exit code of the last executed child process
 * $(MSG_RESULT)         : result of 'npp_sendmsg[ex]' or 'sci_sendmsg'
 * $(MSG_WPARAM)         : wParam (output) of 'npp_sendmsg[ex]' or 'sci_sendmsg'
 * $(MSG_LPARAM)         : lParam (output) of 'npp_sendmsg[ex]' or 'sci_sendmsg'
 * $(SYS.<var>)          : system's environment variable, e.g. $(SYS.PATH)
 *
 */

CScriptEngine::CScriptEngine(CNppExec* pNppExec)
{
    m_pNppExec = pNppExec;

    m_nCmdType = CMDTYPE_UNKNOWN;

    m_DoFunc[CMDTYPE_UNKNOWN]        = &CScriptEngine::Do;
    m_DoFunc[CMDTYPE_NPPEXEC]        = &CScriptEngine::DoNppExec;
    m_DoFunc[CMDTYPE_NPPOPEN]        = &CScriptEngine::DoNppOpen;
    m_DoFunc[CMDTYPE_NPPRUN]         = &CScriptEngine::DoNppRun;
    m_DoFunc[CMDTYPE_NPPSAVE]        = &CScriptEngine::DoNppSave;
    m_DoFunc[CMDTYPE_NPPSWITCH]      = &CScriptEngine::DoNppSwitch;
    m_DoFunc[CMDTYPE_CLS]            = &CScriptEngine::DoCls;
    m_DoFunc[CMDTYPE_CD]             = &CScriptEngine::DoCd;
    m_DoFunc[CMDTYPE_DIR]            = &CScriptEngine::DoDir;
    m_DoFunc[CMDTYPE_NPPSAVEALL]     = &CScriptEngine::DoNppSaveAll;
    m_DoFunc[CMDTYPE_CONLOADFROM]    = &CScriptEngine::DoConLoadFrom;
    m_DoFunc[CMDTYPE_CONSAVETO]      = &CScriptEngine::DoConSaveTo;
    m_DoFunc[CMDTYPE_ECHO]           = &CScriptEngine::DoEcho;
    m_DoFunc[CMDTYPE_NPEDEBUGLOG]    = &CScriptEngine::DoNpeDebugLog;
    m_DoFunc[CMDTYPE_SET]            = &CScriptEngine::DoSet;
    m_DoFunc[CMDTYPE_UNSET]          = &CScriptEngine::DoUnset;
    m_DoFunc[CMDTYPE_NPENOEMPTYVARS] = &CScriptEngine::DoNpeNoEmptyVars;
    m_DoFunc[CMDTYPE_SELSAVETO]      = &CScriptEngine::DoSelSaveTo;
    m_DoFunc[CMDTYPE_NPPCLOSE]       = &CScriptEngine::DoNppClose;
    m_DoFunc[CMDTYPE_INPUTBOX]       = &CScriptEngine::DoInputBox;
    m_DoFunc[CMDTYPE_NPPCONSOLE]     = &CScriptEngine::DoNppConsole;
    m_DoFunc[CMDTYPE_ENVSET]         = &CScriptEngine::DoEnvSet;
    m_DoFunc[CMDTYPE_ENVUNSET]       = &CScriptEngine::DoEnvUnset;
    m_DoFunc[CMDTYPE_NPECONSOLE]     = &CScriptEngine::DoNpeConsole;
    m_DoFunc[CMDTYPE_SELLOADFROM]    = &CScriptEngine::DoSelLoadFrom;
    m_DoFunc[CMDTYPE_SELSETTEXT]     = &CScriptEngine::DoSelSetText;
    m_DoFunc[CMDTYPE_SELSETTEXTEX]   = &CScriptEngine::DoSelSetTextEx;
    m_DoFunc[CMDTYPE_NPECMDALIAS]    = &CScriptEngine::DoNpeCmdAlias;
    m_DoFunc[CMDTYPE_NPPSENDMSG]     = &CScriptEngine::DoNppSendMsg;
    m_DoFunc[CMDTYPE_SCISENDMSG]     = &CScriptEngine::DoSciSendMsg;
    m_DoFunc[CMDTYPE_NPPSENDMSGEX]   = &CScriptEngine::DoNppSendMsgEx;
    m_DoFunc[CMDTYPE_NPPMENUCOMMAND] = &CScriptEngine::DoNppMenuCommand;
    m_DoFunc[CMDTYPE_CONCOLOUR]      = &CScriptEngine::DoConColour;
    m_DoFunc[CMDTYPE_CONFILTER]      = &CScriptEngine::DoConFilter;
    m_DoFunc[CMDTYPE_IF]             = &CScriptEngine::DoIf;
    m_DoFunc[CMDTYPE_LABEL]          = &CScriptEngine::DoLabel;
    m_DoFunc[CMDTYPE_GOTO]           = &CScriptEngine::DoGoTo;
    m_DoFunc[CMDTYPE_NPPSAVEAS]      = &CScriptEngine::DoNppSaveAs;
    m_DoFunc[CMDTYPE_ELSE]           = &CScriptEngine::DoElse;
    m_DoFunc[CMDTYPE_ENDIF]          = &CScriptEngine::DoEndIf;
}

#define INVALID_TSTR_LIST_ITEM ((CListItemT<tstr>*)(-1))

void CScriptEngine::Run()
{
    m_pNppExec->SetRunningScriptEngine(this);

    Runtime::GetLogger().Add(   _T("; CScriptEngine::Run - start") );

    tstr S;
    bool isNppExec = false;
    bool isFirstCmd = true;

    m_execState.nExecCounter = 0;
    m_execState.nExecMaxCount = Runtime::GetOptions().GetInt(OPTI_EXEC_MAXCOUNT);
    m_execState.nGoToCounter = 0;
    m_execState.nGoToMaxCount = Runtime::GetOptions().GetInt(OPTI_GOTO_MAXCOUNT);
    m_execState.pScriptLineCurrent = NULL;
    m_execState.pScriptLineNext = INVALID_TSTR_LIST_ITEM;

    m_execState.ScriptContextList.Add( ScriptContext() );
    {
        ScriptContext& currentScript = m_execState.GetCurrentScriptContext();
        currentScript.CmdRange.pBegin = m_pNppExec->m_CmdList.GetFirst();
        currentScript.CmdRange.pEnd = 0;
        currentScript.IsNppExeced = false;
    }

    CListItemT<tstr>* p = m_pNppExec->m_CmdList.GetFirst();
    while ( p && m_pNppExec->_consoleIsVisible )
    {
        eCmdType nCmdType = CMDTYPE_COMMENT_OR_EMPTY;

        m_execState.pScriptLineCurrent = p;
        m_execState.pScriptLineNext = INVALID_TSTR_LIST_ITEM;

        S = p->GetItem();

        if ( S.length() > 0 )
        {

            Runtime::GetLogger().Clear();
            Runtime::GetLogger().Add(   _T("; command info") );
            Runtime::GetLogger().AddEx( _T("Current command item:  p = 0x%X"), p );
            Runtime::GetLogger().Add(   _T("{") );
            Runtime::GetLogger().IncIndentLevel();
            Runtime::GetLogger().AddEx( _T("_item  = \"%s\""), p->GetItem().c_str() );
            Runtime::GetLogger().AddEx( _T("_prev  = 0x%X"), p->GetPrev() );
            if ( p->GetPrev() )
            {
                const CListItemT<tstr>* pPrev = p->GetPrev();
                Runtime::GetLogger().Add(   _T("{") );
                Runtime::GetLogger().IncIndentLevel();
                Runtime::GetLogger().AddEx( _T("_item = \"%s\""), pPrev->GetItem().c_str() );
                Runtime::GetLogger().AddEx( _T("_prev = 0x%X;  _next = 0x%X;  _owner = 0x%X"), 
                    pPrev->GetPrev(), pPrev->GetNext(), pPrev->GetOwner() );
                Runtime::GetLogger().DecIndentLevel();
                Runtime::GetLogger().Add(   _T("}") );
            }
            Runtime::GetLogger().AddEx( _T("_next  = 0x%X"), p->GetNext() ); 
            if ( p->GetNext() )
            {
                const CListItemT<tstr>* pNext = p->GetNext();
                Runtime::GetLogger().Add(   _T("{") );
                Runtime::GetLogger().IncIndentLevel();
                Runtime::GetLogger().AddEx( _T("_item = \"%s\""), pNext->GetItem().c_str() );
                Runtime::GetLogger().AddEx( _T("_prev = 0x%X;  _next = 0x%X;  _owner = 0x%X"), 
                    pNext->GetPrev(), pNext->GetNext(), pNext->GetOwner() );
                Runtime::GetLogger().DecIndentLevel();
                Runtime::GetLogger().Add(   _T("}") );
            }
            Runtime::GetLogger().AddEx( _T("_owner = 0x%X"), p->GetOwner() );
            Runtime::GetLogger().DecIndentLevel();
            Runtime::GetLogger().Add(   _T("}") );
        
            Runtime::GetLogger().Add(   _T("; executing ModifyCommandLine") );

            nCmdType = modifyCommandLine(S);
            if ( nCmdType != CMDTYPE_COMMENT_OR_EMPTY )
            {
                ScriptContext& currentScript = m_execState.GetCurrentScriptContext();
                const int ifDepth = currentScript.GetIfDepth();
                const eIfState ifState = currentScript.GetIfState();
                if ( (ifState == IF_WANT_SILENT_ENDIF) ||
                     ((ifState == IF_WANT_ENDIF) && (nCmdType != CMDTYPE_ENDIF)) ||
                     ((ifState == IF_WANT_ELSE) && (nCmdType != CMDTYPE_ELSE && nCmdType != CMDTYPE_ENDIF))
                   )
                {
                    Runtime::GetLogger().AddEx( _T("; skipping - waiting for %s"),
                        (ifState == IF_WANT_ENDIF) ? CMD_ENDIF : CMD_ELSE );
                
                    if ( nCmdType == CMDTYPE_IF )
                    {
                        // nested IF inside a conditional block that is being skipped
                        currentScript.PushIfState(IF_WANT_SILENT_ENDIF);
                        // skip the nested IF...ENDIF block as well - mark it as "silent"
                    }
                    else if ( nCmdType == CMDTYPE_ENDIF )
                    {
                        // nested ENDIF inside a conditional block that is being skipped
                        if ( ifState == IF_WANT_SILENT_ENDIF ) // <-- this condition is redundant, but let it be just in case
                        {
                            // nested "silent" IF...ENDIF block is completed
                            currentScript.PopIfState();
                        }
                    }
                }
                else
                {
                    m_nCmdType = nCmdType;
                    if ( isFirstCmd && (m_nCmdType == CMDTYPE_NPPEXEC) )
                    {
                        isNppExec = true;
                    }
                    else 
                    {
                        if ( isFirstCmd )
                        {
                            isFirstCmd = false;
                        }
                        if ( isNppExec )
                        {
                            if ( m_nCmdType != CMDTYPE_NPPCONSOLE )
                            {
                                m_pNppExec->showConsoleDialog(false);
                            }
                            isNppExec = false;
                        }
                    }

                    if ( m_nCmdType == CMDTYPE_UNKNOWN )
                    {
                        // run child process (CScriptEngine::Do)
                        if ( S.length() > CONSOLECOMMAND_BUFSIZE - 1 )
                        {
                            // avoid buffer overrun
                            S.SetSize(CONSOLECOMMAND_BUFSIZE - 1);
                        }
                    }

                    m_sCmdParams = S;
                    (this->*m_DoFunc[m_nCmdType])(S);

                    // The same currentScript object is used here to handle NPP_EXEC as well
                    // (it's safe because ScriptContextList.DeleteLast() is not called before)
                    if ( (ifState == IF_MAYBE_ELSE) &&
                         (currentScript.GetIfState() == IF_MAYBE_ELSE) &&
                         (currentScript.GetIfDepth() == ifDepth) )
                    {
                        // The IfState was not changed, so remove IF_MAYBE_ELSE
                        currentScript.SetIfState(IF_NONE);
                    }
                }

                if ( m_pNppExec->m_SuspendedScriptAction.isActive() )
                {
                    m_pNppExec->m_SuspendedScriptAction.stopWaitingTimer();
                    break;
                }
            }
        }

        p = (m_execState.pScriptLineNext == INVALID_TSTR_LIST_ITEM) ? p->GetNext() : m_execState.pScriptLineNext;

        ScriptContext& currentScript = m_execState.GetCurrentScriptContext();
        if ( currentScript.IsNppExeced )
        {
            if ( p == currentScript.CmdRange.pEnd )
            {

                Runtime::GetLogger().AddEx( _T("; script context removed: { Name = \"%s\"; CmdRange = [0x%X, 0x%X) }"), 
                    currentScript.ScriptName.c_str(), currentScript.CmdRange.pBegin, currentScript.CmdRange.pEnd ); 

                CListT<tstr>& CmdList = m_pNppExec->m_CmdList;
                CListItemT<tstr>* pItem = currentScript.CmdRange.pBegin;
                while ( pItem != currentScript.CmdRange.pEnd )
                {
                    CListItemT<tstr>* pNext = pItem->GetNext();
                    CmdList.Delete(pItem);
                    pItem = pNext;
                }
                m_execState.ScriptContextList.DeleteLast();
            }
        }
    } // while

    if ( isNppExec )
    {
        // the whole script contains just NPP_EXEC commands
        // the Console is still hidden, it must be shown
        m_pNppExec->showConsoleDialog(false);
    }

    m_pNppExec->_consoleIsVisible = m_pNppExec->isConsoleDialogVisible();

    Runtime::GetLogger().Add(   _T("; CScriptEngine::Run - end") );

    m_pNppExec->SetRunningScriptEngine(0);
}

CScriptEngine::eCmdType CScriptEngine::getCmdType(tstr& Cmd)
{
    typedef struct stCmdTypes {
        const TCHAR* szCmd;
        eCmdType     nCmdType;
    } tCmdTypes;

    const int CMD_COUNT = 47;
    static const tCmdTypes CmdTypes[CMD_COUNT] = 
    {
        { CMD_CLS,            CMDTYPE_CLS            },
        { CMD_CD,             CMDTYPE_CD             },
        { CMD_DIR,            CMDTYPE_DIR            },
        { CMD_ECHO,           CMDTYPE_ECHO           },
        { CMD_CONLOADFROM,    CMDTYPE_CONLOADFROM    },
        { CMD_CONLOAD,        CMDTYPE_CONLOADFROM    },
        { CMD_CONSAVETO,      CMDTYPE_CONSAVETO      },
        { CMD_CONSAVE,        CMDTYPE_CONSAVETO      },
        { CMD_NPPCLOSE,       CMDTYPE_NPPCLOSE       },
        { CMD_NPPCONSOLE,     CMDTYPE_NPPCONSOLE     },
        { CMD_NPPEXEC,        CMDTYPE_NPPEXEC        },
        { CMD_NPPOPEN,        CMDTYPE_NPPOPEN        },
        { CMD_NPPRUN,         CMDTYPE_NPPRUN         },
        { CMD_NPPSAVE,        CMDTYPE_NPPSAVE        },
        { CMD_NPPSAVEAS,      CMDTYPE_NPPSAVEAS      },
        { CMD_NPPSAVEALL,     CMDTYPE_NPPSAVEALL     },
        { CMD_NPPSWITCH,      CMDTYPE_NPPSWITCH      },
        { CMD_NPPSENDMSG,     CMDTYPE_NPPSENDMSG     },
        { CMD_NPPSENDMSGEX,   CMDTYPE_NPPSENDMSGEX   },
        { CMD_NPPMENUCOMMAND, CMDTYPE_NPPMENUCOMMAND },
        { CMD_SCISENDMSG,     CMDTYPE_SCISENDMSG     },
        { CMD_SET,            CMDTYPE_SET            },
        { CMD_UNSET,          CMDTYPE_UNSET          },
        { CMD_ENVSET,         CMDTYPE_ENVSET         },
        { CMD_ENVUNSET,       CMDTYPE_ENVUNSET       },
        { CMD_SETENV,         CMDTYPE_ENVSET         },
        { CMD_UNSETENV,       CMDTYPE_ENVUNSET       },
        { CMD_INPUTBOX,       CMDTYPE_INPUTBOX       },
        { CMD_SELLOADFROM,    CMDTYPE_SELLOADFROM    },
        { CMD_SELLOAD,        CMDTYPE_SELLOADFROM    },
        { CMD_SELSAVETO,      CMDTYPE_SELSAVETO      },
        { CMD_SELSAVE,        CMDTYPE_SELSAVETO      },
        { CMD_SELSETTEXTEX,   CMDTYPE_SELSETTEXTEX   },
        { CMD_SELSETTEXT,     CMDTYPE_SELSETTEXT     },
        { CMD_NPECONSOLE,     CMDTYPE_NPECONSOLE     },
        { CMD_NPEDEBUGLOG,    CMDTYPE_NPEDEBUGLOG    },
        { CMD_NPEDEBUG,       CMDTYPE_NPEDEBUGLOG    },
        { CMD_NPENOEMPTYVARS, CMDTYPE_NPENOEMPTYVARS },
        { CMD_NPECMDALIAS,    CMDTYPE_NPECMDALIAS    },
        { CMD_IF,             CMDTYPE_IF             },
        { CMD_ELSE,           CMDTYPE_ELSE           },
        { CMD_ENDIF,          CMDTYPE_ENDIF          },
        { CMD_LABEL,          CMDTYPE_LABEL          },
        { CMD_GOTO,           CMDTYPE_GOTO           },
        { CMD_CONCOLOR,       CMDTYPE_CONCOLOUR      },
        { CMD_CONCOLOUR,      CMDTYPE_CONCOLOUR      },
        { CMD_CONFILTER,      CMDTYPE_CONFILTER      }
    };

    Runtime::GetLogger().Add(   _T("GetCmdType()") );
    Runtime::GetLogger().Add(   _T("{") );
    Runtime::GetLogger().IncIndentLevel();
    Runtime::GetLogger().AddEx( _T("[in]  \"%s\""), Cmd.c_str() );

    DelLeadingTabSpaces(Cmd);
    DelTrailingTabSpaces(Cmd);

    if ( Cmd.IsEmpty() )
    {

        Runtime::GetLogger().AddEx( _T("[ret] %d (empty command)"), CMDTYPE_COMMENT_OR_EMPTY );
        Runtime::GetLogger().DecIndentLevel();
        Runtime::GetLogger().Add(   _T("}") );

        return CMDTYPE_COMMENT_OR_EMPTY;
    }

    const TCHAR* pAliasNppExec = Runtime::GetOptions().GetStr(OPTS_ALIAS_CMD_NPPEXEC);
    if ( pAliasNppExec && pAliasNppExec[0] )
    {
        if ( pAliasNppExec[0] == Cmd.GetAt(0) ) // the alias is one character
        {
            if ( (Cmd.GetAt(0) != _T('\\') || Cmd.GetAt(1) != _T('\\')) &&  // not "\\..."
                 (Cmd.GetAt(0) != _T('/') || Cmd.GetAt(1) != _T('/')) )     // not "//..."
            {
                Cmd.Delete(0, 1);

                Runtime::GetLogger().AddEx( _T("[ret] 0x%X (%s)"), CMDTYPE_NPPEXEC, CMD_NPPEXEC );
                Runtime::GetLogger().DecIndentLevel();
                Runtime::GetLogger().Add(   _T("}") );

                return CMDTYPE_NPPEXEC;
            }
        }
    }

    Runtime::GetNppExec().GetMacroVars().CheckCmdAliases(Cmd);

    if ( Cmd.GetAt(0) == _T(':') && Cmd.GetAt(1) == _T(':') )
    {
        Runtime::GetNppExec().GetConsole().PrintError( _T("- can not use \"::\" at the beginning of line!") );

        Runtime::GetLogger().AddEx( _T("[ret] %d (command starts with ::)"), CMDTYPE_COMMENT_OR_EMPTY );
        Runtime::GetLogger().DecIndentLevel();
        Runtime::GetLogger().Add(   _T("}") );

        return CMDTYPE_COMMENT_OR_EMPTY;
    }

    if ( DEFAULT_ALIAS_CMD_LABEL == Cmd.GetAt(0) )
    {
        Cmd.Delete(0, 1);

        Runtime::GetLogger().AddEx( _T("[ret] 0x%X (%s)"), CMDTYPE_LABEL, CMD_LABEL );
        Runtime::GetLogger().DecIndentLevel();
        Runtime::GetLogger().Add(   _T("}") );

        return CMDTYPE_LABEL;
    }

    tstr S = Cmd;
    if (S.length() > 0)  ::CharUpper( (LPTSTR) S.c_str() );
  
    eCmdType nCmdType = CMDTYPE_UNKNOWN;
    for (int j = 0; j < CMD_COUNT; j++)
    {
        const TCHAR* cmd_str = CmdTypes[j].szCmd;
        if (S.StartsWith(cmd_str))
        {
            int i = lstrlen(cmd_str);

            const TCHAR    next_ch = S.GetAt(i);
            const eCmdType cmd_type = CmdTypes[j].nCmdType;

            if ( IsTabSpaceOrEmptyChar(next_ch) || 
                 ((cmd_type == CMDTYPE_CD) && 
                  (next_ch == _T('\\') || next_ch == _T('/') || next_ch == _T('.'))) )
            {
                nCmdType = cmd_type;
                Cmd.Delete(0, i);

                Runtime::GetLogger().AddEx( _T("[ret] 0x%X (%s)"), cmd_type, cmd_str );

                break;
            }
        }
    }
  
    if (nCmdType == CMDTYPE_UNKNOWN)
    {
        Runtime::GetLogger().AddEx( _T("[ret] 0x%X (unknown)"), nCmdType );
    }
    Runtime::GetLogger().DecIndentLevel();
    Runtime::GetLogger().Add(   _T("}") );
  
    return nCmdType;
}

CScriptEngine::eCmdType CScriptEngine::modifyCommandLine(tstr& Cmd)
{
    Runtime::GetLogger().Add(   _T("ModifyCommandLine()") );
    Runtime::GetLogger().Add(   _T("{") );
    Runtime::GetLogger().IncIndentLevel();
    Runtime::GetLogger().AddEx( _T("[in]  \"%s\""), Cmd.c_str() );
    
    if ( isCommentOrEmpty(Cmd) )
    {
    
        Runtime::GetLogger().Add(   _T("; it\'s a comment or empty string") );
        Runtime::GetLogger().Add(   _T("; command argument(s):") );
        Runtime::GetLogger().AddEx( _T("[out] \"%s\""), Cmd.c_str() );
        Runtime::GetLogger().Add(   _T("; command type:") );
        Runtime::GetLogger().AddEx( _T("[ret] %d"), CMDTYPE_COMMENT_OR_EMPTY );
        Runtime::GetLogger().DecIndentLevel();
        Runtime::GetLogger().Add(   _T("}") );
      
        return CMDTYPE_COMMENT_OR_EMPTY;
    }
  
    // ... checking commands ...

    const eCmdType nCmdType = getCmdType(Cmd);
    
    if ( nCmdType == CMDTYPE_COMMENT_OR_EMPTY )
        return nCmdType;

    DelLeadingTabSpaces(Cmd);
    if ( Cmd.IsEmpty() || (nCmdType == CMDTYPE_CLS) )
    {
    
        Runtime::GetLogger().Add(   _T("; no arguments given") );
        Runtime::GetLogger().Add(   _T("; command argument(s):") );
        Runtime::GetLogger().AddEx( _T("[out] \"%s\""), Cmd.c_str() );
        Runtime::GetLogger().Add(   _T("; command type:") );
        Runtime::GetLogger().AddEx( _T("[ret] 0x%X"), nCmdType );
        Runtime::GetLogger().DecIndentLevel();
        Runtime::GetLogger().Add(   _T("}") );
      
        return nCmdType;
    }
  
    if ( (nCmdType != CMDTYPE_NPPSENDMSG) &&
         (nCmdType != CMDTYPE_SCISENDMSG) &&
         (nCmdType != CMDTYPE_NPPSENDMSGEX) &&
         (nCmdType != CMDTYPE_NPECMDALIAS) &&
         (nCmdType != CMDTYPE_CONFILTER) )
    {
        // ... checking script's arguments ...
      
        CStrSplitT<TCHAR> args;
        Runtime::GetNppExec().GetMacroVars().CheckCmdArgs(Cmd, args);

        // ... checking notepad++'s macro-variables ...

        Runtime::GetNppExec().GetMacroVars().CheckNppMacroVars(Cmd);

        // ... checking plugin's macro-variables ...

        Runtime::GetNppExec().GetMacroVars().CheckPluginMacroVars(Cmd);

        // ... checking user's macro-variables ...

        Runtime::GetNppExec().GetMacroVars().CheckUserMacroVars(Cmd, nCmdType);

        // ... checking empty macro-variables ...
      
        if (nCmdType != CMDTYPE_UNSET) // required for 'unset $(var)'
            Runtime::GetNppExec().GetMacroVars().CheckEmptyMacroVars(Cmd, nCmdType);
    }
    // we have to process macro-vars inside doSendMsg()
    // because macro-var's string may contain double-quotes
  
    // ... do we need "" around the command's argument? ...
    
    bool bDone = false;
    if ( (nCmdType == CMDTYPE_NPECMDALIAS) ||
         (nCmdType == CMDTYPE_NPPSENDMSG) ||
         (nCmdType == CMDTYPE_SCISENDMSG) ||
         (nCmdType == CMDTYPE_NPPSENDMSGEX) ||
         (nCmdType == CMDTYPE_CONCOLOUR) ||
         (nCmdType == CMDTYPE_CONFILTER) ||
         (nCmdType == CMDTYPE_IF) || 
         (nCmdType == CMDTYPE_GOTO) )
    {
        bDone = true;
    }
    else if ( (nCmdType != 0) &&
              (nCmdType != CMDTYPE_NPPRUN) &&
              (nCmdType != CMDTYPE_NPPEXEC) &&
              (nCmdType != CMDTYPE_ECHO) &&
              (nCmdType != CMDTYPE_SET) &&
              (nCmdType != CMDTYPE_SELSAVETO) &&
              (nCmdType != CMDTYPE_INPUTBOX) )
    {
        if (Cmd.GetAt(0) == _T('\"'))
            Cmd.Delete(0, 1);
    
        int i = Cmd.length() - 1;
        if (Cmd.GetAt(i) == _T('\"'))
            Cmd.Delete(i);
    
        bDone = true; // we don't need '\"' in file_name
    }

    // ... adding '\"' to the command if it's needed ...
    if ( !bDone )
    {
        bool bHasSpaces = false;
        // disabled by default  because of problems 
        // for executables without extension i.e. 
        // "cmd /c app.exe"  <-- "cmd" is without extension
        if (Runtime::GetOptions().GetBool(OPTB_PATH_AUTODBLQUOTES))
        {
            if (!bDone && (Cmd.GetAt(0) != _T('\"')))
            {
                int i = 0;
                int j = 0;
                while (!bDone && (i < Cmd.length()))
                {
                    if (Cmd[i] == _T(' '))
                    {
                        bHasSpaces = true;
                        j = i - 1;
                        while (!bDone && j >= 0)
                        {
                            const TCHAR ch = Cmd[j];
                            if (ch == _T('.'))
                            {
                                Cmd.Insert(i, _T('\"'));
                                Cmd.Insert(0, _T('\"'));
                                bDone = true;
                            }
                            else if (ch == _T('\\') || ch == _T('/'))
                            {
                                j = 0; // j-- makes j<0 so this loop is over
                            }
                            j--;
                        }
                    }
                    i++;
                }
            }
        }
    }
    
    Runtime::GetLogger().Add(   _T("; command argument(s):") );
    Runtime::GetLogger().AddEx( _T("[out] \"%s\""), Cmd.c_str() );
    Runtime::GetLogger().Add(   _T("; command type:") );
    Runtime::GetLogger().AddEx( _T("[ret] 0x%X"), nCmdType );
    Runtime::GetLogger().DecIndentLevel();
    Runtime::GetLogger().Add(   _T("}") );
  
    return nCmdType;
}

int CScriptEngine::getOnOffParam(const tstr& param)
{
    if ( param.IsEmpty() )
        return PARAM_EMPTY; // no param
    
    if ( param == _T("1") )
        return PARAM_ON;

    if ( param == _T("0") )
        return PARAM_OFF;

    if ( param == _T("?") )
        return PARAM_KEEP;

    if ( param == _T("+") )
        return PARAM_ENABLE;

    if ( param == _T("-") )
        return PARAM_DISABLE;

    tstr S = param;
    ::CharUpper( S.c_str() );

    if ( (S == _T("ON")) || (S == _T("TRUE")) )
        return PARAM_ON;

    if ( (S == _T("OFF")) || (S == _T("FALSE")) )
        return PARAM_OFF;

    if ( S == _T("KEEP") )
        return PARAM_KEEP;

    if ( S == _T("ENABLE") )
        return PARAM_ENABLE;

    if ( S == _T("DISABLE") )
        return PARAM_DISABLE;

    return PARAM_UNKNOWN; // unknown
}

bool CScriptEngine::isCommentOrEmpty(tstr& Cmd)
{
    const tstr comment = Runtime::GetOptions().GetStr(OPTS_COMMENTDELIMITER);
    if (comment.length() > 0)
    {
        int i = Cmd.Find(comment.c_str()); // comment
        if ((i >= 0) && 
            ((comment != _T("//")) || (Cmd.GetAt(i-1) != _T(':')))) // skip :// e.g. http://
        {
            Cmd.Delete(i, -1); // delete all after "//"

            Runtime::GetLogger().AddEx( _T("; comment removed: everything after %s"), comment.c_str() );

        }
    }
  
    DelLeadingTabSpaces(Cmd);
    DelTrailingTabSpaces(Cmd);

    return Cmd.IsEmpty();
}

void CScriptEngine::errorCmdNoParam(const TCHAR* cszCmd)
{
    tstr Err = cszCmd;
    Err += _T(':');

    Runtime::GetLogger().Add(   _T("; argument(s) expected, but none given") );
          
    m_pNppExec->GetConsole().PrintMessage( Err.c_str() );
    m_pNppExec->GetConsole().PrintError( _T("- empty command") );
    
    m_nCmdType = CMDTYPE_UNKNOWN;
}

void CScriptEngine::messageConsole(const TCHAR* cszCmd, const TCHAR* cszParams)
{
    tstr S = cszCmd;
    S += _T(": ");
    S += cszParams;
    m_pNppExec->GetConsole().PrintMessage( S.c_str() );
}

bool CScriptEngine::reportCmdAndParams(const TCHAR* cszCmd, const tstr& params, unsigned int uFlags)
{
    if ( params.IsEmpty() )
    {
        if ( uFlags & fReportEmptyParam )
            errorCmdNoParam(cszCmd);

        if ( uFlags & fFailIfEmptyParam )
            return false;
    }
    
    Runtime::GetLogger().AddEx( _T("; executing %s %s"), cszCmd, params.c_str() );

    if ( uFlags & fMessageToConsole )
    {
        if ( params.length() > MAX_VAR_LENGTH2SHOW )
        {
            tstr S;

            S.Append( params.c_str(), MAX_VAR_LENGTH2SHOW - 5 );
            S += _T("(...)");
            messageConsole( cszCmd, S.c_str() );
        }
        else
        {
            messageConsole( cszCmd, params.c_str() );
        }
    }

    return true;
}

void CScriptEngine::updateFocus()
{
    if ( (!m_pNppExec->m_hFocusedWindowBeforeScriptStarted) ||
         (m_pNppExec->m_hFocusedWindowBeforeScriptStarted == m_pNppExec->GetConsole().GetConsoleWnd()) ||
         (m_pNppExec->m_hFocusedWindowBeforeScriptStarted == m_pNppExec->GetConsole().GetParentWnd()) ||
         (m_pNppExec->m_hFocusedWindowBeforeScriptStarted == m_pNppExec->GetScintillaHandle()) )
    {
        ::SendMessage( m_pNppExec->GetScintillaHandle(), WM_SETFOCUS, 0, 0 );
        m_pNppExec->m_hFocusedWindowBeforeScriptStarted = m_pNppExec->GetScintillaHandle();
    }
}

CScriptEngine::eCmdResult CScriptEngine::Do(const tstr& params)
{
    if ( params.IsEmpty() )
    {
        return CMDRESULT_FAILED;
    }

    eCmdResult nCmdResult = CMDRESULT_FAILED;

    Runtime::GetLogger().AddEx( _T("; starting child process \"%s\""), params.c_str() );

    m_pNppExec->m_strChildProcessOutput.Clear();

    m_pNppExec->GetConsole().PrintMessage( params.c_str() );
    lstrcpy( m_pNppExec->_consoleCmdLine, params.c_str() );
    if ( m_pNppExec->CreateChildProcess(
           m_pNppExec->GetConsole().GetParentWnd(), 
           m_pNppExec->_consoleCmdLine
         ) )
    // this function does not return until child process exists
    {
        nCmdResult = CMDRESULT_SUCCEEDED;
    }

    Runtime::GetLogger().Add(   _T("; child process finished") );

    TCHAR szExitCode[50];
    c_base::_tint2str(m_pNppExec->m_nChildProcessExitCode, szExitCode);

    tstr varName = MACRO_EXITCODE;
    m_pNppExec->GetMacroVars().SetUserMacroVar( varName, szExitCode );

    if ( Runtime::GetOptions().GetBool(OPTB_CONSOLE_SETOUTPUTVAR) )
    {
        tstr& OutputVar = m_pNppExec->m_strChildProcessOutput;
        if ( OutputVar.GetLastChar() == _T('\n') )
            OutputVar.SetSize(OutputVar.length() - 1);
        if ( OutputVar.GetFirstChar() == _T('\n') )
            OutputVar.Delete(0, 1);
        
        // $(OUTPUT)
        varName = MACRO_OUTPUT;
        m_pNppExec->GetMacroVars().SetUserMacroVar( varName, OutputVar );

        // $(OUTPUTL)
        varName = MACRO_OUTPUTL;

        int i = OutputVar.length() - 1;
        while ( (i >= 0) && (OutputVar[i] != _T('\n')) )  --i;
        if ( i >= 0 )
        {
            tstr varValue;

            ++i;
            varValue.Copy( OutputVar.c_str() + i, OutputVar.length() - i );
            m_pNppExec->GetMacroVars().SetUserMacroVar( varName, varValue );
        }
        else
            m_pNppExec->GetMacroVars().SetUserMacroVar( varName, OutputVar );

        // $(OUTPUT1)
        varName = MACRO_OUTPUT1;

        i = OutputVar.Find( _T('\n') );
        if ( i >= 0 )
        {
            tstr varValue;

            varValue.Copy( OutputVar.c_str(), i );
            m_pNppExec->GetMacroVars().SetUserMacroVar( varName, varValue );
        }
        else
            m_pNppExec->GetMacroVars().SetUserMacroVar( varName, OutputVar );
    }
    m_pNppExec->m_strChildProcessOutput.Clear();

    return nCmdResult;
}

CScriptEngine::eCmdResult CScriptEngine::DoCd(const tstr& params)
{
    TCHAR szPath[FILEPATH_BUFSIZE];
    eCmdResult nCmdResult = CMDRESULT_SUCCEEDED;

    reportCmdAndParams( CMD_CD, params, fMessageToConsole );

    // changing current directory

    if ( params.length() > 0 )
    {
        if ( ((params[0] == _T('\\')) && (params.GetAt(1) != _T('\\'))) || 
            ((params[0] == _T('/')) && (params.GetAt(1) != _T('/'))) )
        {
            // root directory of current drive e.g. 'C:\', 'D:\' etc.
            GetCurrentDirectory( FILEPATH_BUFSIZE - 1, szPath );
            if ( szPath[1] == _T(':') )
            {
                szPath[2] = _T('\\');
                szPath[3] = 0;
              
                Runtime::GetLogger().AddEx( _T("; changed to \"%s%s\""), szPath, params.c_str() + 1 );
              
                nCmdResult = SetCurrentDirectory( szPath ) ? CMDRESULT_SUCCEEDED : CMDRESULT_FAILED;
                if ( params[1] )
                {
                    nCmdResult = SetCurrentDirectory( params.c_str() + 1 ) ? CMDRESULT_SUCCEEDED : CMDRESULT_FAILED;
                }
            }
        }
        else
        {
            int ofs = 0;

            if ( (params[1] == _T(':')) && 
                 (params[2] == _T('\\') || params[2] == _T('/')) && 
                 (params[3] != 0) )
            {
                // changing the drive
                szPath[0] = params[0];
                szPath[1] = params[1];
                szPath[2] = 0;
                nCmdResult = SetCurrentDirectory( szPath ) ? CMDRESULT_SUCCEEDED : CMDRESULT_FAILED;
                ofs = 2;
            }
            // set current directory
            nCmdResult = SetCurrentDirectory(params.c_str() + ofs) ? CMDRESULT_SUCCEEDED : CMDRESULT_FAILED;
            if ( nCmdResult == CMDRESULT_FAILED )
            {
                // trying to set the parent directory...
                for ( int i = params.length() - 1; i > ofs; i-- )
                {
                    if ( params[i] == _T('\\') || params[i] == _T('/') )
                    {
                        lstrcpyn( szPath, params.c_str() + ofs, i - ofs + 1 );
                        SetCurrentDirectory( szPath );
                        break;
                    }
                }
            }
        }
    }
    GetCurrentDirectory( FILEPATH_BUFSIZE - 1, szPath );
    tstr S = _T("Current directory: ");
    S += szPath;
    m_pNppExec->GetConsole().PrintMessage( S.c_str(), false );

    return CMDRESULT_DONE;
}

CScriptEngine::eCmdResult CScriptEngine::DoCls(const tstr& params)
{
    m_pNppExec->GetConsole().ClearText();

    return CMDRESULT_SUCCEEDED;
}

static BOOL getColorFromStr(const TCHAR* szColor, COLORREF* pColor)
{
    if ( szColor && *szColor )
    {
        c_base::byte_t bt[4];

        int n = c_base::_thexstr2buf( szColor, bt, 4 );
        if ( n >= 3 )
        {
            *pColor = RGB( bt[0], bt[1], bt[2] );
            return TRUE;
        }

        if ( n <= 1 )
        {
            if ( szColor[0] == _T('0') && szColor[1] == 0 )
            {
                *pColor = 0; // handle "0" as correct default value
                return TRUE;
            }
        }
    }

    return FALSE;
}

CScriptEngine::eCmdResult CScriptEngine::DoConColour(const tstr& params)
{
    reportCmdAndParams( CMD_CONCOLOUR, params, fMessageToConsole );

    eCmdResult nCmdResult = CMDRESULT_SUCCEEDED;

    if ( params.length() > 0 )
    {
        tstr colorParams = params;
        ::CharUpper(colorParams.c_str());

        const int posFG = colorParams.Find(_T("FG"));
        const int posBG = colorParams.Find(_T("BG"));

        if ( posFG >= 0 )
        {
            tstr colorFG;
            int n = (posBG > posFG) ? (posBG - posFG) : (colorParams.length() - posFG);
            
            colorFG.Append(colorParams.c_str() + posFG, n);
            n = colorFG.Find(_T('='));
            if ( n >= 0 )
            {
                COLORREF color;

                colorFG.Delete(0, n + 1);
                DelLeadingTabSpaces(colorFG);
                DelTrailingTabSpaces(colorFG);
                if ( (!colorFG.IsEmpty()) && getColorFromStr(colorFG.c_str(), &color) )
                {
                    m_pNppExec->GetConsole().setCurrentColorTextNorm(color);
                }
                else
                {
                    m_pNppExec->GetConsole().PrintError( _T("- incorrect value of \'FG\'") );
                    nCmdResult = CMDRESULT_FAILED;
                }
            }
            else
            {
                m_pNppExec->GetConsole().PrintError( _T("- \'FG\' found, but no value specified") );
                nCmdResult = CMDRESULT_FAILED;
            }
        }

        if ( posBG >= 0 )
        {
            tstr colorBG;
            int n = (posFG > posBG) ? (posFG - posBG) : (colorParams.length() - posBG);

            colorBG.Append(colorParams.c_str() + posBG, n);
            n = colorBG.Find(_T('='));
            if ( n >= 0 )
            {
                COLORREF color;

                colorBG.Delete(0, n + 1);
                DelLeadingTabSpaces(colorBG);
                DelTrailingTabSpaces(colorBG);
                if ( (!colorBG.IsEmpty()) && getColorFromStr(colorBG.c_str(), &color) )
                {
                    m_pNppExec->GetConsole().setCurrentColorBkgnd(color);
                    m_pNppExec->GetConsole().UpdateColours();
                }
                else
                {
                    m_pNppExec->GetConsole().PrintError( _T("- incorrect value of \'BG\'") );
                    nCmdResult = CMDRESULT_FAILED;
                }
            }
            else
            {
                m_pNppExec->GetConsole().PrintError( _T("- \'BG\' found, but no value specified") );
                nCmdResult = CMDRESULT_FAILED;
            }
        }
    }
    else
    {
        tstr S;
        TCHAR buf[16];
        COLORREF color;

        color = m_pNppExec->GetConsole().getCurrentColorTextNorm();
        buf[0] = 0;
        c_base::_tbuf2hexstr( (const c_base::byte_t*) &color, 3, buf, 16, _T(" ") );
        S = _T("Foreground (text) colour:  ");
        S += buf;
        m_pNppExec->GetConsole().PrintMessage( S.c_str(), false );

        color = m_pNppExec->GetConsole().getCurrentColorBkgnd();
        buf[0] = 0;
        c_base::_tbuf2hexstr( (const c_base::byte_t*) &color, 3, buf, 16, _T(" ") );
        S = _T("Background colour:         ");
        S += buf;
        m_pNppExec->GetConsole().PrintMessage( S.c_str(), false );
    }

    return CMDRESULT_DONE;
}

CScriptEngine::eCmdResult CScriptEngine::DoConFilter(const tstr& params)
{
    if ( !reportCmdAndParams( CMD_CONFILTER, params, fMessageToConsole | fReportEmptyParam | fFailIfEmptyParam ) )
        return CMDRESULT_FAILED;

    // +(-)i1..5    Include
    // +(-)x1..5    eXclude
    // +(-)fr1..4   Find+Replace (case-insensitive)
    // +(-)frc1..4  Find+Replace (match case)
    // +(-)h1..10   Highlight

    enum eOptionState {
        stateUnknown = 0,
        stateOn,
        stateOff
    };

    enum eOptionType {
        typeUnknown = 0,
        typeInclude,
        typeExclude,
        typeFindReplaceIgnoreCase,
        typeFindReplaceMatchCase,
        typeHighlight
    };

    bool isHighlightChanged = false;

    tstr sOption;
    CStrSplitT<TCHAR> args;
    const int n = args.SplitToArgs(params);
    for ( int i = 0; i < n; i++ )
    {
        sOption = args.GetArg(i);
        ::CharUpper( sOption.c_str() );

        eOptionState state = stateUnknown;
        if ( sOption.GetAt(0) == _T('+') )
            state = stateOn;
        else if ( sOption.GetAt(0) == _T('-') )
            state = stateOff;

        if ( state != stateUnknown )
        {
            eOptionType type = typeUnknown;
            switch ( sOption.GetAt(1) )
            {
                case _T('I'):
                    type = typeInclude;
                    break;
                case _T('X'):
                    type = typeExclude;
                    break;
                case _T('F'):
                    if ( sOption.GetAt(2) == _T('R') )
                    {
                        if ( sOption.GetAt(3) == _T('C') )
                            type = typeFindReplaceMatchCase;
                        else
                            type = typeFindReplaceIgnoreCase;
                    }
                    break;
                case _T('H'):
                    type = typeHighlight;
                    break;
            }

            if ( type != typeUnknown )
            {
                const TCHAR* pIndex = sOption.c_str();
                if ( type == typeFindReplaceMatchCase )
                    pIndex += 4; // skip "+FRC"
                else if ( type == typeFindReplaceIgnoreCase )
                    pIndex += 3; // skip "+FR"
                else
                    pIndex += 2; // skip "+I" or "+X" or "+H"

                const int nIndex = c_base::_tstr2int(pIndex) - 1;
                const bool bEnable = (state == stateOn) ? true : false;
                switch ( type )
                {
                    case typeInclude:
                        if ( nIndex >= 0 && nIndex < CConsoleOutputFilterDlg::FILTER_ITEMS )
                        {
                            const int nIndexMask = (0x01 << nIndex);
                            int nConFltrInclMask = Runtime::GetOptions().GetInt(OPTI_CONFLTR_INCLMASK);
                            nConFltrInclMask |= nIndexMask;
                            if ( !bEnable )  nConFltrInclMask ^= nIndexMask;
                            Runtime::GetOptions().SetInt(OPTI_CONFLTR_INCLMASK, nConFltrInclMask);
                            if ( bEnable )  Runtime::GetOptions().SetBool(OPTB_CONFLTR_ENABLE, true);
                        }
                        break;
                    case typeExclude:
                        if ( nIndex >= 0 && nIndex < CConsoleOutputFilterDlg::FILTER_ITEMS )
                        {
                            const int nIndexMask = (0x01 << nIndex);
                            int nConFltrExclMask = Runtime::GetOptions().GetInt(OPTI_CONFLTR_EXCLMASK);
                            nConFltrExclMask |= nIndexMask;
                            if ( !bEnable )  nConFltrExclMask ^= nIndexMask;
                            Runtime::GetOptions().SetInt(OPTI_CONFLTR_EXCLMASK, nConFltrExclMask);
                            if ( bEnable )  Runtime::GetOptions().SetBool(OPTB_CONFLTR_ENABLE, true);
                        }
                        break;
                    case typeFindReplaceIgnoreCase:
                    case typeFindReplaceMatchCase:
                        if ( nIndex >= 0 && nIndex < CConsoleOutputFilterDlg::REPLACE_ITEMS )
                        {
                            const int nIndexMask = (0x01 << nIndex);
                            const bool bMatchCase = (type == typeFindReplaceMatchCase) ? true : false;
                            int nRplcFltrFindMask = Runtime::GetOptions().GetInt(OPTI_CONFLTR_R_FINDMASK);
                            nRplcFltrFindMask |= nIndexMask;
                            if ( !bEnable )  nRplcFltrFindMask ^= nIndexMask;
                            Runtime::GetOptions().SetInt(OPTI_CONFLTR_R_FINDMASK, nRplcFltrFindMask);
                            int nRplcFltrCaseMask = Runtime::GetOptions().GetInt(OPTI_CONFLTR_R_CASEMASK);
                            nRplcFltrCaseMask |= nIndexMask;
                            if ( !bMatchCase )  nRplcFltrCaseMask ^= nIndexMask;
                            Runtime::GetOptions().SetInt(OPTI_CONFLTR_R_CASEMASK, nRplcFltrCaseMask);
                            if ( bEnable )  Runtime::GetOptions().SetBool(OPTB_CONFLTR_R_ENABLE, true);
                        }
                        break;
                    case typeHighlight:
                        if ( nIndex >= 0 && nIndex < CConsoleOutputFilterDlg::RECOGNITION_ITEMS )
                        {
                            Runtime::GetWarningAnalyzer().EnableEffect(nIndex, bEnable);
                            isHighlightChanged = true;
                        }
                        break;
                }
            }
        }
    }

    if ( isHighlightChanged )
    {
        m_pNppExec->UpdateGoToErrorMenuItem();
    }

    return CMDRESULT_DONE;
}

CScriptEngine::eCmdResult CScriptEngine::DoConLoadFrom(const tstr& params)
{
    if ( !reportCmdAndParams( CMD_CONLOADFROM, params, fMessageToConsole | fReportEmptyParam | fFailIfEmptyParam ) )
        return CMDRESULT_FAILED;

    int nBytes = m_pNppExec->conLoadFrom(params.c_str());
    if ( nBytes < 0 )
    {
        m_pNppExec->GetConsole().PrintError( _T("- can not open the file or it\'s empty") );
        return CMDRESULT_FAILED;
    }

    return CMDRESULT_DONE;
}

CScriptEngine::eCmdResult CScriptEngine::DoConSaveTo(const tstr& params)
{
    if ( !reportCmdAndParams( CMD_CONSAVETO, params, fMessageToConsole | fReportEmptyParam | fFailIfEmptyParam ) )
        return CMDRESULT_FAILED;
        
    tstr S;
    int  nBytes = m_pNppExec->conSaveTo(params.c_str());
    if ( nBytes >= 0 )
    {
        TCHAR num[32];
        wsprintf( num, _T("%ld"), nBytes );
        S = _T("- OK, ");
        S += num;
        S += _T(" bytes have been written to \"");
    }
    else
    {
      S = _T("- failed to write to \"");
    }
    S += params.c_str();
    S += _T("\"");
    if ( nBytes >= 0 )
    {
        m_pNppExec->GetConsole().PrintMessage( S.c_str() );
    }
    else
    {
        m_pNppExec->GetConsole().PrintError( S.c_str() );
        return CMDRESULT_FAILED;
    }

    return CMDRESULT_DONE;
}

CScriptEngine::eCmdResult CScriptEngine::DoDir(const tstr& params)
{
    const TCHAR* cszPathAndFilter = params.IsEmpty() ? _T("*") : params.c_str();
    reportCmdAndParams( CMD_DIR, cszPathAndFilter, fMessageToConsole );

    tstr Path;
    tstr Filter;
    int  nFilterPos = FileFilterPos( cszPathAndFilter );
    
    GetPathAndFilter( cszPathAndFilter, nFilterPos, Path, Filter );

    Runtime::GetLogger().AddEx( _T("; searching \"%s\" for \"%s\""), Path.c_str(), Filter.c_str() );

    PrintDirContent( m_pNppExec, Path.c_str(), Filter.c_str() );

    return CMDRESULT_DONE;
}

CScriptEngine::eCmdResult CScriptEngine::DoEcho(const tstr& params)
{
    reportCmdAndParams( CMD_ECHO, params, 0 );
     
    m_pNppExec->GetConsole().PrintMessage( params.c_str(), false );

    return CMDRESULT_DONE;
}

CScriptEngine::eCmdResult CScriptEngine::DoElse(const tstr& params)
{
    ScriptContext& currentScript = m_execState.GetCurrentScriptContext();
    const eIfState ifState = currentScript.GetIfState();

    unsigned int uFlags = 0;
    if ( ifState == IF_NONE || ifState == IF_WANT_ELSE || ifState == IF_MAYBE_ELSE )
        uFlags |= fMessageToConsole;
    reportCmdAndParams( CMD_ELSE, params, uFlags );
    
    if ( ifState == IF_NONE )
    {
        m_pNppExec->GetConsole().PrintError( _T("- unexpected ELSE found, without preceding IF") );
        return CMDRESULT_FAILED;
    }
    else if ( ifState == IF_EXECUTING )
    {

        Runtime::GetLogger().Add(   _T("; IF ... ELSE found, skipping everything up to ENDIF") );

        currentScript.SetIfState(IF_WANT_ENDIF);
    }
    else if ( ifState == IF_WANT_ELSE || ifState == IF_MAYBE_ELSE )
    {

        Runtime::GetLogger().Add( _T("; ELSE found") );

        // TODO: check the condition
        if ( params.IsEmpty() )
        {
            // ELSE without condition
            currentScript.SetIfState(IF_EXECUTING);
        }
        else
        {
            // ELSE with condition
            tstr paramsUpperCase = params;
            ::CharUpper( paramsUpperCase.c_str() );

            const int numIf = 2;
            static const TCHAR* arrIf[numIf] = {
                _T("IF "),
                _T("IF\t"),
            };

            int n = -1;
            for ( int i = 0; i < numIf && n < 0; i++ )
            {
                n = paramsUpperCase.RFind( arrIf[i] );
            }

            if ( n < 0 )
            {
                m_pNppExec->GetConsole().PrintError( _T("- ELSE IF expected, but IF was not found") );
                return CMDRESULT_FAILED;
            }

            tstr ifParams;
            ifParams.Copy( params.c_str() + n + 3 );
            DelLeadingTabSpaces(ifParams);
            DelTrailingTabSpaces(ifParams);

            DoIf(ifParams);
        }
    }

    return CMDRESULT_SUCCEEDED;
}

CScriptEngine::eCmdResult CScriptEngine::DoEndIf(const tstr& params)
{
    reportCmdAndParams( CMD_ENDIF, params, fMessageToConsole );

    if ( !params.IsEmpty() )
    {
        m_pNppExec->GetConsole().PrintError( _T("- unexpected parameter(s)") );
        return CMDRESULT_FAILED;
    }

    ScriptContext& currentScript = m_execState.GetCurrentScriptContext();
    const eIfState ifState = currentScript.GetIfState();
    if ( ifState == IF_NONE )
    {
        m_pNppExec->GetConsole().PrintError( _T("- unexpected ENDIF found, without preceding IF") );
        return CMDRESULT_FAILED;
    }
    else
    {

        Runtime::GetLogger().Add( _T("; IF ... ENDIF found, done") );

        currentScript.PopIfState();
    }

    return CMDRESULT_SUCCEEDED;
}

CScriptEngine::eCmdResult CScriptEngine::DoEnvSet(const tstr& params)
{
    if ( !reportCmdAndParams( CMD_ENVSET, params, fMessageToConsole | fReportEmptyParam | fFailIfEmptyParam ) )
        return CMDRESULT_FAILED;

    bool isInternal = false;
    
    CStrSplitT<TCHAR> args;
    if ( args.Split(params, _T("="), 2) == 2 )
    {
        isInternal = true;
        
        // set the value
        tstr& varName = args.Arg(0);
        tstr& varValue = args.Arg(1);
        
        DelLeadingTabSpaces(varName);
        DelTrailingTabSpaces(varName);
        DelLeadingTabSpaces(varValue);
        DelTrailingTabSpaces(varValue);

        if ( varName.length() > 0 )
        {
            ::CharUpper( varName.c_str() );

            if ( g_LocalEnvVarNames.find(varName) == g_LocalEnvVarNames.end() )
            {
                // not Local Env Var; maybe Global?
                tEnvVars::const_iterator itrGVar = g_GlobalEnvVars.find(varName);
                if ( itrGVar == g_GlobalEnvVars.end() )
                {
                    // not in the Global Env Vars List; but maybe it's Global?
                    DWORD nLen = GetEnvironmentVariable(varName.c_str(), NULL, 0);
                    if ( nLen > 0 )
                    {
                        // is Global Env Var
                        TCHAR* pStr = new TCHAR[nLen + 2];
                        if ( pStr )
                        {
                            nLen = GetEnvironmentVariable(varName.c_str(), pStr, nLen + 1);
                            if (nLen > 0)
                            {
                                g_GlobalEnvVars[varName] = pStr;
                                itrGVar = g_GlobalEnvVars.find(varName);
                            }
                            delete [] pStr;
                        }
                    }
                    if ( itrGVar == g_GlobalEnvVars.end() )
                    {
                        // not Global Env Var; add to the Local
                        g_LocalEnvVarNames[varName] = 0;
                    }
                }
            }
            SetEnvironmentVariable( varName.c_str(), varValue.c_str() );
        }
        else
        {
            m_pNppExec->GetConsole().PrintError( _T("- no variable name specified") );
            return CMDRESULT_FAILED;
        }
    }

    // show the value
    tstr& varName = args.Arg(0);
        
    DelLeadingTabSpaces(varName);
    DelTrailingTabSpaces(varName);

    if ( varName.length() > 0 )
    {
        ::CharUpper( varName.c_str() );
            
        bool  bSysVarOK = false;
        DWORD nLen = GetEnvironmentVariable(varName.c_str(), NULL, 0);
        if ( nLen > 0 )
        {
            TCHAR* pStr = new TCHAR[nLen + 2];
            if ( pStr )
            {
                nLen = GetEnvironmentVariable(varName.c_str(), pStr, nLen + 1);
                if ( nLen > 0 )
                {
                    tstr S = _T("$(SYS.");
                    S += varName;
                    S += _T(") = ");
                    if ( nLen > MAX_VAR_LENGTH2SHOW )
                    {
                        S.Append( pStr, MAX_VAR_LENGTH2SHOW - 5 );
                        S += _T("(...)");
                    }
                    else
                    {
                        S += pStr;
                    }
                    m_pNppExec->GetConsole().PrintMessage( S.c_str(), isInternal );
                    bSysVarOK = true;
                }
                delete [] pStr;
            }
        }
        if ( !bSysVarOK )
        {
            tstr S = _T("$(SYS.");
            S += varName;
            S += _T(") is empty or does not exist");
            m_pNppExec->GetConsole().PrintMessage( S.c_str(), false );
        }
    }
    else
    {
        m_pNppExec->GetConsole().PrintError( _T("- no variable name specified") );
        return CMDRESULT_FAILED;
    }

    return CMDRESULT_DONE;
}

CScriptEngine::eCmdResult CScriptEngine::DoEnvUnset(const tstr& params)
{
    if ( !reportCmdAndParams( CMD_ENVUNSET, params, fMessageToConsole | fReportEmptyParam | fFailIfEmptyParam ) )
        return CMDRESULT_FAILED;

    // removes the value of environment variable, restores the initial value

    tstr varName = params;
    
    DelLeadingTabSpaces(varName);
    DelTrailingTabSpaces(varName);

    if ( varName.length() > 0 )
    {
        ::CharUpper( varName.c_str() );

        BOOL bResult = FALSE;
        BOOL bRemoved = FALSE;
        tEnvVars::iterator itrLVar = g_LocalEnvVarNames.find(varName);
        if ( itrLVar != g_LocalEnvVarNames.end() )
        {
            // is Local Env Var; remove
            bResult = SetEnvironmentVariable( varName.c_str(), NULL );
            g_LocalEnvVarNames.erase(itrLVar);
            bRemoved = TRUE;
        }
        else
        {
            // not Local Env Var; maybe Global?
            tEnvVars::iterator itrGVar = g_GlobalEnvVars.find(varName);
            if ( itrGVar != g_GlobalEnvVars.end() )
            {
                // is modified Global Env Var; restore the initial value
                bResult = SetEnvironmentVariable( varName.c_str(), itrGVar->second.c_str() );
                g_GlobalEnvVars.erase(itrGVar);
            }
            else
            {
                // unsafe: maybe non-modified Global Env Var
            }
        }

        tstr S = _T("$(SYS.");
        S += varName;
        if ( bResult )
        {
            S += _T(") has been ");
            S += (bRemoved ? _T("removed") : _T("restored"));
        }
        else
        {
            S += _T(")");
            S.Insert(0, _T("- can not unset this environment variable: "));
        }
        m_pNppExec->GetConsole().PrintMessage( S.c_str(), bResult ? true : false );
    }
    else
    {
        m_pNppExec->GetConsole().PrintError( _T("- no variable name specified") );
        return CMDRESULT_FAILED;
    }

    return CMDRESULT_DONE;
}

CScriptEngine::eCmdResult CScriptEngine::DoGoTo(const tstr& params)
{
    if ( !reportCmdAndParams( CMD_GOTO, params, fMessageToConsole | fReportEmptyParam | fFailIfEmptyParam ) )
        return CMDRESULT_FAILED;

    tstr labelName = params;
    DelLeadingTabSpaces(labelName);
    DelTrailingTabSpaces(labelName);
    ::CharUpper(labelName.c_str());

    if ( labelName.IsEmpty() )
    {
        m_pNppExec->GetConsole().PrintError( _T("- label name is empty") );
        return CMDRESULT_FAILED;
    }

    ScriptContext& currentScript = m_execState.GetCurrentScriptContext();
    tLabels& Labels = currentScript.Labels;
    tLabels::const_iterator itrLabel = Labels.find(labelName);
    if ( itrLabel == Labels.end() )
    {
        Runtime::GetLogger().Activate(false);

        tstr Cmd;
        CListItemT<tstr>* p = m_execState.pScriptLineCurrent->GetNext();
        while ( p && (p != currentScript.CmdRange.pEnd) )
        {
            Cmd = p->GetItem();
            if ( getCmdType(Cmd) == CMDTYPE_LABEL )
            {
                DelLeadingTabSpaces(Cmd);
                DelTrailingTabSpaces(Cmd);
                ::CharUpper(Cmd.c_str());
                if ( Labels.find(Cmd) == Labels.end() )
                {
                    Labels[Cmd] = p->GetNext(); // label points to the next command
                }
                if ( Cmd == labelName )
                {
                    itrLabel = Labels.find(labelName);
                    break;
                }
            }
            p = p->GetNext();
        }

        Runtime::GetLogger().Activate(true);

        if ( itrLabel == Labels.end() )
        {
            tstr err = _T("- no label found: ");
            err += labelName;
            m_pNppExec->GetConsole().PrintError( err.c_str() );
            return CMDRESULT_FAILED;
        }
    }

    bool bContinue = true;

    m_execState.nGoToCounter++;
    if (m_execState.nGoToCounter > m_execState.nGoToMaxCount)
    {
        TCHAR szMsg[500];

        m_execState.nGoToCounter = 0;
        bContinue = false;
        ::wsprintf(szMsg, 
            _T("GOTO was performed more than %ld times.\n") \
            _T("Abort execution of this script?\n") \
            _T("(Press Yes to abort or No to continue execution)"),
            m_execState.nGoToMaxCount
        );
        if (::MessageBox(m_pNppExec->m_nppData._nppHandle, szMsg, 
                _T("NppExec Warning: Possible infinite loop"),
                  MB_YESNO | MB_ICONWARNING) == IDNO)
        {
            bContinue = true;
        }
    }

    if ( bContinue )
    {
        m_execState.pScriptLineNext = itrLabel->second;

        Runtime::GetLogger().AddEx( _T("Jumping to command item p = 0x%X { \"%s\" }"),
            m_execState.pScriptLineNext, 
            m_execState.pScriptLineNext ? m_execState.pScriptLineNext->GetItem().c_str() : _T("<NULL>") );

    }
    else
    {
        m_execState.pScriptLineNext = NULL;

        Runtime::GetLogger().Add(  _T("; Script execution aborted by user") );

    }

    return CMDRESULT_DONE;
}

enum eDecNumberType {
  DNT_NOTNUMBER = 0,
  DNT_INTNUMBER,
  DNT_FLOATNUMBER
};

static eDecNumberType isPureDecNumber(const TCHAR* s)
{
    if ( s )
    {
        if ( (*s == _T('-')) || (*s == _T('+')) )
        {
            ++s;
        }

        if ( !isDecNumChar(*s) )
            return DNT_NOTNUMBER; // we need at least one numeric char

        while ( isDecNumChar( *(++s) ) ) ;
        
        if ( *s == 0 )
            return DNT_INTNUMBER; // OK, we are at the end of string

        // end of floating number's mantissa...
        if ( *s == _T('.') )
        {
            while ( isDecNumChar( *(++s) ) ) ;

            if ( *s == 0 )
                return DNT_FLOATNUMBER; // e.g. "123." or "123.45"
        }

        // check for floating number's exponent...
        if ( (*s == _T('e')) || (*s == _T('E')) )
        {
            ++s;
            if ( (*s == _T('-')) || (*s == _T('+')) )
            {
                ++s;
            }

            while ( isDecNumChar(*s) ) 
            {
                if ( *(++s) == 0 )
                    return DNT_FLOATNUMBER; // OK, we are at the end of string
            }
        }
    }
    return DNT_NOTNUMBER;
}

enum eCondType {
    COND_NONE = 0,
    COND_EQUAL,
    COND_NOTEQUAL,
    COND_LESSTHAN,
    COND_LESSEQUAL,
    COND_GREATTHAN,
    COND_GREATEQUAL,
    COND_EQUALNOCASE
};

template<typename T> class OperandComparator
{
    public:
        OperandComparator(const T& t1, const T& t2) : m_t1(t1), m_t2(t2)
        {
        }

        bool compare(int condType)
        {
            bool ret = false;
            switch ( condType )
            {
                case COND_EQUAL:       ret = eq(); break;
                case COND_NOTEQUAL:    ret = ne(); break;
                case COND_LESSTHAN:    ret = lt(); break;
                case COND_LESSEQUAL:   ret = le(); break;
                case COND_GREATTHAN:   ret = gt(); break;
                case COND_GREATEQUAL:  ret = ge(); break;
                case COND_EQUALNOCASE: ret = eq_i(); break;
            }
            return ret;
        }

        bool eq() const { return (m_t1 == m_t2); }
        bool ne() const { return (m_t1 != m_t2); }
        bool lt() const { return (m_t1 < m_t2); }
        bool le() const { return (m_t1 <= m_t2); }
        bool gt() const { return (m_t1 > m_t2); }
        bool ge() const { return (m_t1 >= m_t2); }
        bool eq_i() const { return (m_t1 == m_t2); }

    protected:
        const T& m_t1;
        const T& m_t2;
};

template<> bool OperandComparator<tstr>::eq_i() const
{
    return ( ::CompareString(
        LOCALE_USER_DEFAULT, NORM_IGNORECASE, 
        m_t1.c_str(), m_t1.length(), 
        m_t2.c_str(), m_t2.length() ) == CSTR_EQUAL );
}

class CondOperand
{
    public:
        CondOperand(const tstr& s) : m_value_dbl(0), m_value_int(0), m_value_str(s)
        {
            m_type = isPureDecNumber(s.c_str());
            if ( m_type == DNT_INTNUMBER )
            {
                m_value_int = c_base::_tstr2int(s.c_str());
                m_value_dbl = m_value_int; // to be able to compare as double
            }
            else if ( m_type == DNT_FLOATNUMBER )
            {
                m_value_dbl = _t_str2f(s.c_str());
            }
        }

        const TCHAR* type_as_str() const
        {
            switch ( type() )
            {
                case DNT_NOTNUMBER:
                    return _T("str");
                case DNT_INTNUMBER:
                    return _T("int");
                case DNT_FLOATNUMBER:
                    return _T("dbl");
            }
            return _T("");
        }

        inline eDecNumberType type() const { return m_type; }
        inline const tstr& value_str() const { return m_value_str; }
        inline double value_dbl() const { return m_value_dbl; }
        inline int value_int() const { return m_value_int; }

    protected:
        tstr   m_value_str;
        double m_value_dbl;
        int    m_value_int;
        eDecNumberType m_type;
};

static bool IsConditionTrue(const tstr& Condition, bool* pHasSyntaxError)
{
    if ( pHasSyntaxError )  *pHasSyntaxError = false;

    CStrSplitT<TCHAR> args;

    typedef struct sCond {
        const TCHAR* szCond;
        int          nCondType;
    } tCond;

    const int numCond = 9;
    static const tCond arrCond[numCond] = {
        { _T("=="), COND_EQUAL       },
        { _T("!="), COND_NOTEQUAL    },
        { _T("<>"), COND_NOTEQUAL    },
        { _T(">="), COND_GREATEQUAL  },
        { _T("<="), COND_LESSEQUAL   },
        { _T("~="), COND_EQUALNOCASE },
        { _T("="),  COND_EQUAL       },
        { _T("<"),  COND_LESSTHAN    },
        { _T(">"),  COND_GREATTHAN   }
    };

    int  condType = COND_NONE;
    tstr cond;
    tstr op1;
    tstr op2;

    if ( args.SplitToArgs(Condition) == 3 )
    {
        cond = args.GetArg(1);
        op1 = args.GetArg(0);
        op2 = args.GetArg(2);

        for ( int i = 0; i < numCond && condType == COND_NONE; i++ )
        {
            if ( cond == arrCond[i].szCond )
            {
                condType = arrCond[i].nCondType;
            }
        }

        if ( condType != COND_NONE )
        {
            // restore removed quotes, if any
            int n = Condition.Find(op1);
            if ( Condition.GetAt(n - 1) == _T('\"') )
            {
                op1.Insert(0, _T('\"'));
                op1 += _T('\"');
            }

            n = Condition.RFind(op2);
            if ( Condition.GetAt(n - 1) == _T('\"') )
            {
                op2.Insert(0, _T('\"'));
                op2 += _T('\"');
            }
        }
    }
    else
    {
        for ( int i = 0; i < numCond && condType == COND_NONE; i++ )
        {
            int n = Condition.Find(arrCond[i].szCond);
            if ( n >= 0 )
            {
                condType = arrCond[i].nCondType;
                cond = arrCond[i].szCond;
                op1.Copy(Condition.c_str(), n);
                op2.Copy(Condition.c_str() + n + lstrlen(arrCond[i].szCond));

                DelLeadingTabSpaces(op1);
                DelTrailingTabSpaces(op1);

                DelLeadingTabSpaces(op2);
                DelTrailingTabSpaces(op2);
            }
        }
    }

    bool ret = false;

    if ( condType != COND_NONE )
    {

        Runtime::GetLogger().Add(   _T("IsConditionTrue()") );
        Runtime::GetLogger().Add(   _T("{") );
        Runtime::GetLogger().IncIndentLevel();

        // As the original quotes around op1 and op2 are kept (if any),
        // we treat "123" as a string and 123 as a number
        CondOperand co1(op1);
        CondOperand co2(op2);

        Runtime::GetLogger().AddEx( _T("op1 (%s) :  %s"), co1.type_as_str(), op1.c_str() );
        Runtime::GetLogger().AddEx( _T("op2 (%s) :  %s"), co2.type_as_str(), op2.c_str() );
        Runtime::GetLogger().AddEx( _T("condition :  %s"), cond.c_str() );

        if ( (co1.type() == DNT_NOTNUMBER) || (co2.type() == DNT_NOTNUMBER) )
        {
            // compare as string values
            ret = OperandComparator<tstr>(co1.value_str(), co2.value_str()).compare(condType);
        }
        else if ( (co1.type() == DNT_FLOATNUMBER) || (co2.type() == DNT_FLOATNUMBER) )
        {
            // compare as floating-point values
            ret = OperandComparator<double>(co1.value_dbl(), co2.value_dbl()).compare(condType);
        }
        else
        {
            // compare as integer values
            ret = OperandComparator<int>(co1.value_int(), co2.value_int()).compare(condType);
        }

        Runtime::GetLogger().DecIndentLevel();
        Runtime::GetLogger().Add(   _T("}") );

    }
    else
    {
        if ( pHasSyntaxError )  *pHasSyntaxError = true;
        ret = false;
    }

    return ret;
}

CScriptEngine::eCmdResult CScriptEngine::DoIf(const tstr& params)
{
    enum eIfMode {
        IF_GOTO = 0,
        IF_THEN
    };

    if ( !reportCmdAndParams( CMD_IF, params, fMessageToConsole | fReportEmptyParam | fFailIfEmptyParam ) )
        return CMDRESULT_FAILED;

    tstr paramsUpperCase = params;
    ::CharUpper( paramsUpperCase.c_str() );

    const int numGoTo = 4;
    static const TCHAR* arrGoTo[numGoTo] = {
        _T(" GOTO "),
        _T("\tGOTO\t"),
        _T("\tGOTO "),
        _T(" GOTO\t")
    };

    const int numThen = 2;
    static const TCHAR* arrThen[numThen] = {
        _T(" THEN"),
        _T("\tTHEN")
    };

    int mode = IF_GOTO;
    int n = -1;
    for ( int i = 0; i < numGoTo && n < 0; i++ )
    {
        n = paramsUpperCase.RFind( arrGoTo[i] );
    }

    if ( n < 0 )
    {
        if ( paramsUpperCase.EndsWith(_T(" GOTO")) || paramsUpperCase.EndsWith(_T("\tGOTO")) )
        {
            m_pNppExec->GetConsole().PrintError( _T("- empty goto-label specified") );
            return CMDRESULT_FAILED;
        }

        mode = IF_THEN;
        for ( int i = 0; i < numGoTo && n < 0; i++ )
        {
            n = paramsUpperCase.RFind( arrThen[i] );
        }

        if ( n < 0 )
            Runtime::GetLogger().Add(   _T("- no \"goto\" or \"then\" specified, assuming \"then\"") );
    }

    tstr ifCondition;
    ifCondition.Copy( params.c_str(), n );
    DelLeadingTabSpaces(ifCondition);
    DelTrailingTabSpaces(ifCondition);

    if ( ifCondition.IsEmpty() )
    {
        m_pNppExec->GetConsole().PrintError( _T("- empty if-condition specified") );
        return CMDRESULT_FAILED;
    }

    tstr labelName;

    if ( mode == IF_GOTO )
    {
        labelName.Copy( params.c_str() + n + 6 );
        DelLeadingTabSpaces(labelName);
        DelTrailingTabSpaces(labelName);
        ::CharUpper(labelName.c_str());

        if ( labelName.IsEmpty() )
        {
            m_pNppExec->GetConsole().PrintError( _T("- empty goto-label specified") );
            return CMDRESULT_FAILED;
        }
    }

    bool hasSyntaxError = false;
    bool isConditionOK = IsConditionTrue(ifCondition, &hasSyntaxError);

    if ( hasSyntaxError )
    {
        m_pNppExec->GetConsole().PrintError( _T("- syntax error in the if-condition") );
        return CMDRESULT_FAILED;
    }

    ScriptContext& currentScript = m_execState.GetCurrentScriptContext();
    if ( mode == IF_GOTO )
    {
        if ( isConditionOK )
        {

            Runtime::GetLogger().Add(   _T("The condition is true.") );

            DoGoTo(labelName);
        }
        else
        {

            Runtime::GetLogger().Add(   _T("The condition is false; proceeding to next line") );

            currentScript.PushIfState(IF_MAYBE_ELSE); // ELSE may appear after IF ... GOTO
        }
    }
    else // IF_THEN
    {
        if ( isConditionOK )
        {

            Runtime::GetLogger().Add(   _T("The condition is true; executing lines under the IF") );

            currentScript.PushIfState(IF_EXECUTING);
        }
        else
        {

            Runtime::GetLogger().Add(   _T("The condition is false; waiting for ELSE or ENDIF") );

            currentScript.PushIfState(IF_WANT_ELSE);
        }
    }

    return CMDRESULT_DONE;
}

CScriptEngine::eCmdResult CScriptEngine::DoInputBox(const tstr& params)
{
    if ( !reportCmdAndParams( CMD_INPUTBOX, params, fMessageToConsole | fReportEmptyParam | fFailIfEmptyParam ) )
        return CMDRESULT_FAILED;

    runInputBox(m_pNppExec, params.c_str());

    return CMDRESULT_DONE;
}

CScriptEngine::eCmdResult CScriptEngine::DoLabel(const tstr& params)
{
    if ( !reportCmdAndParams( CMD_LABEL, params, fReportEmptyParam | fFailIfEmptyParam ) )
        return CMDRESULT_FAILED;

    tstr labelName = params;
    DelLeadingTabSpaces(labelName);
    DelTrailingTabSpaces(labelName);
    ::CharUpper(labelName.c_str());

    ScriptContext& currentScript = m_execState.GetCurrentScriptContext();
    tLabels& Labels = currentScript.Labels;
    tLabels::const_iterator itrLabel = Labels.find(labelName);
    if ( itrLabel != Labels.end() )
    {
        if ( itrLabel->second != m_execState.pScriptLineCurrent->GetNext() )
        {
            tstr err = _T("- duplicated label found: ");
            err += labelName;
            m_pNppExec->GetConsole().PrintError( err.c_str() );
            return CMDRESULT_FAILED;
        }
    }

    CListItemT<tstr>* p = m_execState.pScriptLineCurrent->GetNext();
    Labels[labelName] = p; // label points to the next command

    Runtime::GetLogger().AddEx( _T("; label %s -> command item p = 0x%X { \"%s\" }"), 
        labelName.c_str(), p, p ? p->GetItem().c_str() : _T("<NULL>") );

    return CMDRESULT_DONE;
}

static void appendOnOff(bool bOn, tstr& S1, tstr& S2)
{
    if ( bOn )
    {
        S1 += _T("+");
        S2 += _T("on");
    }
    else
    {
        S1 += _T("-");
        S2 += _T("off");
    }
}

typedef struct sIntMapping {
    int n;
    const TCHAR* str;
} tIntMapping;

static void appendInt(int n, tstr& S1, tstr& S2, const tIntMapping* mappings)
{
    TCHAR num[16];

    wsprintf( num, _T("%u"), n );
    S1 += num;
    for ( ; mappings->str != NULL; ++mappings )
    {
        if ( mappings->n == n )
        {
            S2 += mappings->str;
            break;
        }
    }
}

static void appendEnc(unsigned int enc_opt, bool bInput, tstr& S1, tstr& S2)
{
    unsigned int enc;
    const TCHAR* encName;
    TCHAR        encNum[5];

    if ( bInput )
    {
        enc = CConsoleEncodingDlg::getInputEncoding(enc_opt);
        encName = CConsoleEncodingDlg::getInputEncodingName(enc_opt);
    }
    else
    {
        enc = CConsoleEncodingDlg::getOutputEncoding(enc_opt);
        encName = CConsoleEncodingDlg::getOutputEncodingName(enc_opt);
    }

    wsprintf( encNum, _T("%u"), enc );

    S1 += encNum;
    S2 += encName;
}

CScriptEngine::eCmdResult CScriptEngine::DoNpeCmdAlias(const tstr& params)
{
    reportCmdAndParams( CMD_NPECMDALIAS, params, fMessageToConsole );

    tstr aliasName;
    tstr aliasValue;
    tstr S;

    CNppExecMacroVars::tMacroVars& cmdAliases = m_pNppExec->GetMacroVars().GetCmdAliases();

    if ( params.IsEmpty() )
    {

        Runtime::GetLogger().Add(   _T("; no arguments given - showing all command aliases") );

    }
    else
    {
        CStrSplitT<TCHAR> args;

        if ( args.Split(params, _T("="), 2) == 2 )
        {
            // set the value
            aliasName = args.Arg(0);
            aliasValue = args.Arg(1);

            DelLeadingTabSpaces(aliasName);
            DelTrailingTabSpaces(aliasName);
            DelLeadingTabSpaces(aliasValue);
            DelTrailingTabSpaces(aliasValue);

            if ( aliasName.IsEmpty() )
            {
                // in case of 'params' is a string which starts with "="
                if ( args.Split(aliasValue, _T("="), 2) == 2 )
                {
                    aliasName = _T("=");
                    aliasName += args.Arg(0);
                    aliasValue = args.Arg(1);

                    DelTrailingTabSpaces(aliasName);
                    DelLeadingTabSpaces(aliasValue);
                }
            }
            
            if ( aliasName.length() > 0 )
            {
                ::CharUpper( aliasName.c_str() );

                CNppExecMacroVars::tMacroVars::iterator itrAlias = cmdAliases.find(aliasName);
                if ( itrAlias != cmdAliases.end() )
                {
                    if ( aliasValue.length() > 0 )
                    {
                        // update
                        itrAlias->second = aliasValue;
                        
                        S = aliasName;
                        S += _T(" -> ");
                        S += aliasValue;
                    }
                    else
                    {
                        // remove
                        cmdAliases.erase(itrAlias);

                        S = _T("- command alias has been removed: ");
                        S += aliasName;
                    }

                    m_pNppExec->GetConsole().PrintMessage( S.c_str() );
                }
                else
                {
                    if ( aliasValue.length() > 0 )
                    {
                        // add
                        cmdAliases[aliasName] = aliasValue;

                        S = aliasName;
                        S += _T(" -> ");
                        S += aliasValue;
                        m_pNppExec->GetConsole().PrintMessage( S.c_str() );
                        /*
                        else
                        {
                            m_pNppExec->GetConsole().PrintError( _T("- failed to add new command alias") );
                        }
                        */
                    }
                    else
                    {
                        // tried to remove non-existent command alias
                        S = _T("- no such command alias: ");
                        S += aliasName;
                        m_pNppExec->GetConsole().PrintMessage( S.c_str(), false );
                    }
                }

                HMENU hMenu = m_pNppExec->GetNppMainMenu();
                if ( hMenu )
                {
                    const bool bEnable = !cmdAliases.empty();
                    ::EnableMenuItem( hMenu, g_funcItem[N_NOCMDALIASES]._cmdID,
                        MF_BYCOMMAND | (bEnable ? MF_ENABLED : MF_GRAYED) );
                }
                
                return CMDRESULT_DONE;
            }
            else
            {
                if ( aliasValue.length() > 0 )
                {
                    m_pNppExec->GetConsole().PrintError( _T("- no command alias name specified") );
                    return CMDRESULT_FAILED;
                }
                else
                {
                    aliasName = _T("=");
                }
            }
        }
        else
        {
            aliasName = args.Arg(0);
            DelLeadingTabSpaces(aliasName);
            DelTrailingTabSpaces(aliasName);

            if ( aliasName.length() > 0 )
            {
                ::CharUpper( aliasName.c_str() );
            }
        }
        
    }

    if ( cmdAliases.empty() )
    {
        m_pNppExec->GetConsole().PrintMessage( _T("- no command aliases"), false );
    }
    else
    {
        if ( aliasName.IsEmpty() )
        {
            CNppExecMacroVars::tMacroVars::const_iterator itrAlias = cmdAliases.begin();
            for ( ; itrAlias != cmdAliases.end(); ++itrAlias )
            {
                S = itrAlias->first;
                S += _T(" -> ");
                S += itrAlias->second;
                m_pNppExec->GetConsole().PrintMessage( S.c_str(), false );
            }
        }
        else
        {
            bool bFound = false;

            CNppExecMacroVars::tMacroVars::const_iterator itrAlias = cmdAliases.find(aliasName);
            if ( itrAlias != cmdAliases.end() )
            {
                S = itrAlias->first;
                S += _T(" -> ");
                S += itrAlias->second;
                m_pNppExec->GetConsole().PrintMessage( S.c_str(), false );
                bFound = true;
            }

            if ( !bFound )
            {
                tstr t = _T("- no such command alias: ");
                t += aliasName;
                m_pNppExec->GetConsole().PrintMessage( t.c_str(), false );
            }
        }
    }

    return CMDRESULT_DONE;
}

CScriptEngine::eCmdResult CScriptEngine::DoNpeConsole(const tstr& params)
{
    reportCmdAndParams( CMD_NPECONSOLE, params, fMessageToConsole );

    bool isSilent = false;
    
    if ( params.length() > 0 )
    {
        CStrSplitT<TCHAR> args;
        int n = args.SplitToArgs(params);
        if ( n > 0 )
        {
            bool isEncChanged = false;
            bool isFilterChanged = false;
            
            for ( int i = 0; i < n; i++ )
            {
                bool isOK = false;
                tstr arg = args.GetArg(i);
                if ( arg.length() == 2 )
                {
                    // a+/a-     append mode on/off
                    // d+/d-     follow current directory on/off
                    // h+/h-     console commands history on/off
                    // m+/m-     console internal messages on/off
                    // q+/q-     command aliases on/off
                    // v+/v-     set the $(OUTPUT) variable on/off
                    // f+/f-     console output filter on/off
                    // r+/r-     console output replace filter on/off
                    // k0..3     catch NppExec's shortcut keys on/off
                    // o0/o1/o2  console output encoding: ANSI/OEM/UTF8
                    // i0/i1/i2  console input encoding: ANSI/OEM/UTF8
                    // --        silent (don't print Console mode info)

                    switch ( arg[0] )
                    {
                        case _T('a'):
                        case _T('A'):
                            if ( arg[1] == _T('+') || arg[1] == _T('-') )
                            {
                                bool bOn = ( arg[1] == _T('+') );
                                Runtime::GetOptions().SetBool(OPTB_CONSOLE_APPENDMODE, bOn);
                                isOK = true;
                            }
                            break;

                        case _T('d'):
                        case _T('D'):
                            if ( arg[1] == _T('+') || arg[1] == _T('-') )
                            {
                                bool bOn = ( arg[1] == _T('+') );
                                HMENU hMenu = m_pNppExec->GetNppMainMenu();
                                if ( hMenu )
                                {
                                    ::CheckMenuItem(hMenu, g_funcItem[N_CDCURDIR]._cmdID,
                                        MF_BYCOMMAND | (bOn ? MF_CHECKED : MF_UNCHECKED));
                                }
                                Runtime::GetOptions().SetBool(OPTB_CONSOLE_CDCURDIR, bOn);
                                isOK = true;
                            }
                            break;

                        case _T('h'):
                        case _T('H'):
                            if ( arg[1] == _T('+') || arg[1] == _T('-') )
                            {
                                bool bOn = ( arg[1] == _T('+') );
                                HMENU hMenu = m_pNppExec->GetNppMainMenu();
                                if ( hMenu )
                                {
                                    ::CheckMenuItem(hMenu, g_funcItem[N_CMDHISTORY]._cmdID,
                                        MF_BYCOMMAND | (bOn ? MF_CHECKED : MF_UNCHECKED));
                                }
                                Runtime::GetOptions().SetBool(OPTB_CONSOLE_CMDHISTORY, bOn);
                                isOK = true;
                            }
                            break;

                        case _T('m'):
                        case _T('M'):
                            if ( arg[1] == _T('+') || arg[1] == _T('-') )
                            {
                                bool bOn = ( arg[1] == _T('-') ); // inverse
                                HMENU hMenu = m_pNppExec->GetNppMainMenu();
                                if ( hMenu )
                                {
                                    ::CheckMenuItem(hMenu, g_funcItem[N_NOINTMSGS]._cmdID,
                                        MF_BYCOMMAND | (bOn ? MF_CHECKED : MF_UNCHECKED));
                                }
                                Runtime::GetOptions().SetBool(OPTB_CONSOLE_NOINTMSGS, bOn);
                                isOK = true;
                            }
                            break;

                        case _T('o'):
                        case _T('O'):
                            {
                                int enc = (int) (arg[1] - _T('0'));
                                if ( (enc >= CConsoleEncodingDlg::ENC_ANSI) &&
                                     (enc < CConsoleEncodingDlg::ENC_TOTAL) )
                                {
                                    unsigned int enc_opt = Runtime::GetOptions().GetUint(OPTU_CONSOLE_ENCODING);
                                    enc_opt &= 0xFFF0;
                                    enc_opt |= enc;
                                    Runtime::GetOptions().SetUint(OPTU_CONSOLE_ENCODING, enc_opt);
                                    isEncChanged = true;
                                    isOK = true;
                                }
                            }
                            break;

                        case _T('i'):
                        case _T('I'):
                            {
                                int enc = (int) (arg[1] - _T('0'));
                                if ( (enc >= CConsoleEncodingDlg::ENC_ANSI) &&
                                     (enc < CConsoleEncodingDlg::ENC_TOTAL) )
                                {
                                    unsigned int enc_opt = Runtime::GetOptions().GetUint(OPTU_CONSOLE_ENCODING);
                                    enc_opt &= 0xFF0F;
                                    enc_opt |= (enc * 0x10);
                                    Runtime::GetOptions().SetUint(OPTU_CONSOLE_ENCODING, enc_opt);
                                    isEncChanged = true;
                                    isOK = true;
                                }
                            }
                            break;

                        case _T('q'):
                        case _T('Q'):
                            if ( arg[1] == _T('+') || arg[1] == _T('-') )
                            {
                                bool bOn = ( arg[1] == _T('-') ); // inverse
                                HMENU hMenu = m_pNppExec->GetNppMainMenu();
                                if ( hMenu )
                                {
                                    ::CheckMenuItem(hMenu, g_funcItem[N_NOCMDALIASES]._cmdID,
                                        MF_BYCOMMAND | (bOn ? MF_CHECKED : MF_UNCHECKED));
                                }
                                Runtime::GetOptions().SetBool(OPTB_CONSOLE_NOCMDALIASES, bOn);
                                isOK = true;
                            }
                            break;

                        case _T('v'):
                        case _T('V'):
                            if ( arg[1] == _T('+') || arg[1] == _T('-') )
                            {
                                bool bOn = ( arg[1] == _T('+') );
                                Runtime::GetOptions().SetBool(OPTB_CONSOLE_SETOUTPUTVAR, bOn);
                                isOK = true;
                            }
                            break;

                        case _T('f'):
                        case _T('F'):
                            if ( arg[1] == _T('+') || arg[1] == _T('-') )
                            {
                                bool bOn = ( arg[1] == _T('+') );
                                Runtime::GetOptions().SetBool(OPTB_CONFLTR_ENABLE, bOn);
                                isFilterChanged = true;
                                isOK = true;
                            }
                            break;

                        case _T('r'):
                        case _T('R'):
                            if ( arg[1] == _T('+') || arg[1] == _T('-') )
                            {
                                bool bOn = ( arg[1] == _T('+') );
                                Runtime::GetOptions().SetBool(OPTB_CONFLTR_R_ENABLE, bOn);
                                isFilterChanged = true;
                                isOK = true;
                            }
                            break;

                        case _T('k'):
                        case _T('K'):
                            {
                                unsigned int k = (unsigned int) (arg[1] - _T('0'));
                                if ( (k >= ConsoleDlg::CSK_OFF) &&
                                     (k <= ConsoleDlg::CSK_ALL) )
                                {
                                    Runtime::GetOptions().SetUint(OPTU_CONSOLE_CATCHSHORTCUTKEYS, k);
                                    isOK = true;
                                }
                            }
                            break;

                        case _T('-'):
                            if ( arg[1] == _T('-') )
                            {
                                isSilent = true;
                                isOK = true;
                            }
                            break;
                    }
                }
                if ( !isOK )
                {
                    arg.Insert( 0, _T("- unknown parameter: ") );
                    m_pNppExec->GetConsole().PrintError( arg.c_str() );
                }
            }

            if ( isEncChanged )
            {
                unsigned int enc_opt = Runtime::GetOptions().GetUint(OPTU_CONSOLE_ENCODING);
                if ( enc_opt > 0xFF )
                {
                    // "input as output" flag is set
                    unsigned int encIn  = CConsoleEncodingDlg::getInputEncoding(enc_opt);
                    unsigned int encOut = CConsoleEncodingDlg::getOutputEncoding(enc_opt);
                    if ( encIn != encOut )
                    {
                        // remove the "input as output" flag
                        Runtime::GetOptions().SetUint(OPTU_CONSOLE_ENCODING, enc_opt & 0xFF);
                    }
                }
                m_pNppExec->UpdateConsoleEncoding();
            }

            if ( isFilterChanged )
            {
                m_pNppExec->UpdateOutputFilterMenuItem();
            }
        }
    }

    if ( !isSilent )
    {
        tstr S1 = _T("Console mode: ");
        tstr S2 = _T("; ");
        // append
        S1 += _T("a");
        S2 += _T("append: ");
        appendOnOff( Runtime::GetOptions().GetBool(OPTB_CONSOLE_APPENDMODE), S1, S2 );
        // cd_curdir
        S1 += _T(" d");
        S2 += _T(", cd_curdir: ");
        appendOnOff( Runtime::GetOptions().GetBool(OPTB_CONSOLE_CDCURDIR), S1, S2 );
        // cmd_history
        S1 += _T(" h");
        S2 += _T(", cmd_history: ");
        appendOnOff( Runtime::GetOptions().GetBool(OPTB_CONSOLE_CMDHISTORY), S1, S2 );
        // int_msgs
        S1 += _T(" m");
        S2 += _T_RE_EOL _T("; int_msgs: ");
        appendOnOff( !Runtime::GetOptions().GetBool(OPTB_CONSOLE_NOINTMSGS), S1, S2 );
        // cmd_aliases
        S1 += _T(" q");
        S2 += _T(", cmd_aliases: ");
        appendOnOff( !Runtime::GetOptions().GetBool(OPTB_CONSOLE_NOCMDALIASES), S1, S2 );
        // $(OUTPUT)
        S1 += _T(" v");
        S2 += _T(", output_var: ");
        appendOnOff( Runtime::GetOptions().GetBool(OPTB_CONSOLE_SETOUTPUTVAR), S1, S2 );
        // filters
        S1 += _T(" f");
        S2 += _T_RE_EOL _T("; filter: ");
        appendOnOff( Runtime::GetOptions().GetBool(OPTB_CONFLTR_ENABLE), S1, S2 );
        S1 += _T(" r");
        S2 += _T(", replace_filter: ");
        appendOnOff( Runtime::GetOptions().GetBool(OPTB_CONFLTR_R_ENABLE), S1, S2 );
        // shortcut_keys
        S1 += _T(" k");
        S2 += _T_RE_EOL _T("; shortcut_keys: ");
        {
            const tIntMapping mappings[] = {
                { ConsoleDlg::CSK_OFF, _T("off") },
                { ConsoleDlg::CSK_STD, _T("std") },
                { ConsoleDlg::CSK_USR, _T("usr") },
                { ConsoleDlg::CSK_ALL, _T("std+usr") },
                { 0x00, NULL } // trailing element with .str=NULL
            };
            appendInt( Runtime::GetOptions().GetUint(OPTU_CONSOLE_CATCHSHORTCUTKEYS), S1, S2, mappings );
        }
        // out_enc
        unsigned int enc_opt = Runtime::GetOptions().GetUint(OPTU_CONSOLE_ENCODING);
        S1 += _T(" o");
        S2 += _T_RE_EOL _T("; out_enc: ");
        appendEnc( enc_opt, false, S1, S2 );
        // in_enc
        S1 += _T(" i");
        S2 += _T(", in_enc: ");
        appendEnc( enc_opt, true, S1, S2 );
        // finally...
        m_pNppExec->GetConsole().PrintMessage( S1.c_str(), false );
        m_pNppExec->GetConsole().PrintMessage( S2.c_str(), false );
    }

    return CMDRESULT_DONE;
}

CScriptEngine::eCmdResult CScriptEngine::DoNpeDebugLog(const tstr& params)
{
    reportCmdAndParams( CMD_NPEDEBUGLOG, params, fMessageToConsole );

    int nParam = getOnOffParam(params);
    if (nParam == PARAM_ON)
    {
        Runtime::GetOptions().SetBool(OPTB_NPE_DEBUGLOG, true);
        Runtime::GetLogger().SetOutputMode(true, CNppExec::printScriptString);
    }
    else if (nParam == PARAM_OFF)
    {
        Runtime::GetOptions().SetBool(OPTB_NPE_DEBUGLOG, false);
        Runtime::GetLogger().SetOutputMode(false);
    }
    m_pNppExec->GetConsole().PrintMessage( Runtime::GetOptions().GetBool(OPTB_NPE_DEBUGLOG) ? 
        _T("Debug Log is On (1)") : _T("Debug Log is Off (0)"), false );

    return CMDRESULT_DONE;
}

CScriptEngine::eCmdResult CScriptEngine::DoNpeNoEmptyVars(const tstr& params)
{
    reportCmdAndParams( CMD_NPENOEMPTYVARS, params, fMessageToConsole );

    int nParam = getOnOffParam(params);
    if (nParam == PARAM_ON)
    {
        Runtime::GetOptions().SetBool(OPTB_CONSOLE_NOEMPTYVARS, true);
    }
    else if (nParam == PARAM_OFF)
    {
        Runtime::GetOptions().SetBool(OPTB_CONSOLE_NOEMPTYVARS, false);
    }
    m_pNppExec->GetConsole().PrintMessage( Runtime::GetOptions().GetBool(OPTB_CONSOLE_NOEMPTYVARS) ? 
          _T("Replace empty (uninitialized) vars with empty str:  On (1)") : 
            _T("Replace empty (uninitialized) vars with empty str:  Off (0)"), false );

    return CMDRESULT_DONE;
}

CScriptEngine::eCmdResult CScriptEngine::DoNppClose(const tstr& params)
{
    TCHAR szFileName[FILEPATH_BUFSIZE];

    reportCmdAndParams( CMD_NPPCLOSE, params, 0 );

    bool bCurrentFile = params.IsEmpty();
    if ( bCurrentFile )
    {
        Runtime::GetLogger().Add(   _T("; no file name specified - closing current file") );

        // file name is not specified i.e. closing current file
        m_pNppExec->SendNppMsg(NPPM_GETFULLCURRENTPATH,
          (WPARAM) (FILEPATH_BUFSIZE - 1), (LPARAM) szFileName);
    }
    else
    {
        Runtime::GetLogger().Add(   _T("; retrieving full file name") );
    }

    messageConsole( CMD_NPPCLOSE, bCurrentFile ? szFileName : params.c_str() );

    if ( bCurrentFile || m_pNppExec->nppSwitchToDocument(params.c_str(), true) )
    {
        m_pNppExec->SendNppMsg(NPPM_MENUCOMMAND, 0, IDM_FILE_CLOSE);
    }
    else
        m_pNppExec->GetConsole().PrintError( _T("- no such file opened") );

    return CMDRESULT_DONE;
}

CScriptEngine::eCmdResult CScriptEngine::DoNppConsole(const tstr& params)
{
    reportCmdAndParams( CMD_NPPCONSOLE, params, 0 );

    switch ( getOnOffParam(params) )
    {
        case PARAM_EMPTY:
            errorCmdNoParam(CMD_NPPCONSOLE);
            break;

        case PARAM_ON:
            m_pNppExec->showConsoleDialog(false);
            break;

        case PARAM_OFF:
            //m_pNppExec->verifyConsoleDialogExists();
            if ( m_pNppExec->isConsoleDialogVisible() )
            {
                m_pNppExec->showConsoleDialog(true);
            }
            m_pNppExec->_consoleIsVisible = true;
            updateFocus();
            break;

        case PARAM_KEEP:
            //m_pNppExec->verifyConsoleDialogExists();
            if ( !m_pNppExec->isConsoleDialogVisible() )
            {
                m_pNppExec->_consoleIsVisible = true;
                updateFocus();
            }
            break;

        case PARAM_ENABLE:
            m_pNppExec->GetConsole().SetOutputEnabled(true);
            m_pNppExec->GetConsole().LockConsoleEndPos();
            break;

        case PARAM_DISABLE:
            m_pNppExec->GetConsole().LockConsoleEndPos();
            m_pNppExec->GetConsole().SetOutputEnabled(false);
            break;

        default:
            // show error
            m_pNppExec->showConsoleDialog(false);
            {
                tstr Err = _T("Unknown ");
                Err += CMD_NPPCONSOLE;
                Err += _T(" parameter: ");
                Err += params;
                m_pNppExec->GetConsole().PrintError( Err.c_str() );
            }
            break;
    }

    Runtime::GetLogger().AddEx( _T("; the Console is %s"), m_pNppExec->isConsoleDialogVisible() ? _T("On") : _T("Off") );

    return CMDRESULT_DONE;
}

CScriptEngine::eCmdResult CScriptEngine::DoNppExec(const tstr& params)
{
    if ( !reportCmdAndParams( CMD_NPPEXEC, params, fMessageToConsole | fReportEmptyParam | fFailIfEmptyParam ) )
        return CMDRESULT_FAILED;

    // inserting commands from a script or a file into m_pNppExec->m_CmdList
        
    CStrSplitT<TCHAR> args;
    args.SplitToArgs(params);

    Runtime::GetLogger().Add(   _T("; args") );
    Runtime::GetLogger().Add(   _T("; {") );
    for (int i = 0; i < args.GetArgCount(); i++)
    {
        Runtime::GetLogger().AddEx( _T("; %s[%d], \"%s\""), 
          Runtime::GetLogger().GetIndentStr(), i, args.GetArg(i).c_str() );
    }
    Runtime::GetLogger().Add(   _T("; }") );

    tstr scriptName = args.GetArg(0);
    if (scriptName.length() > 0)  ::CharUpper(scriptName.c_str());
    if (!m_execState.GetScriptContextItemPtr(scriptName))
    {        
        bool bContinue = true;
          
        m_execState.nExecCounter++;
        if (m_execState.nExecCounter > m_execState.nExecMaxCount)
        {
            TCHAR szMsg[500];

            m_execState.nExecCounter = 0;
            bContinue = false;
            ::wsprintf(szMsg, 
                _T("%s was performed more than %ld times.\n") \
                _T("Abort execution of this script?\n") \
                _T("(Press Yes to abort or No to continue execution)"),
                CMD_NPPEXEC,
                m_execState.nExecMaxCount
            );
            if (::MessageBox(m_pNppExec->m_nppData._nppHandle, szMsg, 
                    _T("NppExec Warning: Possible infinite loop"),
                      MB_YESNO | MB_ICONWARNING) == IDNO)
            {
                bContinue = true;
            }
        }

        if (bContinue)
        {  
            CFileBufT<TCHAR> fbuf;
            CNppScript       Script;
            tstr             line;
            ScriptContext    scriptContext;

            if (m_pNppExec->m_ScriptList.GetScript(args.GetArg(0), Script))
            {
              
                Runtime::GetLogger().AddEx( _T("; adding commands from the script \"%s\""), args.GetArg(0).c_str() );  
                
                scriptContext.ScriptName = scriptName;
                scriptContext.CmdRange.pBegin = m_execState.pScriptLineCurrent;
                scriptContext.CmdRange.pEnd = m_execState.pScriptLineCurrent->GetNext();
                scriptContext.IsNppExeced = true;

                int n = 0;
                CListItemT<tstr>* pline = m_execState.pScriptLineCurrent;
                CListItemT<tstr>* pscriptline = Script.GetFirst();
                while (pscriptline)
                {
                    //line = "";
                    line = pscriptline->GetItem();
                    if (line.length() > 0)
                    {
                        ++n;

                        Runtime::GetLogger().AddEx( _T("; + line %d:  %s"), n, line.c_str() );
                    
                        m_pNppExec->GetMacroVars().CheckCmdArgs(line, args);
                        pline = m_pNppExec->m_CmdList.Insert(pline, true, line);
                    }
                    pscriptline = pscriptline->GetNext();
                }

                if (n != 0)
                {
                    scriptContext.CmdRange.pBegin = scriptContext.CmdRange.pBegin->GetNext();
                    m_execState.ScriptContextList.Add(scriptContext);

                    Runtime::GetLogger().AddEx( _T("; script context added: { Name = \"%s\"; CmdRange = [0x%X, 0x%X) }"), 
                        scriptContext.ScriptName.c_str(), scriptContext.CmdRange.pBegin, scriptContext.CmdRange.pEnd ); 

                }
            }
            else 
            {
              
                Runtime::GetLogger().AddEx( _T("; there is no script with such name (\"%s\")"), args.GetArg(0).c_str() ); 
                Runtime::GetLogger().Add(   _T("; trying to load the script from a file") ); 
              
                if ( !c_base::_tmatch_mask(_T("*.exe"), args.GetArg(0).c_str()) )
                {
                    // not exe-file

                    tstr fileName = args.GetArg(0);

                    if ( fileName.length() > 1 )
                    {
                        m_pNppExec->nppConvertToFullPathName(fileName, true);
                    }

                    if (fbuf.LoadFromFile(fileName.c_str()))
                    {
                
                        Runtime::GetLogger().AddEx( _T("; loading the script from a file \"%s\""), fileName.c_str() );
                  
                        scriptContext.ScriptName = scriptName;
                        scriptContext.CmdRange.pBegin = m_execState.pScriptLineCurrent;
                        scriptContext.CmdRange.pEnd = m_execState.pScriptLineCurrent->GetNext();
                        scriptContext.IsNppExeced = true;

                        int n = 0;
                        CListItemT<tstr>* pline = m_execState.pScriptLineCurrent;
                        while (fbuf.GetLine(line) >= 0)
                        {
                            if (line.length() > 0)
                            {
                                ++n;
                    
                                Runtime::GetLogger().AddEx( _T("; + line %d:  %s"), n, line.c_str() );
                      
                                m_pNppExec->GetMacroVars().CheckCmdArgs(line, args);
                                pline = m_pNppExec->m_CmdList.Insert(pline, true, line);
                            }
                        }

                        if (n != 0)
                        {
                            scriptContext.CmdRange.pBegin = scriptContext.CmdRange.pBegin->GetNext();
                            m_execState.ScriptContextList.Add(scriptContext);

                            Runtime::GetLogger().AddEx( _T("; script context added: { Name = \"%s\"; CmdRange = [0x%X, 0x%X) }"), 
                                scriptContext.ScriptName.c_str(), scriptContext.CmdRange.pBegin, scriptContext.CmdRange.pEnd ); 

                        }
                    }
                    else
                    {
                
                        Runtime::GetLogger().Add(   _T("; there is no file with such name ") ); 
                  
                        m_pNppExec->GetConsole().PrintError( _T("- can not open specified file or it is empty") );
                    }
                }
                else
                {
                    // exe-file
                    m_pNppExec->GetConsole().PrintError( _T("- syntax error: script file expected, executable file given") );
                    m_pNppExec->GetConsole().PrintError( _T("; If you want to run an exe-file, don\'t use NPP_EXEC command!") );
                    m_pNppExec->GetConsole().PrintError( _T("; (type HELP NPP_EXEC in the Console window for details)") );
                    m_pNppExec->GetConsole().PrintError( _T("; To run program.exe type just") );
                    m_pNppExec->GetConsole().PrintError( _T(";     program.exe") );
                    m_pNppExec->GetConsole().PrintError( _T("; instead of") );
                    m_pNppExec->GetConsole().PrintError( _T(";     npp_exec program.exe") );
                }
            }
        }
        else
        {
            m_execState.pScriptLineNext = NULL;

            Runtime::GetLogger().Add(  _T("; Script execution aborted to prevent possible infinite loop") );

        }

    }
    else
    {
        Runtime::GetLogger().Add(   _T("; the script with the same name is being executed") );
            
        m_pNppExec->GetConsole().PrintError( _T("- can\'t exec the same file at the same time") );
    }

    return CMDRESULT_DONE;
}

CScriptEngine::eCmdResult CScriptEngine::DoNppMenuCommand(const tstr& params)
{
    if ( !reportCmdAndParams( CMD_NPPMENUCOMMAND, params, fMessageToConsole | fReportEmptyParam | fFailIfEmptyParam ) )
        return CMDRESULT_FAILED;

    tstr parsedItemPath, parsedSep;
    int menuItemId = m_pNppExec->nppGetMenuItemIdByName(params, parsedItemPath, parsedSep);
    if ( menuItemId > 0 )
    {

        Runtime::GetLogger().AddEx( _T("; menu item separator: %s"), 
            parsedSep.IsEmpty() ? _T("<none>") : parsedSep.c_str() );
        Runtime::GetLogger().AddEx( _T("; menu item found: id = %d"), menuItemId );

        m_pNppExec->SendNppMsg(NPPM_MENUCOMMAND, 0, menuItemId);
    }
    else
    {
        tstr err = _T("The menu item was not found. Parsed item path: ");
        if ( !parsedItemPath.IsEmpty() )
            err += parsedItemPath;
        else
            err += _T("<none>");

        Runtime::GetLogger().AddEx( _T("; menu item separator: %s"), 
            parsedSep.IsEmpty() ? _T("<none>") : parsedSep.c_str() );

        m_pNppExec->GetConsole().PrintError( err.c_str() );
    }

    return CMDRESULT_DONE;
}

CScriptEngine::eCmdResult CScriptEngine::DoNppOpen(const tstr& params)
{
    if ( !reportCmdAndParams( CMD_NPPOPEN, params, fMessageToConsole | fReportEmptyParam | fFailIfEmptyParam ) )
        return CMDRESULT_FAILED;

    // opening a file in Notepad++

    int nFilterPos = FileFilterPos( params.c_str() );
    if ( nFilterPos < 0 )
    {
      
      Runtime::GetLogger().Add(   _T("; direct file (path)name specified") );
        
      // direct file (path)name
      if (!m_pNppExec->SendNppMsg(NPPM_RELOADFILE, (WPARAM) FALSE, (LPARAM) params.c_str()))
      {
        m_pNppExec->SendNppMsg(NPPM_DOOPEN, (WPARAM) 0, (LPARAM) params.c_str());
      }
    }
    else
    {
          
        Runtime::GetLogger().Add(   _T("; file (path)name mask specified") );
            
        // file (path)name mask (e.g. "C:\Documents\*.txt")
        tstr         Path;
        tstr         Filter;
        CListT<tstr> FilesList;

        GetPathAndFilter( params.c_str(), nFilterPos, Path, Filter );

        Runtime::GetLogger().AddEx( _T("; searching \"%s\" for \"%s\""), Path.c_str(), Filter.c_str() );
          
        GetFilePathNamesList( Path.c_str(), Filter.c_str(), FilesList );

        tstr S;
        CListItemT<tstr>* ptr = FilesList.GetFirst();
        while ( ptr )
        {
            S = ptr->GetItem();
            if (!m_pNppExec->SendNppMsg(NPPM_RELOADFILE, (WPARAM) FALSE, (LPARAM) S.c_str()))
            {
                m_pNppExec->SendNppMsg(NPPM_DOOPEN, (WPARAM) 0, (LPARAM) S.c_str());
            }
            ptr = ptr->GetNext();
        }
    }

    return CMDRESULT_DONE;
}

CScriptEngine::eCmdResult CScriptEngine::DoNppRun(const tstr& params)
{
    if ( !reportCmdAndParams( CMD_NPPRUN, params, fMessageToConsole | fReportEmptyParam | fFailIfEmptyParam ) )
        return CMDRESULT_FAILED;

    // run a command
    

    //#ifdef UNICODE
    //    char szCmdLineA[FILEPATH_BUFSIZE];
    //    int len = WideCharToMultiByte(CP_ACP, 0, szCmdLine, -1, szCmdLineA, FILEPATH_BUFSIZE - 1, NULL, NULL);
    //    szCmdLineA[len] = 0;
    //    WinExec(szCmdLineA, SW_SHOWNORMAL);
    //#else
    //    WinExec(szCmdLine, SW_SHOWNORMAL);
    //#endif

    CStrSplitT<TCHAR> args;
    CStrT<TCHAR> arg1, arg2;
    args.SplitToArgs(params);
    if ( args.GetArgCount() > 1 )
    {
        if ( args.GetArgCount() == 2 )
        {
            if ( SearchPath(NULL, params.c_str(), NULL, 0, NULL, NULL) != 0 )
            {
                arg1 = params;
            }
            else
            {
                arg1 = args.Arg(0);
                arg2 = args.Arg(1);
            }
        }
        else
        {
            arg1 = args.Arg(0);
            for ( int j = 1; j < args.GetArgCount(); j++ )
            {
                bool bdq = false;
                const tstr& arg = args.Arg(j);
                if ( isFullPath(arg) || 
                     (arg.Find(_T('.')) >= 0) ||
                     (arg.Find(_T(' ')) >= 0) ||
                     (arg.Find(_T('\t')) >= 0) )
                {
                    bdq = true;
                }
                if ( arg2.length() > 0 )  arg2.Append( _T(' ') );
                if ( bdq )  arg2.Append( _T('\"') );
                arg2.Append( arg );
                if ( bdq )  arg2.Append( _T('\"') );
            }
        }
    }
    else
    {
        arg1 = args.Arg(0);
    }
        
    Runtime::GetLogger().Add(   _T("; ShellExecute() arguments") ); 
    Runtime::GetLogger().Add(   _T("; {") );
    Runtime::GetLogger().AddEx( _T("; %s[file]   %s"), Runtime::GetLogger().GetIndentStr(), arg1.c_str() );
    Runtime::GetLogger().AddEx( _T("; %s[params] %s"), Runtime::GetLogger().GetIndentStr(), arg2.c_str() );
    Runtime::GetLogger().Add(   _T("; }") );
    
    tstr S;
    int  nResult = (int) ShellExecute( NULL, _T("open"), arg1.c_str(), 
           (arg2.length() > 0) ? arg2.c_str() : NULL, NULL, SW_SHOWNORMAL );
    switch ( nResult )
    {
        case 0:
            S = _T("- the operating system is out of memory or resources");
            break;
        case ERROR_FILE_NOT_FOUND:
            S = _T("- the specified file was not found");
            break;
        case ERROR_PATH_NOT_FOUND:
            S = _T("- the specified path was not found");
            break;
        case ERROR_BAD_FORMAT:
            S = _T("- the .exe file is invalid (non-Microsoft Win32 .exe or error in .exe image)");
            break;
        case SE_ERR_ACCESSDENIED:
            S = _T("- the operating system denied access to the specified file");
            break;
        case SE_ERR_ASSOCINCOMPLETE:
            S = _T("- the file name association is incomplete or invalid");
            break; 
        case SE_ERR_DDEBUSY:
            S = _T("- the DDE transaction could not be completed because other DDE transactions were being processed");
            break;
        case SE_ERR_DDEFAIL:
            S = _T("- the DDE transaction failed");
            break;
        case SE_ERR_DDETIMEOUT:
            S = _T("- the DDE transaction could not be completed because the request timed out");
            break;
        case SE_ERR_DLLNOTFOUND:
            S = _T("- the specified dynamic-link library (DLL) was not found");
            break;
        case SE_ERR_NOASSOC:
            S = _T("- there is no application associated with the given file name extension");
            break;
        case SE_ERR_OOM:
            S = _T("- there was not enough memory to complete the operation");
            break;
        case SE_ERR_SHARE:
            S = _T("- a sharing violation occurred");
            break;
        default:
            S = _T("");
            break;
    }
    if ( S.length() > 0 )
    {
        m_pNppExec->GetConsole().PrintError( S.c_str() );
    }

    return CMDRESULT_DONE;
}

CScriptEngine::eCmdResult CScriptEngine::DoNppSave(const tstr& params)
{
    TCHAR szFileName[FILEPATH_BUFSIZE];

    reportCmdAndParams( CMD_NPPSAVE, params, 0 );
          
    // save a file
    bool bCurrentFile = params.IsEmpty();
    if ( bCurrentFile )
    {
          
        Runtime::GetLogger().Add(   _T("; no file name specified - saving current file") );
            
        // file name is not specified i.e. saving current file
        m_pNppExec->SendNppMsg( NPPM_GETFULLCURRENTPATH,
          (WPARAM) (FILEPATH_BUFSIZE - 1), (LPARAM) szFileName );
    }
    else
    {
          
        Runtime::GetLogger().Add(   _T("; retrieving full file name") );
            
        // file name is specified
        m_pNppExec->nppSwitchToDocument( params.c_str(), true );
    }
        
    messageConsole( CMD_NPPSAVE, bCurrentFile ? szFileName : params.c_str() );
    
    Runtime::GetLogger().Add(   _T("; saving") );

    m_pNppExec->SendNppMsg( NPPM_SAVECURRENTFILE, 0, 0 );

    return CMDRESULT_DONE;
}

CScriptEngine::eCmdResult CScriptEngine::DoNppSaveAs(const tstr& params)
{
    if ( !reportCmdAndParams( CMD_NPPSAVEAS, params, fMessageToConsole | fReportEmptyParam | fFailIfEmptyParam ) )
        return CMDRESULT_FAILED;

    Runtime::GetLogger().Add(   _T("; saving") );

    m_pNppExec->SendNppMsg( NPPM_SAVECURRENTFILEAS, 0, (LPARAM) params.c_str() );

    return CMDRESULT_DONE;
}

CScriptEngine::eCmdResult CScriptEngine::DoNppSaveAll(const tstr& params)
{
    Runtime::GetLogger().AddEx( _T("; executing %s"), CMD_NPPSAVEALL );
          
    m_pNppExec->GetConsole().PrintMessage(CMD_NPPSAVEALL);
    m_pNppExec->nppSaveAllFiles();

    return CMDRESULT_DONE;
}

CScriptEngine::eCmdResult CScriptEngine::DoNppSendMsg(const tstr& params)
{
    return doSendMsg(params, CMDTYPE_NPPSENDMSG);
}

CScriptEngine::eCmdResult CScriptEngine::DoNppSendMsgEx(const tstr& params)
{
    return doSendMsg(params, CMDTYPE_NPPSENDMSGEX);
}

CScriptEngine::eCmdResult CScriptEngine::DoSciSendMsg(const tstr& params)
{
    return doSendMsg(params, CMDTYPE_SCISENDMSG);
}

CScriptEngine::eCmdResult CScriptEngine::doSendMsg(const tstr& params, int cmdType)
{
    tstr cmdName;
    HWND hWnd = NULL;
    bool isMsgEx = false;
    
    switch ( cmdType )
    {
        case CMDTYPE_NPPSENDMSG:
            cmdName = CMD_NPPSENDMSG;
            hWnd = m_pNppExec->m_nppData._nppHandle;
            break;

        case CMDTYPE_SCISENDMSG:
            cmdName = CMD_SCISENDMSG;
            hWnd = m_pNppExec->GetScintillaHandle();
            break;

        case CMDTYPE_NPPSENDMSGEX:
            cmdName = CMD_NPPSENDMSGEX;
            isMsgEx = true;
            break;

        default:
            // unsupported command
            return CMDRESULT_FAILED;
    }

    if ( !reportCmdAndParams( cmdName.c_str(), params, fMessageToConsole | fReportEmptyParam | fFailIfEmptyParam ) )
        return CMDRESULT_FAILED;

    // send message to Notepad++
    
    // npp(sci)_msg <uMsg>
    // npp(sci)_msg <uMsg> <wParam>
    // npp(sci)_msg <uMsg> <wParam> <lParam>
    
    tstr val;
    tstr calcErr;
    const TCHAR* s = params.c_str();

    for ( int n = 0; n < (isMsgEx ? 2 : 1); n++ )
    {
        bool isValidParam = true;
        bool hasDblQuote = false;

        s = get_param(s, val, SEP_TABSPACE, &hasDblQuote); // uMsg or hWnd
        if ( !val.IsEmpty() )
        {

            if ( isMsgEx && n == 0 )
                Runtime::GetLogger().Add(   _T("; hWnd parameter...") );
            else
                Runtime::GetLogger().Add(   _T("; uMsg parameter...") );

            CStrSplitT<TCHAR> args;
            m_pNppExec->GetMacroVars().CheckCmdArgs(val, args);
            m_pNppExec->GetMacroVars().CheckAllMacroVars(val, true);

            if ( isMsgEx && n == 0 )
            {
                // hWnd
                if ( hasDblQuote || val.GetAt(0) == _T('@') )
                {
                    isValidParam = false;
                }
                else
                {
                    // !!! not compatible with x64 (if any)
                    hWnd = (HWND) c_base::_tstr2uint( val.c_str() );
                    if ( !::IsWindow(hWnd) )
                        isValidParam = false;
                }
            }
        }
        else
        {
            if ( isMsgEx && n == 0 )
                isValidParam = false;
        }

        if ( isMsgEx && !isValidParam )
        {
            // hwnd should be a valid window handle
            m_pNppExec->GetConsole().PrintError( _T("- incorrect hWnd: should be a valid window handle") );
            return CMDRESULT_FAILED;
        }
    }

    if ( (!isDecNumChar(val.GetAt(0))) &&
         (val.GetAt(0) != _T('-') || !isDecNumChar(val.GetAt(1))) )
    {
        calc2(val, calcErr, val); // try to calculate
    }

    UINT uMsg = c_base::_tstr2uint( val.c_str() );
    if ( uMsg == 0 )
    {
        m_pNppExec->GetConsole().PrintError( _T("- uMsg is 0 (no message to send)") );
        return CMDRESULT_FAILED;
    }

    if ( *s == 0 )
    {
        // npp(sci)_msg <uMsg>

        if ( isMsgEx )
            Runtime::GetLogger().AddEx( _T("; hWnd  = %d (0x%X)"), hWnd, hWnd );
        Runtime::GetLogger().AddEx( _T("; uMsg  = %d (0x%X)"), uMsg, uMsg );
        Runtime::GetLogger().AddEx( _T("; wType = %s,  wParam = \"0\""), STR_PARAMTYPE[PT_INT] );
        Runtime::GetLogger().AddEx( _T("; lType = %s,  lParam = \"0\""), STR_PARAMTYPE[PT_INT] );

        LRESULT lResult = ::SendMessage( hWnd, uMsg, 0, 0 );
        
        // RESULT
        tstr varName;
        tstr varValue;
        TCHAR szNum[50];

        varName = MACRO_MSG_RESULT;
        szNum[0] = 0;
        c_base::_tint2str( (int) lResult, szNum );
        varValue = szNum;
        m_pNppExec->GetMacroVars().SetUserMacroVar(varName, varValue);
        
        // WPARAM
        varName = MACRO_MSG_WPARAM;
        varValue.Clear();
        m_pNppExec->GetMacroVars().SetUserMacroVar(varName, varValue);
        
        // LPARAM
        varName = MACRO_MSG_LPARAM;
        varValue.Clear();
        m_pNppExec->GetMacroVars().SetUserMacroVar(varName, varValue);
        
        return CMDRESULT_SUCCEEDED;
    }

    int  paramType[2] = { PT_INT, PT_INT };
    tstr paramValue[2] = { _T("0"), _T("0") };

    // npp(sci)_msg <uMsg> <wParam>
    // npp(sci)_msg <uMsg> <wParam> <lParam>
    
    for ( int n = 0; n < 2; n++ )
    {
        bool hasDblQuote = false;
        bool hasBracket = false;
        tstr& val = paramValue[n];
        s = get_param(s, val, SEP_TABSPACE, &hasDblQuote, &hasBracket); // wType or lType
        if ( !val.IsEmpty() )
        {

            if ( n == 0 )
                Runtime::GetLogger().Add(   _T("; wParam parameter...") );
            else
                Runtime::GetLogger().Add(   _T("; lParam parameter...") );

            CStrSplitT<TCHAR> args;
            m_pNppExec->GetMacroVars().CheckCmdArgs(val, args);
            m_pNppExec->GetMacroVars().CheckAllMacroVars(val, true);

            if ( hasBracket )
            {
                if ( val.GetLastChar() != _T(']') )
                {
                    //  starts with '[', but no ']' in the end
                    m_pNppExec->GetConsole().PrintError( _T("- hex-string parameter should end with \']\'") );
                    return CMDRESULT_FAILED;
                }
            }
        }

        if ( val.GetAt(0) == _T('@') )
        {
            val.Delete(0, 1);
            if ( val.IsEmpty() && !hasDblQuote )
            {
                // empty, no '"'
                m_pNppExec->GetConsole().PrintError( _T("- empty pointer parameter specified") );
                return CMDRESULT_FAILED;
            }

            if ( hasBracket )
                paramType[n] = PT_PHEXSTR;
            else if ( hasDblQuote )
                paramType[n] = PT_PSTR;
            else
                paramType[n] = PT_PINT;
        }
        else
        {
            if ( hasBracket )
                paramType[n] = PT_HEXSTR;
            else if ( hasDblQuote )
                paramType[n] = PT_STR;
            else
                paramType[n] = PT_INT;
        }

        switch ( paramType[n] )
        {
            case PT_INT:
            case PT_PINT:
            {
                if ( (!isDecNumChar(val.GetAt(0))) &&
                     (val.GetAt(0) != _T('-') || !isDecNumChar(val.GetAt(1))) )
                {
                    if ( !calc2(val, calcErr, val) ) // try to calculate
                    {
                        m_pNppExec->GetConsole().PrintError( _T("- string parameter specified without \"\"") );
                        return CMDRESULT_FAILED;
                    }
                }
                break;
            }
        }

        if ( *s == 0 )
            break;
    }

    if ( isMsgEx )
        Runtime::GetLogger().AddEx( _T("; hWnd  = %d (0x%X)"), hWnd, hWnd );
    Runtime::GetLogger().AddEx( _T("; uMsg  = %d (0x%X)"), uMsg, uMsg );
    Runtime::GetLogger().AddEx( _T("; wType = %s,  wParam = \"%s\""), 
      STR_PARAMTYPE[paramType[0]], paramValue[0].c_str() );
    Runtime::GetLogger().AddEx( _T("; lType = %s,  lParam = \"%s\""), 
      STR_PARAMTYPE[paramType[1]], paramValue[1].c_str() );
        
    // npp(sci)_msg <uMsg> <wParam> <lParam>

    const int PARAM_BUF_SIZE = Runtime::GetOptions().GetInt(OPTI_SENDMSG_MAXBUFLEN);
    TCHAR* pParam[2] = { NULL, NULL };
    char*  pSciParam[2] = { NULL, NULL };
    DWORD  dwParam[2] = { 0, 0 };
    WPARAM wParam = 0;
    LPARAM lParam = 0;
    TCHAR  szNum[50];

    for ( int n = 0; n < 2; n++ )
    {
        switch ( paramType[n] )
        {
            case PT_INT:
                dwParam[n] = c_base::_tstr2int( paramValue[n].c_str() );
                if ( n == 0 )
                    wParam = dwParam[n];
                else
                    lParam = dwParam[n];
                break;

            case PT_PINT:
                dwParam[n] = c_base::_tstr2int( paramValue[n].c_str() );
                if ( n == 0 )
                    wParam = (WPARAM) &dwParam[n];
                else
                    lParam = (LPARAM) &dwParam[n];
                break;

            case PT_STR:
            case PT_PSTR:
            case PT_HEXSTR:
            case PT_PHEXSTR:
                if ( paramType[n] == PT_HEXSTR || paramType[n] == PT_PHEXSTR )
                {
                    int nHexBufSize = PARAM_BUF_SIZE;
                    if ( cmdType != CMDTYPE_SCISENDMSG )
                        nHexBufSize *= sizeof(TCHAR);
                    c_base::byte_t* pHexBuf = new c_base::byte_t[nHexBufSize];
                    if ( pHexBuf )
                    {
                        memset( pHexBuf, 0, nHexBufSize );

                        tstr& val = paramValue[n];
                        const TCHAR* pHexStr = val.c_str() + 1; // skip leading '['
                        val.SetSize( val.length() - 1 ); // exlude trailing ']'
                        c_base::_thexstrex2buf( pHexStr, pHexBuf, nHexBufSize - sizeof(TCHAR) );

                        if ( cmdType == CMDTYPE_SCISENDMSG )
                            pSciParam[n] = (char*) pHexBuf;
                        else
                            pParam[n] = (TCHAR*) pHexBuf;
                    }
                }
                else
                {
                    if ( cmdType == CMDTYPE_SCISENDMSG )
                    {
                        pSciParam[n] = new char[PARAM_BUF_SIZE];
                        if ( pSciParam[n] )
                        {
                            memset( pSciParam[n], 0, PARAM_BUF_SIZE*sizeof(char) );
                            if ( !paramValue[n].IsEmpty() )
                            {
                                char* pp = SciTextFromLPCTSTR(paramValue[n].c_str(), hWnd);
                                if ( pp )
                                {
                                    lstrcpyA(pSciParam[n], pp);
                                    #ifdef UNICODE
                                        delete [] pp;
                                    #else
                                        if ( pp != paramValue[n].c_str() )
                                            delete [] pp;
                                    #endif
                                }
                            }
                        }
                    }
                    else
                    {
                        pParam[n] = new TCHAR[PARAM_BUF_SIZE];
                        if ( pParam[n] )
                        {
                            memset( pParam[n], 0, PARAM_BUF_SIZE*sizeof(TCHAR) );
                            lstrcpy( pParam[n], paramValue[n].c_str() );
                        }
                    }
                }
                if ( pParam[n] )
                {
                    if ( n == 0 )
                        wParam = (WPARAM) pParam[n];
                    else
                        lParam = (LPARAM) pParam[n];
                }
                else if ( pSciParam[n] )
                {
                    if ( n == 0 )
                        wParam = (WPARAM) pSciParam[n];
                    else
                        lParam = (LPARAM) pSciParam[n];
                }
                else
                {
                    m_pNppExec->GetConsole().PrintError( _T("- could not allocate memory for a parameter") );
                    return CMDRESULT_FAILED;
                }
                break;
        }
    }

    LRESULT lResult = ::SendMessage( hWnd, uMsg, wParam, lParam );

    for ( int n = 0; n < 2; n++ )
    {
        if ( paramType[n] == PT_STR || paramType[n] == PT_HEXSTR )
        {
            if ( pSciParam[n] )
            {
                #ifdef UNICODE
                    delete [] pSciParam[n];
                #else
                    if ( pSciParam[n] != paramValue[n].c_str() )
                        delete [] pSciParam[n];
                #endif

                pSciParam[n] = NULL;
            }
            if ( pParam[n] )
            {
                delete [] pParam[n];
                pParam[n] = NULL;
            }
        }
    }

    int  i;
    tstr varName;
    tstr varValue;
    
    for ( int n = 0; n < 2; n++ )
    {
        varName = (n == 0) ? MACRO_MSG_WPARAM : MACRO_MSG_LPARAM;

        switch ( paramType[n] )
        {
            case PT_PINT:
                szNum[0] = 0;
                c_base::_tint2str( (int) dwParam[n], szNum );
                varValue = szNum;
                m_pNppExec->GetMacroVars().SetUserMacroVar(varName, varValue);
                break;

            case PT_PSTR:
                if ( cmdType == CMDTYPE_SCISENDMSG )
                {
                    TCHAR* pp = SciTextToLPTSTR(pSciParam[n], hWnd);
                    varValue = pp;
                    #ifdef UNICODE
                        delete [] pp;
                    #else
                        if ( pp != pSciParam[n] )
                            delete [] pp;
                    #endif
                    delete [] pSciParam[n];
                    pSciParam[n] = NULL;
                }
                else
                {
                    varValue = pParam[n];
                    delete [] pParam[n];
                    pParam[n] = NULL;
                }
                for ( i = varValue.length() - 1; i >= 0; i-- )
                {
                    const TCHAR ch = varValue[i];
                    if ( ch == _T('\r') || ch == _T('\n') )
                        varValue.Delete(i, 1); // exclude trailing '\r' or '\n'
                    else
                        break;
                }
                m_pNppExec->GetMacroVars().SetUserMacroVar(varName, varValue);
                break;

            case PT_PHEXSTR:
                varValue = _T("[ ");
                {
                    const int nBytesInLine = 16;
                    const int nBytesToShow = 256;
                    c_base::byte_t* pHexBuf;
                    c_base::byte_t* p;
                    TCHAR szBytesLine[3*nBytesInLine + 2];

                    if ( cmdType == CMDTYPE_SCISENDMSG )
                        pHexBuf = (c_base::byte_t *) pSciParam[n];
                    else
                        pHexBuf = (c_base::byte_t *) pParam[n];

                    p = pHexBuf;
                    while ( p < pHexBuf + nBytesToShow )
                    {
                        if ( p != pHexBuf )  varValue += _T_RE_EOL _T("  ");
                        c_base::_tbuf2hexstr(p, nBytesInLine, szBytesLine, 3*nBytesInLine + 2, _T(" "));
                        varValue += szBytesLine;
                        p += nBytesInLine;
                    }

                    delete [] pHexBuf;
                    if ( cmdType == CMDTYPE_SCISENDMSG )
                        pSciParam[n] = NULL;
                    else
                        pParam[n] = NULL;
                }
                varValue += _T(" ]");
                m_pNppExec->GetMacroVars().SetUserMacroVar(varName, varValue);
                break;

            default:
                varValue.Clear();
                m_pNppExec->GetMacroVars().SetUserMacroVar(varName, varValue);
                break;
        }
    }

    varName = MACRO_MSG_RESULT;
    szNum[0] = 0;
    c_base::_tint2str( (int) lResult, szNum );
    varValue = szNum;
    m_pNppExec->GetMacroVars().SetUserMacroVar(varName, varValue);

    return CMDRESULT_DONE;
}

CScriptEngine::eCmdResult CScriptEngine::DoNppSwitch(const tstr& params)
{
    if ( !reportCmdAndParams( CMD_NPPSWITCH, params, fMessageToConsole | fReportEmptyParam | fFailIfEmptyParam ) )
        return CMDRESULT_FAILED;

    if ( !m_pNppExec->nppSwitchToDocument(params.c_str(), true) )
        m_pNppExec->GetConsole().PrintError( _T("- no such file opened") );

    return CMDRESULT_DONE;
}

CScriptEngine::eCmdResult CScriptEngine::DoSelLoadFrom(const tstr& params)
{
    if ( !reportCmdAndParams( CMD_SELLOADFROM, params, fMessageToConsole | fReportEmptyParam | fFailIfEmptyParam ) )
        return CMDRESULT_FAILED;

    int nBytes = m_pNppExec->selLoadFrom(params.c_str());
    if ( nBytes < 0 )
    {
        m_pNppExec->GetConsole().PrintError( _T("- can not open the file or it\'s empty") );
    }

    return CMDRESULT_DONE;
}

CScriptEngine::eCmdResult CScriptEngine::DoSelSaveTo(const tstr& params)
{
    if ( !reportCmdAndParams( CMD_SELSAVETO, params, fMessageToConsole | fReportEmptyParam | fFailIfEmptyParam ) )
        return CMDRESULT_FAILED;

    tstr  S;
    TCHAR szCmdLine[FILEPATH_BUFSIZE + 20];
    lstrcpy(szCmdLine, params.c_str());

    int nBytes = m_pNppExec->selSaveTo(szCmdLine);
    if ( nBytes >= 0 )
    {
        TCHAR num[32];
        wsprintf( num, _T("%ld"), nBytes );
        S = _T("- OK, ");
        S += num;
        S += _T(" bytes have been written to \"");
    }
    else
    {
        S = _T("- failed to write to \"");
    }
    S += (szCmdLine[0] == _T('\"')) ? (szCmdLine + 1) : szCmdLine;
    S += _T("\"");
    if ( nBytes >= 0 )
        m_pNppExec->GetConsole().PrintMessage( S.c_str() );
    else
        m_pNppExec->GetConsole().PrintError( S.c_str() );

    return CMDRESULT_DONE;
}

CScriptEngine::eCmdResult CScriptEngine::DoSelSetText(const tstr& params)
{
    if ( !reportCmdAndParams( CMD_SELSETTEXT, params, fMessageToConsole | fReportEmptyParam | fFailIfEmptyParam ) )
        return CMDRESULT_FAILED;

    m_pNppExec->selSetText( params.c_str() );

    return CMDRESULT_DONE;
}

CScriptEngine::eCmdResult CScriptEngine::DoSelSetTextEx(const tstr& params)
{
    if ( !reportCmdAndParams( CMD_SELSETTEXTEX, params, fMessageToConsole | fReportEmptyParam | fFailIfEmptyParam ) )
        return CMDRESULT_FAILED;

    int  pos = params.Find( _T('\\') );
    if ( pos >= 0 )
    {
        tstr S = params;
        while ( pos < S.length() )
        {
            if ( S[pos] == _T('\\') )
            {
                switch ( S.GetAt(pos + 1) )
                {
                    case _T('n'):
                        S[pos] = _T('\n');
                        S.Delete(pos + 1, 1);
                        break;
                    case _T('r'):
                        S[pos] = _T('\r');
                        S.Delete(pos + 1, 1);
                        break;
                    case _T('t'):
                        S[pos] = _T('\t');
                        S.Delete(pos + 1, 1);
                        break;
                    case _T('\\'):
                        S.Delete(pos + 1, 1);
                        break;
                }
            }
            ++pos;
        }
        m_pNppExec->selSetText( S.c_str() );
    }
    else
        m_pNppExec->selSetText( params.c_str() );

    return CMDRESULT_DONE;
}

CScriptEngine::eCmdResult CScriptEngine::DoSet(const tstr& params)
{
    reportCmdAndParams( CMD_SET, params, fMessageToConsole );
          
    // sets the value of user's variable
    // refer to CheckUserMacroVars() though

    bool isInternal = false;
    bool bLocalVar = false;
    
    tstr varName;
    if ( params.IsEmpty() )
    {
          
        Runtime::GetLogger().Add(   _T("; no arguments given - showing all user\'s variables") );

    }
    else
    {
        varName = params;
        int k = varName.Find( _T("=") );
        if ( k >= 0 )  
        {
            varName.SetSize(k);
            isInternal = true;
        }
        DelLeadingTabSpaces(varName);
        DelTrailingTabSpaces(varName);
        bLocalVar = m_pNppExec->GetMacroVars().IsLocalMacroVar(varName);
        if ( varName.length() > 0 )
        {
            if ( varName.StartsWith(_T("$(")) == false )
                varName.Insert(0, _T("$("));
            if ( varName.GetLastChar() != _T(')') )
                varName.Append(_T(')'));
            ::CharUpper( varName.c_str() );
        }
    }

    const CNppExecMacroVars::tMacroVars& userLocalMacroVars = m_pNppExec->GetMacroVars().GetUserLocalMacroVars();
    const CNppExecMacroVars::tMacroVars& userMacroVars = m_pNppExec->GetMacroVars().GetUserMacroVars();

    if ( userLocalMacroVars.empty() && bLocalVar )
    {
        m_pNppExec->GetConsole().PrintMessage( _T("- no user-defined local variables"), false );
    }
    else if ( userLocalMacroVars.empty() && userMacroVars.empty() )
    {
        m_pNppExec->GetConsole().PrintMessage( _T("- no user-defined variables"), false );
    }
    else
    {
        if ( varName.IsEmpty() )
        {
            if ( bLocalVar )
            {
                PrintMacroVarFunc func(m_pNppExec);
                CNppExecMacroVars::tMacroVars::const_iterator itrVar = userLocalMacroVars.begin();
                for ( ; itrVar != userLocalMacroVars.end(); ++itrVar )
                {
                    func(itrVar, true);
                }
            }
            else
                IterateUserMacroVars<PrintMacroVarFunc>(userMacroVars, userLocalMacroVars, PrintMacroVarFunc(m_pNppExec));
        }
        else
        {
            const CNppExecMacroVars::tMacroVars& macroVars = bLocalVar ? userLocalMacroVars : userMacroVars;
            CNppExecMacroVars::tMacroVars::const_iterator itrVar = macroVars.find(varName);
            if ( itrVar != macroVars.end() )
            {
                tstr S = bLocalVar ? _T("local ") : _T("");
                S += itrVar->first;
                S += _T(" = ");
                if ( itrVar->second.length() > MAX_VAR_LENGTH2SHOW )
                {
                    S.Append( itrVar->second.c_str(), MAX_VAR_LENGTH2SHOW - 5 );
                    S += _T("(...)");
                }
                else
                {
                    S += itrVar->second;
                }
                m_pNppExec->GetConsole().PrintMessage( S.c_str(), isInternal );
            }
            else
            {
                tstr t = _T("- no such user\'s ");
                if ( bLocalVar ) t += _T("local ");
                t += _T("variable: ");
                t += varName;
                m_pNppExec->GetConsole().PrintMessage( t.c_str(), false );
            }
        }
    }

    return CMDRESULT_DONE;
}

CScriptEngine::eCmdResult CScriptEngine::DoUnset(const tstr& params)
{
    if ( params.IsEmpty() )
    {
        errorCmdNoParam(CMD_UNSET);
        return CMDRESULT_FAILED;
    }

    // this functionality is implemented in CheckUserMacroVars()

    return CMDRESULT_DONE;
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

CNppExecMacroVars::CNppExecMacroVars() : m_pNppExec(0)
{
}

CNppExec* CNppExecMacroVars::GetNppExec() const
{
    return m_pNppExec;
}

void CNppExecMacroVars::SetNppExec(CNppExec* pNppExec)
{
    m_pNppExec = pNppExec;
}

bool CNppExecMacroVars::IsLocalMacroVar(tstr& varName)
{
    bool isLocal = false;
    int n = varName.Find(_T(' '));
    if ( n < 0 )
    {
        n = varName.Find(_T('\t'));
        if ( n < 0 )
            n = varName.length();
    }
    if ( n == 5 ) // length of "local"
    {
        tstr Prefix;
        Prefix.Copy( varName.c_str(), n );
        ::CharLower( Prefix.c_str() );
        if ( Prefix == _T("local") )
        {
            isLocal = true;
            varName.Delete(0, n);
            DelLeadingTabSpaces(varName);
        }
    }
    return isLocal;
}

bool CNppExecMacroVars::ContainsMacroVar(const tstr& S)
{
    return ( (S.Find(_T("$(")) >= 0) ? true : false );
}

CNppExecMacroVars::tMacroVars& CNppExecMacroVars::GetUserLocalMacroVars()
{
    CScriptEngine* pRunningScriptEngine = m_pNppExec->GetRunningScriptEngine();
    if ( pRunningScriptEngine )
    {
        CScriptEngine::ExecState& execState = pRunningScriptEngine->GetExecState();
        CListItemT<CScriptEngine::ScriptContext>* pItem = execState.ScriptContextList.GetLast();
        if ( pItem )
        {
            CScriptEngine::ScriptContext& scriptContext = pItem->GetItem();
            return scriptContext.MacroVars;
        }
    }
    return m_UserLocalMacroVars0;
}

CNppExecMacroVars::tMacroVars& CNppExecMacroVars::GetUserMacroVars()
{
    return m_UserMacroVars;
}

CNppExecMacroVars::tMacroVars& CNppExecMacroVars::GetCmdAliases()
{
    return m_CmdAliases;
}

void CNppExecMacroVars::CheckCmdArgs(tstr& Cmd, const CStrSplitT<TCHAR>& args)
{
  
  Runtime::GetLogger().Add(   _T("CheckCmdArgs()") );
  Runtime::GetLogger().Add(   _T("{") );
  Runtime::GetLogger().IncIndentLevel();
  Runtime::GetLogger().AddEx( _T("[in]  \"%s\""), Cmd.c_str() );   
    
  if ( ContainsMacroVar(Cmd) )
  {
    tstr S = Cmd;
    ::CharUpper( (LPTSTR) S.c_str() );
      
    TCHAR szNum[3*sizeof(int) + 2];
    c_base::_tint2str(args.GetArgCount(), szNum);

    int   len = lstrlen(MACRO_ARGC);
    int   pos = 0;
    while ( (pos = S.Find(MACRO_ARGC, pos)) >= 0 )
    {
      S.Replace(pos, len, szNum);
      Cmd.Replace(pos, len, szNum);
      pos += lstrlen(szNum);
    }

    int  len1 = lstrlen(MACRO_ARGV);
    int  len2 = lstrlen(MACRO_RARGV);
    bool bReverse = false;
    if ( (pos = S.Find(MACRO_RARGV)) >= 0 )
    {
      len = len2;
      bReverse = true;
    }
    else if ( (pos = S.Find(MACRO_ARGV)) >= 0 )
    {
      len = len1;
      bReverse = false;
    }
    while ( pos >= 0 )
    {
      switch ( S.GetAt(pos+len) )
      {
        case 0:
        {
          const tstr& t = bReverse ? args.GetRArgs() : args.GetArgs();
          S.Replace( pos, len, t.c_str(), t.length() );
          Cmd.Replace( pos, len, t.c_str(), t.length() );
          pos += t.length();
          break;
        }
        case _T(')'):
        {
          const tstr& t = bReverse ? args.GetRArgs() : args.GetArgs();
          S.Replace( pos, len + 1, t.c_str(), t.length() );
          Cmd.Replace( pos, len + 1, t.c_str(), t.length() );
          pos += t.length();
          break;
        }
        case _T('['):
        {
          int   k = 0;
          TCHAR ch = S.GetAt(pos + len + k + 1);

          szNum[0] = 0;
          while ( isDecNumChar(ch) && (k < 3*sizeof(int) + 1) )
          {
            szNum[k] = ch;
            ++k;
            ch = S.GetAt(pos + len + k + 1);
          }
          szNum[k] = 0;
          while ( (ch != _T(')')) && (ch != 0) )
          {
            ++k;
            ch = S.GetAt(pos + len + k + 1);
          }
          const tstr& t = bReverse ? args.GetRArg( _ttoi(szNum) ) : args.GetArg( _ttoi(szNum) );
          S.Replace( pos, len + k + 2, t.c_str(), t.length() );
          Cmd.Replace( pos, len + k + 2, t.c_str(), t.length() );
          pos += t.length();
          break;
        }
        default:
        {
          pos += len;
          break;
        }
      }
      
      int pos0 = pos;
      if ( (pos = S.Find(MACRO_RARGV, pos0)) >= 0 )
      {
        len = len2;
        bReverse = true;
      }
      else if ( (pos = S.Find(MACRO_ARGV, pos0)) >= 0 )
      {
        len = len1;
        bReverse = false;
      }
    }
  }

  Runtime::GetLogger().AddEx( _T("[out] \"%s\""), Cmd.c_str() );
  Runtime::GetLogger().DecIndentLevel();
  Runtime::GetLogger().Add(   _T("}") );

}

void CNppExecMacroVars::CheckCmdAliases(tstr& S)
{
    
    Runtime::GetLogger().Add(   _T("CheckCmdAliases()") );
    Runtime::GetLogger().Add(   _T("{") );
    Runtime::GetLogger().IncIndentLevel();

    if ( !Runtime::GetOptions().GetBool(OPTB_CONSOLE_NOCMDALIASES) )
    {

        Runtime::GetLogger().Add(   _T("; command aliases enabled") );
        Runtime::GetLogger().AddEx( _T("[in]  \"%s\""), S.c_str() );
        
        //DelLeadingTabSpaces(S);
        
        if ( S.length() > 0 )
        {
            tstr t = S;

            if ( t.length() > 0 )
            {
                ::CharUpper( (LPTSTR) t.c_str() );

                tMacroVars::const_iterator itrAlias = m_CmdAliases.begin();
                for ( ; itrAlias != m_CmdAliases.end(); ++itrAlias )
                {
                    const tstr& aliasName = itrAlias->first;
                    const int len = aliasName.length();
                    if ( (len > 0) &&
                         (c_base::_tstr_unsafe_cmpn(aliasName.c_str(), t.c_str(), len) == 0) )
                    {
                        const TCHAR ch = t.GetAt(len);
                        if ( IsTabSpaceOrEmptyChar(ch) )
                        {
                            const tstr& aliasValue = itrAlias->second;
                            S.Replace( 0, len, aliasValue.c_str(), aliasValue.length() );
                            break;
                        }
                    }
                }
            }
        }

        Runtime::GetLogger().AddEx( _T("[out] \"%s\""), S.c_str() );

    }
    else
    {
        
        Runtime::GetLogger().Add(   _T("; command aliases disabled") );

    }

    Runtime::GetLogger().DecIndentLevel();
    Runtime::GetLogger().Add(   _T("}") );

}

void CNppExecMacroVars::CheckNppMacroVars(tstr& S)
{
  const int    NPPVAR_COUNT = 7 + 2;
  const TCHAR* NPPVAR_STRINGS[NPPVAR_COUNT] = {
    MACRO_FILE_FULLPATH,
    MACRO_FILE_DIRPATH,
    MACRO_FILE_FULLNAME,
    MACRO_FILE_NAMEONLY,
    MACRO_FILE_EXTONLY,
    MACRO_NPP_DIRECTORY,
    MACRO_CURRENT_WORD,
    
    MACRO_CURRENT_LINE,   // (int) line number
    MACRO_CURRENT_COLUMN  // (int) column number
  };
  const UINT   NPPVAR_MESSAGES[NPPVAR_COUNT] = {
    NPPM_GETFULLCURRENTPATH,
    NPPM_GETCURRENTDIRECTORY,
    NPPM_GETFILENAME,
    NPPM_GETNAMEPART,
    NPPM_GETEXTPART,
    NPPM_GETNPPDIRECTORY,
    NPPM_GETCURRENTWORD,
    
    NPPM_GETCURRENTLINE,
    NPPM_GETCURRENTCOLUMN
  };
  
  Runtime::GetLogger().Add(   _T("CheckNppMacroVars()") );
  Runtime::GetLogger().Add(   _T("{") );
  Runtime::GetLogger().IncIndentLevel();
  Runtime::GetLogger().AddEx( _T("[in]  \"%s\""), S.c_str() );
  
  if ( ContainsMacroVar(S) )
  {
    tstr Cmd = S;
    ::CharUpper( (LPTSTR) Cmd.c_str() );

    const int MACRO_SIZE = CONSOLECOMMAND_BUFSIZE;
    TCHAR     szMacro[MACRO_SIZE];

    for (int j = 0; j < NPPVAR_COUNT; j++)
    {
      const TCHAR* macro_str = NPPVAR_STRINGS[j];
      const int    macro_len = lstrlen(macro_str);

      int  len = -1;
      int  pos = 0;
      while ( (pos = Cmd.Find(macro_str, /*macro_len,*/ pos)) >= 0 )
      {
        if (len < 0)
        {
          szMacro[0] = 0;
          if (j < 7)
          {
            m_pNppExec->SendNppMsg(NPPVAR_MESSAGES[j], 
              (WPARAM) (MACRO_SIZE - 1), (LPARAM) szMacro);
          }
          else
          {
            int nn = (int) m_pNppExec->SendNppMsg(NPPVAR_MESSAGES[j], 0, 0);
            wsprintf(szMacro, _T("%d"), nn);
          }
          len = lstrlen(szMacro);
        }
        S.Replace(pos, macro_len, szMacro, len);
        Cmd.Replace(pos, macro_len, szMacro, len);
        pos += len;
      }
    }
  }

  Runtime::GetLogger().AddEx( _T("[out] \"%s\""), S.c_str() );
  Runtime::GetLogger().DecIndentLevel();
  Runtime::GetLogger().Add(   _T("}") );
  
}

void CNppExecMacroVars::CheckPluginMacroVars(tstr& S)
{
  
  Runtime::GetLogger().Add(   _T("CheckPluginMacroVars()") );
  Runtime::GetLogger().Add(   _T("{") );
  Runtime::GetLogger().IncIndentLevel();
  Runtime::GetLogger().AddEx( _T("[in]  \"%s\""), S.c_str() );  
    
  if ( ContainsMacroVar(S) )
  {
    tstr Cmd = S;
    ::CharUpper( (LPTSTR) Cmd.c_str() );

    const int MACRO_SIZE = 0x200;
    TCHAR     szMacro[MACRO_SIZE];

    bool bMacroOK = false;
    int len = lstrlen(MACRO_DOCNUMBER); // "$(#"
    int pos = 0;
    while ((pos = Cmd.Find(MACRO_DOCNUMBER, pos)) >= 0)
    {
      int   k;
      tstr  snum = _T("");
      
      for (k = pos + len; k < Cmd.length(); k++)
      {
        if (isDecNumChar(Cmd[k]))
          snum += Cmd[k];
        else
          break;
      }
      for (; k < Cmd.length(); k++)
      {
        if (Cmd[k] == _T(')'))
          break;
      }
      k -= pos;
      Cmd.Delete(pos, k+1);
      S.Delete(pos, k+1);

      if (snum.length() > 0)
      {
        k = _ttoi(snum.c_str());
        if (k > 0) 
        {
          // #doc = 1..nbFiles
          if (!bMacroOK)
          {
            m_pNppExec->nppGetOpenFileNames();
            bMacroOK = true;
          }
          if (k <= m_pNppExec->npp_nbFiles)
          {
            lstrcpy(szMacro, m_pNppExec->npp_bufFileNames[k-1]);
            Cmd.Insert(pos, szMacro);
            S.Insert(pos, szMacro);
            pos += lstrlen(szMacro);
          }
        }
        else if (k == 0)
        {
          // #doc = 0 means notepad++ full path
          ::GetModuleFileName(NULL, szMacro, MACRO_SIZE-1);
          Cmd.Insert(pos, szMacro);
          S.Insert(pos, szMacro);
          pos += lstrlen(szMacro);
        }
      }

    }

    tstr sub;
    len = lstrlen(MACRO_SYSVAR);
    pos = 0;
    while ((pos = Cmd.Find(MACRO_SYSVAR, pos)) >= 0)
    {
      bool bSysVarOK = false;
      int  i1 = pos + len;
      int  i2 = i1;
      while ((i2 < Cmd.length()) && (Cmd[i2] != _T(')')))  ++i2;
      sub.Copy(Cmd.c_str() + i1, i2 - i1);
      if (sub.length() > 0)
      {
        DWORD nLen = GetEnvironmentVariable(sub.c_str(), NULL, 0);
        if (nLen > 0)
        {
          TCHAR* pStr = new TCHAR[nLen + 2];
          if (pStr)
          {
            nLen = GetEnvironmentVariable(sub.c_str(), pStr, nLen + 1);
            if (nLen > 0)
            {
              Cmd.Replace(pos, i2 - pos + 1, pStr, nLen);
              S.Replace(pos, i2 - pos + 1, pStr, nLen);
              pos += nLen;
              bSysVarOK = true;
            }
            delete [] pStr;
          }
        }
      }
      if (!bSysVarOK)
      {
        Cmd.Replace(pos, i2 - pos + 1, _T(""), 0);
        S.Replace(pos, i2 - pos + 1, _T(""), 0);
      }
    }
    
    len = lstrlen(MACRO_LEFT_VIEW_FILE);
    pos = 0;
    while ((pos = Cmd.Find(MACRO_LEFT_VIEW_FILE, pos)) >= 0)
    {
      int ind = (int) m_pNppExec->SendNppMsg(NPPM_GETCURRENTDOCINDEX, MAIN_VIEW, MAIN_VIEW);
      if (m_pNppExec->nppGetOpenFileNamesInView(PRIMARY_VIEW, ind + 1) == ind + 1)
      {
        Cmd.Replace(pos, len, m_pNppExec->npp_bufFileNames.GetAt(ind));
        S.Replace(pos, len, m_pNppExec->npp_bufFileNames.GetAt(ind));
        pos += lstrlen(m_pNppExec->npp_bufFileNames.GetAt(ind));
      }
      else
      {
        Cmd.Replace(pos, len, _T(""), 0);
        S.Replace(pos, len, _T(""), 0);
      }
    }

    len = lstrlen(MACRO_RIGHT_VIEW_FILE);
    pos = 0;
    while ((pos = Cmd.Find(MACRO_RIGHT_VIEW_FILE, pos)) >= 0)
    {
      int ind = (int) m_pNppExec->SendNppMsg(NPPM_GETCURRENTDOCINDEX, SUB_VIEW, SUB_VIEW);
      if (m_pNppExec->nppGetOpenFileNamesInView(SECOND_VIEW, ind + 1) == ind + 1)
      {
        Cmd.Replace(pos, len, m_pNppExec->npp_bufFileNames.GetAt(ind));
        S.Replace(pos, len, m_pNppExec->npp_bufFileNames.GetAt(ind));
        pos += lstrlen(m_pNppExec->npp_bufFileNames.GetAt(ind));
      }
      else
      {
        Cmd.Replace(pos, len, _T(""), 0);
        S.Replace(pos, len, _T(""), 0);
      }
    }

    len = lstrlen(MACRO_CURRENT_WORKING_DIR);
    pos = 0;
    while ((pos = Cmd.Find(MACRO_CURRENT_WORKING_DIR, pos)) >= 0)
    {
        TCHAR szPath[FILEPATH_BUFSIZE];
        DWORD dwLen;

        szPath[0] = 0;
        dwLen = GetCurrentDirectory(FILEPATH_BUFSIZE - 1, szPath);
        Cmd.Replace(pos, len, szPath);
        S.Replace(pos, len, szPath);
        pos += (int) dwLen;
    }
    
    len = lstrlen(MACRO_PLUGINS_CONFIG_DIR);
    pos = 0;
    while ((pos = Cmd.Find(MACRO_PLUGINS_CONFIG_DIR, pos)) >= 0)
    {
      Cmd.Replace(pos, len, m_pNppExec->getConfigPath());
      S.Replace(pos, len, m_pNppExec->getConfigPath());
      pos += lstrlen(m_pNppExec->getConfigPath());
    }

  }

  Runtime::GetLogger().AddEx( _T("[out] \"%s\""), S.c_str() );
  Runtime::GetLogger().DecIndentLevel();
  Runtime::GetLogger().Add(   _T("}") );

}

void CNppExecMacroVars::CheckUserMacroVars(tstr& S, int nCmdType )
{
  
  Runtime::GetLogger().Add(   _T("CheckUserMacroVars()") );
  Runtime::GetLogger().Add(   _T("{") );
  Runtime::GetLogger().IncIndentLevel();
  Runtime::GetLogger().AddEx( _T("[in]  \"%s\""), S.c_str() );
    
  if ( nCmdType == CScriptEngine::CMDTYPE_SET )
  {
    
    Runtime::GetLogger().AddEx( _T("; %s command found"), CMD_SET );
      
    const TCHAR* DEF_OP  = _T("=");
    const TCHAR* CALC_OP = _T("~");
    const int i1 = S.Find(DEF_OP);    
    const int i2 = S.Find(CALC_OP);
    const bool bSep1 = ((i1 < i2 && i1 >= 0) || (i2 < 0));
    const TCHAR* sep = bSep1 ? DEF_OP : CALC_OP;
    CStrSplitT<TCHAR> args;

    if ( args.Split(S, sep, 2) == 2 )
    {
      tstr& varName = args.Arg(0);
      tstr& varValue = args.Arg(1);
      DelLeadingTabSpaces(varName);
      DelTrailingTabSpaces(varName);
      DelLeadingTabSpaces(varValue);
      DelTrailingTabSpaces(varValue);

      CheckUserMacroVars(varValue);
      CheckEmptyMacroVars(varValue);
      
      const tMacroVars& userLocalMacroVars = GetUserLocalMacroVars();
      const tMacroVars& userMacroVars = GetUserMacroVars();
      IterateUserMacroVars<SubstituteMacroVarFunc>(userMacroVars, userLocalMacroVars, SubstituteMacroVarFunc(varValue));

      if ( !bSep1 )
      {
        StrCalc(varValue, m_pNppExec).Process();
      }

      bool bLocalVar = IsLocalMacroVar(varName);

      S = bLocalVar ? _T("local ") : _T("");
      S += varName;
      S += _T(" = ");
      S += varValue;

      if ( SetUserMacroVar(varName, varValue, false, bLocalVar) )
      {
        
          Runtime::GetLogger().AddEx( _T("; OK: %s%s = %s"), bLocalVar ? _T("local ") : _T(""), varName.c_str(), varValue.c_str() );

      }
      else
      {

        Runtime::GetLogger().AddEx( _T("; failed to set %s%s = %s"), bLocalVar ? _T("local ") : _T(""), varName.c_str(), varValue.c_str() );

      }
    }
    else
    {
      // "set $(var)" returns the value of var
      // "set" returns all vars and values
    }
  }
  else if ( nCmdType == CScriptEngine::CMDTYPE_UNSET )
  {
    
    Runtime::GetLogger().AddEx( _T("; %s command found"), CMD_UNSET );

    tstr varName = S;
    int k = varName.Find( _T("=") );
    if ( k >= 0 )  varName.SetSize(k);
    DelLeadingTabSpaces(varName);
    DelTrailingTabSpaces(varName);

    bool bLocalVar = IsLocalMacroVar(varName);
    if ( SetUserMacroVar(varName, S, true, bLocalVar) )
    {

      Runtime::GetLogger().AddEx( _T("; OK: %s%s has been removed"), bLocalVar ? _T("local ") : _T(""), varName.c_str() );

      tstr t = _T("- the user\'s ");
      if ( bLocalVar ) t += _T("local ");
      t += _T("variable has been removed: ");
      t += varName;
      m_pNppExec->GetConsole().PrintMessage( t.c_str() /*, false*/ );
    }
    else
    {

      Runtime::GetLogger().AddEx( _T("; failed to unset %s%s (no such user\'s variable)"), bLocalVar ? _T("local ") : _T(""), varName.c_str() );

      tstr t = _T("- no such user\'s ");
      if ( bLocalVar ) t += _T("local ");
      t += _T("variable: ");
      t += varName;
      m_pNppExec->GetConsole().PrintMessage( t.c_str(), false );
    }
  }
  else
  {
    const tMacroVars& userLocalMacroVars = GetUserLocalMacroVars();
    const tMacroVars& userMacroVars = GetUserMacroVars();
    IterateUserMacroVars<SubstituteMacroVarFunc>(userMacroVars, userLocalMacroVars, SubstituteMacroVarFunc(S));
  }

  Runtime::GetLogger().AddEx( _T("[out] \"%s\""), S.c_str() );
  Runtime::GetLogger().DecIndentLevel();
  Runtime::GetLogger().Add(   _T("}") );

}

void CNppExecMacroVars::CheckEmptyMacroVars(tstr& S, int nCmdType )
{
    
  Runtime::GetLogger().Add(   _T("CheckEmptyMacroVars()") );
  Runtime::GetLogger().Add(   _T("{") );
  Runtime::GetLogger().IncIndentLevel();
  Runtime::GetLogger().AddEx( _T("[in]  \"%s\""), S.c_str() );   
    
  if ( Runtime::GetOptions().GetBool(OPTB_CONSOLE_NOEMPTYVARS) )
  {
    
    Runtime::GetLogger().Add(   _T("; the function is enabled") );  
      
    CStrSplitT<TCHAR> args;
    if ( (nCmdType == CScriptEngine::CMDTYPE_SET) || 
         (nCmdType == CScriptEngine::CMDTYPE_UNSET) )
    {
      if ( args.Split(S, _T("="), 2) == 2 )
      {
        S = args.Arg(1);
      }
    }
    
    if ( S.length() > 0 )
    {
      int i;
      while ( (i = S.Find(_T("$("))) >= 0 )
      {
        int j = S.Find(_T(')'), i + 1);
        if ( j > 0 )
          S.Delete(i, j - i + 1);
        else
          S.Delete(i, -1);
      }
    }

    if ( (nCmdType == CScriptEngine::CMDTYPE_SET) || 
         (nCmdType == CScriptEngine::CMDTYPE_UNSET) )
    {
      if (args.GetArgCount() == 2)
      {
        S.Insert( 0, _T("=") );
        S.Insert( 0, args.Arg(0) );
      }
    }
  }
  else
  {
    
    Runtime::GetLogger().Add(   _T("; the function is disabled") );  

  }

  Runtime::GetLogger().AddEx( _T("[out] \"%s\""), S.c_str() );
  Runtime::GetLogger().DecIndentLevel();
  Runtime::GetLogger().Add(   _T("}") );

}

bool CNppExecMacroVars::CheckAllMacroVars(tstr& S, bool useLogging)
{
    if ( ContainsMacroVar(S) )
    {
        if ( !useLogging )
            Runtime::GetLogger().Activate(false);

        CheckNppMacroVars(S);
        CheckPluginMacroVars(S);
        CheckUserMacroVars(S);
        CheckEmptyMacroVars(S);

        if ( !useLogging )
            Runtime::GetLogger().Activate(true);

        return true;
    }

    return false;
}

bool CNppExecMacroVars::SetUserMacroVar(tstr& varName, const tstr& varValue, bool bRemove , bool bLocal )
{
  if ( varName.length() > 0 )
  {
    if ( varName.StartsWith(_T("$(")) == false )
      varName.Insert(0, _T("$("));
    if ( varName.GetLastChar() != _T(')') )
      varName.Append(_T(')'));
    
    ::CharUpper( varName.c_str() );

    tMacroVars& macroVars = bLocal ? GetUserLocalMacroVars() : GetUserMacroVars();
    tMacroVars::iterator itrVar = macroVars.find(varName);
    if ( itrVar != macroVars.end() )
    {
      // existing variable
      if ( !bRemove )  
        itrVar->second = varValue;
      else
        macroVars.erase(itrVar);
      return true;
    }
    else if ( !bRemove )
    {
      // new variable
      macroVars[varName] = varValue;
      return true;
    }
  }
  return false;
}


CNppExecMacroVars::StrCalc::StrCalc(tstr& varValue, CNppExec* pNppExec)
  : m_varValue(varValue), m_pNppExec(pNppExec), m_calcType(CT_FPARSER), m_pVar(0)
{
}

void CNppExecMacroVars::StrCalc::Process()
{
    m_calcType = StrCalc::CT_FPARSER;
    m_param.Clear();
        
    // check for 'strlen', 'strupper', 'strlower', 'substr' and so on
    m_pVar = m_varValue.c_str();
    m_pVar = get_param(m_pVar, m_param);
    if ( m_param.length() > 5 )
    {
        typedef struct sCalcType {
            const TCHAR* szCalcType;
            int nCalcType;
        } tCalcType;

        const int numCalcType = 10;
        static const tCalcType arrCalcType[numCalcType] = {
            { _T("STRLENUTF8"), CT_STRLENUTF8 },
            { _T("STRLENU"),    CT_STRLENUTF8 },
            { _T("STRLENA"),    CT_STRLEN     },
            { _T("STRLEN"),     CT_STRLEN     },
            { _T("STRUPPER"),   CT_STRUPPER   },
            { _T("STRLOWER"),   CT_STRLOWER   },
            { _T("SUBSTR"),     CT_SUBSTR     },
            { _T("STRFIND"),    CT_STRFIND    },
            { _T("STRRFIND"),   CT_STRRFIND   },
            { _T("STRREPLACE"), CT_STRREPLACE }
        };

        ::CharUpper( m_param.c_str() );

        for ( int i = 0; i < numCalcType && m_calcType == CT_FPARSER; i++ )
        {
            if ( m_param == arrCalcType[i].szCalcType )
            {
                m_calcType = arrCalcType[i].nCalcType;
            }
        }
    }

    if ( m_calcType == CT_FPARSER )
    {
        // calc
        calcFParser();
    }
    else if ( m_calcType == CT_STRLEN || m_calcType == CT_STRLENUTF8 )
    {
        calcStrLen();
    }
    else if ( m_calcType == CT_STRUPPER || m_calcType == CT_STRLOWER )
    {
        calcStrCase();
    }
    else if ( m_calcType == CT_SUBSTR )
    {
        calcSubStr();
    }
    else if ( m_calcType == CT_STRFIND || m_calcType == CT_STRRFIND )
    {
        calcStrFind();
    }
    else if ( m_calcType == CT_STRREPLACE )
    {
        calcStrRplc();
    }
}

void CNppExecMacroVars::StrCalc::calcFParser()
{
    tstr calcError;

    if ( calc2(m_varValue, calcError, m_varValue) )
    {
      
        Runtime::GetLogger().AddEx( _T("; fparser calc result: %s"), m_varValue.c_str() );
    
    }
    else
    {
        calcError.Insert( 0, _T("- fparser calc error: ") );
        m_pNppExec->GetConsole().PrintError( calcError.c_str() );
    }
}

void CNppExecMacroVars::StrCalc::calcStrLen()
{
    int len = 0;

    if ( *m_pVar )
    {
        if ( m_calcType == CT_STRLEN )
        {
            len = GetStrUnsafeLength(m_pVar);
        }
        else
        {
            #ifdef UNICODE
                len = ::WideCharToMultiByte(CP_UTF8, 0, m_pVar, -1, NULL, 0, NULL, NULL);
            #else
                wchar_t* pwVar = SysUniConv::newMultiByteToUnicode(m_pVar);
                len = ::WideCharToMultiByte(CP_UTF8, 0, pwVar, -1, NULL, 0, NULL, NULL);
                delete [] pwVar;
            #endif

            --len; // without trailing '\0'
        }
    }

    TCHAR szNum[50];
    c_base::_tint2str( len, szNum );
    m_varValue = szNum;

    Runtime::GetLogger().AddEx( 
      _T("; strlen%s: %s"), 
      (m_calcType == CT_STRLEN) ? _T("") : _T("utf8"),
      m_varValue.c_str() 
    );

}

void CNppExecMacroVars::StrCalc::calcStrCase()
{
    if ( *m_pVar )
    {
        m_varValue = m_pVar;
        if ( m_calcType == CT_STRUPPER )
            ::CharUpper( m_varValue.c_str() );
        else
            ::CharLower( m_varValue.c_str() );

        Runtime::GetLogger().AddEx( 
          _T("; %s: %s"), 
          (m_calcType == CT_STRUPPER) ? _T("strupper") : _T("strlower"),
          m_varValue.c_str() 
        );

    }
}

void CNppExecMacroVars::StrCalc::calcSubStr()
{
    if ( *m_pVar )
    {
        m_pVar = get_param(m_pVar, m_param);
        if ( isDecNumChar(m_param.GetAt(0)) || 
             (m_param.GetAt(0) == _T('-') && isDecNumChar(m_param.GetAt(1))) )
        {
            int pos = c_base::_tstr2int( m_param.c_str() );
            if ( *m_pVar )
            {
                m_pVar = get_param(m_pVar, m_param);
                if ( isDecNumChar(m_param.GetAt(0)) ||
                     (m_param.GetAt(0) == _T('-')) )
                {
                    int len = lstrlen(m_pVar);
                    int count = (m_param == _T("-")) ? len : c_base::_tstr2int( m_param.c_str() );

                    if ( pos < 0 )
                    {
                        // get pos characters from the end of string
                        pos += len;
                        if ( pos < 0 )
                            pos = 0;
                    }
                    else
                    {
                        if ( pos > len )
                            count = 0;
                    }

                    if ( count < 0 )
                    {
                        count += (len - pos);
                        if ( count < 0 )
                            count = 0;
                    }

                    if ( count > len - pos )
                    {
                        count = len - pos;
                    }

                    if ( count == 0 )
                        m_varValue.Clear();
                    else
                        m_varValue.Copy(m_pVar + pos, count);

                    Runtime::GetLogger().AddEx( 
                      _T("; substr: %s"), 
                      m_varValue.c_str() 
                    );

                }
                else
                {
                    m_pNppExec->GetConsole().PrintError( _T("- failed to get 2nd parameter of SUBSTR: a number or \'-\' expected") );
                }
            }
            else
            {
                m_pNppExec->GetConsole().PrintError( _T("- failed to get 2nd parameter of SUBSTR") );
            }
        }
        else
        {
            m_pNppExec->GetConsole().PrintError( _T("- failed to get 1st parameter of SUBSTR: a number expected") );
        }
    }
    else
    {
        m_pNppExec->GetConsole().PrintError( _T("- failed to get 1st parameter of SUBSTR") );
    }
}

void CNppExecMacroVars::StrCalc::calcStrFind()
{
    CStrSplitT<TCHAR> args;

    const int n = args.SplitToArgs(m_pVar);
    if ( n == 2 )
    {
        const tstr& S = args.GetArg(0);
        const tstr& SFind = args.GetArg(1);
        int pos = (m_calcType == CT_STRFIND) ? S.Find(SFind) : S.RFind(SFind);

        TCHAR szNum[50];
        c_base::_tint2str( pos, szNum );
        m_varValue = szNum;

        Runtime::GetLogger().AddEx( 
          _T("; %s: %s"), 
          (m_calcType == CT_STRFIND) ? _T("strfind") : _T("strrfind"),
          m_varValue.c_str() 
        );

    }
    else if ( n < 2 )
    {
        m_pNppExec->GetConsole().PrintError( _T("- not enough parameters given: 2 parameters expected") );
    }
    else
    {
        m_pNppExec->GetConsole().PrintError( _T("- too much parameters given: 2 parameters expected") );
        m_pNppExec->GetConsole().PrintError( _T("- try to enclose the parameters with quotes, e.g. \"s\" \"sfind\"") );
    }
}

void CNppExecMacroVars::StrCalc::calcStrRplc()
{
    CStrSplitT<TCHAR> args;

    const int n = args.SplitToArgs(m_pVar);
    if ( n == 3 )
    {
        const tstr& SFind = args.GetArg(1);
        const tstr& SReplace = args.GetArg(2);
        m_varValue = args.GetArg(0);
        m_varValue.Replace( SFind.c_str(), SReplace.c_str() );

        Runtime::GetLogger().AddEx( 
          _T("; strreplace: %s"), 
          m_varValue.c_str() 
        );

    }
    else if ( n < 3 )
    {
        m_pNppExec->GetConsole().PrintError( _T("- not enough parameters given: 3 parameters expected") );
    }
    else
    {
        m_pNppExec->GetConsole().PrintError( _T("- too much parameters given: 3 parameters expected") );
        m_pNppExec->GetConsole().PrintError( _T("- try to enclose the parameters with quotes, e.g. \"s\" \"sfind\" \"sreplace\"") );
    }
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

DWORD WINAPI dwCreateConsoleProcess(LPVOID)
{
    CScriptEngine scriptEngine(&Runtime::GetNppExec());

    scriptEngine.Run();

    if ( Runtime::GetNppExec().m_CmdList.GetCount() > 0 )
    {
        const int nLastCmdType = scriptEngine.GetLastCmdType();
        if ( nLastCmdType != CScriptEngine::CMDTYPE_CLS )
        {
            if ( (nLastCmdType != CScriptEngine::CMDTYPE_NPPCONSOLE) ||
                 (CScriptEngine::getOnOffParam(scriptEngine.GetLastCmdParams()) != CScriptEngine::PARAM_ENABLE) || 
                 (Runtime::GetNppExec().GetConsole().IsOutputEnabledN() > 1) )
            {
                Runtime::GetNppExec().GetConsole().PrintMessage( _T("================ READY ================"), false );
                ConsoleDlg::GoToError_nCurrentLine = -1;
            }
            Runtime::GetNppExec().GetConsole().RestoreDefaultTextStyle(true);
        }
    }
  
    Runtime::GetNppExec().CloseScriptThreadHandle();
    NotifyExternalPluginResult(NPE_RESULT_OK);
    
    if ( Runtime::GetNppExec().m_SuspendedScriptAction.isActive() )
    {
        Runtime::GetNppExec().m_SuspendedScriptAction.stopWaitingTimer();

        CommunicationInfo* pci = new CommunicationInfo;
        if ( pci )
        {
            pci->internalMsg = NPEM_SUSPENDEDACTION;
            pci->srcModuleName = Runtime::GetNppExec().getPluginDllName();
            pci->info = NULL;
          
            ::PostMessage( Runtime::GetNppExec().m_nppData._nppHandle, NPPM_MSGTOPLUGIN,
                (WPARAM) Runtime::GetNppExec().getPluginDllName(), (LPARAM) pci );
        }
    }
    
    return 0;
}


LRESULT CALLBACK nppPluginWndProc(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam)
{
  if (uMessage == WM_CLOSE)
  {
    if (Runtime::GetNppExec()._consoleProcessIsRunning)
    {
      int nClose = MessageBox(hWnd, _T("Child process is still active.\n") \
        _T("It will be terminated if you close Notepad++.\nClose the program?"), 
        _T("Notepad++ : NppExec Warning"), MB_YESNO | MB_ICONWARNING);
      
      if (nClose == IDYES)
      {
        Runtime::GetNppExec()._consoleProcessBreak = true;
      }
      else
      {
        if ( !Runtime::GetNppExec().isConsoleDialogVisible() )
          Runtime::GetNppExec().showConsoleDialog(false);
        ::SetFocus( Runtime::GetNppExec().GetConsole().GetConsoleWnd() );
        return 0;
      }

    }
  }
  
#ifdef UNICODE
  return ::CallWindowProcW(nppOriginalWndProc, hWnd, uMessage, wParam, lParam);
#else
  return ( g_bIsNppUnicode ?
             ::CallWindowProcW(nppOriginalWndProc, hWnd, uMessage, wParam, lParam) :
                 ::CallWindowProcA(nppOriginalWndProc, hWnd, uMessage, wParam, lParam) );
#endif
}


int FileFilterPos(const TCHAR* szFilePath)
{
  if ( !szFilePath )
    return -1;

  const TCHAR* p = szFilePath;
  while ( *p )
  {
    if ( (*p == _T('*')) || (*p == _T('?')) )
      return ( (int) (p - szFilePath) );
    ++p;
  }
  return -1;
}

void GetPathAndFilter(const TCHAR* szPathAndFilter, 
       int nFilterPos, tstr& out_Path, tstr& out_Filter)
{
  TCHAR path[FILEPATH_BUFSIZE];

  out_Path.Clear();
  int pos = nFilterPos;
  while ( --pos >= 0 )
  {
    if ( (szPathAndFilter[pos] == _T('\\')) || 
         (szPathAndFilter[pos] == _T('/')) )
    {
      out_Path.Copy( szPathAndFilter, pos );
      break;
    }
  }

  if ( (nFilterPos == 1) && 
       (szPathAndFilter[0] == _T('\\') || szPathAndFilter[0] == _T('/')) )
  {
    out_Path.Copy( szPathAndFilter, 1 );
  }
  
  if ( nFilterPos >= 0)
  {
    if ( pos < 0 )  pos = -1;
    out_Filter.Copy( szPathAndFilter + 1 + pos );
  }
  else
  {
    out_Path = szPathAndFilter;
    out_Filter = _T("*");
  }
  
  if ( out_Path.IsEmpty() )
  {
    path[0] = 0;
    GetCurrentDirectory( FILEPATH_BUFSIZE - 1, path );
    out_Path = path;
  }
  else if ( !isFullPath(out_Path) )
  {
    // is not full path
    path[0] = 0;
    GetCurrentDirectory( FILEPATH_BUFSIZE - 1, path );
    if ( (out_Path.GetAt(0) == _T('\\') && out_Path.GetAt(1) != _T('\\')) || 
         (out_Path.GetAt(0) == _T('/') && out_Path.GetAt(1) != _T('/')) )
    {
      path[2] = 0;
    }
    else
    {
      int len = lstrlen( path );
      if ( path[len - 1] != _T('\\') )
      {
        path[len++] = _T('\\');
        path[len] = 0;
      }
    }
    out_Path.Insert( 0, path );
  }
}

int FilterOK(const TCHAR* mask, const TCHAR* str)
{
  tstr S = str;
  if (S.length() > 0)  ::CharLower( S.c_str() );
  return c_base::_tmatch_mask( mask, S.c_str() );
}

void GetFilePathNamesList(const TCHAR* szPath, const TCHAR* szFilter, 
       CListT<tstr>& FilesList)
{
    FilesList.DeleteAll();
  
    if ( (!szPath) || (szPath[0] == 0) )
        return;

    tstr S = szPath;
    if ( S.GetLastChar() != _T('\\') )  S += _T('\\');
    S += _T("*.*");

    CDirFileLister FileLst;

    if ( !FileLst.FindNext(S.c_str(), 
            CDirFileLister::ESF_FILES | CDirFileLister::ESF_SORTED) )
        return;
  
    tstr  Fltr;
    tstr  FilePathName;
    bool  bFilter = (szFilter && (szFilter[0] != 0)) ? true : false;
    if ( bFilter )
    {
        Fltr = szFilter;
        ::CharLower( Fltr.c_str() );
    }
    S.SetSize( S.length() - 3 ); // removing last "*.*"

    const TCHAR* pszFileName;
    unsigned int len;
    
    do
    {
        pszFileName = FileLst.GetItem(&len);
        if ( (!bFilter) || FilterOK(Fltr.c_str(), pszFileName) )
        {
            FilePathName = S;
            FilePathName.Append( pszFileName, len );
            FilesList.Add( FilePathName );
        }
    }
    while ( FileLst.GetNext() );
  
}

void PrintDirContent(CNppExec* pNppExec, const TCHAR* szPath, const TCHAR* szFilter)
{
    if ( (!szPath) || (szPath[0] == 0) )
        return;

    tstr S = szPath;
    if ( S.GetLastChar() != _T('\\') )  S += _T('\\');
    S += _T("*.*");

    CDirFileLister DirFileLst;

    if ( !DirFileLst.FindNext(S.c_str(), 
            CDirFileLister::ESF_DIRS | CDirFileLister::ESF_FILES | 
              CDirFileLister::ESF_PLACEDIRSFIRST | CDirFileLister::ESF_SORTED) )
        return;

    tstr  Fltr;
    bool  bFilter = (szFilter && (szFilter[0] != 0)) ? true : false;
    if ( bFilter )
    {
        Fltr = szFilter;
        ::CharLower( Fltr.c_str() );
    }

    const TCHAR* pszFileName;
    unsigned int len;
    bool         isDir;

    do
    {
        pszFileName = DirFileLst.GetItem(&len, &isDir);
        if ( (!bFilter) || FilterOK(Fltr.c_str(), pszFileName) )
        {
            if ( isDir )
            {
                S = _T("<DIR> ");
                S.Append( pszFileName, len );
                pszFileName = S.c_str();
            }
            pNppExec->GetConsole().PrintStr( pszFileName, true );
        }
    }
    while ( DirFileLst.GetNext() );

}

void runInputBox(CNppExec* pNppExec, const TCHAR* szMessage)
{
    const int MAX_VAR_FIELDS = 20;

    TCHAR szVarFldName[40];
    tstr  varName;
    tstr  S;

    Runtime::GetLogger().Add(   _T("runInputBox()") );
    Runtime::GetLogger().Add(   _T("{") );
    Runtime::GetLogger().IncIndentLevel();
    Runtime::GetLogger().AddEx( _T("[in]  %s"), szMessage );
    
    // init the InputBox dialog values
    InputBoxDlg.m_InputMessage = szMessage;
    InputBoxDlg.m_InputVarName = MACRO_INPUT;
    InputBoxDlg.m_InputVarName += _T(" =");
    InputBoxDlg.setInputBoxType(CInputBoxDlg::IBT_INPUTBOX);
    
    // show the InputBox dialog
    pNppExec->PluginDialogBox(IDD_INPUTBOX, InputBoxDlgProc);

    Runtime::GetLogger().Add(   _T("; InputBox returned") );

    // preparing the output value
    pNppExec->GetMacroVars().CheckAllMacroVars(InputBoxDlg.m_OutputValue, true);

    // removing previous values
    varName = MACRO_INPUT;
    pNppExec->GetMacroVars().SetUserMacroVar(varName, _T(""), true);
    for ( int i = 1; i <= MAX_VAR_FIELDS; i++ )
    {
        wsprintf(szVarFldName, MACRO_INPUTFMT, i);
        varName = szVarFldName;
        pNppExec->GetMacroVars().SetUserMacroVar(varName, _T(""), true);
    }
    
    // getting new values
    CStrSplitT<TCHAR> fields;
    int nFields = fields.SplitToArgs(InputBoxDlg.m_OutputValue, MAX_VAR_FIELDS);

    // setting new values
    varName = MACRO_INPUT;
    pNppExec->GetMacroVars().SetUserMacroVar(varName, InputBoxDlg.m_OutputValue);
    S = varName;
    S += _T(" = ");
    S += InputBoxDlg.m_OutputValue;
    pNppExec->GetConsole().PrintMessage( S.c_str(), true );

    Runtime::GetLogger().AddEx( _T("[out] %s"), S.c_str() );

    for ( int i = 1; i <= nFields; i++ )
    {
        wsprintf(szVarFldName, MACRO_INPUTFMT, i);
        varName = szVarFldName;
        pNppExec->GetMacroVars().SetUserMacroVar(varName, fields.GetArg(i - 1));
        S = varName;
        S += _T(" = ");
        S += fields.GetArg(i - 1);
        pNppExec->GetConsole().PrintMessage( S.c_str(), true );

        Runtime::GetLogger().AddEx( _T("[out] %s"), S.c_str() );

    }

    Runtime::GetLogger().DecIndentLevel();
    Runtime::GetLogger().Add(   _T("}") );
    
    
    // restore the focus
    if ( (pNppExec->m_hFocusedWindowBeforeScriptStarted == pNppExec->GetConsole().GetConsoleWnd()) ||
         (pNppExec->m_hFocusedWindowBeforeScriptStarted == pNppExec->GetConsole().GetParentWnd()) )
    {
        if ( ::IsWindowVisible(pNppExec->GetConsole().GetParentWnd()) )
        {
            pNppExec->SendNppMsg(NPPM_DMMSHOW, 0, (LPARAM) pNppExec->GetConsole().GetParentWnd());
        }
    }
}

char* SciTextFromLPCTSTR(LPCTSTR pText, HWND hSci)
{
    char* pOutSciText = NULL;
    int   nSciCodePage = (int) ::SendMessage(hSci, SCI_GETCODEPAGE, 0, 0);

    switch ( nSciCodePage )
    {
        case 0:          // ANSI
            #ifdef UNICODE
                pOutSciText = SysUniConv::newUnicodeToMultiByte(pText, -1, CP_ACP);
            #else
                pOutSciText = (char *) pText;
            #endif
            break;

        case SC_CP_UTF8: // UTF-8
            #ifdef UNICODE
                pOutSciText = SysUniConv::newUnicodeToUTF8(pText, -1);
            #else
                pOutSciText = SysUniConv::newMultiByteToUTF8(pText, -1, CP_ACP);
            #endif
            break;

        default:         // multi-byte encoding
            #ifdef UNICODE
                pOutSciText = SysUniConv::newUnicodeToMultiByte(pText, -1, CP_ACP);
            #else
                pOutSciText = (char *) pText;
            #endif
            break;
    }

    return pOutSciText;
}

LPTSTR SciTextToLPTSTR(const char* pSciText, HWND hSci)
{
    LPTSTR pOutText = NULL;
    int    nSciCodePage = (int) ::SendMessage(hSci, SCI_GETCODEPAGE, 0, 0);

    switch ( nSciCodePage )
    {
        case SC_CP_UTF8:
            #ifdef UNICODE
                pOutText = SysUniConv::newUTF8ToUnicode(pSciText);
            #else
                pOutText = SysUniConv::newUTF8ToMultiByte(pSciText, -1, CP_ACP);
            #endif
            break;

        default:
            #ifdef UNICODE
                pOutText = SysUniConv::newMultiByteToUnicode(pSciText, -1, CP_ACP);
            #else
                pOutText = (char *) pSciText;
            #endif
            break;
    }

    return pOutText;
}
