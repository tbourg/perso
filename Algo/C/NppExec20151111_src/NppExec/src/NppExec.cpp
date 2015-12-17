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

/****************************************************************************
 * NppExec plugin ver. 0.5.9.9 for Notepad++
 * by DV <dvv81 @ ukr.net>, December 2006 - November 2015
 * Powered by Function Parser (C) Juha Nieminen, Joel Yliluoma
 ****************************************************************************
 *
 * Abilities:
 *   1) Run multiple commands from the "Execute..." dialog
 *   2) Run stand-alone command from the Console Dlg
 *   3) Separate console for each NppExec.dll (e.g. NppExec1.dll, NppExec2.dll)
 *   4) Additional commands:
 *        cls - clear Console screen
 *        cd - shows current path
 *        cd <path> - changes current directory (absolute or relative)
 *        cd <drive:\path> - changes current drive and directory
 *        dir - lists subdirs and files
 *        dir <mask> - lists subdirs and files matched the mask
 *        dir <path\mask> - lists subdirs and files matched the mask
 *        echo <text> - prints a text in the Console 
 *        if <condition> goto <label> - jumps to the label if the condition is true
 *        goto <label> - jumps to the label
 *        set - shows all user's variables
 *        set <var> - shows the value of user's variable <var>
 *        set <var> = <value> - sets the value of user's variable <var>
 *        set <var> ~ <math expression> - calculates the math expression
 *        set <var> ~ strlen <string> - calculates the string length
 *        set <var> ~ strlenutf8 <string> - calculates the UTF-8 string length
 *        set <var> ~ strupper <string> - returns the string in upper case
 *        set <var> ~ strlower <string> - returns the string in lower case
 *        set <var> ~ substr <pos> <len> <string> - returns the substring
 *        set <var> ~ strfind <s> <t> - returns the first position of <t> in <s>
 *        set <var> ~ strrfind <s> <t> - returns the last position of <t> in <s>
 *        set <var> ~ strreplace <s> <t0> <t1> - replaces all <t0> with <t1>
 *        set local - shows all user's local variables
 *        set local <var> - shows the value of user's local variable <var>
 *        set local <var> = ... - sets the value of user's local variable <var>
 *        set local <var> ~ ... - calculates the value of user's local variable
 *        unset <var> - removes user's variable <var>
 *        unset local <var> - removes user's local variable <var>
 *        env_set <var> - shows the value of environment variable <var>
 *        env_set <var> = <value> - sets the value of environment variable <var>
 *        env_unset <var> - removes/restores the environment variable <var>
 *        inputbox "message" - shows InputBox, sets $(INPUT)
 *        inputbox "message" : initial_value - InputBox, sets $(INPUT)
 *        inputbox "message" : "value_name" : initial_value - InputBox customization
 *        con_colour <colours> - sets the Console's colours
 *        con_filter <filters> - enables/disables the Console's output filters
 *        con_loadfrom <file> - loads a file's content to the Console
 *        con_load <file> - see "con_loadfrom"
 *        con_saveto <file> - saves the Console's content to a file
 *        con_save - see "con_saveto"
 *        sel_loadfrom <file> - replace current selection with a file's content
 *        sel_load <file> - see "sel_loadfrom"
 *        sel_saveto <file> - save the selected text to a file
 *        sel_saveto <file> : <encoding> - save the selected text to a file
 *        sel_save <file> : <encoding> - see "sel_saveto"
 *        sel_settext <text> - replace current selection with the text specified
 *        sel_settext+ <text> - replace current selection with the text specified
 *        npp_exec <script> - execute commands from specified script
 *        npp_exec <file> - execute commands from specified file           (*)
 *        npp_close - close current file in Notepad++
 *        npp_close <file> - close specified file opened in Notepad++      (*)
 *        npp_console <on/off/keep> - show/hide the Console window
 *        npp_console <enable/disable> - enable/disable output to the Console
 *        npp_console <1/0/?> - show/hide the Console window
 *        npp_console <+/-> - enable/disable output to the Console
 *        npp_menucommand <menu\item\name> - executes (invokes) a menu item
 *        npp_open <file> - open a file in Notepad++
 *        npp_open <mask> - open files matched the mask
 *        npp_open <path\mask> - open files matched the mask
 *        npp_run <command> - run external process/command
 *        npp_save - save current file in Notepad++
 *        npp_save <file> - save a file in Notepad++ (if it's opened)      (*)
 *        npp_saveas <file> - save current file with a new (path)name
 *        npp_saveall - save all modified files
 *        npp_switch <file> - switch to specified opened file              (*)
 *        npp_sendmsg <msg> - send a message (msg) to Notepad++
 *        npp_sendmsg <msg> <wparam> - message with parameter (wparam)
 *        npp_sendmsg <msg> <wparam> <lparam> - msg to Notepad++
 *        npp_sendmsgex <hwnd> <msg> <wparam> <lparam> - msg to hwnd
 *        sci_sendmsg <msg> - send a message (msg) to current Scintilla
 *        sci_sendmsg <msg> <wparam> - message with parameter (wparam)
 *        sci_sendmsg <msg> <wparam> <lparam> - msg to Scintilla
 *        npe_cmdalias - show all command aliases
 *        npe_cmdalias <alias> - shows the value of command alias
 *        npe_cmdalias <alias> = - removes the command alias
 *        npe_cmdalias <alias> = <command> - sets the command alias
 *        npe_console <options> - set/modify Console options/mode
 *        npe_debuglog <on/off> - enable/disable Debug Log
 *        npe_noemptyvars <1/0> - enable/disable replacement of empty vars
 *        (*) these commands work with a partial file path/name also
 *            i.e.  npp_save c:\dir\f.txt  is the same as  npp_save f.txt
 *   5) Additional console commands (Console Dlg only):
 *        help       - show available commands
 *        ver        - show plugin's version
 *        manual     - show NppExec User Guide
 *        about      - show NppExec Help/About
 *        CTRL+C     - terminate current child process
 *        CTRL+BREAK - terminate current child process
 *        CTRL+Z     - send ^Z to current child process
 *   6) All Notepad++ environment variables are supported:
 *        $(FULL_CURRENT_PATH)  : E:\my Web\main\welcome.html 
 *        $(CURRENT_DIRECTORY)  : E:\my Web\main
 *        $(FILE_NAME)          : welcome.html 
 *        $(NAME_PART)          : welcome 
 *        $(EXT_PART)           : .html
 *        $(NPP_DIRECTORY)      : the full path of notepad++'s directory
 *        $(CURRENT_WORD)       : word(s) you selected in Notepad++
 *        $(CURRENT_LINE)       : current line number
 *        $(CURRENT_COLUMN)     : current column number
 *   7) Additional environment variables:
 *        $(#0)                 : C:\Program Files\Notepad++\notepad++.exe
 *        $(#N), N=1,2,3...     : full path of the Nth opened document
 *        $(LEFT_VIEW_FILE)     : current file path-name in primary (left) view
 *        $(RIGHT_VIEW_FILE)    : current file path-name in second (right) view
 *        $(PLUGINS_CONFIG_DIR) : full path of the plugins configuration directory
 *        $(CWD)                : current working directory of NppExec (use "cd" to change it)
 *        $(ARGC)               : number of arguments passed to the NPP_EXEC command
 *        $(ARGV)               : all arguments passed to the NPP_EXEC command after the script name
 *        $(ARGV[0])            : script name - first parameter of the NPP_EXEC command
 *        $(ARGV[N])            : Nth argument (N=1,2,3...)
 *        $(RARGV)              : all arguments in reverse order (except the script name)
 *        $(RARGV[N])           : Nth argument in reverse order (N=1,2,3...)
 *        $(INPUT)              : this value is set by the 'inputbox' command
 *        $(INPUT[N])           : Nth field of the $(INPUT) value (N=1,2,3...)
 *        $(OUTPUT)             : this value can be set by the child process, see npe_console v+
 *        $(OUTPUT1)            : first line in $(OUTPUT)
 *        $(OUTPUTL)            : last line in $(OUTPUT)
 *        $(EXITCODE)           : exit code of the last executed child process
 *        $(MSG_RESULT)         : result of 'npp_sendmsg[ex]' or 'sci_sendmsg'
 *        $(MSG_WPARAM)         : wParam (output) of 'npp_sendmsg[ex]' or 'sci_sendmsg'
 *        $(MSG_LPARAM)         : lParam (output) of 'npp_sendmsg[ex]' or 'sci_sendmsg'
 *        $(SYS.<var>)          : system's environment variable, e.g. $(SYS.PATH)
 *
 ****************************************************************************/

// actually, the following line does nothing :-)
// #define _TEST_ONLY_

#include "NppExec.h"
#include "NppExecEngine.h"
#include "resource.h"
#include "DlgDoExec.h"
#include "DlgConsole.h"
#include "DlgHelpAbout.h"
#include "DlgAdvancedOptions.h"
#include "DlgInputBox.h"
#include "DlgConsoleEncoding.h"
#include "cpp/CFileBufT.h"
#include "cpp/StrSplitT.h"
#include "c_base/MatchMask.h"
#include "c_base/HexStr.h"
#include "CFileModificationWatcher.h"
#include "encodings/SysUniConv.h"
#include "c_base/str_func.h"

#include "PluginInterface.h"  // docking feature
#include "Docking.h"          // docking feature
#include "menuCmdID.h"

#ifndef __MINGW32__
  #include <HtmlHelp.h>
#endif

#include <shellapi.h>
#include <list>
#include <time.h>


#define  MAX_PLUGIN_NAME  60
TCHAR PLUGIN_NAME[MAX_PLUGIN_NAME] = _T("NppExec");
TCHAR PLUGIN_NAME_DLL[MAX_PLUGIN_NAME + 6] = _T("NppExec.dll");
TCHAR INI_FILENAME[MAX_PLUGIN_NAME + 6] = _T("NppExec.ini");

const TCHAR SZ_CONSOLE_HELP_INFO[] = _T("NppExec Help Info:") _T_RE_EOL \
  _T("- You can execute commands and scripts directly from the Console window.") _T_RE_EOL \
  _T("- Commands are case-insensitive (i.e. NPP_OPEN is the same as Npp_Open).") _T_RE_EOL \
  _T("- Type HELP in the Console window to see all available commands and environment variables.") _T_RE_EOL \
  _T("- Type HELP <COMMAND> to get detailed information about specified command (e.g. HELP NPP_EXEC).") _T_RE_EOL \
  _T("- You can find additional information inside these files: \"NppExec.txt\" and \"NppExec_TechInfo.txt\".") _T_RE_EOL \
  _T("- You can disable this Help Info in NppExec\'s Advanced Options.") _T_RE_EOL \
  _T("================ READY ================");

const TCHAR INI_SECTION_CONSOLE[]          = _T("Console");
const TCHAR INI_SECTION_OPTIONS[]          = _T("Options");
const TCHAR INI_SECTION_CONSOLEFILTER[]    = _T("ConsoleOutputFilter");
const TCHAR INI_SECTION_FILTERSEXCLUDE[]   = _T("FiltersExclude");
const TCHAR INI_SECTION_FILTERSINCLUDE[]   = _T("FiltersInclude");
const TCHAR INI_SECTION_FILTERSREPLACE[]   = _T("FiltersReplace");
const TCHAR INI_SECTION_FILTERSHIGHLIGHT[] = _T("FiltersHighlight");
const TCHAR INI_SECTION_USERMENU[]         = _T("UserMenu");
const TCHAR INI_SECTION_INPUTBOX[]         = _T("InputBox");
const TCHAR INI_SECTION_EXITBOX[]          = _T("ExitBox");

#ifdef _GVCRESTORE
const TCHAR INI_SECTION_RESTORE[]          = _T("Restore");
#endif

const TCHAR SCRIPTFILE_TEMP[]              = _T("npes_temp.txt");
const TCHAR SCRIPTFILE_SAVED[]             = _T("npes_saved.txt");
TCHAR       CMDHISTORY_FILENAME[100]       = _T("npec_cmdhistory.txt\0");
const TCHAR PROP_NPPEXEC_DLL[]             = _T("NppExec_dll_exists");

const int   DEFAULT_STARTUPTIMEOUT_MS      = 240;
const int   DEFAULT_CYCLETIMEOUT_MS        = 120;
const int   DEFAULT_EXITTIMEOUT_MS         = CSuspendedScriptAction::MAX_TIMEOUT_MS;
const int   DEFAULT_PATH_AUTODBLQUOTES     = 0;
const int   DEFAULT_CMDHISTORY_MAXITEMS    = 256;
const int   DEFAULT_EXEC_MAXCOUNT          = 100;
const int   DEFAULT_GOTO_MAXCOUNT          = 10000;
const int   DEFAULT_RICHEDIT_MAXTEXTLEN    = 4*1024*1024; // 4 MB
const int   DEFAULT_SENDMSG_MAXBUFLEN      = 4*1024*1024; // 4 M symbols
const TCHAR DEFAULT_COMMENTDELIMITER[]     = _T("//");
const TCHAR DEFAULT_HELPFILE[]             = _T("doc\\NppExec_Manual.chm");
const TCHAR DEFAULT_LOGSDIR[]              = _T("");
const TCHAR DEFAULT_SCRIPTSDIR[]           = _T("");
const int   DEFAULT_AUTOSAVE_SECONDS       = 0; // disabled (example: 5*60 = 5 minutes)

const wchar_t DEFAULT_NULCHAR_UNICODE     = 0x25E6; // 0x25E6 - the "White Bullet" symbol
const char    DEFAULT_NULCHAR_ANSI        = 0x17; // 0x17 - the "End of Text Block" symbol

const CStaticOptionsManager::OPT_ITEM optArray[OPT_COUNT] = {
    // --- read-write options ---

    // [Options]
    { OPTS_PLUGIN_HOTKEY, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_OPTIONS, _T("HotKey"), 0, _T("F6") },
    { OPTI_TOOLBARBTN,    OPTT_INT | OPTF_READWRITE,
      INI_SECTION_OPTIONS, _T("ToolbarBtn"), 1, NULL },
    { OPTB_WATCHSCRIPTFILE, OPTT_BOOL | OPTF_READWRITE,
      INI_SECTION_OPTIONS, _T("WatchScriptFile"), 1, NULL },
    { OPTS_SCRIPT_NPPSTART, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_OPTIONS, _T("ScriptNppStart"), 0, NULL },
    { OPTS_SCRIPT_NPPEXIT, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_OPTIONS, _T("ScriptNppExit"), 0, NULL },
    // [Console]
    { OPTI_CONSOLE_VISIBLE, OPTT_INT | OPTF_READWRITE,
      INI_SECTION_CONSOLE, _T("Visible"), -1, NULL },
    { OPTB_CONSOLE_SHOWHELP, OPTT_INT | OPTF_READWRITE,
      INI_SECTION_CONSOLE, _T("ShowHelp"), -1, NULL },
    { OPTU_CONSOLE_ENCODING, OPTT_INT | OPTF_READWRITE,
      INI_SECTION_CONSOLE, _T("Encoding"), -1, NULL },
    { OPTB_CONSOLE_WORDWRAP, OPTT_BOOL | OPTF_READWRITE, 
      INI_SECTION_CONSOLE, _T("WordWrap"), 0, NULL },
    { OPTB_CONSOLE_HIDETOGGLED, OPTT_BOOL | OPTF_READWRITE,
      INI_SECTION_CONSOLE, _T("HideToggled"), 0, NULL },
    //#ifdef _GVC
    { OPTB_CONSOLE_SAVEONEXECUTE, OPTT_INT | OPTF_READWRITE,
      INI_SECTION_CONSOLE, _T("SaveOnExecute"), -1, NULL },
    //#endif 
    { OPTB_CONSOLE_CDCURDIR, OPTT_INT | OPTF_READWRITE,
      INI_SECTION_CONSOLE, _T("CdCurDir"), 0, NULL },
    { OPTB_CONSOLE_CMDHISTORY, OPTT_INT | OPTF_READWRITE,
      INI_SECTION_CONSOLE, _T("CmdHistory"), -1, NULL },
    { OPTB_CONSOLE_SAVECMDHISTORY, OPTT_INT | OPTF_READWRITE,
      INI_SECTION_CONSOLE, _T("SaveCmdHistory"), -1, NULL },

    { OPTI_RICHEDIT_MAXTEXTLEN, OPTT_INT | OPTF_READWRITE,
      INI_SECTION_CONSOLE, _T("RichEdit_MaxTextLength"),
      DEFAULT_RICHEDIT_MAXTEXTLEN, NULL },
    { OPTI_SENDMSG_MAXBUFLEN, OPTT_INT | OPTF_READWRITE,
      INI_SECTION_CONSOLE, _T("SendMsg_MaxBufLength"),
      DEFAULT_SENDMSG_MAXBUFLEN, NULL },
    { OPTS_CALC_PRECISION, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_CONSOLE, _T("Calc_Precision"),
      0, NULL }, // see "NppExecEngine.cpp", calc_precision
    { OPTS_COMMENTDELIMITER, OPTT_STR | OPTF_READWRITE, 
      INI_SECTION_CONSOLE, _T("CommentDelimiter"), 
      0, DEFAULT_COMMENTDELIMITER },

    { OPTD_COLOR_TEXTNORM, OPTT_DATA | OPTF_READWRITE, 
      INI_SECTION_CONSOLE, _T("TextColorNormal"), 0, NULL },
    { OPTD_COLOR_TEXTERR, OPTT_DATA | OPTF_READWRITE, 
      INI_SECTION_CONSOLE, _T("TextColorError"), 0, NULL },
    { OPTD_COLOR_TEXTMSG, OPTT_DATA | OPTF_READWRITE, 
      INI_SECTION_CONSOLE, _T("TextColorMessage"), 0, NULL },
    { OPTD_COLOR_BKGND, OPTT_DATA | OPTF_READWRITE, 
      INI_SECTION_CONSOLE, _T("BackgroundColor"), 0, NULL },

  #ifdef _SCROLL_TO_LATEST  
    { OPTB_CONSOLE_SCROLL2LATEST, OPTT_INT | OPTF_READWRITE,
      INI_SECTION_CONSOLE, _T("ScrollToLatest"), -1, NULL },
  #endif

    { OPTB_CONSOLE_NOINTMSGS, OPTT_INT | OPTF_READWRITE,
      INI_SECTION_CONSOLE, _T("NoInternalMsgs"), -1, NULL },
    { OPTD_CONSOLE_FONT, OPTT_DATA | OPTF_READWRITE,
      INI_SECTION_CONSOLE, _T("Font"), 0, NULL },
    //[ConsoleOutputFilter]
    { OPTB_CONFLTR_ENABLE, OPTT_BOOL | OPTF_READWRITE,
      INI_SECTION_CONSOLEFILTER, _T("Enable"), 0, NULL },
    { OPTB_CONFLTR_EXCLALLEMPTY, OPTT_BOOL | OPTF_READWRITE,
      INI_SECTION_CONSOLEFILTER, _T("ExcludeAllEmpty"), 0, NULL },
    { OPTB_CONFLTR_EXCLDUPEMPTY, OPTT_BOOL | OPTF_READWRITE,
      INI_SECTION_CONSOLEFILTER, _T("ExcludeDupEmpty"), 0, NULL },
    { OPTI_CONFLTR_INCLMASK, OPTT_INT | OPTF_READWRITE,
      INI_SECTION_CONSOLEFILTER, _T("IncludeMask"), 0, NULL },
    { OPTI_CONFLTR_EXCLMASK, OPTT_INT | OPTF_READWRITE,
      INI_SECTION_CONSOLEFILTER, _T("ExcludeMask"), 0, NULL },
    { OPTS_CONFLTR_INCLLINE1, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_CONSOLEFILTER, _T("IncludeLine0"), 0, NULL },
    { OPTS_CONFLTR_INCLLINE2, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_CONSOLEFILTER, _T("IncludeLine1"), 0, NULL },
    { OPTS_CONFLTR_INCLLINE3, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_CONSOLEFILTER, _T("IncludeLine2"), 0, NULL },
    { OPTS_CONFLTR_INCLLINE4, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_CONSOLEFILTER, _T("IncludeLine3"), 0, NULL },
    { OPTS_CONFLTR_INCLLINE5, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_CONSOLEFILTER, _T("IncludeLine4"), 0, NULL },
    { OPTS_CONFLTR_INCLLINE6, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_CONSOLEFILTER, _T("IncludeLine5"), 0, NULL },
    { OPTS_CONFLTR_INCLLINE7, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_CONSOLEFILTER, _T("IncludeLine6"), 0, NULL },
    { OPTS_CONFLTR_INCLLINE8, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_CONSOLEFILTER, _T("IncludeLine7"), 0, NULL },
    { OPTS_CONFLTR_EXCLLINE1, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_CONSOLEFILTER, _T("ExcludeLine0"), 0, NULL },
    { OPTS_CONFLTR_EXCLLINE2, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_CONSOLEFILTER, _T("ExcludeLine1"), 0, NULL },
    { OPTS_CONFLTR_EXCLLINE3, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_CONSOLEFILTER, _T("ExcludeLine2"), 0, NULL },
    { OPTS_CONFLTR_EXCLLINE4, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_CONSOLEFILTER, _T("ExcludeLine3"), 0, NULL },
    { OPTS_CONFLTR_EXCLLINE5, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_CONSOLEFILTER, _T("ExcludeLine4"), 0, NULL },
    { OPTS_CONFLTR_EXCLLINE6, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_CONSOLEFILTER, _T("ExcludeLine5"), 0, NULL },
    { OPTS_CONFLTR_EXCLLINE7, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_CONSOLEFILTER, _T("ExcludeLine6"), 0, NULL },
    { OPTS_CONFLTR_EXCLLINE8, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_CONSOLEFILTER, _T("ExcludeLine7"), 0, NULL },

    { OPTB_CONFLTR_R_ENABLE, OPTT_BOOL | OPTF_READWRITE,
      INI_SECTION_CONSOLEFILTER, _T("REnable"), 0, NULL },
    { OPTB_CONFLTR_R_EXCLEMPTY, OPTT_BOOL | OPTF_READWRITE,
      INI_SECTION_CONSOLEFILTER, _T("RExcludeEmpty"), 0, NULL },
    { OPTI_CONFLTR_R_FINDMASK, OPTT_INT | OPTF_READWRITE,
      INI_SECTION_CONSOLEFILTER, _T("RFindMask"), 0, NULL },
    { OPTI_CONFLTR_R_CASEMASK, OPTT_INT | OPTF_READWRITE,
      INI_SECTION_CONSOLEFILTER, _T("RCaseMask"), 0, NULL },
    { OPTS_CONFLTR_R_FIND1, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_CONSOLEFILTER, _T("RFind0"), 0, NULL },
    { OPTS_CONFLTR_R_FIND2, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_CONSOLEFILTER, _T("RFind1"), 0, NULL },
    { OPTS_CONFLTR_R_FIND3, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_CONSOLEFILTER, _T("RFind2"), 0, NULL },
    { OPTS_CONFLTR_R_FIND4, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_CONSOLEFILTER, _T("RFind3"), 0, NULL },
    { OPTS_CONFLTR_R_FIND5, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_CONSOLEFILTER, _T("RFind4"), 0, NULL },
    { OPTS_CONFLTR_R_FIND6, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_CONSOLEFILTER, _T("RFind5"), 0, NULL },
    { OPTS_CONFLTR_R_FIND7, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_CONSOLEFILTER, _T("RFind6"), 0, NULL },
    { OPTS_CONFLTR_R_FIND8, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_CONSOLEFILTER, _T("RFind7"), 0, NULL },
    { OPTS_CONFLTR_R_RPLC1, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_CONSOLEFILTER, _T("RRplc0"), 0, NULL },
    { OPTS_CONFLTR_R_RPLC2, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_CONSOLEFILTER, _T("RRplc1"), 0, NULL },
    { OPTS_CONFLTR_R_RPLC3, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_CONSOLEFILTER, _T("RRplc2"), 0, NULL },
    { OPTS_CONFLTR_R_RPLC4, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_CONSOLEFILTER, _T("RRplc3"), 0, NULL },
    { OPTS_CONFLTR_R_RPLC5, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_CONSOLEFILTER, _T("RRplc4"), 0, NULL },
    { OPTS_CONFLTR_R_RPLC6, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_CONSOLEFILTER, _T("RRplc5"), 0, NULL },
    { OPTS_CONFLTR_R_RPLC7, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_CONSOLEFILTER, _T("RRplc6"), 0, NULL },
    { OPTS_CONFLTR_R_RPLC8, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_CONSOLEFILTER, _T("RRplc7"), 0, NULL },

    //#ifdef _NIB  
    { OPTS_CONFLTR_RCGNMSK1, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_CONSOLEFILTER, _T("RecognitionMask0"), 0, NULL },
    { OPTS_CONFLTR_RCGNEFF1, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_CONSOLEFILTER, _T("RecognitionEffect0"), 0, NULL },
    { OPTS_CONFLTR_RCGNMSK2, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_CONSOLEFILTER, _T("RecognitionMask1"), 0, NULL },
    { OPTS_CONFLTR_RCGNEFF2, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_CONSOLEFILTER, _T("RecognitionEffect1"), 0, NULL },
    { OPTS_CONFLTR_RCGNMSK3, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_CONSOLEFILTER, _T("RecognitionMask2"), 0, NULL },
    { OPTS_CONFLTR_RCGNEFF3, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_CONSOLEFILTER, _T("RecognitionEffect2"), 0, NULL },
    { OPTS_CONFLTR_RCGNMSK4, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_CONSOLEFILTER, _T("RecognitionMask3"), 0, NULL },
    { OPTS_CONFLTR_RCGNEFF4, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_CONSOLEFILTER, _T("RecognitionEffect3"), 0, NULL },
    { OPTS_CONFLTR_RCGNMSK5, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_CONSOLEFILTER, _T("RecognitionMask4"), 0, NULL },
    { OPTS_CONFLTR_RCGNEFF5, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_CONSOLEFILTER, _T("RecognitionEffect4"), 0, NULL },
    { OPTS_CONFLTR_RCGNMSK6, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_CONSOLEFILTER, _T("RecognitionMask5"), 0, NULL },
    { OPTS_CONFLTR_RCGNEFF6, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_CONSOLEFILTER, _T("RecognitionEffect5"), 0, NULL },
    { OPTS_CONFLTR_RCGNMSK7, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_CONSOLEFILTER, _T("RecognitionMask6"), 0, NULL },
    { OPTS_CONFLTR_RCGNEFF7, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_CONSOLEFILTER, _T("RecognitionEffect6"), 0, NULL },
    { OPTS_CONFLTR_RCGNMSK8, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_CONSOLEFILTER, _T("RecognitionMask7"), 0, NULL },
    { OPTS_CONFLTR_RCGNEFF8, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_CONSOLEFILTER, _T("RecognitionEffect7"), 0, NULL },
    { OPTS_CONFLTR_RCGNMSK9, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_CONSOLEFILTER, _T("RecognitionMask8"), 0, NULL },
    { OPTS_CONFLTR_RCGNEFF9, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_CONSOLEFILTER, _T("RecognitionEffect8"), 0, NULL },
    { OPTS_CONFLTR_RCGNMSK10, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_CONSOLEFILTER, _T("RecognitionMask9"), 0, NULL },
    { OPTS_CONFLTR_RCGNEFF10, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_CONSOLEFILTER, _T("RecognitionEffect9"), 0, NULL },
  
  #ifdef _GVCRESTORE
    { OPTI_SELECTED_SCRIPT, OPTT_INT | OPTF_READWRITE,
      INI_SECTION_RESTORE, _T("LastSelectedScript"), 0, NULL },
  #endif
  
  //#endif

    // [FiltersExclude]
    { OPTS_FILTERS_EXCL1, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_FILTERSEXCLUDE, _T("0"), 0, NULL },
    { OPTS_FILTERS_EXCL2, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_FILTERSEXCLUDE, _T("1"), 0, NULL },
    { OPTS_FILTERS_EXCL3, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_FILTERSEXCLUDE, _T("2"), 0, NULL },
    { OPTS_FILTERS_EXCL4, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_FILTERSEXCLUDE, _T("3"), 0, NULL },
    { OPTS_FILTERS_EXCL5, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_FILTERSEXCLUDE, _T("4"), 0, NULL },
    { OPTS_FILTERS_EXCL6, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_FILTERSEXCLUDE, _T("5"), 0, NULL },
    { OPTS_FILTERS_EXCL7, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_FILTERSEXCLUDE, _T("6"), 0, NULL },
    { OPTS_FILTERS_EXCL8, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_FILTERSEXCLUDE, _T("7"), 0, NULL },
    { OPTS_FILTERS_EXCL9, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_FILTERSEXCLUDE, _T("8"), 0, NULL },
    { OPTS_FILTERS_EXCL10, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_FILTERSEXCLUDE, _T("9"), 0, NULL },
    { OPTS_FILTERS_EXCL11, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_FILTERSEXCLUDE, _T("10"), 0, NULL },
    { OPTS_FILTERS_EXCL12, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_FILTERSEXCLUDE, _T("11"), 0, NULL },
    { OPTS_FILTERS_EXCL13, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_FILTERSEXCLUDE, _T("12"), 0, NULL },
    { OPTS_FILTERS_EXCL14, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_FILTERSEXCLUDE, _T("13"), 0, NULL },
    { OPTS_FILTERS_EXCL15, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_FILTERSEXCLUDE, _T("14"), 0, NULL },
    { OPTS_FILTERS_EXCL16, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_FILTERSEXCLUDE, _T("15"), 0, NULL },
    { OPTS_FILTERS_EXCL17, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_FILTERSEXCLUDE, _T("16"), 0, NULL },
    { OPTS_FILTERS_EXCL18, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_FILTERSEXCLUDE, _T("17"), 0, NULL },
    { OPTS_FILTERS_EXCL19, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_FILTERSEXCLUDE, _T("18"), 0, NULL },
    { OPTS_FILTERS_EXCL20, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_FILTERSEXCLUDE, _T("19"), 0, NULL },

    // [FiltersInclude]
    { OPTS_FILTERS_INCL1, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_FILTERSINCLUDE, _T("0"), 0, NULL },
    { OPTS_FILTERS_INCL2, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_FILTERSINCLUDE, _T("1"), 0, NULL },
    { OPTS_FILTERS_INCL3, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_FILTERSINCLUDE, _T("2"), 0, NULL },
    { OPTS_FILTERS_INCL4, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_FILTERSINCLUDE, _T("3"), 0, NULL },
    { OPTS_FILTERS_INCL5, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_FILTERSINCLUDE, _T("4"), 0, NULL },
    { OPTS_FILTERS_INCL6, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_FILTERSINCLUDE, _T("5"), 0, NULL },
    { OPTS_FILTERS_INCL7, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_FILTERSINCLUDE, _T("6"), 0, NULL },
    { OPTS_FILTERS_INCL8, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_FILTERSINCLUDE, _T("7"), 0, NULL },
    { OPTS_FILTERS_INCL9, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_FILTERSINCLUDE, _T("8"), 0, NULL },
    { OPTS_FILTERS_INCL10, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_FILTERSINCLUDE, _T("9"), 0, NULL },
    { OPTS_FILTERS_INCL11, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_FILTERSINCLUDE, _T("10"), 0, NULL },
    { OPTS_FILTERS_INCL12, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_FILTERSINCLUDE, _T("11"), 0, NULL },
    { OPTS_FILTERS_INCL13, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_FILTERSINCLUDE, _T("12"), 0, NULL },
    { OPTS_FILTERS_INCL14, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_FILTERSINCLUDE, _T("13"), 0, NULL },
    { OPTS_FILTERS_INCL15, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_FILTERSINCLUDE, _T("14"), 0, NULL },
    { OPTS_FILTERS_INCL16, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_FILTERSINCLUDE, _T("15"), 0, NULL },
    { OPTS_FILTERS_INCL17, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_FILTERSINCLUDE, _T("16"), 0, NULL },
    { OPTS_FILTERS_INCL18, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_FILTERSINCLUDE, _T("17"), 0, NULL },
    { OPTS_FILTERS_INCL19, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_FILTERSINCLUDE, _T("18"), 0, NULL },
    { OPTS_FILTERS_INCL20, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_FILTERSINCLUDE, _T("19"), 0, NULL },

    // [FiltersReplace]
    { OPTS_FILTERS_R_FIND1, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_FILTERSREPLACE, _T("F0"), 0, NULL },
    { OPTS_FILTERS_R_FIND2, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_FILTERSREPLACE, _T("F1"), 0, NULL },
    { OPTS_FILTERS_R_FIND3, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_FILTERSREPLACE, _T("F2"), 0, NULL },
    { OPTS_FILTERS_R_FIND4, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_FILTERSREPLACE, _T("F3"), 0, NULL },
    { OPTS_FILTERS_R_FIND5, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_FILTERSREPLACE, _T("F4"), 0, NULL },
    { OPTS_FILTERS_R_FIND6, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_FILTERSREPLACE, _T("F5"), 0, NULL },
    { OPTS_FILTERS_R_FIND7, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_FILTERSREPLACE, _T("F6"), 0, NULL },
    { OPTS_FILTERS_R_FIND8, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_FILTERSREPLACE, _T("F7"), 0, NULL },
    { OPTS_FILTERS_R_FIND9, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_FILTERSREPLACE, _T("F8"), 0, NULL },
    { OPTS_FILTERS_R_FIND10, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_FILTERSREPLACE, _T("F9"), 0, NULL },
    { OPTS_FILTERS_R_FIND11, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_FILTERSREPLACE, _T("F10"), 0, NULL },
    { OPTS_FILTERS_R_FIND12, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_FILTERSREPLACE, _T("F11"), 0, NULL },
    { OPTS_FILTERS_R_FIND13, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_FILTERSREPLACE, _T("F12"), 0, NULL },
    { OPTS_FILTERS_R_FIND14, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_FILTERSREPLACE, _T("F13"), 0, NULL },
    { OPTS_FILTERS_R_FIND15, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_FILTERSREPLACE, _T("F14"), 0, NULL },
    { OPTS_FILTERS_R_FIND16, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_FILTERSREPLACE, _T("F15"), 0, NULL },
    { OPTS_FILTERS_R_FIND17, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_FILTERSREPLACE, _T("F16"), 0, NULL },
    { OPTS_FILTERS_R_FIND18, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_FILTERSREPLACE, _T("F17"), 0, NULL },
    { OPTS_FILTERS_R_FIND19, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_FILTERSREPLACE, _T("F18"), 0, NULL },
    { OPTS_FILTERS_R_FIND20, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_FILTERSREPLACE, _T("F19"), 0, NULL },
    { OPTS_FILTERS_R_RPLC1, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_FILTERSREPLACE, _T("R0"), 0, NULL },
    { OPTS_FILTERS_R_RPLC2, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_FILTERSREPLACE, _T("R1"), 0, NULL },
    { OPTS_FILTERS_R_RPLC3, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_FILTERSREPLACE, _T("R2"), 0, NULL },
    { OPTS_FILTERS_R_RPLC4, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_FILTERSREPLACE, _T("R3"), 0, NULL },
    { OPTS_FILTERS_R_RPLC5, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_FILTERSREPLACE, _T("R4"), 0, NULL },
    { OPTS_FILTERS_R_RPLC6, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_FILTERSREPLACE, _T("R5"), 0, NULL },
    { OPTS_FILTERS_R_RPLC7, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_FILTERSREPLACE, _T("R6"), 0, NULL },
    { OPTS_FILTERS_R_RPLC8, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_FILTERSREPLACE, _T("R7"), 0, NULL },
    { OPTS_FILTERS_R_RPLC9, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_FILTERSREPLACE, _T("R8"), 0, NULL },
    { OPTS_FILTERS_R_RPLC10, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_FILTERSREPLACE, _T("R9"), 0, NULL },
    { OPTS_FILTERS_R_RPLC11, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_FILTERSREPLACE, _T("R10"), 0, NULL },
    { OPTS_FILTERS_R_RPLC12, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_FILTERSREPLACE, _T("R11"), 0, NULL },
    { OPTS_FILTERS_R_RPLC13, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_FILTERSREPLACE, _T("R12"), 0, NULL },
    { OPTS_FILTERS_R_RPLC14, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_FILTERSREPLACE, _T("R13"), 0, NULL },
    { OPTS_FILTERS_R_RPLC15, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_FILTERSREPLACE, _T("R14"), 0, NULL },
    { OPTS_FILTERS_R_RPLC16, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_FILTERSREPLACE, _T("R15"), 0, NULL },
    { OPTS_FILTERS_R_RPLC17, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_FILTERSREPLACE, _T("R16"), 0, NULL },
    { OPTS_FILTERS_R_RPLC18, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_FILTERSREPLACE, _T("R17"), 0, NULL },
    { OPTS_FILTERS_R_RPLC19, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_FILTERSREPLACE, _T("R18"), 0, NULL },
    { OPTS_FILTERS_R_RPLC20, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_FILTERSREPLACE, _T("R19"), 0, NULL },
      
    // [FiltersHighlight]
    { OPTS_FILTERS_HGLT1, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_FILTERSHIGHLIGHT, _T("0"), 0, NULL },
    { OPTS_FILTERS_HGLT2, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_FILTERSHIGHLIGHT, _T("1"), 0, NULL },
    { OPTS_FILTERS_HGLT3, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_FILTERSHIGHLIGHT, _T("2"), 0, NULL },
    { OPTS_FILTERS_HGLT4, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_FILTERSHIGHLIGHT, _T("3"), 0, NULL },
    { OPTS_FILTERS_HGLT5, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_FILTERSHIGHLIGHT, _T("4"), 0, NULL },
    { OPTS_FILTERS_HGLT6, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_FILTERSHIGHLIGHT, _T("5"), 0, NULL },
    { OPTS_FILTERS_HGLT7, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_FILTERSHIGHLIGHT, _T("6"), 0, NULL },
    { OPTS_FILTERS_HGLT8, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_FILTERSHIGHLIGHT, _T("7"), 0, NULL },
    { OPTS_FILTERS_HGLT9, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_FILTERSHIGHLIGHT, _T("8"), 0, NULL },
    { OPTS_FILTERS_HGLT10, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_FILTERSHIGHLIGHT, _T("9"), 0, NULL },
    { OPTS_FILTERS_HGLT11, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_FILTERSHIGHLIGHT, _T("10"), 0, NULL },
    { OPTS_FILTERS_HGLT12, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_FILTERSHIGHLIGHT, _T("11"), 0, NULL },
    { OPTS_FILTERS_HGLT13, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_FILTERSHIGHLIGHT, _T("12"), 0, NULL },
    { OPTS_FILTERS_HGLT14, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_FILTERSHIGHLIGHT, _T("13"), 0, NULL },
    { OPTS_FILTERS_HGLT15, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_FILTERSHIGHLIGHT, _T("14"), 0, NULL },
    { OPTS_FILTERS_HGLT16, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_FILTERSHIGHLIGHT, _T("15"), 0, NULL },
    { OPTS_FILTERS_HGLT17, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_FILTERSHIGHLIGHT, _T("16"), 0, NULL },
    { OPTS_FILTERS_HGLT18, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_FILTERSHIGHLIGHT, _T("17"), 0, NULL },
    { OPTS_FILTERS_HGLT19, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_FILTERSHIGHLIGHT, _T("18"), 0, NULL },
    { OPTS_FILTERS_HGLT20, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_FILTERSHIGHLIGHT, _T("19"), 0, NULL },
  
    // [UserMenu]
    { OPTB_USERMENU_NPPMACROS, OPTT_INT | OPTF_READWRITE,
      INI_SECTION_USERMENU, _T("NppMacrosSubmenu"), 0, NULL },
    { OPTS_USERMENU_ITEM01, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_USERMENU, _T("0"), 0, NULL },
    { OPTS_USERMENU_ITEM02, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_USERMENU, _T("1"), 0, NULL },
    { OPTS_USERMENU_ITEM03, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_USERMENU, _T("2"), 0, NULL },
    { OPTS_USERMENU_ITEM04, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_USERMENU, _T("3"), 0, NULL },
    { OPTS_USERMENU_ITEM05, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_USERMENU, _T("4"), 0, NULL },
    { OPTS_USERMENU_ITEM06, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_USERMENU, _T("5"), 0, NULL },
    { OPTS_USERMENU_ITEM07, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_USERMENU, _T("6"), 0, NULL },
    { OPTS_USERMENU_ITEM08, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_USERMENU, _T("7"), 0, NULL },
    { OPTS_USERMENU_ITEM09, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_USERMENU, _T("8"), 0, NULL },
    { OPTS_USERMENU_ITEM10, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_USERMENU, _T("9"), 0, NULL },
    { OPTS_USERMENU_ITEM11, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_USERMENU, _T("10"), 0, NULL },
    { OPTS_USERMENU_ITEM12, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_USERMENU, _T("11"), 0, NULL },
    { OPTS_USERMENU_ITEM13, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_USERMENU, _T("12"), 0, NULL },
    { OPTS_USERMENU_ITEM14, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_USERMENU, _T("13"), 0, NULL },
    { OPTS_USERMENU_ITEM15, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_USERMENU, _T("14"), 0, NULL },
    { OPTS_USERMENU_ITEM16, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_USERMENU, _T("15"), 0, NULL },
    { OPTS_USERMENU_ITEM17, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_USERMENU, _T("16"), 0, NULL },
    { OPTS_USERMENU_ITEM18, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_USERMENU, _T("17"), 0, NULL },
    { OPTS_USERMENU_ITEM19, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_USERMENU, _T("18"), 0, NULL },
    { OPTS_USERMENU_ITEM20, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_USERMENU, _T("19"), 0, NULL },
    { OPTS_USERMENU_ITEM21, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_USERMENU, _T("20"), 0, NULL },
    { OPTS_USERMENU_ITEM22, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_USERMENU, _T("21"), 0, NULL },
    { OPTS_USERMENU_ITEM23, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_USERMENU, _T("22"), 0, NULL },
    { OPTS_USERMENU_ITEM24, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_USERMENU, _T("23"), 0, NULL },
    { OPTS_USERMENU_ITEM25, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_USERMENU, _T("24"), 0, NULL },
    { OPTS_USERMENU_ITEM26, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_USERMENU, _T("25"), 0, NULL },
    { OPTS_USERMENU_ITEM27, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_USERMENU, _T("26"), 0, NULL },
    { OPTS_USERMENU_ITEM28, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_USERMENU, _T("27"), 0, NULL },
    { OPTS_USERMENU_ITEM29, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_USERMENU, _T("28"), 0, NULL },
    { OPTS_USERMENU_ITEM30, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_USERMENU, _T("29"), 0, NULL },
    { OPTS_USERMENU_ITEM31, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_USERMENU, _T("30"), 0, NULL },
    { OPTS_USERMENU_ITEM32, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_USERMENU, _T("31"), 0, NULL },
    { OPTS_USERMENU_ITEM33, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_USERMENU, _T("32"), 0, NULL },
    { OPTS_USERMENU_ITEM34, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_USERMENU, _T("33"), 0, NULL },
    { OPTS_USERMENU_ITEM35, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_USERMENU, _T("34"), 0, NULL },
    { OPTS_USERMENU_ITEM36, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_USERMENU, _T("35"), 0, NULL },
    { OPTS_USERMENU_ITEM37, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_USERMENU, _T("36"), 0, NULL },
    { OPTS_USERMENU_ITEM38, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_USERMENU, _T("37"), 0, NULL },
    { OPTS_USERMENU_ITEM39, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_USERMENU, _T("38"), 0, NULL },
    { OPTS_USERMENU_ITEM40, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_USERMENU, _T("39"), 0, NULL },
    { OPTS_USERMENU_ITEM41, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_USERMENU, _T("40"), 0, NULL },
    { OPTS_USERMENU_ITEM42, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_USERMENU, _T("41"), 0, NULL },
    { OPTS_USERMENU_ITEM43, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_USERMENU, _T("42"), 0, NULL },
    { OPTS_USERMENU_ITEM44, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_USERMENU, _T("43"), 0, NULL },
    { OPTS_USERMENU_ITEM45, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_USERMENU, _T("44"), 0, NULL },
    { OPTS_USERMENU_ITEM46, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_USERMENU, _T("45"), 0, NULL },
    { OPTS_USERMENU_ITEM47, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_USERMENU, _T("46"), 0, NULL },
    { OPTS_USERMENU_ITEM48, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_USERMENU, _T("47"), 0, NULL },
    { OPTS_USERMENU_ITEM49, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_USERMENU, _T("48"), 0, NULL },
    { OPTS_USERMENU_ITEM50, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_USERMENU, _T("49"), 0, NULL },
    { OPTS_USERMENU_ITEM51, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_USERMENU, _T("50"), 0, NULL },
    { OPTS_USERMENU_ITEM52, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_USERMENU, _T("51"), 0, NULL },
    { OPTS_USERMENU_ITEM53, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_USERMENU, _T("52"), 0, NULL },
    { OPTS_USERMENU_ITEM54, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_USERMENU, _T("53"), 0, NULL },
    { OPTS_USERMENU_ITEM55, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_USERMENU, _T("54"), 0, NULL },
    { OPTS_USERMENU_ITEM56, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_USERMENU, _T("55"), 0, NULL },
    { OPTS_USERMENU_ITEM57, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_USERMENU, _T("56"), 0, NULL },
    { OPTS_USERMENU_ITEM58, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_USERMENU, _T("57"), 0, NULL },
    { OPTS_USERMENU_ITEM59, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_USERMENU, _T("58"), 0, NULL },
    { OPTS_USERMENU_ITEM60, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_USERMENU, _T("59"), 0, NULL },
    { OPTS_USERMENU_ITEM61, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_USERMENU, _T("60"), 0, NULL },
    { OPTS_USERMENU_ITEM62, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_USERMENU, _T("61"), 0, NULL },
    { OPTS_USERMENU_ITEM63, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_USERMENU, _T("62"), 0, NULL },
    { OPTS_USERMENU_ITEM64, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_USERMENU, _T("63"), 0, NULL },
    { OPTS_USERMENU_ITEM65, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_USERMENU, _T("64"), 0, NULL },
    { OPTS_USERMENU_ITEM66, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_USERMENU, _T("65"), 0, NULL },
    { OPTS_USERMENU_ITEM67, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_USERMENU, _T("66"), 0, NULL },
    { OPTS_USERMENU_ITEM68, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_USERMENU, _T("67"), 0, NULL },
    { OPTS_USERMENU_ITEM69, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_USERMENU, _T("68"), 0, NULL },
    { OPTS_USERMENU_ITEM70, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_USERMENU, _T("69"), 0, NULL },
    { OPTS_USERMENU_ITEM71, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_USERMENU, _T("70"), 0, NULL },
    { OPTS_USERMENU_ITEM72, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_USERMENU, _T("71"), 0, NULL },
    { OPTS_USERMENU_ITEM73, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_USERMENU, _T("72"), 0, NULL },
    { OPTS_USERMENU_ITEM74, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_USERMENU, _T("73"), 0, NULL },
    { OPTS_USERMENU_ITEM75, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_USERMENU, _T("74"), 0, NULL },
    { OPTS_USERMENU_ITEM76, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_USERMENU, _T("75"), 0, NULL },
    { OPTS_USERMENU_ITEM77, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_USERMENU, _T("76"), 0, NULL },
    { OPTS_USERMENU_ITEM78, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_USERMENU, _T("77"), 0, NULL },
    { OPTS_USERMENU_ITEM79, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_USERMENU, _T("78"), 0, NULL },
    { OPTS_USERMENU_ITEM80, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_USERMENU, _T("79"), 0, NULL },
    { OPTS_USERMENU_ITEM81, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_USERMENU, _T("80"), 0, NULL },
    { OPTS_USERMENU_ITEM82, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_USERMENU, _T("81"), 0, NULL },
    { OPTS_USERMENU_ITEM83, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_USERMENU, _T("82"), 0, NULL },
    { OPTS_USERMENU_ITEM84, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_USERMENU, _T("83"), 0, NULL },
    { OPTS_USERMENU_ITEM85, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_USERMENU, _T("84"), 0, NULL },
    { OPTS_USERMENU_ITEM86, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_USERMENU, _T("85"), 0, NULL },
    { OPTS_USERMENU_ITEM87, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_USERMENU, _T("86"), 0, NULL },
    { OPTS_USERMENU_ITEM88, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_USERMENU, _T("87"), 0, NULL },
    { OPTS_USERMENU_ITEM89, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_USERMENU, _T("88"), 0, NULL },
    { OPTS_USERMENU_ITEM90, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_USERMENU, _T("89"), 0, NULL },
    { OPTS_USERMENU_ITEM91, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_USERMENU, _T("90"), 0, NULL },
    { OPTS_USERMENU_ITEM92, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_USERMENU, _T("91"), 0, NULL },
    { OPTS_USERMENU_ITEM93, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_USERMENU, _T("92"), 0, NULL },
    { OPTS_USERMENU_ITEM94, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_USERMENU, _T("93"), 0, NULL },
    { OPTS_USERMENU_ITEM95, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_USERMENU, _T("94"), 0, NULL },
    { OPTS_USERMENU_ITEM96, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_USERMENU, _T("95"), 0, NULL },
    { OPTS_USERMENU_ITEM97, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_USERMENU, _T("96"), 0, NULL },
    { OPTS_USERMENU_ITEM98, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_USERMENU, _T("97"), 0, NULL },
    { OPTS_USERMENU_ITEM99, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_USERMENU, _T("98"), 0, NULL },
    { OPTS_USERMENU_ITEM100, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_USERMENU, _T("99"), 0, NULL },

    // [InputBox]
    { OPTS_INPUTBOX_VALUE1, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_INPUTBOX, _T("I0"), 0, NULL },
    { OPTS_INPUTBOX_VALUE2, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_INPUTBOX, _T("I1"), 0, NULL },
    { OPTS_INPUTBOX_VALUE3, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_INPUTBOX, _T("I2"), 0, NULL },
    { OPTS_INPUTBOX_VALUE4, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_INPUTBOX, _T("I3"), 0, NULL },
    { OPTS_INPUTBOX_VALUE5, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_INPUTBOX, _T("I4"), 0, NULL },
    { OPTS_INPUTBOX_VALUE6, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_INPUTBOX, _T("I5"), 0, NULL },
    { OPTS_INPUTBOX_VALUE7, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_INPUTBOX, _T("I6"), 0, NULL },
    { OPTS_INPUTBOX_VALUE8, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_INPUTBOX, _T("I7"), 0, NULL },
    { OPTS_INPUTBOX_VALUE9, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_INPUTBOX, _T("I8"), 0, NULL },
    { OPTS_INPUTBOX_VALUE10, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_INPUTBOX, _T("I9"), 0, NULL },
    { OPTS_INPUTBOX_VALUE11, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_INPUTBOX, _T("I10"), 0, NULL },
    { OPTS_INPUTBOX_VALUE12, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_INPUTBOX, _T("I11"), 0, NULL },
    { OPTS_INPUTBOX_VALUE13, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_INPUTBOX, _T("I12"), 0, NULL },
    { OPTS_INPUTBOX_VALUE14, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_INPUTBOX, _T("I13"), 0, NULL },
    { OPTS_INPUTBOX_VALUE15, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_INPUTBOX, _T("I14"), 0, NULL },
    { OPTS_INPUTBOX_VALUE16, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_INPUTBOX, _T("I15"), 0, NULL },
    { OPTS_INPUTBOX_VALUE17, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_INPUTBOX, _T("I16"), 0, NULL },
    { OPTS_INPUTBOX_VALUE18, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_INPUTBOX, _T("I17"), 0, NULL },
    { OPTS_INPUTBOX_VALUE19, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_INPUTBOX, _T("I18"), 0, NULL },
    { OPTS_INPUTBOX_VALUE20, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_INPUTBOX, _T("I19"), 0, NULL },

    // [ExitBox]
    { OPTS_EXITBOX_VALUE1, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_EXITBOX, _T("I0"), 0, NULL },
    { OPTS_EXITBOX_VALUE2, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_EXITBOX, _T("I1"), 0, NULL },
    { OPTS_EXITBOX_VALUE3, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_EXITBOX, _T("I2"), 0, NULL },
    { OPTS_EXITBOX_VALUE4, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_EXITBOX, _T("I3"), 0, NULL },
    { OPTS_EXITBOX_VALUE5, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_EXITBOX, _T("I4"), 0, NULL },
    { OPTS_EXITBOX_VALUE6, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_EXITBOX, _T("I5"), 0, NULL },
    { OPTS_EXITBOX_VALUE7, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_EXITBOX, _T("I6"), 0, NULL },
    { OPTS_EXITBOX_VALUE8, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_EXITBOX, _T("I7"), 0, NULL },
    { OPTS_EXITBOX_VALUE9, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_EXITBOX, _T("I8"), 0, NULL },
    { OPTS_EXITBOX_VALUE10, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_EXITBOX, _T("I9"), 0, NULL },
    { OPTS_EXITBOX_VALUE11, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_EXITBOX, _T("I10"), 0, NULL },
    { OPTS_EXITBOX_VALUE12, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_EXITBOX, _T("I11"), 0, NULL },
    { OPTS_EXITBOX_VALUE13, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_EXITBOX, _T("I12"), 0, NULL },
    { OPTS_EXITBOX_VALUE14, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_EXITBOX, _T("I13"), 0, NULL },
    { OPTS_EXITBOX_VALUE15, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_EXITBOX, _T("I14"), 0, NULL },
    { OPTS_EXITBOX_VALUE16, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_EXITBOX, _T("I15"), 0, NULL },
    { OPTS_EXITBOX_VALUE17, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_EXITBOX, _T("I16"), 0, NULL },
    { OPTS_EXITBOX_VALUE18, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_EXITBOX, _T("I17"), 0, NULL },
    { OPTS_EXITBOX_VALUE19, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_EXITBOX, _T("I18"), 0, NULL },
    { OPTS_EXITBOX_VALUE20, OPTT_STR | OPTF_READWRITE,
      INI_SECTION_EXITBOX, _T("I19"), 0, NULL },
      
    // --- read-only options ---

    // [Options]
    { OPTS_PLUGIN_HELPFILE, OPTT_STR | OPTF_READONLY,
      INI_SECTION_OPTIONS, _T("HelpFile"),
      0, DEFAULT_HELPFILE },
    { OPTS_PLUGIN_LOGSDIR, OPTT_STR | OPTF_READONLY,
      INI_SECTION_OPTIONS, _T("LogsDir"),
      0, DEFAULT_LOGSDIR },
    { OPTS_PLUGIN_SCRIPTSDIR, OPTT_STR | OPTF_READONLY,
      INI_SECTION_OPTIONS, _T("ScriptsDir"),
      0, DEFAULT_SCRIPTSDIR },
    { OPTU_PLUGIN_AUTOSAVE_SECONDS, OPTT_INT | OPTF_READONLY,
      INI_SECTION_OPTIONS, _T("AutoSave_Seconds"),
      DEFAULT_AUTOSAVE_SECONDS, NULL },
    // [Console]
    { OPTU_CHILDP_STARTUPTIMEOUT_MS, OPTT_INT | OPTF_READONLY,
      INI_SECTION_CONSOLE, _T("ChildProcess_StartupTimeout_ms"), 
      DEFAULT_STARTUPTIMEOUT_MS, NULL },
    { OPTU_CHILDP_CYCLETIMEOUT_MS, OPTT_INT | OPTF_READONLY,
      INI_SECTION_CONSOLE, _T("ChildProcess_CycleTimeout_ms"), 
      DEFAULT_CYCLETIMEOUT_MS, NULL },
    { OPTU_CHILDP_EXITTIMEOUT_MS, OPTT_INT | OPTF_READONLY,
      INI_SECTION_CONSOLE, _T("ChildProcess_ExitTimeout_ms"),
      DEFAULT_EXITTIMEOUT_MS, NULL },
    { OPTB_PATH_AUTODBLQUOTES, OPTT_BOOL | OPTF_READONLY,
      INI_SECTION_CONSOLE, _T("Path_AutoDblQuotes"), 
      DEFAULT_PATH_AUTODBLQUOTES, NULL },
    { OPTI_CMDHISTORY_MAXITEMS, OPTT_INT | OPTF_READONLY,
      INI_SECTION_CONSOLE, _T("CmdHistory_MaxItems"), 
      DEFAULT_CMDHISTORY_MAXITEMS, NULL },
    { OPTI_EXEC_MAXCOUNT, OPTT_INT | OPTF_READONLY,
      INI_SECTION_CONSOLE, _T("Exec_MaxCount"), 
      DEFAULT_EXEC_MAXCOUNT, NULL },
    { OPTI_GOTO_MAXCOUNT, OPTT_INT | OPTF_READONLY,
      INI_SECTION_CONSOLE, _T("GoTo_MaxCount"),
      DEFAULT_GOTO_MAXCOUNT, NULL },
    { OPTB_CONSOLE_NOEMPTYVARS, OPTT_BOOL | OPTF_READONLY,
      INI_SECTION_CONSOLE, _T("NoEmptyVars"), 1, NULL },
    { OPTS_ALIAS_CMD_NPPEXEC, OPTT_STR | OPTF_READONLY,
      INI_SECTION_CONSOLE, _T("Alias_Cmd_NppExec"), 
      0, DEFAULT_ALIAS_CMD_NPPEXEC },
    { OPTS_KEY_ENTER, OPTT_STR | OPTF_READONLY,
      INI_SECTION_CONSOLE, _T("Key_Enter"),
      0, DEFAULT_KEY_ENTER },
    { OPTD_CONSOLE_NULCHAR, OPTT_DATA | OPTF_READONLY,
      INI_SECTION_CONSOLE, _T("Console_NulChar"),
      0, NULL }, // <-- this value is adjusted inside ReadOptions()
    
    // --- internal options ---
    { OPTU_PLUGIN_HOTKEY, OPTT_INT | OPTF_INTERNAL,
      NULL, NULL, VK_F6, NULL },
    { OPTB_NPE_DEBUGLOG, OPTT_BOOL | OPTF_INTERNAL,
      NULL, NULL, 0, NULL },
    { OPTB_CONSOLE_APPENDMODE, OPTT_BOOL | OPTF_INTERNAL,
      NULL, NULL, 0, NULL },
    { OPTU_CONSOLE_CATCHSHORTCUTKEYS, OPTT_INT | OPTF_INTERNAL,
      NULL, NULL, ConsoleDlg::CSK_ALL, NULL },
    { OPTB_CONSOLE_SETOUTPUTVAR, OPTT_BOOL | OPTF_INTERNAL,
      NULL, NULL, 0, NULL },
    { OPTB_CONSOLE_NOCMDALIASES, OPTT_BOOL | OPTF_INTERNAL,
      NULL, NULL, 0, NULL }
};

CStaticOptionsManager g_opt(_T("NppExec"), optArray, OPT_COUNT);

bool              g_bIsNppReady = false;
bool              g_bInitialized = false;
bool              g_bUseThreadForScriptExecution = true;
bool              g_bOptionsSavedOnNppnShutdown = false;
int               g_nUserMenuItems = 0;
FuncItem          g_funcItem[nbFunc + MAX_USERMENU_ITEMS + 1];
ShortcutKey       g_funcShortcut[nbFunc + MAX_USERMENU_ITEMS + 1];
CNppExec          g_nppExec;
CSimpleLogger     g_scriptLog;
//#ifdef _NIB
CWarningAnalyzer  g_WarnAn;
//#endif

namespace Runtime
{
    CNppExec& GetNppExec()
    {
        return g_nppExec;
    }

    CSimpleLogger& GetLogger()
    {
        return g_scriptLog;
    }

    CStaticOptionsManager& GetOptions()
    {
        return g_opt;
    }

    CWarningAnalyzer& GetWarningAnalyzer()
    {
        return g_WarnAn;
    }

};

COLORREF g_colorTextNorm = COLOR_CON_TEXTNORM;
COLORREF g_colorTextErr  = COLOR_CON_TEXTERR;
COLORREF g_colorTextMsg  = COLOR_CON_TEXTMSG;
COLORREF g_colorBkgnd    = COLOR_CON_BKGND;


extern BOOL                     g_bIsNppUnicode;
extern WNDPROC                  nppOriginalWndProc;
extern CConsoleOutputFilterDlg  ConsoleOutputFilterDlg;
extern CConsoleEncodingDlg      ConsoleEncodingDlg;
extern CInputBoxDlg             InputBoxDlg;
extern CFileModificationWatcher g_scriptFileWatcher;


void empty_func(void)           { /* empty function */ }
void cmdhistory_func(void)      { Runtime::GetNppExec().OnCmdHistory(); }
void do_exec_func(void)         { Runtime::GetNppExec().OnDoExec(); }
//#ifdef _NIB
void direct_exec_func(void)     { Runtime::GetNppExec().OnDirectExec(true); }
//#endif
void show_console_func(void)    { Runtime::GetNppExec().OnShowConsoleDlg(); }
void toggle_console_func(void)  { Runtime::GetNppExec().OnToggleConsoleDlg(); }
void go_to_next_error(void)     { ConsoleDlg::GoToError(1); }
void go_to_prev_error(void)     { ConsoleDlg::GoToError(-1); }
void console_enc_func(void)     { Runtime::GetNppExec().OnConsoleEncoding(); }
void output_f_func(void)        { Runtime::GetNppExec().OnOutputFilter(); }
void adv_opt_func(void)         { Runtime::GetNppExec().OnAdvancedOptions(); }
//#ifdef _GVC
void saveonexecute_func(void)   { Runtime::GetNppExec().OnSaveOnExecute(); }
//#endif
void cdcurdir_func(void)        { Runtime::GetNppExec().OnCdCurDir(); }
void usermenu_item00_func(void) { Runtime::GetNppExec().OnUserMenuItem(0); }
void usermenu_item01_func(void) { Runtime::GetNppExec().OnUserMenuItem(1); }
void usermenu_item02_func(void) { Runtime::GetNppExec().OnUserMenuItem(2); }
void usermenu_item03_func(void) { Runtime::GetNppExec().OnUserMenuItem(3); }
void usermenu_item04_func(void) { Runtime::GetNppExec().OnUserMenuItem(4); }
void usermenu_item05_func(void) { Runtime::GetNppExec().OnUserMenuItem(5); }
void usermenu_item06_func(void) { Runtime::GetNppExec().OnUserMenuItem(6); }
void usermenu_item07_func(void) { Runtime::GetNppExec().OnUserMenuItem(7); }
void usermenu_item08_func(void) { Runtime::GetNppExec().OnUserMenuItem(8); }
void usermenu_item09_func(void) { Runtime::GetNppExec().OnUserMenuItem(9); }
void usermenu_item10_func(void) { Runtime::GetNppExec().OnUserMenuItem(10); }
void usermenu_item11_func(void) { Runtime::GetNppExec().OnUserMenuItem(11); }
void usermenu_item12_func(void) { Runtime::GetNppExec().OnUserMenuItem(12); }
void usermenu_item13_func(void) { Runtime::GetNppExec().OnUserMenuItem(13); }
void usermenu_item14_func(void) { Runtime::GetNppExec().OnUserMenuItem(14); }
void usermenu_item15_func(void) { Runtime::GetNppExec().OnUserMenuItem(15); }
void usermenu_item16_func(void) { Runtime::GetNppExec().OnUserMenuItem(16); }
void usermenu_item17_func(void) { Runtime::GetNppExec().OnUserMenuItem(17); }
void usermenu_item18_func(void) { Runtime::GetNppExec().OnUserMenuItem(18); }
void usermenu_item19_func(void) { Runtime::GetNppExec().OnUserMenuItem(19); }
void usermenu_item20_func(void) { Runtime::GetNppExec().OnUserMenuItem(20); }
void usermenu_item21_func(void) { Runtime::GetNppExec().OnUserMenuItem(21); }
void usermenu_item22_func(void) { Runtime::GetNppExec().OnUserMenuItem(22); }
void usermenu_item23_func(void) { Runtime::GetNppExec().OnUserMenuItem(23); }
void usermenu_item24_func(void) { Runtime::GetNppExec().OnUserMenuItem(24); }
void usermenu_item25_func(void) { Runtime::GetNppExec().OnUserMenuItem(25); }
void usermenu_item26_func(void) { Runtime::GetNppExec().OnUserMenuItem(26); }
void usermenu_item27_func(void) { Runtime::GetNppExec().OnUserMenuItem(27); }
void usermenu_item28_func(void) { Runtime::GetNppExec().OnUserMenuItem(28); }
void usermenu_item29_func(void) { Runtime::GetNppExec().OnUserMenuItem(29); }
void usermenu_item30_func(void) { Runtime::GetNppExec().OnUserMenuItem(30); }
void usermenu_item31_func(void) { Runtime::GetNppExec().OnUserMenuItem(31); }
void usermenu_item32_func(void) { Runtime::GetNppExec().OnUserMenuItem(32); }
void usermenu_item33_func(void) { Runtime::GetNppExec().OnUserMenuItem(33); }
void usermenu_item34_func(void) { Runtime::GetNppExec().OnUserMenuItem(34); }
void usermenu_item35_func(void) { Runtime::GetNppExec().OnUserMenuItem(35); }
void usermenu_item36_func(void) { Runtime::GetNppExec().OnUserMenuItem(36); }
void usermenu_item37_func(void) { Runtime::GetNppExec().OnUserMenuItem(37); }
void usermenu_item38_func(void) { Runtime::GetNppExec().OnUserMenuItem(38); }
void usermenu_item39_func(void) { Runtime::GetNppExec().OnUserMenuItem(39); }
void usermenu_item40_func(void) { Runtime::GetNppExec().OnUserMenuItem(40); }
void usermenu_item41_func(void) { Runtime::GetNppExec().OnUserMenuItem(41); }
void usermenu_item42_func(void) { Runtime::GetNppExec().OnUserMenuItem(42); }
void usermenu_item43_func(void) { Runtime::GetNppExec().OnUserMenuItem(43); }
void usermenu_item44_func(void) { Runtime::GetNppExec().OnUserMenuItem(44); }
void usermenu_item45_func(void) { Runtime::GetNppExec().OnUserMenuItem(45); }
void usermenu_item46_func(void) { Runtime::GetNppExec().OnUserMenuItem(46); }
void usermenu_item47_func(void) { Runtime::GetNppExec().OnUserMenuItem(47); }
void usermenu_item48_func(void) { Runtime::GetNppExec().OnUserMenuItem(48); }
void usermenu_item49_func(void) { Runtime::GetNppExec().OnUserMenuItem(49); }
void usermenu_item50_func(void) { Runtime::GetNppExec().OnUserMenuItem(50); }
void usermenu_item51_func(void) { Runtime::GetNppExec().OnUserMenuItem(51); }
void usermenu_item52_func(void) { Runtime::GetNppExec().OnUserMenuItem(52); }
void usermenu_item53_func(void) { Runtime::GetNppExec().OnUserMenuItem(53); }
void usermenu_item54_func(void) { Runtime::GetNppExec().OnUserMenuItem(54); }
void usermenu_item55_func(void) { Runtime::GetNppExec().OnUserMenuItem(55); }
void usermenu_item56_func(void) { Runtime::GetNppExec().OnUserMenuItem(56); }
void usermenu_item57_func(void) { Runtime::GetNppExec().OnUserMenuItem(57); }
void usermenu_item58_func(void) { Runtime::GetNppExec().OnUserMenuItem(58); }
void usermenu_item59_func(void) { Runtime::GetNppExec().OnUserMenuItem(59); }
void usermenu_item60_func(void) { Runtime::GetNppExec().OnUserMenuItem(60); }
void usermenu_item61_func(void) { Runtime::GetNppExec().OnUserMenuItem(61); }
void usermenu_item62_func(void) { Runtime::GetNppExec().OnUserMenuItem(62); }
void usermenu_item63_func(void) { Runtime::GetNppExec().OnUserMenuItem(63); }
void usermenu_item64_func(void) { Runtime::GetNppExec().OnUserMenuItem(64); }
void usermenu_item65_func(void) { Runtime::GetNppExec().OnUserMenuItem(65); }
void usermenu_item66_func(void) { Runtime::GetNppExec().OnUserMenuItem(66); }
void usermenu_item67_func(void) { Runtime::GetNppExec().OnUserMenuItem(67); }
void usermenu_item68_func(void) { Runtime::GetNppExec().OnUserMenuItem(68); }
void usermenu_item69_func(void) { Runtime::GetNppExec().OnUserMenuItem(69); }
void usermenu_item70_func(void) { Runtime::GetNppExec().OnUserMenuItem(70); }
void usermenu_item71_func(void) { Runtime::GetNppExec().OnUserMenuItem(71); }
void usermenu_item72_func(void) { Runtime::GetNppExec().OnUserMenuItem(72); }
void usermenu_item73_func(void) { Runtime::GetNppExec().OnUserMenuItem(73); }
void usermenu_item74_func(void) { Runtime::GetNppExec().OnUserMenuItem(74); }
void usermenu_item75_func(void) { Runtime::GetNppExec().OnUserMenuItem(75); }
void usermenu_item76_func(void) { Runtime::GetNppExec().OnUserMenuItem(76); }
void usermenu_item77_func(void) { Runtime::GetNppExec().OnUserMenuItem(77); }
void usermenu_item78_func(void) { Runtime::GetNppExec().OnUserMenuItem(78); }
void usermenu_item79_func(void) { Runtime::GetNppExec().OnUserMenuItem(79); }
void usermenu_item80_func(void) { Runtime::GetNppExec().OnUserMenuItem(80); }
void usermenu_item81_func(void) { Runtime::GetNppExec().OnUserMenuItem(81); }
void usermenu_item82_func(void) { Runtime::GetNppExec().OnUserMenuItem(82); }
void usermenu_item83_func(void) { Runtime::GetNppExec().OnUserMenuItem(83); }
void usermenu_item84_func(void) { Runtime::GetNppExec().OnUserMenuItem(84); }
void usermenu_item85_func(void) { Runtime::GetNppExec().OnUserMenuItem(85); }
void usermenu_item86_func(void) { Runtime::GetNppExec().OnUserMenuItem(86); }
void usermenu_item87_func(void) { Runtime::GetNppExec().OnUserMenuItem(87); }
void usermenu_item88_func(void) { Runtime::GetNppExec().OnUserMenuItem(88); }
void usermenu_item89_func(void) { Runtime::GetNppExec().OnUserMenuItem(89); }
void usermenu_item90_func(void) { Runtime::GetNppExec().OnUserMenuItem(90); }
void usermenu_item91_func(void) { Runtime::GetNppExec().OnUserMenuItem(91); }
void usermenu_item92_func(void) { Runtime::GetNppExec().OnUserMenuItem(92); }
void usermenu_item93_func(void) { Runtime::GetNppExec().OnUserMenuItem(93); }
void usermenu_item94_func(void) { Runtime::GetNppExec().OnUserMenuItem(94); }
void usermenu_item95_func(void) { Runtime::GetNppExec().OnUserMenuItem(95); }
void usermenu_item96_func(void) { Runtime::GetNppExec().OnUserMenuItem(96); }
void usermenu_item97_func(void) { Runtime::GetNppExec().OnUserMenuItem(97); }
void usermenu_item98_func(void) { Runtime::GetNppExec().OnUserMenuItem(98); }
void usermenu_item99_func(void) { Runtime::GetNppExec().OnUserMenuItem(99); }

void (*pUserMenuFuncs[MAX_USERMENU_ITEMS])(void) = {
    usermenu_item00_func,
    usermenu_item01_func,
    usermenu_item02_func,
    usermenu_item03_func,
    usermenu_item04_func,
    usermenu_item05_func,
    usermenu_item06_func,
    usermenu_item07_func,
    usermenu_item08_func,
    usermenu_item09_func,
    usermenu_item10_func,
    usermenu_item11_func,
    usermenu_item12_func,
    usermenu_item13_func,
    usermenu_item14_func,
    usermenu_item15_func,
    usermenu_item16_func,
    usermenu_item17_func,
    usermenu_item18_func,
    usermenu_item19_func,
    usermenu_item20_func,
    usermenu_item21_func,
    usermenu_item22_func,
    usermenu_item23_func,
    usermenu_item24_func,
    usermenu_item25_func,
    usermenu_item26_func,
    usermenu_item27_func,
    usermenu_item28_func,
    usermenu_item29_func,
    usermenu_item30_func,
    usermenu_item31_func,
    usermenu_item32_func,
    usermenu_item33_func,
    usermenu_item34_func,
    usermenu_item35_func,
    usermenu_item36_func,
    usermenu_item37_func,
    usermenu_item38_func,
    usermenu_item39_func,
    usermenu_item40_func,
    usermenu_item41_func,
    usermenu_item42_func,
    usermenu_item43_func,
    usermenu_item44_func,
    usermenu_item45_func,
    usermenu_item46_func,
    usermenu_item47_func,
    usermenu_item48_func,
    usermenu_item49_func,
    usermenu_item50_func,
    usermenu_item51_func,
    usermenu_item52_func,
    usermenu_item53_func,
    usermenu_item54_func,
    usermenu_item55_func,
    usermenu_item56_func,
    usermenu_item57_func,
    usermenu_item58_func,
    usermenu_item59_func,
    usermenu_item60_func,
    usermenu_item61_func,
    usermenu_item62_func,
    usermenu_item63_func,
    usermenu_item64_func,
    usermenu_item65_func,
    usermenu_item66_func,
    usermenu_item67_func,
    usermenu_item68_func,
    usermenu_item69_func,
    usermenu_item70_func,
    usermenu_item71_func,
    usermenu_item72_func,
    usermenu_item73_func,
    usermenu_item74_func,
    usermenu_item75_func,
    usermenu_item76_func,
    usermenu_item77_func,
    usermenu_item78_func,
    usermenu_item79_func,
    usermenu_item80_func,
    usermenu_item81_func,
    usermenu_item82_func,
    usermenu_item83_func,
    usermenu_item84_func,
    usermenu_item85_func,
    usermenu_item86_func,
    usermenu_item87_func,
    usermenu_item88_func,
    usermenu_item89_func,
    usermenu_item90_func,
    usermenu_item91_func,
    usermenu_item92_func,
    usermenu_item93_func,
    usermenu_item94_func,
    usermenu_item95_func,
    usermenu_item96_func,
    usermenu_item97_func,
    usermenu_item98_func,
    usermenu_item99_func
};

#ifdef _SCROLL_TO_LATEST
void scroll2latest_func(void) { Runtime::GetNppExec().OnScrollToLatest(); }
#endif

void int_msgs_func(void)      { Runtime::GetNppExec().OnNoInternalMsgs(); }
void nocmdaliases_func(void)  { Runtime::GetNppExec().OnNoCmdAliases(); }
void console_font_func(void)  { Runtime::GetNppExec().OnSelectConsoleFont(); } 
void help_manual_func(void)   { Runtime::GetNppExec().OnHelpManual(); }
void help_docs_func(void)     { Runtime::GetNppExec().OnHelpDocs(); }
void help_about_func(void)    { Runtime::GetNppExec().OnHelpAbout(); }
BOOL IsWindowsNT(void);


void InitFuncItem(int            nItem,
                  const TCHAR*   szName, 
                  PFUNCPLUGINCMD pFunc, 
                  //bool           bCheck,
                  ShortcutKey*   pShortcut)
{
  lstrcpy(g_funcItem[nItem]._itemName, szName);
  g_funcItem[nItem]._pFunc = pFunc;
  g_funcItem[nItem]._init2Check = false; //bCheck;
  g_funcItem[nItem]._pShKey = pShortcut;
}

void InitShortcut(int nItem, bool isAlt, bool isCtrl, bool isShift, unsigned char key)
{
  g_funcShortcut[nItem]._isAlt = isAlt;
  g_funcShortcut[nItem]._isCtrl = isCtrl;
  g_funcShortcut[nItem]._isShift = isShift;
  g_funcShortcut[nItem]._key = key;
}

void globalInitialize()
{
  Runtime::GetNppExec().Init();
  Runtime::GetNppExec().ReadOptions();

  // ... Plugin menu ...
  
  // empty shortcuts initialization:
  ::ZeroMemory(g_funcShortcut, (nbFunc + MAX_USERMENU_ITEMS + 1)*sizeof(ShortcutKey));
  
  // init shortcuts:
  InitShortcut(N_DO_EXEC, false, false, false, Runtime::GetOptions().GetUint(OPTU_PLUGIN_HOTKEY));
  InitShortcut(N_DIRECT_EXEC, false, true, false, Runtime::GetOptions().GetUint(OPTU_PLUGIN_HOTKEY));
  InitShortcut(N_OUTPUT_FILTER, false, false, true,  Runtime::GetOptions().GetUint(OPTU_PLUGIN_HOTKEY));
  InitShortcut(N_TOGGLECONSOLE, false, true,  false, VK_OEM_3); // the '~' key

  // init menu items:
  InitFuncItem(N_DO_EXEC,         _T("Execute..."),                    do_exec_func,        &g_funcShortcut[N_DO_EXEC]);
  InitFuncItem(N_DIRECT_EXEC,     _T("Direct Execute Previous"),       direct_exec_func,    &g_funcShortcut[N_DIRECT_EXEC]);
  InitFuncItem(N_SHOWCONSOLE,     _T("Show Console Dialog"),           show_console_func,   &g_funcShortcut[N_SHOWCONSOLE]);
  InitFuncItem(N_TOGGLECONSOLE,   _T("Toggle Console Dialog"),         toggle_console_func, &g_funcShortcut[N_TOGGLECONSOLE]);
  InitFuncItem(N_GOTO_NEXT_ERROR, _T("Go to next error"),              go_to_next_error,    NULL);
  InitFuncItem(N_GOTO_PREV_ERROR, _T("Go to previous error"),          go_to_prev_error,    NULL);
  InitFuncItem(N_SEPARATOR_1,     _T(""),                              /*empty_func*/NULL,  NULL);
  InitFuncItem(N_CMDHISTORY,      _T("Console Commands History"),      cmdhistory_func,     NULL);
  InitFuncItem(N_CONSOLE_ENC,     _T("Console Output..."),             console_enc_func,    NULL);

#ifdef _SCROLL_TO_LATEST      
  InitFuncItem(N_SCROLL2LATEST,   _T("Always scroll to latest line"),  scroll2latest_func,  NULL);
#endif

  InitFuncItem(N_NOINTMSGS,       _T("No internal messages"),          int_msgs_func,       NULL);
  InitFuncItem(N_SAVEONEXECUTE,   _T("Save all files on execute"),     saveonexecute_func,  NULL);
  InitFuncItem(N_CDCURDIR,        _T("Follow $(CURRENT_DIRECTORY)"),   cdcurdir_func,       NULL);
  InitFuncItem(N_NOCMDALIASES,    _T("Disable command aliases"),       nocmdaliases_func,   &g_funcShortcut[N_NOCMDALIASES]);
  InitFuncItem(N_SEPARATOR_2,     _T(""),                              /*empty_func*/NULL,  NULL);
  InitFuncItem(N_OUTPUT_FILTER,   _T("Console Output Filters..."),     output_f_func,       &g_funcShortcut[N_OUTPUT_FILTER]);
  InitFuncItem(N_ADV_OPTIONS,     _T("Advanced Options..."),           adv_opt_func,        NULL);
  InitFuncItem(N_CONSOLE_FONT,    _T("Change Console Font..."),        console_font_func,   NULL);
  InitFuncItem(N_SEPARATOR_3,     _T(""),                              /*empty_func*/NULL,  NULL);
  InitFuncItem(N_HELP_MANUAL,     _T("Help/Manual"),                   help_manual_func,    NULL);
  InitFuncItem(N_HELP_DOCS,       _T("Help/Docs..."),                  help_docs_func,      NULL);
  InitFuncItem(N_HELP_ABOUT,      _T("Help/About..."),                 help_about_func,     NULL);

  for ( int i = 0; i < MAX_USERMENU_ITEMS; i++ )
  {
    int len = 0;
    const TCHAR* pStr = Runtime::GetOptions().GetStr(OPTS_USERMENU_ITEM01 + i, &len);
    if ( len > 0 )
    {
      TCHAR szMenuItem[MAX_SCRIPTNAME];
      
      lstrcpy(szMenuItem, pStr);

      len = c_base::_tstr_unsafe_rfind(szMenuItem, len, cszUserMenuItemSep);
      if ( len >= 0 )
        szMenuItem[len] = 0;
      if ( (len < 0) && (lstrcmp(szMenuItem, cszUserMenuSeparator) == 0) )
      {
        InitFuncItem(nbFunc + g_nUserMenuItems + 1, // g_funcItem[nbFunc] is a separator
          _T(""), empty_func, NULL); // separator, use empty_func here!!!
      }
      else
      {
        InitFuncItem(nbFunc + g_nUserMenuItems + 1, // g_funcItem[nbFunc] is a separator
          szMenuItem, pUserMenuFuncs[g_nUserMenuItems], NULL);
      }
      ++g_nUserMenuItems;
    }
  }

  if ( g_nUserMenuItems > 0 )
  {
    // separator
    InitFuncItem(nbFunc, _T(""), empty_func, NULL); // <-- use empty_func here!!!
  }

  g_bInitialized = true;
}

void globalUninitialize()
{
  if ( g_bInitialized )
  {
    Runtime::GetNppExec().StopAutoSaveTimer();
    if ( !g_bOptionsSavedOnNppnShutdown )
    {
      Runtime::GetNppExec().SaveOptions();
    }
    Runtime::GetNppExec().Free();

    g_bInitialized = false;
  }
}

extern "C" BOOL APIENTRY DllMain( 
                       HINSTANCE hInstance, 
                       DWORD     dwReason, 
                       LPVOID    lpReserved )
{
  Runtime::GetNppExec().m_hDllModule = (HMODULE) hInstance;
  
  switch (dwReason)
  {
    case DLL_PROCESS_ATTACH:
    {
      g_bInitialized = false;
      g_bIsNppReady = false;
        
      //now in the setInfo:
      //globalInitialize();

    }                                                      
      break;

    case DLL_PROCESS_DETACH:
    {
      globalUninitialize();
      g_bIsNppReady = false;

    }
      break;

    case DLL_THREAD_ATTACH:
      break;

    case DLL_THREAD_DETACH:
      break;

    default:
      break;
  }

  return TRUE;
}

extern "C" __declspec(dllexport) void setInfo(NppData notpadPlusData)
{
    Runtime::GetNppExec().m_nppData = notpadPlusData;

    if ( ::GetProp(notpadPlusData._nppHandle, PROP_NPPEXEC_DLL) )
    {
        Runtime::GetNppExec().m_bAnotherNppExecDllExists = true;
    }
    ::SetProp(notpadPlusData._nppHandle, PROP_NPPEXEC_DLL, Runtime::GetNppExec().m_hDllModule);

    int ver = (int) ::SendMessage(notpadPlusData._nppHandle, NPPM_GETNPPVERSION, 0, 0);
    if ( (HIWORD(ver) < 5) ||
         ((HIWORD(ver) == 5) && (LOWORD(ver) < 1)) )
    {
      ::MessageBox(
          notpadPlusData._nppHandle, 
          _T("This version of NppExec requires Notepad++ ver. 5.1 or higher"),
          _T("NppExec Initialization Error"),
          MB_OK | MB_ICONERROR
      );
      return;
    }

    g_bIsNppUnicode = ::IsWindowUnicode(notpadPlusData._nppHandle);
    
    globalInitialize();
    
}

extern "C" __declspec(dllexport) const TCHAR * getName()
{
  return PLUGIN_NAME;
}

extern "C" __declspec(dllexport) FuncItem * getFuncsArray(int *nbF)
{
  if (g_nUserMenuItems > 0)
    *nbF = nbFunc + g_nUserMenuItems + 1;
  else
    *nbF = nbFunc;
  return g_funcItem;
}

void UpdateCurrentDirectory()
{
    if ( Runtime::GetOptions().GetBool(OPTB_CONSOLE_CDCURDIR) )
    {
        TCHAR szFileDir[FILEPATH_BUFSIZE]; 

        szFileDir[0] = 0;
        Runtime::GetNppExec().SendNppMsg( NPPM_GETCURRENTDIRECTORY,
            (WPARAM) (FILEPATH_BUFSIZE - 1), (LPARAM) szFileDir);
        if ( szFileDir[0] )
        {
            ::SetCurrentDirectory(szFileDir);
        }
    }
}

extern "C" __declspec(dllexport) void beNotified(SCNotification *notifyCode)
{
    static bool bNppReady = false;

    if ( notifyCode->nmhdr.code == NPPN_SHORTCUTREMAPPED )
    {
        const int n = (g_nUserMenuItems > 0) ? (nbFunc + g_nUserMenuItems + 1) : nbFunc;
        for ( int i = 0; i < n; i++ )
        {
            if ( g_funcItem[i]._cmdID == notifyCode->nmhdr.idFrom )
            {
                if ( g_funcItem[i]._pFunc && (g_funcItem[i]._pFunc != empty_func) )
                {
                    ShortcutKey* pKey = (ShortcutKey *) notifyCode->nmhdr.hwndFrom;
                    ::CopyMemory( &g_funcShortcut[i], pKey, sizeof(ShortcutKey) );
                }
                break;
            }
        }
        return;
    }
  
    if ( notifyCode->nmhdr.hwndFrom == Runtime::GetNppExec().m_nppData._nppHandle )
    {
  
        if ( notifyCode->nmhdr.code == NPPN_BUFFERACTIVATED || 
             notifyCode->nmhdr.code == NPPN_FILESAVED )
        {
            if ( bNppReady )
                UpdateCurrentDirectory();
        }

        else if ( notifyCode->nmhdr.code == NPPN_READY )
        {
            if ( Runtime::GetLogger().IsLogFileOpen() )
            {
                Runtime::GetLogger().Add( _T("; NPPN_READY - start") );
                Runtime::GetLogger().IncIndentLevel();
            }

            bNppReady = true;
            g_bIsNppReady = true;
            g_bUseThreadForScriptExecution = true; // just in case

            HANDLE hProp = ::GetProp(Runtime::GetNppExec().m_nppData._nppHandle, PROP_NPPEXEC_DLL);
            if ( hProp && (hProp != Runtime::GetNppExec().m_hDllModule) )
            {
                Runtime::GetNppExec().m_bAnotherNppExecDllExists = true;
            }

// get rid of the "conversion from 'LONG_PTR' to 'LONG'" shit
// get rid of the "conversion from 'LONG' to 'WNDPROC'" shit
// (Microsoft itself does not know what is 'LONG_PTR'?)
#pragma warning(push)
#pragma warning(disable: 4244)
#pragma warning(disable: 4312)

#ifdef UNICODE
            nppOriginalWndProc = (WNDPROC) SetWindowLongPtrW( 
                Runtime::GetNppExec().m_nppData._nppHandle, GWLP_WNDPROC, 
                (LONG_PTR) nppPluginWndProc );
#else
            if ( g_bIsNppUnicode )
            {
                nppOriginalWndProc = (WNDPROC) SetWindowLongPtrW( 
                  Runtime::GetNppExec().m_nppData._nppHandle, GWLP_WNDPROC, 
                    (LONG_PTR) nppPluginWndProc );
            }
            else
            {
                nppOriginalWndProc = (WNDPROC) SetWindowLongPtrA( 
                  Runtime::GetNppExec().m_nppData._nppHandle, GWLP_WNDPROC, 
                    (LONG_PTR) nppPluginWndProc );
            }
#endif

#pragma warning(pop)

            UpdateCurrentDirectory();
        
            HMENU hMenu = Runtime::GetNppExec().GetNppMainMenu();
            if ( hMenu )
            {
                ModifyMenu(hMenu, g_funcItem[N_SEPARATOR_1]._cmdID, 
                  MF_BYCOMMAND | MF_SEPARATOR, g_funcItem[N_SEPARATOR_1]._cmdID, NULL);

                ModifyMenu(hMenu, g_funcItem[N_SEPARATOR_2]._cmdID, 
                  MF_BYCOMMAND | MF_SEPARATOR, g_funcItem[N_SEPARATOR_2]._cmdID, NULL);

                ModifyMenu(hMenu, g_funcItem[N_SEPARATOR_3]._cmdID, 
                  MF_BYCOMMAND | MF_SEPARATOR, g_funcItem[N_SEPARATOR_3]._cmdID, NULL);

                if ( g_nUserMenuItems > 0 )
                {
                    TCHAR        szItemText[128];
                    MENUITEMINFO mii;
                    HMENU        hMacrosMenu = NULL;
            
                    if ( Runtime::GetOptions().GetBool(OPTB_USERMENU_NPPMACROS) )
                    {
                        int iMenuItems = GetMenuItemCount(hMenu);
                        for ( int i = 0; i < iMenuItems; i++ )
                        {
                            HMENU hSubMenu = ::GetSubMenu(hMenu, i);
                            // does IDM_MACRO_STOPRECORDINGMACRO exist here?
                            if ( ::GetMenuState(hSubMenu, IDM_MACRO_STOPRECORDINGMACRO, MF_BYCOMMAND) != -1 )
                            {
                                // this is the Macro sub-menu
                                hMacrosMenu = hSubMenu;
                                break;
                            }
                        }
                    }

                    for ( int i = nbFunc; i < nbFunc + g_nUserMenuItems + 1; i++ )
                    {
                        szItemText[0] = 0;
                        ::ZeroMemory( &mii, sizeof(MENUITEMINFO) );
                        mii.cbSize = sizeof(MENUITEMINFO);
                        mii.fMask = MIIM_FTYPE | MIIM_ID | MIIM_STATE | MIIM_STRING | MIIM_SUBMENU;
                        mii.dwTypeData = (TCHAR*) szItemText;
                        mii.cch = 128 - 1;

                        if ( ::GetMenuItemInfo(hMenu, g_funcItem[i]._cmdID, FALSE, &mii) )
                        {
                            if ( hMacrosMenu )
                            {
                                ::RemoveMenu( hMenu, g_funcItem[i]._cmdID, MF_BYCOMMAND );
                                if ( (i == nbFunc) || (szItemText[0] == 0) )
                                    ::AppendMenu( hMacrosMenu, MF_SEPARATOR, 0, NULL ); // separator
                                else
                                    ::InsertMenuItem( hMacrosMenu, g_funcItem[i]._cmdID, FALSE, &mii );
                            }
                            else
                            {
                                if ( (i == nbFunc) || (szItemText[0] == 0) )
                                {
                                    // separator
                                    mii.fMask = MIIM_FTYPE | MIIM_ID | MIIM_STATE | MIIM_STRING | MIIM_SUBMENU;
                                    mii.fType = MFT_SEPARATOR;
                                    mii.wID = 0;
                                    ::SetMenuItemInfo(hMenu, g_funcItem[i]._cmdID, FALSE, &mii);
                                }
                            }
                        }
                    }

                } // <--- g_nUserMenuItems > 0

                CheckMenuItem(hMenu, g_funcItem[N_CMDHISTORY]._cmdID,
                  MF_BYCOMMAND | (Runtime::GetOptions().GetBool(OPTB_CONSOLE_CMDHISTORY) ? MF_CHECKED : MF_UNCHECKED));

#ifdef _SCROLL_TO_LATEST        
                CheckMenuItem(hMenu, g_funcItem[N_SCROLL2LATEST]._cmdID,
                  MF_BYCOMMAND | (Runtime::GetOptions().GetBool(OPTB_CONSOLE_SCROLL2LATEST) ? MF_CHECKED : MF_UNCHECKED));
#endif

                CheckMenuItem(hMenu, g_funcItem[N_NOINTMSGS]._cmdID,
                  MF_BYCOMMAND | (Runtime::GetOptions().GetBool(OPTB_CONSOLE_NOINTMSGS) ? MF_CHECKED : MF_UNCHECKED));
      
                Runtime::GetNppExec().UpdateConsoleEncoding();

                Runtime::GetNppExec().UpdateOutputFilterMenuItem();
                Runtime::GetNppExec().UpdateGoToErrorMenuItem();

                //#ifdef _GVC
                CheckMenuItem(hMenu, g_funcItem[N_SAVEONEXECUTE]._cmdID,
                  MF_BYCOMMAND | (Runtime::GetOptions().GetBool(OPTB_CONSOLE_SAVEONEXECUTE) ? MF_CHECKED : MF_UNCHECKED));
                //#endif

                CheckMenuItem(hMenu, g_funcItem[N_CDCURDIR]._cmdID,
                  MF_BYCOMMAND | (Runtime::GetOptions().GetBool(OPTB_CONSOLE_CDCURDIR) ? MF_CHECKED : MF_UNCHECKED));

                EnableMenuItem(hMenu, g_funcItem[N_NOCMDALIASES]._cmdID,
                  MF_BYCOMMAND | MF_GRAYED );

                /*
                EnableMenuItem(hMenu, g_funcItem[N_CONSOLE_FONT]._cmdID,
                  MF_BYCOMMAND | (Runtime::GetNppExec()._consoleIsVisible ? MF_ENABLED : MF_GRAYED) );
                */
            } // <--- hMenu
    
            const int n = (g_nUserMenuItems > 0) ? (nbFunc + g_nUserMenuItems + 1) : nbFunc;
            for ( int i = 0; i < n; i++ )
            {
                if ( g_funcItem[i]._pFunc && (g_funcItem[i]._pFunc != empty_func) )
                {
                    Runtime::GetNppExec().SendNppMsg( NPPM_GETSHORTCUTBYCMDID, 
                        g_funcItem[i]._cmdID, (LPARAM) &g_funcShortcut[i] );
                }
            }
      
            if ( (Runtime::GetOptions().GetInt(OPTI_CONSOLE_VISIBLE) == CAdvOptDlg::CON_YES) || 
                 Runtime::GetOptions().GetBool(OPTB_CONSOLE_SHOWHELP) )
            {
                Runtime::GetNppExec().showConsoleDialog(false);
            }

            if ( Runtime::GetOptions().GetBool(OPTB_CONSOLE_SHOWHELP) )
            {
                Runtime::GetNppExec().printConsoleHelpInfo();
            }
      
            Runtime::GetNppExec().DoExecScript( Runtime::GetOptions().GetStr(OPTS_SCRIPT_NPPSTART), false );

            if ( Runtime::GetLogger().IsLogFileOpen() )
            {
                Runtime::GetLogger().DecIndentLevel();
                Runtime::GetLogger().Add( _T("; NPPN_READY - end") );
            }

        } // NPPN_READY

        else if (notifyCode->nmhdr.code == NPPN_SHUTDOWN)
        {

            if ( Runtime::GetLogger().IsLogFileOpen() )
            {
                Runtime::GetLogger().Add( _T("; NPPN_SHUTDOWN - start") );
                Runtime::GetLogger().IncIndentLevel();
            }
        
            g_bUseThreadForScriptExecution = false; 
            // "ugly" solution: 
            // script must not be run in different thread
            // when exiting (see CNppExec::CreateScriptThread)
            Runtime::GetNppExec().DoExecScript( Runtime::GetOptions().GetStr(OPTS_SCRIPT_NPPEXIT), false );
            
            if ( ::GetProp(Runtime::GetNppExec().m_nppData._nppHandle, PROP_NPPEXEC_DLL) )
            {
                ::RemoveProp(Runtime::GetNppExec().m_nppData._nppHandle, PROP_NPPEXEC_DLL);
            }

            /*if ( nppOriginalWndProc )
            {
                if ( g_bIsNppUnicode )
                {
                    ::SetWindowLongPtrW(Runtime::GetNppExec().m_nppData._nppHandle, 
                        GWLP_WNDPROC, (LONG_PTR) nppOriginalWndProc);
                }
                else
                {
                    ::SetWindowLongPtrA(Runtime::GetNppExec().m_nppData._nppHandle, 
                        GWLP_WNDPROC, (LONG_PTR) nppOriginalWndProc);
                }
            }*/

            // to be sure the options are saved even in case of some 
            // issues at Notepad++'s exit (e.g. crash of some plugin)
            Runtime::GetNppExec().SaveOptions();
            g_bOptionsSavedOnNppnShutdown = true;

            if ( Runtime::GetLogger().IsLogFileOpen() )
            {
                Runtime::GetLogger().DecIndentLevel();
                Runtime::GetLogger().Add( _T("; NPPN_SHUTDOWN - end") );
            }

        } // NPPN_SHUTDOWN
    
        else if ( notifyCode->nmhdr.code == NPPN_TBMODIFICATION )
        {
            const int nToolbarBtn = Runtime::GetOptions().GetInt(OPTI_TOOLBARBTN);
            if ( nToolbarBtn )
            {
                Runtime::GetNppExec().m_TB_Icon.hToolbarBmp = (HBITMAP) ::LoadImage( 
                  (HINSTANCE) Runtime::GetNppExec().m_hDllModule, MAKEINTRESOURCE(IDI_CONSOLEBITMAP), 
                    IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE | LR_LOADMAP3DCOLORS );

                int cmdID = g_funcItem[N_SHOWCONSOLE]._cmdID;
                if ( nToolbarBtn == 2 )
                    cmdID = g_funcItem[N_DO_EXEC]._cmdID;
                else if ( nToolbarBtn == 3 )
                    cmdID = g_funcItem[N_DIRECT_EXEC]._cmdID;
                Runtime::GetNppExec().SendNppMsg( NPPM_ADDTOOLBARICON, 
                  (WPARAM) cmdID, (LPARAM) &Runtime::GetNppExec().m_TB_Icon );
            }
        } // NPPN_TBMODIFICATION
    
    }

}

extern "C" __declspec(dllexport) LRESULT messageProc(UINT Message, WPARAM wParam, LPARAM lParam)
{
  if ( Message == NPPM_MSGTOPLUGIN )
  {
    CommunicationInfo* pci = (CommunicationInfo *) lParam;
    if ( pci )
    {
      ProcessExternalPluginMsg( pci->internalMsg, pci->srcModuleName, pci->info );
      if ( pci->internalMsg == NPEM_SUSPENDEDACTION )
      {
        delete pci;
      }
    }
  }
  return TRUE;
}

#ifdef UNICODE
extern "C" __declspec(dllexport) BOOL isUnicode()
{
  return TRUE;
}
#endif

///////////////////////////////////////////////////////////////////////////
// CNppExec class

CNppExec::CNppExec()
{
  m_MacroVars.SetNppExec(this);
  m_pRunningScriptEngine = 0;

  m_TB_Icon.hToolbarBmp = NULL;
  m_TB_Icon.hToolbarIcon = NULL;

  npp_nbFiles = 0;
  npp_bufFileNames.Clear();

  _consoleFont = NULL;
  _consoleIsVisible = false;
  _consoleProcessBreak = false;
  _consoleProcessIsRunning = false;
  _consoleProcessHandle = NULL;

  m_bAnotherNppExecDllExists = false;
  
  m_TempScriptIsModified = false;
  m_hDllModule = NULL;
  m_nppData._nppHandle = NULL;
  m_nppData._scintillaMainHandle = NULL;
  m_nppData._scintillaSecondHandle = NULL;
  //m_hRichEditDll = LoadLibrary( _T("Riched32.dll") ); // RichEdit 1.0
  m_hRichEditDll = LoadLibrary( _T("Riched20.dll") ); // RichEdit 2.0
  
  // it's not necessary, but just in case :)
  m_hStdInReadPipe = NULL;
  m_hStdInWritePipe = NULL; 
  m_hStdOutReadPipe = NULL;
  m_hStdOutWritePipe = NULL;

  m_hScriptThread = NULL;
  m_idTimerAutoSave = 0;
  m_isSavingOptions = false;
  m_hFocusedWindowBeforeScriptStarted = NULL;

  m_nChildProcessExitCode = 0;

  ::InitializeCriticalSection(&m_csAutoSave);
}

CNppExec::~CNppExec()
{
  ::DeleteCriticalSection(&m_csAutoSave);

  if (m_hRichEditDll != NULL)
    FreeLibrary(m_hRichEditDll);

  if (_consoleFont != NULL)
    ::DeleteObject(_consoleFont);

/*
#ifdef _NIB
  Runtime::GetWarningAnalyzer().~CWarningAnalyzer();
#endif
*/
}

HWND CNppExec::getCurrentScintilla(INT which)
{
  return ((which == 0) ? m_nppData._scintillaMainHandle : 
             m_nppData._scintillaSecondHandle);
}

const TCHAR* isReservedFileName(LPCTSTR cszFileName)
{
    int len = c_base::_tstr_safe_len(cszFileName);
    if ( len > 0 )
    {
        int i = c_base::_tstr_unsafe_rfindoneof( cszFileName, len, _T("\\/") );
        if ( i < 0 )
            i = 0;
        else
            ++i;

        if ( i < len )
        {
            const int RESERVED_NAMES_COUNT = 4;
            const TCHAR* cszReservedNames[RESERVED_NAMES_COUNT] = {
                _T("CON"),
                _T("PRN"),
                _T("AUX"),
                _T("NUL")
                //_T("COM1"),
                //_T("COM2"),
                //_T("LPT1"),
                //_T("LPT2"),
            };

            TCHAR szName[MAX_PATH + 1];
            
            lstrcpy(szName, cszFileName + i);
            i = c_base::_tstr_unsafe_findch( szName, _T('.') );
            if ( i > 0 )
                szName[i] = 0;

            ::CharUpper(szName);
    
            for ( i = 0; i < RESERVED_NAMES_COUNT; i++ )
            {
                if ( lstrcmp(szName, cszReservedNames[i]) == 0 )
                    return cszReservedNames[i];
            }
        }
    }

    return NULL;
}

int CNppExec::conLoadFrom(LPCTSTR cszFile)
{
  CFileBufT<TCHAR> fbuf;
  if ( fbuf.LoadFromFile(cszFile) )
  {
    tstr Line;

    _consoleProcessIsRunning = true;
    _consoleProcessBreak = false;

    while ( _consoleIsVisible && (!_consoleProcessBreak) && (fbuf.GetLine(Line) >= 0) )
    {
      GetConsole().PrintOutput(Line.c_str(), true);
    }

    _consoleProcessIsRunning = false;
    return fbuf.GetBufCount()*sizeof(TCHAR);
  }
  return -1;
}

typedef struct sConSaveStruct {
    FILE* f;
    LONG  size;
} ConSaveStruct;

DWORD CALLBACK reConsoleStreamSave(DWORD_PTR dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb)
{
  ConSaveStruct* pcss = (ConSaveStruct *) dwCookie;
  if ( (cb > 0) && writefile(pcss->f, pbBuff, cb) )
  {
    pcss->size += cb;
    *pcb = cb;
  }
  else
  {
    *pcb = 0;
  }
  return 0;
}

int CNppExec::conSaveTo(LPCTSTR cszFile)
{
  const TCHAR* pReservedName = isReservedFileName(cszFile);
  if ( pReservedName )
  {
      tstr Err = _T("- file name \"");
      Err += pReservedName;
      Err += _T("\" is reserved by OS");
      GetConsole().PrintError( Err.c_str() );
      return -1;
  }
    
  ConSaveStruct css;
  
  css.size = 0;
  css.f = openfile(cszFile, true);
  if ( css.f != NULL )
  {
    EDITSTREAM es;

    if ( sizeof(TCHAR) == sizeof(wchar_t) )
    {
      // Unicode
      writefile( css.f, "\xFF\xFE", 2 );
      css.size = 2;
    }

    es.dwCookie = (DWORD_PTR) &css;
    es.dwError = 0;
    es.pfnCallback = reConsoleStreamSave;
    if ( sizeof(TCHAR) == sizeof(wchar_t) )
      GetConsole().GetConsoleEdit().SendMsg( EM_STREAMOUT, SF_TEXT | SF_UNICODE, (LPARAM) &es );
    else
      GetConsole().GetConsoleEdit().SendMsg( EM_STREAMOUT, SF_TEXT, (LPARAM) &es );
    
    closefile(css.f); 
    return css.size;
  }
  return -1;
}

int CNppExec::selLoadFrom(LPCTSTR cszFile)
{
  CFileBufT<TCHAR> fbuf;
  if ( fbuf.LoadFromFile(cszFile) )
  {
    const TCHAR tch = 0; // trailing '\0'

    fbuf.GetBufPtr()->Append( tch );

    #ifdef UNICODE
      selSetText( fbuf.GetBufData() + 1 ); // skip leading 0xFEFF
    #else
      selSetText( fbuf.GetBufData() );
    #endif

    return (fbuf.GetBufCount() - 1)*sizeof(TCHAR);
  }
  return -1;
}

int CNppExec::selSaveTo(LPTSTR szFileAndEncoding)
{
  enum eEnc {
    encAsIs = 0,
    encANSI,
    encUTF8,
    encUCS2LE
  };

  int enc = encAsIs; // 0 - as is, 1 - ANSI, 2 - UTF-8, 3 - Unicode (UCS-2 LE)

  if ( szFileAndEncoding && (szFileAndEncoding[0] == _T('\"')) )
      ++szFileAndEncoding; // skip first \"
  
  int i = c_base::_tstr_safe_len( szFileAndEncoding );
  i = c_base::_tstr_safe_rfindch( szFileAndEncoding, i, _T(':') );
  if ( i >= 0 ) 
  {
    if ( (szFileAndEncoding[i+1] != _T('\\')) && (szFileAndEncoding[i+1] != _T('/')) )
    {
      TCHAR* p = c_base::_tstr_unsafe_skip_tabspaces(szFileAndEncoding + i + 1);
      if ( *p )
      {
        if ( *p == _T('a') || *p == _T('A') )
          enc = encANSI;
        else if ( *p == _T('u') || *p == _T('U') )
          enc = encUTF8;
        else if ( *p == _T('w') || *p == _T('W') )
          enc = encUCS2LE;
      }
      p = c_base::_tstr_unsafe_rskip_tabspaces(szFileAndEncoding, i);
      if ( *p == _T('\"') )
        *p = 0; // skip last \" in file name
      else
        *(++p) = 0;
    }
  }

  if ( szFileAndEncoding[0] == 0 || szFileAndEncoding[0] == _T(':') )
    return -1;

  const TCHAR* pReservedName = isReservedFileName(szFileAndEncoding);
  if ( pReservedName )
  {
      tstr Err = _T("- file name \"");
      Err += pReservedName;
      Err += _T("\" is reserved by OS");
      GetConsole().PrintError( Err.c_str() );
      return -1;
  }
    
  HWND hSci = GetScintillaHandle();
  int nSciCodePage = (int) ::SendMessage(hSci, SCI_GETCODEPAGE, 0, 0);
  int nSciSelStart = (int) ::SendMessage(hSci, SCI_GETSELECTIONSTART, 0, 0);
  int nSciSelEnd = (int) ::SendMessage(hSci, SCI_GETSELECTIONEND, 0, 0);
  int nSciSelLen = nSciSelEnd - nSciSelStart;

  char* pSciText = new char[nSciSelLen + 2];
  if ( pSciText )
  {
    char* pOutText = NULL;
    int   nOutSize = 0;
  
    pSciText[0] = 0;
    ::SendMessage( hSci, SCI_GETSELTEXT, 0, (LPARAM) pSciText );
    
    if ( nSciSelLen == 0 )
    {
      // no selected text
      pOutText = pSciText; // empty string in this case
    }
    else if ( nSciCodePage == SC_CP_UTF8 )
    {
      switch ( enc )
      {
        case encANSI:  // ANSI
          pOutText = SysUniConv::newUTF8ToMultiByte(pSciText, -1, CP_ACP, &nOutSize);
          delete [] pSciText;
          break;
        case encUCS2LE:  // Unicode
          pOutText = (char *) SysUniConv::newUTF8ToUnicode(pSciText, -1, &nOutSize);
          nOutSize *= 2;
          delete [] pSciText;
          break; 
        default:
          pOutText = pSciText;
          nOutSize = lstrlenA(pOutText);
          break;
      }
    }
    else
    {
      // multi-byte encoding
      if ( nSciCodePage == 0 )  nSciCodePage = CP_ACP;
      switch ( enc )
      {
        case encUTF8:  // UTF-8
          pOutText = SysUniConv::newMultiByteToUTF8(pSciText, -1, nSciCodePage, &nOutSize);
          delete [] pSciText;
          break;
        case encUCS2LE:  // Unicode
          pOutText = (char *) SysUniConv::newMultiByteToUnicode(pSciText, -1, nSciCodePage, &nOutSize);
          nOutSize *= 2;
          delete [] pSciText;
          break; 
        default:
          pOutText = pSciText;
          nOutSize = lstrlenA(pOutText);
          break;
      }
    }

    if ( pOutText )
    {
      int nWritten = -1; // -1 means "failed to write"

      FILE* f = openfile(szFileAndEncoding, true);
      if ( f != NULL )
      {
        nWritten = 0; // 0 bytes written yet
        if ( nSciSelLen > 0 )
        {
          if ( enc == encUCS2LE )
          {
            // UCS-2 LE "BOM"
            if ( writefile(f, (void *) "\xFF\xFE", 2) )
              nWritten += 2;
          }
          else if ( (enc == encUTF8) || (enc == encAsIs && nSciCodePage == SC_CP_UTF8) )
          {
            // UTF-8 "BOM"
            if ( writefile(f, (void *) "\xEF\xBB\xBF", 3) )
              nWritten += 3;
          }
        }
        if ( writefile(f, pOutText, nOutSize) )
          nWritten += nOutSize;
        closefile(f);
      }
        
      delete [] pOutText;
      return nWritten;
    }
  }
    
  return -1;
}

void CNppExec::selSetText(LPCTSTR cszText)
{
  HWND  hSci = GetScintillaHandle();
  char* pBufData = SciTextFromLPCTSTR(cszText, hSci);
  
  if ( pBufData )
  {
    ::SendMessage( hSci, SCI_REPLACESEL, 0, (LPARAM) pBufData );
    #ifdef UNICODE
      delete [] pBufData;
    #else
      if ( pBufData != cszText )
        delete [] pBufData;
    #endif
  }
}

int CNppExec::findFileNameIndexInNppOpenFileNames(const tstr& fileName, bool bGetOpenFileNames)
{
  if (bGetOpenFileNames)
    nppGetOpenFileNames();

  tstr S;
  tstr S1 = fileName;

  S1.Replace(_T('/'), _T('\\'));
  // nightmare begins -->
  {
    // a terrible test paths:
    /*
    NPP_SWITCH ".\..\.\.\\\.\.\\..\\C:\\\.\a\.\\.\b\\..\c\\\..\\\\d\\.\f"
    NPP_SWITCH "..\.\.\\\.\.\\..\\C:\\\.\a\.\\.\b\\..\c\\\..\\\\d\\.\\f"
    NPP_SWITCH "\..\.\.\\\.\.\\..\\C:\\\.\a\.\\.\b\\..\c\\\..\\\\d\\.\\\\f"
    NPP_SWITCH "\.\.\..\\\.\.\\..\\C:\\\.\a\.\\.\b\\..\c\\\..\\\\d\\..\f"
    NPP_SWITCH "\..\..\..\..\\.\.\.\..\..\\\\.\.\\..\\C:\\\.\a\.\\.\b\\..\c\\\..\\\\d\\..\f"
    NPP_SWITCH "x\y\z\..\..\..\..\\.\.\.\..\..\\\\.\.\\..\\C:\\\.\a\.\\.\b\\..f..\f"
    NPP_SWITCH ".\.\.\.\.\.\a\b"
    NPP_SWITCH "\.\.\.\.\.\.\a\b"
    NPP_SWITCH "..\..\..\..\..\a\b"
    NPP_SWITCH "\..\..\..\..\a\b"
    */
    int i;

    // remove all double '\' except the leading "\\"
    // e.g. "C:\\a\\\b\\c" -> "C:\a\b\c"
    i = 2;
    while ((i = S1.Find(_T("\\\\"), i)) > 0)  
      S1.Delete(i, 1);

    // remove all "subpath\..\"
    // e.g. "a\b\..\..\ab" -> "a\..\ab" -> "ab"
    i = 0;
    while ((i = S1.Find(_T("\\..\\"), i)) >= 0)
    {
      int j = (i == 0) ? 0 : S1.RFind(_T('\\'), i - 1);
      S1.Delete(j + 1, i - j + 3); // when RFind returns -1, (j + 1) = 0
      i = (j < 0) ? 0 : j;
    }

    // remove all leading "..\"
    // e.g. "..\..\..\path" -> "path"
    while (S1.StartsWith(_T("..\\")))
      S1.Delete(0, 3);

    // remove all leading ".\"
    // e.g. ".\.\.\path" -> "path"
    while (S1.StartsWith(_T(".\\")))
      S1.Delete(0, 2);

    // replace all "\.\" with "\"
    // e.g. "a\.\b\.\c" -> "a\b\c"
    i = 0;
    while ((i = S1.Find(_T("\\.\\"), i)) >= 0)
      S1.Delete(i, 2);

    // remove leading '\' if the next character is not '\'
    if (S1.GetAt(0) == _T('\\') && S1.GetAt(1) != _T('\\'))
      S1.Delete(0, 1);
  }
  // <-- nightmare ends

  int nFileLevel = 0;
  bool bFullPath = isFullPath(S1);
  if (!bFullPath)
  {
    for (int j = 0; j < S1.length(); ++j)
    {
      if (S1[j] == _T('\\'))
        ++nFileLevel;
    }
  }

  if (S1.length() > 0)
    ::CharUpper(S1.c_str());

  int iPartialMatch1 = -1;
  int iFind1 = -1;
  for (int i = 0; i < npp_nbFiles; ++i)
  { 
    S = npp_bufFileNames[i];
    S.Replace(_T('/'), _T('\\'));

    if (!bFullPath)
    {
      int n = 0;
      int j = S.length() - 1;
      while (j >= 0)
      {
        if (S[j] == _T('\\'))
        {
          if (n == nFileLevel)
          {
            S.Delete(0, j + 1);
            break;
          }
          else
            ++n;
        }
        --j;
      }
    }

    if (S.length() > 0)
      ::CharUpper(S.c_str());

    if (S == S1)
      return i; // complete match

    if (!bFullPath)
    {
      if (iPartialMatch1 == -1 || iFind1 != 0)
      { // no partial match yet or not beginning of file name yet
        int ifind = S.Find(S1);
        if (ifind >= 0) // position inside file name
        {
          if (iFind1 == -1 || ifind < iFind1)
          { // no partial match yet or closer to beginning of file name
            iFind1 = ifind;
            iPartialMatch1 = i;
          }
        }
      }
    }
  }

  return iPartialMatch1; // first partial match or -1
}

int CNppExec::nppConvertToFullPathName(tstr& fileName, bool bGetOpenFileNames)
{
  
  Runtime::GetLogger().Add(   _T("nppConvertToFullPathName()") );
  Runtime::GetLogger().Add(   _T("{") );
  Runtime::GetLogger().IncIndentLevel();
      
  if (!fileName.IsEmpty())
  {
    
    Runtime::GetLogger().AddEx( _T("[in]  \"%s\""), fileName.c_str() );  
      
    if (!isFullPath(fileName))
    {
      int i = findFileNameIndexInNppOpenFileNames(fileName, bGetOpenFileNames);
      if (i != -1)
      {
        fileName = npp_bufFileNames[i];

        Runtime::GetLogger().AddEx( _T("; changed to: \"%s\""), fileName.c_str() ); 
        Runtime::GetLogger().DecIndentLevel();
        Runtime::GetLogger().Add(   _T("}") );

        // full path found
        return fileName.length();
      }
    }
    else
    {

      Runtime::GetLogger().Add(   _T("; already full path") );
      Runtime::GetLogger().DecIndentLevel();
      Runtime::GetLogger().Add(   _T("}") );

      // already full path
      return fileName.length();
    }

  }
  
  Runtime::GetLogger().DecIndentLevel();
  Runtime::GetLogger().Add(   _T("}") );
    
  return 0;
}

static tstr getMenuItemPathSep(const tstr& menuItemPathName)
{
    const int nSeps = 9;
    const TCHAR* szSeps[nSeps] = {
        _T("\\\\\\"),
        _T("///"),
        _T("|||"),
        _T("\\\\"),
        _T("//"),
        _T("||"),
        _T("\\"),
        _T("/"),
        _T("|")
    };
    for ( int i = 0; i < nSeps; i++ )
    {
        if ( menuItemPathName.Find(szSeps[i]) >= 0 )
            return tstr(szSeps[i]);
    }
    return tstr(_T(""));
}

int CNppExec::nppGetMenuItemIdByName(const tstr& menuItemPathName, tstr& parsedPath, tstr& parsedSep)
{
    int menuItemId = -1;
    parsedPath.Clear();
    parsedSep.Clear();

    HMENU hMenu = GetNppMainMenu();
    if ( hMenu )
    {
        tstr sep = getMenuItemPathSep(menuItemPathName);
        parsedSep = sep;

        CStrSplitT<TCHAR> args;
        int level = args.Split(menuItemPathName, sep.c_str());    
        if ( level > 0 )
        {
            typedef std::map<tstr, UINT_PTR> tMenuItems;
            typedef std::list<tMenuItems> tListOfMenuItems;

            static tListOfMenuItems listOfMenuItems;

            tListOfMenuItems::iterator listItr = listOfMenuItems.begin();
            for ( int n = 0; (n < level) && (menuItemId < 0); ++n, ++listItr )
            {
                UINT_PTR uItem = 0;
                const tstr& itemName = args.GetArg(n);
                if ( itemName.IsEmpty() )
                    break;

                if ( listItr != listOfMenuItems.end() )
                {
                    tMenuItems::const_iterator itemItr = listItr->find( itemName );
                    if ( itemItr != listItr->end() )
                    {
                        uItem = itemItr->second;
                        if ( ::IsMenu((HMENU)uItem) )
                            hMenu = (HMENU) uItem;
                        else
                            menuItemId = (int) uItem;
                    }
                }
                if ( uItem == 0 )
                {
                    TCHAR        szItemText[128];
                    MENUITEMINFO mii;
                    tstr         itmText;

                    const int nItems = ::GetMenuItemCount(hMenu);
                    for ( int i = 0; (i < nItems) && (uItem == 0); i++ )
                    {
                        szItemText[0] = 0;
                        ::ZeroMemory( &mii, sizeof(MENUITEMINFO) );
                        mii.cbSize = sizeof(MENUITEMINFO);
                        mii.fMask = MIIM_ID | MIIM_STRING | MIIM_SUBMENU;
                        mii.dwTypeData = (TCHAR*) szItemText;
                        mii.cch = 128 - 1;

                        if ( ::GetMenuItemInfo(hMenu, i, TRUE, &mii) )
                        {
                            itmText = szItemText;
                            if ( itemName.Find(_T('&')) < 0 )
                            {
                                itmText.Replace(_T("&"), _T(""));
                            }
                            if ( itmText.length() > itemName.length() )
                            {
                                itmText.SetSize(itemName.length());
                            }
                            if ( itemName == itmText )
                            {
                                if ( listItr == listOfMenuItems.end() )
                                {
                                    tMenuItems items;
                                    listOfMenuItems.push_back(items);
                                    listItr = listOfMenuItems.end();
                                    --listItr;
                                }
                                if ( mii.hSubMenu )
                                {
                                    uItem = (UINT_PTR) mii.hSubMenu;
                                    hMenu = mii.hSubMenu;
                                }
                                else
                                {
                                    uItem = (UINT_PTR) mii.wID;
                                    menuItemId = mii.wID;
                                }
                                listItr->insert( std::make_pair(itemName, uItem) );
                            }
                        }
                    }
                }
                if ( uItem != 0 )
                {
                    if ( !parsedPath.IsEmpty() )
                        parsedPath += sep;
                    parsedPath += itemName;
                }
                else
                    break;
            }
        }
    }

    return menuItemId;
}

int CNppExec::nppGetOpenFileNames(void)
{
  TCHAR* p;
  
  npp_nbFiles = (int) SendNppMsg(NPPM_GETNBOPENFILES, 0, 0);
  
  npp_bufFileNames.SetSize(npp_nbFiles);  // reserving memory without 
                                          // modification of its content
  for (int i = npp_bufFileNames.GetCount(); i < npp_nbFiles; i++)
  {
    p = new TCHAR[FILEPATH_BUFSIZE];
    if (p != NULL)
      npp_bufFileNames.Append(p);
  }

  if (npp_nbFiles > npp_bufFileNames.GetCount())
    npp_nbFiles = npp_bufFileNames.GetCount();

  SendNppMsg(NPPM_GETOPENFILENAMES, 
      (WPARAM) npp_bufFileNames.GetData(), (LPARAM) npp_nbFiles);

  return npp_nbFiles;
}

int CNppExec::nppGetOpenFileNamesInView(int nView , int nFiles )
{
  if ( (nView != PRIMARY_VIEW) && 
       (nView != SECOND_VIEW) &&
       (nView != ALL_OPEN_FILES) )
  {
    return -1;
  }
  
  if (nFiles < 0)
  {
    nFiles = (int) SendNppMsg(NPPM_GETNBOPENFILES, (WPARAM) nView, (LPARAM) nView);
  }

  npp_bufFileNames.SetSize(nFiles);  // reserving memory without 
                                     // modification of its content
  for (int i = npp_bufFileNames.GetCount(); i < nFiles; i++)
  {
    TCHAR* p = new TCHAR[FILEPATH_BUFSIZE];
    if (p != NULL)
      npp_bufFileNames.Append(p);
  }

  if (nFiles > npp_bufFileNames.GetCount())
    nFiles = npp_bufFileNames.GetCount();

  UINT uMsg = NPPM_GETOPENFILENAMES;
  if (nView == PRIMARY_VIEW)
    uMsg = NPPM_GETOPENFILENAMESPRIMARY;
  else if (nView == SECOND_VIEW)
    uMsg = NPPM_GETOPENFILENAMESSECOND;
  SendNppMsg(uMsg, (WPARAM) npp_bufFileNames.GetData(), (LPARAM) nFiles);

  return nFiles;
}

bool CNppExec::nppSwitchToDocument(const tstr& fileName, bool bGetOpenFileNames)
{
  
  Runtime::GetLogger().Add(   _T("nppSwitchToDocument()") );
  Runtime::GetLogger().Add(   _T("{") );
  Runtime::GetLogger().IncIndentLevel();
  Runtime::GetLogger().AddEx( _T("[in]  \"%s\""), fileName.c_str() );
    
  if (!isFullPath(fileName))
  {
    int i = findFileNameIndexInNppOpenFileNames(fileName, bGetOpenFileNames);
    if (i != -1)
    {
        
      Runtime::GetLogger().AddEx( _T("; changed to: \"%s\""), npp_bufFileNames[i] );  
          
      SendNppMsg(NPPM_SWITCHTOFILE, (WPARAM) 0, (LPARAM) npp_bufFileNames[i]);
    }
    else
    {
      Runtime::GetLogger().Add(   _T("; no similar file name") ); 
    }
    Runtime::GetLogger().DecIndentLevel();
    Runtime::GetLogger().Add(   _T("}") );

    return ( (i != -1) ? true : false );
  }

  Runtime::GetLogger().Add(   _T("; full path specified") );
  Runtime::GetLogger().DecIndentLevel();
  Runtime::GetLogger().Add(   _T("}") );
    
  return ( (SendNppMsg(NPPM_SWITCHTOFILE, 
                (WPARAM) 0, (LPARAM) fileName.c_str()) != 0) ? true : false );
}

void CNppExec::nppSaveAllFiles(void)
{
    // TODO: optimize - maybe there are no dirty files i.e. no need to save

    /*
    int ind = (int) SendNppMsg( NPPM_GETCURRENTDOCINDEX
                                , MAIN_VIEW
                                , MAIN_VIEW
                                );
    */

    SendNppMsg( NPPM_SAVEALLFILES, 0, 0 );

    /*
    SendNppMsg( NPPM_ACTIVATEDOC
                , MAIN_VIEW
                , ind
                );
    */
}

void CNppExec::Console_ClosePipes(void)
{
  ::CloseHandle(m_hStdOutReadPipe);  m_hStdOutReadPipe = NULL;
  ::CloseHandle(m_hStdOutWritePipe); m_hStdOutWritePipe = NULL;
  ::CloseHandle(m_hStdInReadPipe);   m_hStdInReadPipe = NULL;
  ::CloseHandle(m_hStdInWritePipe);  m_hStdInWritePipe = NULL;
}

DWORD CNppExec::Console_ReadPipesAndOutput(CStrT<char>& bufLine, 
         bool& bPrevLineEmpty, int& nPrevState, bool bOutputAll,
         bool& bDoOutputNext)
{
  DWORD       dwBytesRead = 0;
  char        Buf[CONSOLEPIPE_BUFSIZE];
  CStrT<char> outLine;
  
  bool   bSomethingHasBeenReadFromThePipe = false; 
                  // great name for local variable :-)

  const bool bConFltrEnable = Runtime::GetOptions().GetBool(OPTB_CONFLTR_ENABLE);
  const bool bConFltrExclAllEmpty = Runtime::GetOptions().GetBool(OPTB_CONFLTR_EXCLALLEMPTY);
  const bool bConFltrExclDupEmpty = Runtime::GetOptions().GetBool(OPTB_CONFLTR_EXCLDUPEMPTY);
  const int  nConFltrInclMask = Runtime::GetOptions().GetInt(OPTI_CONFLTR_INCLMASK);
  const int  nConFltrExclMask = Runtime::GetOptions().GetInt(OPTI_CONFLTR_EXCLMASK);

  const bool bRplcFltrEnable = Runtime::GetOptions().GetBool(OPTB_CONFLTR_R_ENABLE);
  const bool bRplcFltrExclEmpty = Runtime::GetOptions().GetBool(OPTB_CONFLTR_R_EXCLEMPTY);
  const int  nRplcFltrFindMask = Runtime::GetOptions().GetInt(OPTI_CONFLTR_R_FINDMASK);
  const int  nRplcFltrCaseMask = Runtime::GetOptions().GetInt(OPTI_CONFLTR_R_CASEMASK);

  const bool bOutputVar = Runtime::GetOptions().GetBool(OPTB_CONSOLE_SETOUTPUTVAR);

  const int nBufLineLength = bufLine.length();

  do
  { 
    Sleep(10);  // it prevents from 100% CPU usage while reading!
    dwBytesRead = 0;
    if (!PeekNamedPipe(m_hStdOutReadPipe, NULL, 0, NULL, &dwBytesRead, NULL))
    {
      dwBytesRead = 0;
    }
    if (!dwBytesRead)
    {
      // no data in the pipe
      if (!bSomethingHasBeenReadFromThePipe)
      {
        // did we read something from the pipe already?
        // if no, then let's output the data from bufLine (if any)
        bOutputAll = true;
      }
    }
    if ((dwBytesRead > 0) || bOutputAll)
    {
      // some data is in the Pipe or bOutputAll==true

      bool bContainsData = (dwBytesRead > 0) ? true : false;
      // without bContainsData==true the ReadFile operation will never return

      if (bContainsData)
        ZeroMemory(Buf, CONSOLEPIPE_BUFSIZE);
      dwBytesRead = 0;
      if ((bContainsData 
           && ReadFile(m_hStdOutReadPipe, Buf, (CONSOLEPIPE_BUFSIZE-1)*sizeof(char), &dwBytesRead, NULL)
           && (dwBytesRead > 0)) || bOutputAll)
      {
        // some data has been read from the Pipe or bOutputAll==true
        
        int copy_len;

        Buf[dwBytesRead/sizeof(char)] = 0;
        //bufLine += Buf;
        bufLine.Append( Buf, dwBytesRead/sizeof(char) );

        if (dwBytesRead > 0)
        {
            bSomethingHasBeenReadFromThePipe = true;
        }

        // The following lines are needed for filtered output only.
        // I.e. you can replace all these lines by this one:
        //     GetConsole().PrintOutput(Buf);
        // if you don't need filtered output. (*)
        // (*) But don't forget about Unicode version:
        //     OEM -> WideChar or UTF-8 -> WideChar
        
        //GetConsole().PrintOutput(Buf);

        /**/
        do {
         
            copy_len = -1;

            for (int pos = 0; pos < bufLine.length(); pos++)
            {
                int nIsNewLine = 0;
                if (bufLine[pos] == '\n')
                {
                    nIsNewLine = 1; // BIN: 00000001
                }
                else if (bufLine[pos] == '\r')
                {
                    if (bufLine[pos+1] != '\n')
                    {
                        // not "\r\n" pair
                        if ((bufLine[pos+1] != '\r') || (bufLine.GetAt(pos+2) != '\n'))
                        {
                            // not "\r\r\n" (stupid M$'s line ending)
                            // just "\r"
                            nIsNewLine = 3; // BIN: 00000011
                        }
                    }
                }
                else if (bufLine[pos] == '\b')
                {
                    nIsNewLine = 7; // BIN: 00000111
                }
                
                if (nIsNewLine || (bOutputAll && (pos == bufLine.length()-1)))
                {
                    copy_len = pos;
                    if (!nIsNewLine)
                    {
                        // i.e. bOutputAll is true
                        copy_len++;
                    }
                    else if ((pos > 0) && (bufLine[pos-1] == '\r'))
                    {
                        copy_len--;
                        if ((pos > 1) && (bufLine[pos-2] == '\r'))
                            copy_len--;
                    }

                    outLine.Copy(bufLine.c_str(), copy_len);

                    if (nIsNewLine == 7) // '\b'
                    {
                      // counting "\b\b..." and skip them
                      while (bufLine[pos+1] == '\b')
                      {
                        ++nIsNewLine;
                        ++pos;
                      }
                    }

                    bufLine.Delete(0, pos+1);
                    if ((copy_len > 0) ||
                        ( ((!bConFltrExclAllEmpty) || (!bConFltrEnable)) &&
                          ((!bPrevLineEmpty) || (!bConFltrEnable) || (!bConFltrExclDupEmpty))
                        ))
                    {
                        tstr printLine;
                        bool bOutput = bConFltrEnable ? bDoOutputNext : true;

                        if ( bOutput )
                        {
                            tstr _mask;
                            tstr _line;

                            if ( outLine.length() > 0 )
                            {
                                unsigned int enc = Runtime::GetOptions().GetUint(OPTU_CONSOLE_ENCODING);
                                enc = CConsoleEncodingDlg::getOutputEncoding(enc);
                            
                              #ifdef UNICODE

                                wchar_t* pStr;
                                int lenStr = 0;

                                switch ( enc )
                                {
                                    case CConsoleEncodingDlg::ENC_OEM :
                                        pStr = SysUniConv::newMultiByteToUnicode( outLine.c_str(), outLine.length(), CP_OEMCP, &lenStr );
                                        break;
                                    
                                    case CConsoleEncodingDlg::ENC_UTF8 :
                                        pStr = SysUniConv::newUTF8ToUnicode( outLine.c_str(), outLine.length(), &lenStr );
                                        break;

                                    default:
                                        pStr = SysUniConv::newMultiByteToUnicode( outLine.c_str(), outLine.length(), CP_ACP, &lenStr );
                                        break;
                                }

                                _line.Copy(pStr, lenStr);
                                delete [] pStr;

                                {
                                    wchar_t wchNulChar = CNppConsoleRichEdit::GetNulChar();
                                    if ( wchNulChar != 0 )
                                    {
                                        _line.Replace( wchar_t(0x0000), wchNulChar ); // e.g. to 0x25E6 - the "White Bullet" symbol
                                    }
                                }

                              #else

                                {
                                    char chNulChar = CNppConsoleRichEdit::GetNulChar();
                                    if ( chNulChar != 0 )
                                    {
                                        outLine.Replace( char(0x00), chNulChar ); // e.g. to 0x17 - the "End of Text Block" symbol
                                    }
                                }

                                switch ( enc )
                                {
                                    case CConsoleEncodingDlg::ENC_OEM :
                                        if ( _line.SetSize(outLine.length() + 1) )
                                        {
                                            ::OemToChar( outLine.c_str(), _line.c_str() );
                                            _line.CalculateLength();
                                        }
                                        break;
                                    
                                    case CConsoleEncodingDlg::ENC_UTF8 :
                                        {
                                            char* pStr = SysUniConv::newUTF8ToMultiByte( outLine.c_str() );
                                            if ( pStr )
                                            {
                                                _line = pStr;
                                                delete [] pStr;
                                            }
                                        }
                                        break;

                                    default:
                                        _line = outLine;
                                        break;
                                }

                              #endif

                                printLine = _line;
                                if ( _line.length() )
                                    ::CharLower( _line.c_str() );
                            }

                            // >>> console output filters
                            if ( bConFltrEnable && ((nConFltrInclMask > 0) || (nConFltrExclMask > 0)) )
                            {
                                for (int i = 0; bOutput && 
                                      (i < CConsoleOutputFilterDlg::FILTER_ITEMS); i++)
                                {
                                    int len = 0;
                                    const TCHAR* cszLine = Runtime::GetOptions().GetStr(OPTS_CONFLTR_INCLLINE1 + i, &len);
                                    tstr sLine = cszLine;
                                    if ( GetMacroVars().CheckAllMacroVars(sLine, false) )
                                    {
                                        cszLine = sLine.c_str();
                                        len = sLine.length();
                                    }

                                    if ((nConFltrInclMask & (0x01 << i)) && (len > 0))
                                    {
                                        _mask = cszLine;
                                        ::CharLower(_mask.c_str());
                                        if (!c_base::_tmatch_mask(_mask.c_str(), _line.c_str()))
                                        {
                                            bOutput = false;
                                        }
                                    }

                                    len = 0;
                                    cszLine = Runtime::GetOptions().GetStr(OPTS_CONFLTR_EXCLLINE1 + i, &len);
                                    sLine = cszLine;
                                    if ( GetMacroVars().CheckAllMacroVars(sLine, false) )
                                    {
                                        cszLine = sLine.c_str();
                                        len = sLine.length();
                                    }

                                    if (bOutput && (nConFltrExclMask & (0x01 << i)) && (len > 0))
                                    {
                                        _mask = cszLine;
                                        ::CharLower(_mask.c_str());
                                        if (c_base::_tmatch_mask(_mask.c_str(), _line.c_str()))
                                        {
                                            bOutput = false;
                                        }
                                    }
                                }
                            }
                            // <<< console output filters

                            // >>> console replace filters
                            if ( bOutput && bRplcFltrEnable && (nRplcFltrFindMask > 0) )
                            {
                                bool bModified = false;
                                
                                // >>> for ...
                                for (int i = 0; bOutput && 
                                      (i < CConsoleOutputFilterDlg::REPLACE_ITEMS); i++)
                                {
                                    int lenFind = 0;
                                    const TCHAR* cszFind = Runtime::GetOptions().GetStr(OPTS_CONFLTR_R_FIND1 + i, &lenFind);
                                    tstr sFind = cszFind;
                                    if ( GetMacroVars().CheckAllMacroVars(sFind, false) )
                                    {
                                        cszFind = sFind.c_str();
                                        lenFind = sFind.length();
                                    }

                                    if ( (nRplcFltrFindMask & (0x01 << i)) && 
                                         ( ((lenFind > 0) && (_line.length() > 0)) || 
                                           ((lenFind == 0) && (_line.length() == 0)) )
                                       )
                                    {
                                        int lenRplc = 0;
                                        const TCHAR* cszRplc = Runtime::GetOptions().GetStr(OPTS_CONFLTR_R_RPLC1 + i, &lenRplc);
                                        tstr sRplc = cszRplc;
                                        if ( GetMacroVars().CheckAllMacroVars(sRplc, false) )
                                        {
                                            cszRplc = sRplc.c_str();
                                            lenRplc = sRplc.length();
                                        }

                                        if ( lenFind > 0 )
                                        {
                                            // original string is not empty
                                            
                                            int pos = 0;
                                            if (nRplcFltrCaseMask & (0x01 << i))
                                            {
                                                // match case
                                                while ( (pos = printLine.Find(cszFind, pos)) >= 0 )
                                                {
                                                    bModified = true;

                                                    // both variables must be changed
                                                    // to stay synchronized
                                                    _line.Replace(pos, lenFind, cszRplc, lenRplc);
                                                    printLine.Replace(pos, lenFind, cszRplc, lenRplc);
                                                    pos += lenRplc;
                                                }
                                            }
                                            else
                                            {
                                                // case-insensitive
                                                _mask = cszFind;
                                                ::CharLower( _mask.c_str() );
                                                while ( (pos = _line.Find(_mask.c_str(), pos)) >= 0 )
                                                {
                                                    bModified = true;

                                                    // both variables must be changed
                                                    // to stay synchronized
                                                    _line.Replace(pos, lenFind, cszRplc, lenRplc);
                                                    printLine.Replace(pos, lenFind, cszRplc, lenRplc);
                                                    pos += lenRplc;
                                                }
                                            }
                                        }
                                        else
                                        {
                                            // replacing original empty string with cszRplc

                                            bModified = true;
                                            _line = cszRplc;
                                            printLine = cszRplc;
                                        }

                                        if ( bRplcFltrExclEmpty && bModified && (printLine.length() == 0) )
                                        {
                                            bOutput = false;
                                        }
                                    }
                                }
                                // <<< for ...
                                
                            }
                            // <<< console replace filters
                        }
                            
                        if (bOutput)
                        {
                            if (nPrevState == 3) // '\r'
                            {
                              // M$ sometimes uses such line endings: "\r\r\n"
                              // (stupid M$...)
                              /*
                              if ( GetConsole().IsOutputEnabled() )
                              {
                                int nPos = GetConsole().GetConsoleEdit().ExGetSelPos();
                                int nLen = GetConsole().GetConsoleEdit().LineLength(nPos);
                                GetConsole().GetConsoleEdit().ExSetSel(nPos - nLen, nPos);
                                GetConsole().GetConsoleEdit().ReplaceSelText( _T("") );
                              }
                              */
                            }
                            else if (nPrevState >= 7) // '\b'...
                            {
                              if ( GetConsole().IsOutputEnabled() )
                              {
                                int nPos = GetConsole().GetConsoleEdit().ExGetSelPos();
                                int nLen = (nPrevState - 7) + 1;
                                GetConsole().GetConsoleEdit().ExSetSel(nPos - nLen, nPos);
                                GetConsole().GetConsoleEdit().ReplaceSelText( _T("") );
                              }
                            }
                            
                            if ( bOutputVar )
                            {
                                m_strChildProcessOutput += printLine;
                                if ( nIsNewLine == 1 )
                                {
                                    m_strChildProcessOutput += _T("\n");
                                }
                            }

                            GetConsole().PrintOutput( printLine.c_str(), (nIsNewLine == 1) ? true : false );
                        }

                        // if the current line is not over, then the current filter 
                        // must be applied to the rest of this line
                        bDoOutputNext = bOutput;
                    }
                    bPrevLineEmpty = (copy_len > 0) ? false : true;
                    nPrevState = nIsNewLine;
                    if (nIsNewLine == 1)
                    {
                        // current line is over - abort current filter
                        bDoOutputNext = true;
                    }
                    break;
                }
            
            }
        } while (copy_len >= 0);  
        /**/

      }
    }

  } 
  while ((dwBytesRead > 0) && _consoleIsVisible && !_consoleProcessBreak);

  if (bOutputAll && !dwBytesRead)  dwBytesRead = nBufLineLength;
  return dwBytesRead;
}

// cszCommandLine must be transformed by ModifyCommandLine(...) already
BOOL CNppExec::CreateChildProcess(HWND hParentWnd, LPCTSTR cszCommandLine)
{
  TCHAR                szCmdLine[CONSOLECOMMAND_BUFSIZE];
  SECURITY_DESCRIPTOR  sd;
  SECURITY_ATTRIBUTES  sa;
  PROCESS_INFORMATION  pi;
  STARTUPINFO          si;
  
  m_hStdInReadPipe = NULL;
  m_hStdInWritePipe = NULL; 
  m_hStdOutReadPipe = NULL;
  m_hStdOutWritePipe = NULL;

  m_nChildProcessExitCode = -1;

  if (IsWindowsNT())
  {
    // security stuff for NT
    InitializeSecurityDescriptor(&sd, SECURITY_DESCRIPTOR_REVISION);
    SetSecurityDescriptorDacl(&sd, TRUE, NULL, FALSE);
    sa.lpSecurityDescriptor = &sd;
  }
  else
  {
    sa.lpSecurityDescriptor = NULL;
  }
  sa.nLength = sizeof(SECURITY_ATTRIBUTES);
  sa.bInheritHandle = TRUE;

  const int DEFAULT_PIPE_SIZE = 0;

  // Create the Pipe and get r/w handles
  if (!CreatePipe(&m_hStdOutReadPipe, &m_hStdOutWritePipe, &sa, DEFAULT_PIPE_SIZE))
  {
    GetConsole().PrintError( _T("CreatePipe(<StdOut>) failed") );
    return FALSE;
  }
  if (m_hStdOutWritePipe == NULL)
  {
    if (m_hStdOutReadPipe != NULL)
      CloseHandle(m_hStdOutReadPipe);
    GetConsole().PrintError( _T("hStdOutWritePipe = NULL") );
    return FALSE;
  }
  if (m_hStdOutReadPipe == NULL)
  {
    CloseHandle(m_hStdOutWritePipe);
    GetConsole().PrintError( _T("hStdOutReadPipe = NULL") );
    return FALSE;
  }

  if (!CreatePipe(&m_hStdInReadPipe, &m_hStdInWritePipe, &sa, DEFAULT_PIPE_SIZE))
  {
    GetConsole().PrintError( _T("CreatePipe(<StdIn>) failed") );
    return FALSE;
  }
  if (m_hStdInWritePipe == NULL)
  {
    if (m_hStdInReadPipe != NULL)
      CloseHandle(m_hStdInReadPipe);
    GetConsole().PrintError( _T("hStdInWritePipe = NULL") );
    return FALSE;
  }
  if (m_hStdInReadPipe == NULL)
  {
    CloseHandle(m_hStdInWritePipe);
    GetConsole().PrintError( _T("hStdInReadPipe = NULL") );
    return FALSE;
  }

  SetHandleInformation(m_hStdInWritePipe, HANDLE_FLAG_INHERIT, 0);
  SetHandleInformation(m_hStdOutReadPipe, HANDLE_FLAG_INHERIT, 0);

  /*
  DWORD dwMode = PIPE_READMODE_BYTE | PIPE_NOWAIT;
  SetNamedPipeHandleState(m_hStdOutWritePipe, &dwMode, NULL, NULL);
  dwMode = PIPE_READMODE_BYTE | PIPE_NOWAIT;
  SetNamedPipeHandleState(m_hStdOutReadPipe, &dwMode, NULL, NULL);
  */
    
  // initialize STARTUPINFO struct
  ZeroMemory(&si, sizeof(STARTUPINFO));
  si.cb = sizeof(STARTUPINFO);
  si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
  si.wShowWindow = SW_HIDE;
  si.hStdInput = m_hStdInReadPipe;
  si.hStdOutput = m_hStdOutWritePipe;
  si.hStdError = m_hStdOutWritePipe;

  _consoleStrToWrite[0] = 0;
  _consoleStdInWritePipe = m_hStdInWritePipe;
  
  lstrcpy(szCmdLine, cszCommandLine);
  //ShowWarning(szCmdLine);

  ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));
  
  //AllocConsole();
  
  if (CreateProcess(
        NULL,
        szCmdLine,
        NULL,                        // security
        NULL,                        // security
        TRUE,                        // inherits handles
        CREATE_NEW_PROCESS_GROUP,    // creation flags
        NULL,                        // environment
        NULL,                        // current directory
        &si,                         // startup info
        &pi                          // process info
     ))
  {
    /*{
      typedef BOOL (WINAPI * PFNATTCON)(DWORD);

      WaitForSingleObject(pi.hProcess, Runtime::GetOptions().GetUint(OPTU_CHILDP_STARTUPTIMEOUT_MS));

      HMODULE hKernel32 = GetModuleHandle(_T("kernel32.dll"));
      if ( hKernel32 )
      {
        PFNATTCON pfnAttachConsole = (PFNATTCON) GetProcAddress(hKernel32, "AttachConsole");
        if ( pfnAttachConsole )
        {
          if ( pfnAttachConsole(pi.dwProcessId) )
          {
            SetConsoleCP(CP_UTF8);
            SetConsoleOutputCP(CP_UTF8);
          }
          else
          {
            DWORD dwErr = GetLastError();
            dwErr = dwErr;
          }
        }
      }
    }*/

    CloseHandle(pi.hThread);
    //CloseHandle(hStdOutWritePipe); hStdOutWritePipe = NULL;
    //CloseHandle(hStdInReadPipe); hStdInReadPipe = NULL;

    _consoleProcessHandle = pi.hProcess;
    _consoleProcessBreak = false;
    _consoleProcessIsRunning = true;
    
    GetConsole().PrintMessage( _T("Process started >>>") );
    
    // this pause is necessary for child processes
    // which return immediatelly
    //WaitForSingleObject(pi.hProcess, 120);
    WaitForSingleObject(pi.hProcess, Runtime::GetOptions().GetUint(OPTU_CHILDP_STARTUPTIMEOUT_MS));
        
    CStrT<char>  bufLine;
    bool         bPrevLineEmpty = false;
    bool         bDoOutputNext = true;
    int          nPrevState = 0;
    DWORD        dwRead = 0;
    unsigned int nEmptyCount = 0;
    const DWORD  dwCycleTimeOut = Runtime::GetOptions().GetUint(OPTU_CHILDP_CYCLETIMEOUT_MS);
    const DWORD  dwExitTimeOut = Runtime::GetOptions().GetUint(OPTU_CHILDP_EXITTIMEOUT_MS);
    
    bufLine.Clear(); // just in case :-)
    
    do 
    {
      // inside this cycle: the bOutputAll parameter 
      // must be controlled within Console_ReadPipesAndOutput
      dwRead = Console_ReadPipesAndOutput(bufLine, 
        bPrevLineEmpty, nPrevState, false, bDoOutputNext);

      if ( !g_bUseThreadForScriptExecution )
      {
        // we are not inside a thread
        if ( dwRead == 0 )
        {
          // no output from the child process
          ++nEmptyCount;
          if ( nEmptyCount > (dwExitTimeOut/dwCycleTimeOut) )
          {
            // no output during more than dwExitTimeOut ms
            // let's terminate the process...
            _consoleProcessBreak = true;
          }
        }
        else
          nEmptyCount = 0;
      }

    }
    while ((_consoleProcessIsRunning = (WaitForSingleObject(pi.hProcess, 
              dwCycleTimeOut) == WAIT_TIMEOUT))
        && _consoleIsVisible && !_consoleProcessBreak);
    // NOTE: time-out inside WaitForSingleObject() prevents from 100% CPU usage!

    if (_consoleIsVisible && !_consoleProcessBreak)
    {
      // maybe the child process is exited but not all its data is read
      Console_ReadPipesAndOutput(bufLine, 
        bPrevLineEmpty, nPrevState, true, bDoOutputNext);
      // ShowWarning("All is read!");
    }

    if ( (!_consoleIsVisible) || _consoleProcessBreak )
    {
      if (_consoleProcessIsRunning)
      {
        if (!GenerateConsoleCtrlEvent(CTRL_BREAK_EVENT, pi.dwProcessId))
        //if ( (!GenerateConsoleCtrlEvent(CTRL_C_EVENT, pi.dwProcessId)) &&
        //     (!GenerateConsoleCtrlEvent(CTRL_BREAK_EVENT, pi.dwProcessId)) )
        {
          /*
          DWORD dw = GetLastError();
          TCHAR str[64]; wsprintf(str, "GetLastError() returned %lu", dw);
          ShowError(str); // returns 6: The handle is invalid.
          */
          if (TerminateProcess(pi.hProcess, 0))
          {
            if ( !Runtime::GetOptions().GetBool(OPTB_CONSOLE_NOINTMSGS) )
            {
              GetConsole().PrintMessage( _T("<<< Process has been terminated.") );
            }
            else
            {
              if (nPrevState != 1 /* new line */ )  GetConsole().PrintMessage( _T(""), false );
            }
          }
          else
          {
            GetConsole().PrintError( _T("<<< TerminateProcess() returned FALSE.") );
          }
        }
        else
        {
          GetConsole().PrintMessage( _T("<<< Process is finished normally.") );
        }
      }
    }

    _consoleProcessIsRunning = false;
    _consoleProcessHandle = NULL;
    
    DWORD dwExitCode = (DWORD)(-1);
    ::GetExitCodeProcess(pi.hProcess, &dwExitCode);
    m_nChildProcessExitCode = (int) dwExitCode;

    // Process cleanup
    CloseHandle(pi.hProcess);
    // closing pipe handles
    Console_ClosePipes();

    //FreeConsole();

    if (_consoleIsVisible && !_consoleProcessBreak)
    {
      if ( !Runtime::GetOptions().GetBool(OPTB_CONSOLE_NOINTMSGS) )
      {
        TCHAR szMessage[96];
        wsprintf( szMessage, _T("<<< Process finished. (Exit code %d)"), m_nChildProcessExitCode );
        GetConsole().PrintMessage( szMessage ); 
      }
      else
      {
        if (nPrevState != 1 /* new line */ )  GetConsole().PrintMessage( _T(""), false );
      }
    }

    return TRUE;
  }
  else
  {
    DWORD dwErrorCode = ::GetLastError();

    // closing pipe handles
    Console_ClosePipes();

    //FreeConsole();

    GetConsole().PrintSysError( _T("CreateProcess()"), dwErrorCode );
    return FALSE;
  }
}

void CNppExec::printScriptLog(const TCHAR* str, int)
{
  // static func
  Runtime::GetNppExec().GetConsole().PrintError( str, false );
}

void CNppExec::printScriptString(const TCHAR* str, int)
{
  // static func
  Runtime::GetNppExec().GetConsole().PrintMessage( str, false, false );
}

BOOL CNppExec::CreateScriptThread(void)
{
  BOOL bOK = FALSE;

  /*
  // testing the error case:
  static int n = 2; 
  if (--n == 0) 
    m_hScriptThread = (HANDLE)1;
  */

  if ( !m_hScriptThread )
  {
    m_hFocusedWindowBeforeScriptStarted = ::GetFocus();
    if ( g_bUseThreadForScriptExecution )
    {
      bOK = CreateNewThread(dwCreateConsoleProcess, NULL, &m_hScriptThread);
    }
    else
    {
      dwCreateConsoleProcess(NULL);
      bOK = TRUE;
    }
    if ( !bOK )
    {  
      GetConsole().PrintError( _T("CreateThread() failed") );
    }
  }
  else
  {
    GetConsole().PrintError( _T_RE_EOL _T("INTERNAL ERROR:") _T_RE_EOL _T("CreateScriptThread() has been called while m_hScriptThread is not NULL") _T_RE_EOL, false );
    ShowError( _T("INTERNAL ERROR (NppExec plugin):\n") \
               _T("CreateScriptThread() has been called while m_hScriptThread is not NULL\n\n") \
               _T("Please copy the text you will see in the Console dialog\n") \
               _T("and report about this error to the author") );
    /*
    // if there is an infinite loop or something similar in the thread
    // then TerminateThread can stupidly hang the whole application
    if ( !TerminateThread(m_hScriptThread, 1) )
    {
      GetConsole().PrintError( _T("TerminateThread() failed" ANSI_RE_EOL) );
    }
    */
    CloseScriptThreadHandle();

    GetConsole().PrintError( _T(">>>>>>>>>>>>>> SCRIPT LOG >>>>>>>>>>>>>>"), false );
    Runtime::GetLogger().OutputAll(printScriptLog);
    GetConsole().PrintError( _T("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<") );

    GetConsole().PrintMessage( _T_RE_EOL _T("================ READY ================"), false );
    GetConsole().RestoreDefaultTextStyle(true);
  }

  if ( !bOK )
  {
    NotifyExternalPluginResult(NPE_RESULT_FAILED);
  }

  return bOK;
}

void CNppExec::CloseScriptThreadHandle(void)
{
  if (m_hScriptThread)
  {
    CloseHandle(m_hScriptThread);
    m_hScriptThread = NULL;
  }
}

void CNppExec::Free(void)
{
  StopAutoSaveTimer();

  if (m_TB_Icon.hToolbarBmp)
    ::DeleteObject(m_TB_Icon.hToolbarBmp);
  if (m_TB_Icon.hToolbarIcon)
    ::DestroyIcon(m_TB_Icon.hToolbarIcon);

  if (npp_bufFileNames.GetCount() > 0)
  {
    for (int i = 0; i < npp_bufFileNames.GetCount(); i++)
    {
      if (npp_bufFileNames[i] != NULL)
      {
        delete [] npp_bufFileNames[i];
        npp_bufFileNames[i] = NULL;  // just in case
      }
    }
  }
  
  if (_consoleProcessIsRunning)
  {
    TerminateProcess(_consoleProcessHandle, 0);
    _consoleProcessIsRunning = false;
    CloseHandle(_consoleProcessHandle);
    _consoleProcessHandle = NULL;
    Console_ClosePipes();
    // you can ensure it's working !!!
    // MessageBox(NULL, "Process is terminated", "", 0);
  }

  if (_consoleIsVisible)
  {
    // necessary for the dwCreateConsoleProcess thread
    _consoleIsVisible = false;
    /*
    SendNppMsg(WM_MODELESSDIALOG, 
      (WPARAM) MODELESSDIALOGREMOVE, (LPARAM) GetConsole().GetParentWnd());
    ::DestroyWindow(GetConsole().GetParentWnd());
    */
  }

}

void CNppExec::ExpandToFullConfigPath(TCHAR* out_szPath, const TCHAR* cszFileName)
{
  lstrcpy(out_szPath, m_szConfigPath);
  lstrcat(out_szPath, _T("\\"));
  lstrcat(out_szPath, cszFileName);
}

static void modifyPluginName(void)
{
  // needed in MustDie9x because GetModuleFileName returns 
  // capital letters there (stupid M$...)

  CStrT<TCHAR> name = PLUGIN_NAME;
  if ( name.length() > 0 )
  {
    ::CharUpper( name.c_str() );
    int i = name.Find( _T("NPPEXEC") );
    if ( i >= 0 )
    {
      name.Replace( i, 7, _T("NppExec") );
      if ( i > 1 )
      {
        ::CharLowerBuff( (name.c_str() + 1), i - 1 );
      }
      if ( i + 8 < name.length() )
      {
        ::CharLowerBuff( (name.c_str() + i + 8), name.length() - i - 8 );
      }
      lstrcpy( PLUGIN_NAME, name.c_str() );
      name.Replace( _T("NppExec"), _T("npec_cmdhistory") );
      name.Append( _T(".txt") );
      lstrcpy( CMDHISTORY_FILENAME, name.c_str() );
    }
  }
}

void CNppExec::Init(void)
{
  INT   i;
  TCHAR ch;
  TCHAR szPath[FILEPATH_BUFSIZE];

  lstrcpy(m_szPluginDllName, _T("NppExec.dll"));
  szPath[0] = 0;
  GetModuleFileName(m_hDllModule /*NULL*/ , szPath, FILEPATH_BUFSIZE - 1);
  i = lstrlen(szPath) - 1;
  while (i >= 0 && (ch = szPath[i]) != _T('\\') && ch != _T('/'))  i--;
  if (i > 0)
  {
    lstrcpy(m_szPluginDllName, szPath + i + 1); // "NppExec.dll"
      
    int j = 0;
    while (j < MAX_PLUGIN_NAME)
    {
      ch = szPath[++i];
      if (ch == _T('.'))  ch = 0;
      PLUGIN_NAME[j++] = ch;
      if (!ch)
        break;
    }
    if (j >= MAX_PLUGIN_NAME)
    {
      PLUGIN_NAME[MAX_PLUGIN_NAME - 1] = 0;
    }
  }
  modifyPluginName();
  i = lstrlen(szPath) - 1;
  while (i >= 0 && (ch = szPath[i]) != _T('\\') && ch != _T('/'))  i--;
  if (i >= 0)  szPath[i] = 0;
  lstrcpy(m_szPluginPath, szPath);
  lstrcpy(m_szConfigPath, szPath);
  lstrcat(szPath, _T("\\Config\\*.*"));
  
  lstrcpy( INI_FILENAME, PLUGIN_NAME );
  lstrcat( INI_FILENAME, _T(".ini") );

  lstrcpy( PLUGIN_NAME_DLL, PLUGIN_NAME );
  lstrcat( PLUGIN_NAME_DLL, _T(".dll") );

  if ( m_nppData._nppHandle )
  {
    SendNppMsg( NPPM_GETPLUGINSCONFIGDIR,
      (WPARAM) (FILEPATH_BUFSIZE - 1), (LPARAM) m_szConfigPath );
  }
  else
  {
    WIN32_FIND_DATA fdata;
    HANDLE          fhandle = FindFirstFile(szPath, &fdata);
    if ((fhandle == INVALID_HANDLE_VALUE) || !fhandle)
    {
          //ShowWarning("FindFirstFile failed");
      lstrcat(m_szConfigPath, _T("\\Config"));
      if (CreateDirectory(m_szConfigPath, NULL))
      {
          //ShowWarning("CreateDirectory OK");
      }
      else
      {
          //ShowWarning("CreateDirectory failed");
        lstrcpy(m_szConfigPath, m_szPluginPath);
      }
    }
    else
    {
          //ShowWarning("FindFirstFile OK");
      FindClose(fhandle);
      lstrcat(m_szConfigPath, _T("\\Config"));
    }
  }
  lstrcpy(m_szIniFilePathName, m_szConfigPath);
  lstrcat(m_szIniFilePathName, _T("\\"));
  lstrcat(m_szIniFilePathName, INI_FILENAME);
          //ShowWarning(m_szIniFilePathName);

}

void CNppExec::OnCmdHistory(void)
{
  HMENU hMenu = GetNppMainMenu();
  if (hMenu)
  {
    const bool bCmdHistory = !Runtime::GetOptions().GetBool(OPTB_CONSOLE_CMDHISTORY);
    Runtime::GetOptions().SetBool(OPTB_CONSOLE_CMDHISTORY, bCmdHistory);
    CheckMenuItem(hMenu, g_funcItem[N_CMDHISTORY]._cmdID,
      MF_BYCOMMAND | (bCmdHistory ? MF_CHECKED : MF_UNCHECKED));
  }
}

bool CNppExec::CanStartScriptOrCommand()
{
    if ( _consoleProcessIsRunning )
    {
        //ShowWarning( _T("Console process is still running") );
        
        const CListItemT<tstr>* p = InputBoxDlg.m_InputHistory[CInputBoxDlg::IBT_EXITPROCESS].GetFirst();

        // init the InputBox dialog values
        InputBoxDlg.m_InputMessage = _T("Console process is still running. Send - send the exit message; Cancel [Esc] - continue. Terminate is not recommended.");
        if ( p && (p->GetItem().length() > 0) )
        {
            InputBoxDlg.m_InputMessage += _T(" : ");
            InputBoxDlg.m_InputMessage += p->GetItem();
        }
        InputBoxDlg.m_InputVarName = _T("exit message: ");
        InputBoxDlg.setInputBoxType(CInputBoxDlg::IBT_EXITPROCESS);
    
        // show the InputBox dialog
        INT_PTR ret = PluginDialogBox(IDD_INPUTBOX, InputBoxDlgProc);
        switch ( ret )
        {
            case CInputBoxDlg::RET_OK:
                // OK - send the exit message
                if ( _consoleStdInWritePipe )
                {
                    tstr  s1 = InputBoxDlg.m_OutputValue;
                    s1 += _T("\r\n");

                    GetConsole().PrintStr( s1.c_str(), false );

                    ConsoleWriteInput( s1.c_str() );
                }
                break;
            case CInputBoxDlg::RET_TERMINATE:
                // Terminate
                _consoleProcessBreak = true;
                break;
        }

        if ( ret >= CInputBoxDlg::RET_OK )
        {
            unsigned int uMaxTimeout = Runtime::GetOptions().GetUint(OPTU_CHILDP_EXITTIMEOUT_MS);
            m_SuspendedScriptAction.setActive(true);
            m_SuspendedScriptAction.startWaitingTimer(uMaxTimeout);
        }

        return false;
    }
    
    if ( isScriptRunning() )
    {
        ShowWarning( _T("Another script is still running") );
        return false;
    }
    
    return true;
}

void CNppExec::OnDoExec(void)
{
    m_SuspendedScriptAction.setActive(false);
    m_SuspendedScriptAction.setAction(CSuspendedScriptAction::AT_ONDOEXEC);
    
    if ( !CanStartScriptOrCommand() )
        return;

    if ( Runtime::GetOptions().GetBool(OPTB_CONSOLE_SAVEONEXECUTE) )
    {
        nppSaveAllFiles();
    }

    if ( PluginDialogBox(IDD_DOEXEC, DoExecDlgProc) == 1 )
    {
        if ( !m_CmdList.GetFirst() )
        {
            // adding empty command if m_CmdList is empty 
            m_CmdList.Add( _T("") );
            // now m_CmdList is not empty ;-)
        }
        OnDirectExec(false);
    }
}

void CNppExec::OnDirectExec(bool bCanSaveAll)
{
    m_SuspendedScriptAction.setActive(false);
    m_SuspendedScriptAction.setAction(CSuspendedScriptAction::AT_ONDIRECTEXEC, bCanSaveAll);
    
    if ( !CanStartScriptOrCommand() )
        return;
    
    CListItemT<tstr>* p = m_CmdList.GetFirst();
    if ( !p )
    {
        // m_CmdList is empty (no previous script executed)
        OnDoExec();
        return;
    }
    else
    {
        // OK, m_CmdList is not empty
        if (bCanSaveAll && Runtime::GetOptions().GetBool(OPTB_CONSOLE_SAVEONEXECUTE))
        {
            nppSaveAllFiles();
        }
    }

    tstr S;
    bool bExec = false;
    while ( p && !bExec )
    {
        if ( p->GetItem().length() > 0 )
        {
            S = p->GetItem();
            if ( !CScriptEngine::isCommentOrEmpty(S) )
            {
                bExec = true;
            }
        }
        p = p->GetNext();
    }
     
    if ( bExec )
    {
        const CScriptEngine::eCmdType cmdType = CScriptEngine::getCmdType(S);
        if ( (cmdType == CScriptEngine::CMDTYPE_NPPCONSOLE) ||
             (cmdType == CScriptEngine::CMDTYPE_NPPEXEC) )
        {
            verifyConsoleDialogExists();
            if ( GetConsole().GetParentWnd() )  _consoleIsVisible = true;
        }
        else
        {
            showConsoleDialog(false);
        }

        if ( GetConsole().GetParentWnd() ) 
        {
            if ( !Runtime::GetOptions().GetBool(OPTB_CONSOLE_APPENDMODE) )
            {
                GetConsole().ClearText();
            }
            if ( !CreateScriptThread() )
            {
                _consoleIsVisible = isConsoleDialogVisible();
            }
        }
    }
}

void CNppExec::DoExecScript(LPCTSTR szScriptName, bool bCanSaveAll, LPCTSTR szScriptArguments /* = NULL */ )
{
    m_SuspendedScriptAction.setActive(false);
    m_SuspendedScriptAction.setAction(CSuspendedScriptAction::AT_DOEXECSCRIPT, 
        bCanSaveAll, szScriptName, szScriptArguments);
    
    if ( szScriptName && szScriptName[0] && CanStartScriptOrCommand() )
    {
        tstr S = _T("NPP_EXEC \"");
        S += szScriptName;
        S += _T("\""); //  \"\" are necessary: script name may contain spaces!!!

        if ( szScriptArguments && szScriptArguments[0] )
        {
            S += _T(" ");
            S += szScriptArguments;
        }

        m_CmdList.DeleteAll();
        m_CmdList.Add(S);
        OnDirectExec(bCanSaveAll);
    }
}

void CNppExec::OnNoInternalMsgs(void)
{
  HMENU hMenu = GetNppMainMenu();
  if (hMenu)
  {
    const bool bNoInternalMsgs = !Runtime::GetOptions().GetBool(OPTB_CONSOLE_NOINTMSGS);
    Runtime::GetOptions().SetBool(OPTB_CONSOLE_NOINTMSGS, bNoInternalMsgs);
    CheckMenuItem(hMenu, g_funcItem[N_NOINTMSGS]._cmdID,
      MF_BYCOMMAND | (bNoInternalMsgs ? MF_CHECKED : MF_UNCHECKED));
  }
}

void CNppExec::OnNoCmdAliases(void)
{
    HMENU hMenu = GetNppMainMenu();
    if (hMenu)
    {
        const bool bNoCmdAliases = !Runtime::GetOptions().GetBool(OPTB_CONSOLE_NOCMDALIASES);
        Runtime::GetOptions().SetBool(OPTB_CONSOLE_NOCMDALIASES, bNoCmdAliases);
        CheckMenuItem(hMenu, g_funcItem[N_NOCMDALIASES]._cmdID,
            MF_BYCOMMAND | (bNoCmdAliases ? MF_CHECKED : MF_UNCHECKED));
    }
}

#ifdef _SCROLL_TO_LATEST
void CNppExec::OnScrollToLatest(void)
{
  HMENU hMenu = GetNppMainMenu();
  if (hMenu)
  {
    const bool bScrollToLatest = !Runtime::GetOptions().GetBool(OPTB_CONSOLE_SCROLL2LATEST);
    Runtime::GetOptions().SetBool(OPTB_CONSOLE_SCROLL2LATEST, bScrollToLatest);
    CheckMenuItem(hMenu, g_funcItem[N_SCROLL2LATEST]._cmdID,
      MF_BYCOMMAND | (bScrollToLatest ? MF_CHECKED : MF_UNCHECKED));
  }
}
#endif

void CNppExec::OnConsoleEncoding(void)
{
    ConsoleEncodingDlg.m_enc_opt = Runtime::GetOptions().GetUint(OPTU_CONSOLE_ENCODING);
    if ( PluginDialogBox(IDD_CONSOLE_ENCODING, ConsoleEncodingDlgProc) == 1 )
    {
        Runtime::GetOptions().SetUint(OPTU_CONSOLE_ENCODING, ConsoleEncodingDlg.m_enc_opt);
        UpdateConsoleEncoding();
    }
}

void CNppExec::UpdateConsoleEncoding()
{
    HMENU hMenu = GetNppMainMenu();
    if ( hMenu )
    {
        TCHAR        szItemText[128];
        MENUITEMINFO mii;

        ::ZeroMemory( &mii, sizeof(MENUITEMINFO) );
        mii.cbSize = sizeof(MENUITEMINFO);
        mii.fMask = MIIM_FTYPE | MIIM_ID | MIIM_STATE | MIIM_STRING;
        mii.dwTypeData = (TCHAR*) szItemText;
        mii.cch = 128 - 1;

        if ( ::GetMenuItemInfo(hMenu, g_funcItem[N_CONSOLE_ENC]._cmdID, FALSE, &mii) )
        {
            unsigned int enc_opt = Runtime::GetOptions().GetUint(OPTU_CONSOLE_ENCODING);
            lstrcpy( szItemText, _T("Console Output... {") );
            lstrcat( szItemText, CConsoleEncodingDlg::getOutputEncodingName(enc_opt) );
            lstrcat( szItemText, _T("/") );
            lstrcat( szItemText, CConsoleEncodingDlg::getInputEncodingName(enc_opt) );
            lstrcat( szItemText, _T("}") );
            ::SetMenuItemInfo(hMenu, g_funcItem[N_CONSOLE_ENC]._cmdID, FALSE, &mii);
        }
    }
}

bool CNppExec::ConsoleWriteInput(const TCHAR* szLine, bool bFFlush )
{
    if ( (!szLine) || (!_consoleStdInWritePipe) )
        return false;
    
    char*        pStr = NULL;
    int          len = 0;
    DWORD        dwBytesWritten = 0;
    unsigned int enc = Runtime::GetOptions().GetUint(OPTU_CONSOLE_ENCODING);
        
    enc = CConsoleEncodingDlg::getInputEncoding(enc);

  #ifdef UNICODE
    
    switch ( enc )
    {
        case CConsoleEncodingDlg::ENC_OEM :
            pStr = SysUniConv::newUnicodeToMultiByte( szLine, -1, CP_OEMCP, &len );
            break;
        
        case CConsoleEncodingDlg::ENC_UTF8 :
            pStr = SysUniConv::newUnicodeToUTF8( szLine, -1, &len );
            break;

        default:
            pStr = SysUniConv::newUnicodeToMultiByte( szLine, -1, CP_ACP, &len );
            break;
    }
    
    if ( pStr )
    {
        ::WriteFile(_consoleStdInWritePipe, pStr, len*sizeof(char), &dwBytesWritten, NULL);
        if ( bFFlush )
        {
            // beware! this may hang the application (due to MustDie's pipes)
            ::FlushFileBuffers(_consoleStdInWritePipe);
        }
        delete [] pStr;
    }
                  
  #else
    
    bool bNewMemory = false;
    
    switch ( enc )
    {
        case CConsoleEncodingDlg::ENC_OEM :
            len = lstrlen(szLine);
            pStr = new char[len + 1];
            if ( pStr )
            {
                ::CharToOem(szLine, pStr);
                bNewMemory = true;
            }
            break;

        case CConsoleEncodingDlg::ENC_UTF8 :
            pStr = SysUniConv::newMultiByteToUTF8(szLine, -1, CP_ACP, &len);
            if ( pStr )
            {
                bNewMemory = true;
            }
            break;

        default:
            len = lstrlen(szLine);
            pStr = (char *) szLine;
            bNewMemory = false;
            break;
    }

    if ( pStr )
    {
        ::WriteFile(_consoleStdInWritePipe, pStr, len*sizeof(char), &dwBytesWritten, NULL);
        if ( bFFlush )
        {
            // beware! this may hang the application (due to MustDie's pipes)
            ::FlushFileBuffers(_consoleStdInWritePipe);
        }
        if ( bNewMemory )
            delete [] pStr;
    }
  
  #endif

    return true;
}

void CNppExec::OnOutputFilter(void)
{
  if ( PluginDialogBox(IDD_CONSOLE_FILTER, ConsoleOutputFilterDlgProc) == 1 )
  {
    UpdateOutputFilterMenuItem();
    UpdateGoToErrorMenuItem();
    /*
    if (_consoleIsVisible)
    {
      if (!Runtime::GetOptions().GetBool(OPTB_CONFLTR_ENABLE))
      {
        ::SendMessage(GetConsole().GetParentWnd(), WM_SETTEXT, 0, (LPARAM) " Console ");
      }
      else
      {
        ::SendMessage(GetConsole().GetParentWnd(), WM_SETTEXT, 0, (LPARAM) " Console* ");
      }
    }
    */
  }
}

void CNppExec::UpdateOutputFilterMenuItem()
{
    HMENU hMenu = GetNppMainMenu();
    if ( hMenu )
    {
        bool bFilterEnabled = Runtime::GetOptions().GetBool(OPTB_CONFLTR_ENABLE);
        if ( !bFilterEnabled )
            bFilterEnabled = Runtime::GetOptions().GetBool(OPTB_CONFLTR_R_ENABLE);
        CheckMenuItem(hMenu, g_funcItem[N_OUTPUT_FILTER]._cmdID,
          MF_BYCOMMAND | (bFilterEnabled ? MF_CHECKED : MF_UNCHECKED));
    }
}

void CNppExec::UpdateGoToErrorMenuItem()
{
    HMENU hMenu = GetNppMainMenu();
    if ( hMenu )
    {
        UINT uEnable = Runtime::GetWarningAnalyzer().HasEnabledFilters() ? MF_ENABLED : MF_GRAYED;
        EnableMenuItem(hMenu, g_funcItem[N_GOTO_NEXT_ERROR]._cmdID, MF_BYCOMMAND | uEnable);
        EnableMenuItem(hMenu, g_funcItem[N_GOTO_PREV_ERROR]._cmdID, MF_BYCOMMAND | uEnable);
    }
}

void CNppExec::OnAdvancedOptions(void)
{
    if ( PluginDialogBox(IDD_ADVANCEDOPTIONS, AdvancedOptionsDlgProc) == 1 )
    {
    }
}

//#ifdef _GVC
void CNppExec::OnSaveOnExecute( void )
{
  HMENU hMenu = GetNppMainMenu();
  if (hMenu)
  {
    const bool bSaveOnExecute = !Runtime::GetOptions().GetBool(OPTB_CONSOLE_SAVEONEXECUTE);
    Runtime::GetOptions().SetBool(OPTB_CONSOLE_SAVEONEXECUTE, bSaveOnExecute);
    CheckMenuItem(hMenu, g_funcItem[N_SAVEONEXECUTE]._cmdID,
      MF_BYCOMMAND | (bSaveOnExecute ? MF_CHECKED : MF_UNCHECKED));
  }
}
//#endif

void CNppExec::OnCdCurDir(void)
{
  HMENU hMenu = GetNppMainMenu();
  if (hMenu)
  {  
    const bool bCDCurDir = !Runtime::GetOptions().GetBool(OPTB_CONSOLE_CDCURDIR);
    Runtime::GetOptions().SetBool(OPTB_CONSOLE_CDCURDIR, bCDCurDir);
    CheckMenuItem(hMenu, g_funcItem[N_CDCURDIR]._cmdID,
      MF_BYCOMMAND | (bCDCurDir ? MF_CHECKED : MF_UNCHECKED));
    UpdateCurrentDirectory();
  }
}

void CNppExec::OnSelectConsoleFont(void)
{
  LOGFONT    lf;
  CHOOSEFONT cf;
  HWND       hEd = _consoleIsVisible ? GetConsole().GetConsoleWnd() : NULL;
  HFONT      hEdFont = _consoleFont;
  
  ZeroMemory(&lf, sizeof(LOGFONT));
  GetObject(hEdFont ? hEdFont : GetStockObject(SYSTEM_FONT), sizeof(LOGFONT), &lf);

  ZeroMemory(&cf, sizeof(CHOOSEFONT));
  cf.lStructSize = sizeof(CHOOSEFONT);
  cf.hwndOwner = m_nppData._nppHandle;
  cf.lpLogFont = &lf;
  cf.Flags = CF_FORCEFONTEXIST | CF_INITTOLOGFONTSTRUCT | CF_SCREENFONTS;
  
  HWND hWndFocus = ::GetFocus();
  
  if (ChooseFont(&cf))
  {
    /*
    if (lf.lfHeight > 0) 
      lf.lfHeight = -lf.lfHeight;
    */
    SetConsoleFont(hEd, &lf);
  }

  ::SetFocus(hWndFocus);

}

void CNppExec::OnShowConsoleDlg(void)
{
    if ( g_bIsNppReady || (Runtime::GetOptions().GetInt(OPTI_CONSOLE_VISIBLE) == CAdvOptDlg::CON_AUTO) )
        showConsoleDialog(true);
}

void CNppExec::OnToggleConsoleDlg(void)
{
    if ( !isConsoleDialogVisible() )
    {
        showConsoleDialog(false);
    }
    else
    {
        HWND hFocused = ::GetFocus();

        if ( (GetConsole().GetParentWnd() == hFocused) || 
             IsChild(GetConsole().GetParentWnd(), hFocused) )
        {
            if ( Runtime::GetOptions().GetBool(OPTB_CONSOLE_HIDETOGGLED) && !_consoleProcessIsRunning )
                showConsoleDialog(true);
            else
            {
                ConsoleDlg::EnableTransparency(true);
                ::SetFocus( GetScintillaHandle() );
            }
        }
        else
        {
            ::SetFocus( GetConsole().GetConsoleWnd() );
        }
    }
}

void CNppExec::OnHelpManual(void)
{
    tstr sHelpFile = Runtime::GetOptions().GetStr(OPTS_PLUGIN_HELPFILE);
    if ( !sHelpFile.IsEmpty() )  // not empty
    {
        Runtime::GetNppExec().GetMacroVars().CheckPluginMacroVars(sHelpFile);
        Runtime::GetNppExec().GetMacroVars().CheckNppMacroVars(sHelpFile);

        sHelpFile.Replace( _T('/'), _T('\\') );
        if ( !isFullPath(sHelpFile) )
        {
            // sHelpFile is a relative pathname
            if ( sHelpFile.GetFirstChar() != _T('\\') )
                sHelpFile.Insert( 0, _T('\\') );
            sHelpFile.Insert( 0, m_szPluginPath );
        }
        
#ifndef __MINGW32__
        if ( sHelpFile.Find(_T(".chm")) > 0 )
            ::HtmlHelp( NULL, sHelpFile.c_str(), HH_DISPLAY_TOPIC, NULL );
        else
#endif
            ::ShellExecute( NULL, _T("open"), sHelpFile.c_str(), NULL, NULL, SW_SHOWNORMAL );
    }
}

void CNppExec::OnHelpDocs(void)
{
    const int nHelpDocs = 2;
    const TCHAR* cszDocFileNames[nHelpDocs] = {
        _T("NppExec_TechInfo.txt"),
        _T("NppExec.txt")
    };
    
    tstr doc_file;
    tstr path = getPluginPath();
    path += _T("\\doc\\");

    for ( int i = 0; i < nHelpDocs; i++ )
    {
        doc_file = path;
        doc_file += cszDocFileNames[i];
        SendNppMsg( NPPM_DOOPEN, 0, (LPARAM) doc_file.c_str() );
    }
}

void CNppExec::OnHelpAbout(void)
{
    PluginDialogBox(IDD_HELP_ABOUT, HelpAboutDlgProc);
}

void CNppExec::OnUserMenuItem(int nItemNumber)
{
    if ( nItemNumber >= 0 && nItemNumber < g_nUserMenuItems )
    {
        int n = 0;
        for ( int i = 0; i < MAX_USERMENU_ITEMS; i++ )
        {
            int len = 0;
            const TCHAR* psz = Runtime::GetOptions().GetStr(OPTS_USERMENU_ITEM01 + i, &len);
            if ( len > 0 )
            {
                if ( nItemNumber == n++ )
                {
                    len = c_base::_tstr_unsafe_rfind(psz, len, cszUserMenuItemSep);
                    if ( len > 0 )
                    {
                        len += lstrlen(cszUserMenuItemSep);
                        psz += len; // before cszUserMenuItemSep
                        psz = c_base::_tstr_unsafe_skip_tabspaces(psz);
                        if ( *psz )
                        {
                            DoExecScript(psz, true);
                        }
                    }
                    break;
                }
            }
        }
    }
}

void CNppExec::ReadOptions(void)
{
  // Reading options...
  Runtime::GetOptions().ReadOptions(m_szIniFilePathName);

  // Log File...
  tstr sLogsDir = Runtime::GetOptions().GetStr(OPTS_PLUGIN_LOGSDIR);
  if ( !sLogsDir.IsEmpty() )
  {
    TCHAR buf[16];
    time_t rawTime;
    struct tm * timeInfo;
    ::time(&rawTime);
    timeInfo = ::localtime(&rawTime);

    Runtime::GetNppExec().GetMacroVars().CheckPluginMacroVars(sLogsDir);
    Runtime::GetNppExec().GetMacroVars().CheckNppMacroVars(sLogsDir);

    sLogsDir.Replace( _T('/'), _T('\\') );
    if ( !isFullPath(sLogsDir) )
    {
      tstr sBaseDir = _T("$(SYS.TEMP)");
      Runtime::GetNppExec().GetMacroVars().CheckPluginMacroVars(sBaseDir);
      if ( sBaseDir.IsEmpty() )
      {
        sBaseDir = getConfigPath();
      }
      if ( !sBaseDir.IsEmpty() )
      {
        if ( sBaseDir.GetLastChar() != _T('\\') )
          sBaseDir += _T('\\');
      }
      sLogsDir.Insert( 0, sBaseDir );
    }

    if ( sLogsDir.GetLastChar() != _T('\\') )
      sLogsDir += _T('\\');
    sLogsDir += PLUGIN_NAME;
    sLogsDir += _T('\\');
    buf[0] = 0;
    ::_tcsftime(buf, 15, _T("%Y_%m_%d"), timeInfo); // 2013_09_28
    sLogsDir += buf;

    createDirectoryTree(sLogsDir);
    
    tstr sLogFile = sLogsDir;
    if ( sLogFile.GetLastChar() != _T('\\') )
      sLogFile += _T('\\');
    sLogFile += PLUGIN_NAME;
    sLogFile += _T("_log_");
    buf[0] = 0;
    ::_tcsftime(buf, 15, _T("%H%M"), timeInfo); // 2252
    sLogFile += buf;
    sLogFile += _T(".txt");

    Runtime::GetLogger().SetLogFile( sLogFile.c_str() );
    if ( Runtime::GetLogger().IsLogFileOpen() )
    {
        Runtime::GetLogger().Add( _T("; CNppExec::ReadOptions - start") );
    }
  }
  
  // Verifying options' values...
  if (Runtime::GetOptions().GetInt(OPTU_CHILDP_STARTUPTIMEOUT_MS) < 0)
  {
    Runtime::GetOptions().SetUint(OPTU_CHILDP_STARTUPTIMEOUT_MS, DEFAULT_STARTUPTIMEOUT_MS);
  }
  if (Runtime::GetOptions().GetInt(OPTU_CHILDP_CYCLETIMEOUT_MS) < 0)
  {
    Runtime::GetOptions().SetUint(OPTU_CHILDP_CYCLETIMEOUT_MS, DEFAULT_CYCLETIMEOUT_MS);
  }
  if (Runtime::GetOptions().GetInt(OPTI_CMDHISTORY_MAXITEMS) < 2)
  {
    Runtime::GetOptions().SetInt(OPTI_CMDHISTORY_MAXITEMS, DEFAULT_CMDHISTORY_MAXITEMS);
  }
  if (Runtime::GetOptions().GetInt(OPTI_EXEC_MAXCOUNT) < 2)
  {
    Runtime::GetOptions().SetInt(OPTI_EXEC_MAXCOUNT, DEFAULT_EXEC_MAXCOUNT);
  }
  if (Runtime::GetOptions().GetInt(OPTI_GOTO_MAXCOUNT) < 2)
  {
    Runtime::GetOptions().SetInt(OPTI_GOTO_MAXCOUNT, DEFAULT_GOTO_MAXCOUNT);
  }
  if (Runtime::GetOptions().GetInt(OPTI_RICHEDIT_MAXTEXTLEN) < 0x10000)
  {
    Runtime::GetOptions().SetInt(OPTI_RICHEDIT_MAXTEXTLEN, 0x10000);
  }
  if (Runtime::GetOptions().GetInt(OPTI_SENDMSG_MAXBUFLEN) < 0x10000)
  {
    Runtime::GetOptions().SetInt(OPTI_SENDMSG_MAXBUFLEN, 0x10000);
  }

  _consoleFont = NULL;
  int dataSize = 0;
  const LOGFONT* pLogFont = (const LOGFONT *) Runtime::GetOptions().GetData(OPTD_CONSOLE_FONT, &dataSize);
  if ( pLogFont && (dataSize >= sizeof(LOGFONT)) )
  {
    _consoleFont = CreateFontIndirect(pLogFont);
  }

  dataSize = 0;
  c_base::byte_t* pData = (c_base::byte_t *) Runtime::GetOptions().GetData(OPTD_COLOR_TEXTNORM, &dataSize);
  if ( (dataSize == 3) || (dataSize == 4) )
  {
    g_colorTextNorm = RGB( pData[0], pData[1], pData[2] );
  }

  dataSize = 0;
  pData = (c_base::byte_t *) Runtime::GetOptions().GetData(OPTD_COLOR_TEXTERR, &dataSize);
  if ( (dataSize == 3) || (dataSize == 4) )
  {
    g_colorTextErr = RGB( pData[0], pData[1], pData[2] );
  }

  dataSize = 0;
  pData = (c_base::byte_t *) Runtime::GetOptions().GetData(OPTD_COLOR_TEXTMSG, &dataSize);
  if ( (dataSize == 3) || (dataSize == 4) )
  {
    g_colorTextMsg = RGB( pData[0], pData[1], pData[2] );
  }

  dataSize = 0;
  pData = (c_base::byte_t *) Runtime::GetOptions().GetData(OPTD_COLOR_BKGND, &dataSize);
  if ( (dataSize == 3) || (dataSize == 4) )
  {
    g_colorBkgnd = RGB( pData[0], pData[1], pData[2] );
  }
  
    //#ifdef _NIB
    for (int i = 0; i < CConsoleOutputFilterDlg::RECOGNITION_ITEMS; i++)
    {
        const TCHAR* cszMask = Runtime::GetOptions().GetStr(OPTS_CONFLTR_RCGNMSK1 + i);
        if ( cszMask && cszMask[0] )
        {
            Runtime::GetWarningAnalyzer().SetMask( i, cszMask );
            const tstr eff = Runtime::GetOptions().GetStr(OPTS_CONFLTR_RCGNEFF1 + i);
            if ( eff.length() > 0 )
            {
                CWarningAnalyzer::TEffect Effect;
                Effect.Enable     = ( eff.GetAt(0) == '+' ? true : false );
                Effect.Italic     = ( eff.GetAt(12) == '+' ? true : false );
                Effect.Bold       = ( eff.GetAt(15) == '+' ? true : false );
                Effect.Underlined = ( eff.GetAt(18) == '+' ? true : false );
                Effect.Red        = CWarningAnalyzer::xtou( eff.GetAt(3), eff.GetAt(4) );
                Effect.Green      = CWarningAnalyzer::xtou( eff.GetAt(6), eff.GetAt(7) );
                Effect.Blue       = CWarningAnalyzer::xtou( eff.GetAt(9), eff.GetAt(10) );
                Runtime::GetWarningAnalyzer().SetEffect( i, Effect );
            }  
        }
    }
    //#endif

  for ( int i = 0; i < CConsoleOutputFilterDlg::HISTORY_ITEMS; i++ )
  {
    tstr s1 = Runtime::GetOptions().GetStr(OPTS_FILTERS_EXCL1 + i);
    if ( s1.length() > 0 )
    {
      if ( !ConsoleOutputFilterDlg.m_ExcludeHistory.FindExact(s1) )
        ConsoleOutputFilterDlg.m_ExcludeHistory.Add(s1);
    }
    s1 = Runtime::GetOptions().GetStr(OPTS_FILTERS_INCL1 + i);
    if ( s1.length() > 0 )
    {
      if ( !ConsoleOutputFilterDlg.m_IncludeHistory.FindExact(s1) )
        ConsoleOutputFilterDlg.m_IncludeHistory.Add(s1);
    }
    s1 = Runtime::GetOptions().GetStr(OPTS_FILTERS_R_FIND1 + i);
    if ( s1.length() > 0 )
    {
      if ( !ConsoleOutputFilterDlg.m_RFindHistory.FindExact(s1) )
        ConsoleOutputFilterDlg.m_RFindHistory.Add(s1);
    }
    s1 = Runtime::GetOptions().GetStr(OPTS_FILTERS_R_RPLC1 + i);
    if ( s1.length() > 0 )
    {
      if ( !ConsoleOutputFilterDlg.m_RRplcHistory.FindExact(s1) )
        ConsoleOutputFilterDlg.m_RRplcHistory.Add(s1);
    }
    s1 = Runtime::GetOptions().GetStr(OPTS_FILTERS_HGLT1 + i);
    if ( s1.length() > 0 )
    {
      if ( !ConsoleOutputFilterDlg.m_HighlightHistory.FindExact(s1) )
        ConsoleOutputFilterDlg.m_HighlightHistory.Add(s1);
    }
  }

  for ( int i = 0; i < CInputBoxDlg::HISTORY_ITEMS; i++ )
  {
    tstr s1 = Runtime::GetOptions().GetStr(OPTS_INPUTBOX_VALUE1 + i);
    if ( s1.length() > 0 )
    {
      if ( !InputBoxDlg.m_InputHistory[CInputBoxDlg::IBT_INPUTBOX].FindExact(s1) )
        InputBoxDlg.m_InputHistory[CInputBoxDlg::IBT_INPUTBOX].Add(s1);
    }
    s1 = Runtime::GetOptions().GetStr(OPTS_EXITBOX_VALUE1 + i);
    if ( s1.length() > 0 )
    {
      if ( !InputBoxDlg.m_InputHistory[CInputBoxDlg::IBT_EXITPROCESS].FindExact(s1) )
        InputBoxDlg.m_InputHistory[CInputBoxDlg::IBT_EXITPROCESS].Add(s1);
    }
  }
  
  const TCHAR* cszHotKey = Runtime::GetOptions().GetStr(OPTS_PLUGIN_HOTKEY);
  if ( lstrlen(cszHotKey) > 0 )
  {
    unsigned int nHotKey = VK_F6;

    if ( lstrcmpi(cszHotKey, _T("F1")) == 0 )
      nHotKey = VK_F1;
    else if ( lstrcmpi(cszHotKey, _T("F2")) == 0 )
      nHotKey = VK_F2;
    else if ( lstrcmpi(cszHotKey, _T("F3")) == 0 )
      nHotKey = VK_F3;
    else if ( lstrcmpi(cszHotKey, _T("F4")) == 0 )
      nHotKey = VK_F4;
    else if ( lstrcmpi(cszHotKey, _T("F5")) == 0 )
      nHotKey = VK_F5;
    else if ( lstrcmpi(cszHotKey, _T("F6")) == 0 )
      nHotKey = VK_F6;
    else if ( lstrcmpi(cszHotKey, _T("F7")) == 0 )
      nHotKey = VK_F7;
    else if ( lstrcmpi(cszHotKey, _T("F8")) == 0 )
      nHotKey = VK_F8;
    else if ( lstrcmpi(cszHotKey, _T("F9")) == 0 )
      nHotKey = VK_F9;
    else if ( lstrcmpi(cszHotKey, _T("F10")) == 0 )
      nHotKey = VK_F10;
    else if ( lstrcmpi(cszHotKey, _T("F11")) == 0 )
      nHotKey = VK_F11;
    else if ( lstrcmpi(cszHotKey, _T("F12")) == 0 )
      nHotKey = VK_F12;

    Runtime::GetOptions().SetUint(OPTU_PLUGIN_HOTKEY, nHotKey);
  }

  if (Runtime::GetOptions().GetInt(OPTI_CONSOLE_VISIBLE) < 0)
  {
    Runtime::GetOptions().SetInt(OPTI_CONSOLE_VISIBLE, CAdvOptDlg::CON_AUTO);
  }
  if (Runtime::GetOptions().GetInt(OPTB_CONSOLE_SHOWHELP) < 0)
  {
    Runtime::GetOptions().SetBool(OPTB_CONSOLE_SHOWHELP, false);
  }
  if (Runtime::GetOptions().GetInt(OPTU_CONSOLE_ENCODING) < 0)
  {
    Runtime::GetOptions().SetUint(OPTU_CONSOLE_ENCODING, CConsoleEncodingDlg::defaultEncodingOption());
  }
  //#ifdef _GVC
  if (Runtime::GetOptions().GetInt(OPTB_CONSOLE_SAVEONEXECUTE) < 0)
  {
    Runtime::GetOptions().SetBool(OPTB_CONSOLE_SAVEONEXECUTE, false);
  }
  //#endif
  if (Runtime::GetOptions().GetInt(OPTB_CONSOLE_CMDHISTORY) < 0)
  {
    Runtime::GetOptions().SetBool(OPTB_CONSOLE_CMDHISTORY, true);
  }
  if (Runtime::GetOptions().GetInt(OPTB_CONSOLE_SAVECMDHISTORY) < 0)
  {
    Runtime::GetOptions().SetBool(OPTB_CONSOLE_SAVECMDHISTORY, true);
  }
#ifdef _SCROLL_TO_LATEST  
  if (Runtime::GetOptions().GetInt(OPTB_CONSOLE_SCROLL2LATEST) < 0)
  {
    Runtime::GetOptions().SetBool(OPTB_CONSOLE_SCROLL2LATEST, true);
  }
#endif
  if (Runtime::GetOptions().GetInt(OPTB_CONSOLE_NOINTMSGS) < 0)
  {
    Runtime::GetOptions().SetBool(OPTB_CONSOLE_NOINTMSGS, false);
  }

  {
    tstr sKeyEnter = Runtime::GetOptions().GetStr(OPTS_KEY_ENTER);
    if (sKeyEnter.IsEmpty())
      sKeyEnter = DEFAULT_KEY_ENTER;
    sKeyEnter.Replace(_T("\\n"), _T("\n"));
    sKeyEnter.Replace(_T("\\r"), _T("\r"));
    Runtime::GetOptions().SetStr(OPTS_KEY_ENTER, sKeyEnter.c_str());
  }

  {
    int nNulCharSize = 0;
    const void* pNulCharData = Runtime::GetOptions().GetData(OPTD_CONSOLE_NULCHAR, &nNulCharSize);

#ifdef UNICODE
    wchar_t wch = 0;
    if ( pNulCharData && (nNulCharSize >= (int) sizeof(wchar_t)) )
    {
      wch = *((const wchar_t *) pNulCharData);
      c_base::str_unsafe_reversen( (char *) &wch, (int) (sizeof(wchar_t)/sizeof(char)) ); // reverse byte order
    }
    else if ( pNulCharData && (nNulCharSize >= (int) sizeof(char)) )
    {
      char ch = *((const char *) pNulCharData);
      wch = ch;
    }
    else
    {
      wch = DEFAULT_NULCHAR_UNICODE;
    }
    Runtime::GetOptions().SetData(OPTD_CONSOLE_NULCHAR, &wch, (int) sizeof(wchar_t));
#else
    char ch = 0;
    if ( pNulCharData && (nNulCharSize >= (int) sizeof(char)) )
    {
      ch = *((const char *) pNulCharData);
    }
    else
    {
      ch = DEFAULT_NULCHAR_ANSI;
    }
    Runtime::GetOptions().SetData(OPTD_CONSOLE_NULCHAR, &ch, (int) sizeof(char));
#endif
  }

  TCHAR path[FILEPATH_BUFSIZE];
  ExpandToFullConfigPath(path, SCRIPTFILE_TEMP);

  CFileBufT<TCHAR> fbuf;
  if (fbuf.LoadFromFile(path))
  {
    tstr Line;

    while (fbuf.GetLine(Line) >= 0)
    {
      m_TempScript.Add(Line);
    } 
  }

  ExpandToFullConfigPath(path, SCRIPTFILE_SAVED);
  m_ScriptList.LoadFromFile(path);
  g_scriptFileWatcher.AssignFile(path);

  if ( Runtime::GetLogger().IsLogFileOpen() )
  {
    Runtime::GetLogger().Add( _T("; CNppExec::ReadOptions - end") );
  }

  StartAutoSaveTimer();
}

void CNppExec::StartAutoSaveTimer()
{
  if (m_idTimerAutoSave == 0)
  {
    UINT uTimeoutInMilliseconds = Runtime::GetOptions().GetUint(OPTU_PLUGIN_AUTOSAVE_SECONDS)*1000;
    if (uTimeoutInMilliseconds != 0) // auto-save enabled
    {
      const UINT minTimeout = 10*1000; // 10 seconds

      if (uTimeoutInMilliseconds < minTimeout)
        uTimeoutInMilliseconds = minTimeout;
      m_idTimerAutoSave = ::SetTimer(NULL, 0, uTimeoutInMilliseconds, OnTimer_AutoSaveProc);
    }
  }
}

void CNppExec::StopAutoSaveTimer()
{
  if (m_idTimerAutoSave != 0)
  {
    ::KillTimer(NULL, m_idTimerAutoSave);
    m_idTimerAutoSave = 0;
  }
}

void CALLBACK CNppExec::OnTimer_AutoSaveProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
  Runtime::GetNppExec().SaveOptions();
}

void CNppExec::SaveOptions(void)
{
  if ( m_isSavingOptions )
    return;

  ::EnterCriticalSection(&m_csAutoSave);

  m_isSavingOptions = true;

  if ( Runtime::GetLogger().IsLogFileOpen() )
  {
    Runtime::GetLogger().Add( _T("; CNppExec::SaveOptions - start") );
  }

    //#ifdef _NIB
    for (int i = 0; i < CConsoleOutputFilterDlg::RECOGNITION_ITEMS; i++)
    {
        TCHAR strRcgnMask[OUTPUTFILTER_BUFSIZE];

        if ( *Runtime::GetWarningAnalyzer().GetMask( i, strRcgnMask, OUTPUTFILTER_BUFSIZE - 1 ) != 0 )
        {
            CWarningAnalyzer::TEffect Effect;
            TCHAR strEffect[32];

            Runtime::GetWarningAnalyzer().GetEffect( i, Effect );
            wsprintf( strEffect
                    , _T("%cE %c%c %c%c %c%c %cI %cB %cU")
                    , ( Effect.Enable ? '+' : '-' )                                                               
                    , ( ( (Effect.Red>>4   )&0xF ) > 9 ? 'A' + ( (Effect.Red>>4   )&0xF ) - 0xA : '0' + ( ( Effect.Red>>4   )&0xF ) )
                    , ( ( (Effect.Red      )&0xF ) > 9 ? 'A' + ( (Effect.Red      )&0xF ) - 0xA : '0' + ( ( Effect.Red      )&0xF ) )
                    , ( ( (Effect.Green>>4 )&0xF ) > 9 ? 'A' + ( (Effect.Green>>4 )&0xF ) - 0xA : '0' + ( ( Effect.Green>>4 )&0xF ) )
                    , ( ( (Effect.Green    )&0xF ) > 9 ? 'A' + ( (Effect.Green    )&0xF ) - 0xA : '0' + ( ( Effect.Green    )&0xF ) )
                    , ( ( (Effect.Blue>>4  )&0xF ) > 9 ? 'A' + ( (Effect.Blue>>4  )&0xF ) - 0xA : '0' + ( ( Effect.Blue>>4  )&0xF ) )
                    , ( ( (Effect.Blue     )&0xF ) > 9 ? 'A' + ( (Effect.Blue     )&0xF ) - 0xA : '0' + ( ( Effect.Blue     )&0xF ) )
                    , ( Effect.Italic ? '+' : '-' )                                                               
                    , ( Effect.Bold ? '+' : '-' )
                    , ( Effect.Underlined ? '+' : '-' )
                    );
            Runtime::GetOptions().SetStr(OPTS_CONFLTR_RCGNMSK1 + i, strRcgnMask);
            Runtime::GetOptions().SetStr(OPTS_CONFLTR_RCGNEFF1 + i, strEffect);
        }
        else
        {
            Runtime::GetOptions().SetStr(OPTS_CONFLTR_RCGNMSK1 + i, _T(""));
        }
    }
    //#endif

  CListItemT<tstr>* p1 = ConsoleOutputFilterDlg.m_ExcludeHistory.GetFirst();
  CListItemT<tstr>* p2 = ConsoleOutputFilterDlg.m_IncludeHistory.GetFirst();
  CListItemT<tstr>* p3 = ConsoleOutputFilterDlg.m_RFindHistory.GetFirst();
  CListItemT<tstr>* p4 = ConsoleOutputFilterDlg.m_RRplcHistory.GetFirst();
  CListItemT<tstr>* p5 = ConsoleOutputFilterDlg.m_HighlightHistory.GetFirst();
  for ( int i = 0; i < CConsoleOutputFilterDlg::HISTORY_ITEMS; i++ )
  {
    if ( p1 )
    {
      if ( p1->GetItem().length() > 0 )
        Runtime::GetOptions().SetStr( OPTS_FILTERS_EXCL1 + i, p1->GetItem().c_str() );
      p1 = p1->GetNext();
    }
    if ( p2 )
    {
      if ( p2->GetItem().length() > 0 )
        Runtime::GetOptions().SetStr( OPTS_FILTERS_INCL1 + i, p2->GetItem().c_str() );
      p2 = p2->GetNext();
    }
    if ( p3 )
    {
      if ( p3->GetItem().length() > 0 )
        Runtime::GetOptions().SetStr( OPTS_FILTERS_R_FIND1 + i, p3->GetItem().c_str() );
      p3 = p3->GetNext();
    }
    if ( p4 )
    {
      if ( p4->GetItem().length() > 0 )
        Runtime::GetOptions().SetStr( OPTS_FILTERS_R_RPLC1 + i, p4->GetItem().c_str() );
      p4 = p4->GetNext();
    }
    if ( p5 )
    {
      if ( p5->GetItem().length() > 0 )
        Runtime::GetOptions().SetStr( OPTS_FILTERS_HGLT1 + i, p5->GetItem().c_str() );
      p5 = p5->GetNext();
    }
  }

  CListItemT<tstr>* p6 = InputBoxDlg.m_InputHistory[CInputBoxDlg::IBT_INPUTBOX].GetFirst();
  CListItemT<tstr>* p7 = InputBoxDlg.m_InputHistory[CInputBoxDlg::IBT_EXITPROCESS].GetFirst();
  for ( int i = 0; i < CInputBoxDlg::HISTORY_ITEMS; i++ )
  {
    if ( p6 )
    {
      if ( p6->GetItem().length() > 0 )
        Runtime::GetOptions().SetStr( OPTS_INPUTBOX_VALUE1 + i, p6->GetItem().c_str() );
      p6 = p6->GetNext();
    }
    if ( p7 )
    {
      if ( p7->GetItem().length() > 0 )
        Runtime::GetOptions().SetStr( OPTS_EXITBOX_VALUE1 + i, p7->GetItem().c_str() );
      p7 = p7->GetNext();
    }
  }

  //if ( Runtime::GetOptions().GetUint(OPTU_PLUGIN_HOTKEY) != Runtime::GetOptions().GetUint(OPTU_PLUGIN_HOTKEY, false) )
  // Incorrect comparison because initial value 
  // of OPTU_PLUGIN_HOTKEY is always VK_F6.
  {
    const unsigned int nHotKey = Runtime::GetOptions().GetUint(OPTU_PLUGIN_HOTKEY);  
    TCHAR szHotKey[8];
        
    if ( nHotKey == VK_F1 )
      lstrcpy(szHotKey, _T("F1"));
    else if ( nHotKey == VK_F2 )
      lstrcpy(szHotKey, _T("F2"));
    else if ( nHotKey == VK_F3 )
      lstrcpy(szHotKey, _T("F3"));
    else if ( nHotKey == VK_F4 )
      lstrcpy(szHotKey, _T("F4"));
    else if ( nHotKey == VK_F5 )
      lstrcpy(szHotKey, _T("F5"));
    else if ( nHotKey == VK_F6 )
      lstrcpy(szHotKey, _T("F6"));
    else if ( nHotKey == VK_F7 )
      lstrcpy(szHotKey, _T("F7"));
    else if ( nHotKey == VK_F8 )
      lstrcpy(szHotKey, _T("F8"));
    else if ( nHotKey == VK_F9 )
      lstrcpy(szHotKey, _T("F9"));
    else if ( nHotKey == VK_F10 )
      lstrcpy(szHotKey, _T("F10"));
    else if ( nHotKey == VK_F11 )
      lstrcpy(szHotKey, _T("F11"));
    else if ( nHotKey == VK_F12 )
      lstrcpy(szHotKey, _T("F12"));
    else
      lstrcpy(szHotKey, _T("F6"));

    Runtime::GetOptions().SetStr(OPTS_PLUGIN_HOTKEY, szHotKey);
  }
    
  Runtime::GetOptions().SaveOptions(m_szIniFilePathName);
  SaveScripts();

  ConsoleDlg::SaveCmdHistory();

  if ( Runtime::GetLogger().IsLogFileOpen() )
  {
    Runtime::GetLogger().Add( _T("; CNppExec::SaveOptions - end") );
  }

  m_isSavingOptions = false;

  ::LeaveCriticalSection(&m_csAutoSave);
}

void CNppExec::SaveScripts(void)
{
  TCHAR             path[FILEPATH_BUFSIZE];
  CListItemT<tstr>* p;
  CFileBufT<TCHAR>  fbuf;
  tstr              Line;

  if (m_TempScriptIsModified)
  {
    fbuf.GetBufPtr()->Clear();
    p = m_TempScript.GetFirst();
    while (p)
    {
      Line = p->GetItem();
      if (p != m_TempScript.GetLast())
        Line.Append(_T("\r\n"), 2);
      fbuf.GetBufPtr()->Append(Line.c_str(), Line.length());
      p = p->GetNext();
    }
    ExpandToFullConfigPath(path, SCRIPTFILE_TEMP);
    fbuf.SaveToFile(path);
    m_TempScriptIsModified = false;
  }

  if (m_ScriptList.IsModified())
  {
    ExpandToFullConfigPath(path, SCRIPTFILE_SAVED);
    m_ScriptList.SaveToFile(path);
    g_scriptFileWatcher.UpdateFileInfo();
  }

}

HWND CNppExec::GetScintillaHandle(void)
{
  INT currentEdit;
  SendNppMsg(NPPM_GETCURRENTSCINTILLA, 0, (LPARAM)&currentEdit);
  return getCurrentScintilla(currentEdit);
}

#define NOTEPADPLUS_USER_INTERNAL  (WM_USER + 0000)
#define NPPM_INTERNAL_GETMENU      (NOTEPADPLUS_USER_INTERNAL + 14)

HMENU CNppExec::GetNppMainMenu()
{
    HMENU hMenu;

    // Notepad++ ver > 6.3
    hMenu = (HMENU) SendNppMsg(NPPM_GETMENUHANDLE, 1, 0);
    if ( hMenu && IsMenu(hMenu) )
        return hMenu;

    // Notepad++ ver <= 6.3
    hMenu = (HMENU) SendNppMsg(NPPM_INTERNAL_GETMENU, 0, 0);
    if ( hMenu && IsMenu(hMenu) )
        return hMenu;

    return ::GetMenu(m_nppData._nppHandle);
}

void CNppExec::SetConsoleFont(HWND hEd, const LOGFONT* plf)
{
  if (_consoleFont)
    ::DeleteObject(_consoleFont);
  Runtime::GetOptions().SetData(OPTD_CONSOLE_FONT, plf, sizeof(LOGFONT));
  _consoleFont = CreateFontIndirect(plf);
  if (hEd && _consoleFont)
  {
    GetConsole().GetConsoleEdit().SetFont(plf);
    GetConsole().UpdateColours();
  }
}

void CNppExec::setConsoleVisible(bool consoleIsVisible)
{
  _consoleIsVisible = consoleIsVisible;

  SendNppMsg(NPPM_SETMENUITEMCHECK, 
    g_funcItem[N_SHOWCONSOLE]._cmdID, (LPARAM) consoleIsVisible);
  /*
  HMENU hMenu = GetNppMainMenu();
  if (hMenu)
  {
    EnableMenuItem(hMenu, g_funcItem[N_SHOWCONSOLE]._cmdID,
      MF_BYCOMMAND | (_consoleIsVisible ? MF_GRAYED : MF_ENABLED) );
  }
  */
}

void CNppExec::verifyConsoleDialogExists()
{
    HWND hConsoleParentWnd = GetConsole().GetParentWnd();

    if ( !hConsoleParentWnd )
    {
        hConsoleParentWnd = CreateDialog(
          (HINSTANCE) m_hDllModule,
          MAKEINTRESOURCE(IDD_CONSOLE),
          m_nppData._nppHandle,
          ConsoleDlgProc);

        GetConsole().SetParentWnd(hConsoleParentWnd);
    }

    if (hConsoleParentWnd) 
    {
        GetConsole().SetConsoleWnd( GetDlgItem(hConsoleParentWnd, IDC_RE_CONSOLE) );

        //::ShowWindow(hConsoleParentWnd, SW_HIDE);
    }
}

bool CNppExec::isConsoleDialogVisible()
{
    if ( GetConsole().GetParentWnd() )
        return ::IsWindowVisible(GetConsole().GetParentWnd()) ? true : false;
    else
        return _consoleIsVisible;
}

void CNppExec::showConsoleDialog(bool bCanHide)
{
  if (!isConsoleDialogVisible())
  {
    verifyConsoleDialogExists();
    if (GetConsole().GetParentWnd()) 
    {
      ConsoleDlg::DockDialog();
      SendNppMsg(NPPM_DMMSHOW, 0, (LPARAM) GetConsole().GetParentWnd());
      setConsoleVisible(true);
    }
  }
  else if (bCanHide)
  {
    if ( GetConsole().GetParentWnd() )
    {
      ::SetWindowLong(GetConsole().GetParentWnd(), DWL_MSGRESULT, 0L);
      ::SendMessage(GetConsole().GetParentWnd(), WM_SYSCOMMAND, SC_CLOSE, 0);
      if (::GetWindowLong(GetConsole().GetParentWnd(), DWL_MSGRESULT) != 1L)
      {
        SendNppMsg(NPPM_DMMHIDE, 0, (LPARAM) GetConsole().GetParentWnd());
        //setConsoleVisible(false);
      }
    }
  }
  else
  {
    if ( ConsoleDlg::EnableTransparency(false) )
      ::SetFocus( GetConsole().GetConsoleWnd() );
  }
}

void CNppExec::printConsoleHelpInfo()
{
  GetConsole().PrintMessage(SZ_CONSOLE_HELP_INFO, false);
}

void CNppExec::ShowError(LPCTSTR szMessage)
{
  ::MessageBox(m_nppData._nppHandle, szMessage, 
      _T("Notepad++ (NppExec) Error"), MB_OK | MB_ICONERROR);
}

void CNppExec::ShowWarning(LPCTSTR szMessage)
{
  ::MessageBox(m_nppData._nppHandle, szMessage, 
      _T("Notepad++ (NppExec) Warning"), MB_OK | MB_ICONWARNING);
}

INT_PTR CNppExec::PluginDialogBox(UINT idDlg, DLGPROC lpDlgProc)
{
  HWND hWndFocus = ::GetFocus();
  INT_PTR nRet = ::DialogBox( (HINSTANCE) m_hDllModule, 
      MAKEINTRESOURCE(idDlg), m_nppData._nppHandle, lpDlgProc );
  ::SetFocus(hWndFocus);
  return nRet;
}

LRESULT CNppExec::SendNppMsg(UINT uMsg, WPARAM wParam, LPARAM lParam) // to Notepad++
{
    return ::SendMessage(m_nppData._nppHandle, uMsg, wParam, lParam);
}

//-------------------------------------------------------------------------

void CALLBACK CSuspendedScriptAction::SuspendedActionTimerProc(HWND hWnd, 
                UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
    CSuspendedScriptAction& action = Runtime::GetNppExec().m_SuspendedScriptAction;

    if ( idEvent == action.getTimerEventID() )
    {
        // the time is out
        action.setActive(false);
        action.stopWaitingTimer();
    }
}

//-------------------------------------------------------------------------

BOOL CreateNewThread(LPTHREAD_START_ROUTINE lpFunc, LPVOID lpParam,
                     HANDLE* lphThread)
{
  DWORD  dwThreadID;
  HANDLE hThread;
          
  hThread = CreateThread(
    NULL,
    0,
    lpFunc,
    lpParam,
    CREATE_SUSPENDED,
    &dwThreadID);
  
  if (hThread == NULL)
    return FALSE;
  
  ResumeThread(hThread);
  if (!lphThread)
  {
    CloseHandle(hThread);
  }
  else
  {
    *lphThread = hThread;
  }
  return TRUE;
}

BOOL IsWindowsNT(void)
{
  OSVERSIONINFO osv;
  osv.dwOSVersionInfoSize = sizeof(osv);
  GetVersionEx(&osv);
  return (osv.dwPlatformId == VER_PLATFORM_WIN32_NT);
}

bool isFullPath(const tstr& path)
{
    return isFullPath( path.c_str() );
}

bool isFullPath(const TCHAR* path)
{
    if ( path[0] )  // not empty
    {
        switch ( path[1] )
        {
            case _T(':') :
                return true;                  // "X:..."
            case _T('\\') :
            case _T('/') :
                return (path[0] == path[1]);  // "\\..." or "//..."
        }
    }
    return false;
}

static bool impl_createDir(const TCHAR* dir)
{
    if ( !::CreateDirectory(dir, NULL) )
    {
        if ( ::GetLastError() != ERROR_ALREADY_EXISTS )
            return false;
    }
    return true;
}

static bool impl_createDirectoryTree(const tstr& dir)
{
    tstr inter_dir = dir;
    inter_dir.Replace( _T('/'), _T('\\') );
    int n = inter_dir.Find( _T('\\'), 3 ); // skip "C:\"
    while ( n >= 0 )
    {
        inter_dir[n] = 0;
        if ( !impl_createDir(inter_dir.c_str()) )
            return false;

        inter_dir[n] = _T('\\');
        n = inter_dir.Find( _T('\\'), n + 1 );
    }
    return impl_createDir(inter_dir.c_str());
}

bool createDirectoryTree(const tstr& dir)
{
    return impl_createDirectoryTree(dir);
}

bool createDirectoryTree(const TCHAR* dir)
{
    return impl_createDirectoryTree(dir);
}


//-------------------------------------------------------------------------

CNppConsoleRichEdit::CNppConsoleRichEdit() : CAnyRichEdit()
{
}

//void CNppConsoleRichEdit::AddLine(const TCHAR* cszLine, BOOL bScrollText /* = FALSE */, 
//                                  COLORREF color /* = RGB(0,0,0) */, 
//                                  DWORD dwMask /* = CFM_EFFECTS */, DWORD dwEffects /* = 0 */)
//{
//    tstr S;
//    TCHAR ch = GetNulChar();
//    if ( ch != 0 )
//    {
//        S = cszLine;
//        S.Replace( TCHAR(0), ch );
//        cszLine = S.c_str();
//    }
//
//    CAnyRichEdit::AddLine(cszLine, bScrollText, color, dwMask, dwEffects);
//}

//void CNppConsoleRichEdit::AddStr(const TCHAR* cszStr, BOOL bScrollText /* = FALSE */, 
//                                 COLORREF color /* = RGB(0,0,0) */, 
//                                 DWORD dwMask /* = CFM_EFFECTS */, DWORD dwEffects /* = 0 */)
//{
//    tstr S;
//    TCHAR ch = GetNulChar();
//    if ( ch != 0 )
//    {
//        S = cszStr;
//        S.Replace( TCHAR(0), ch );
//        cszStr = S.c_str();
//    }
//
//    CAnyRichEdit::AddStr(cszStr, bScrollText, color, dwMask, dwEffects);
//}

TCHAR CNppConsoleRichEdit::GetNulChar()
{
    TCHAR ch = 0;
    int nNulCharSize = 0;
    const TCHAR* pNulCharData = (const TCHAR *) Runtime::GetOptions().GetData(OPTD_CONSOLE_NULCHAR, &nNulCharSize);
    if ( pNulCharData && (nNulCharSize == (int) sizeof(TCHAR)) )
    {
        ch = *pNulCharData;
    }
    return ch;
}

//-------------------------------------------------------------------------

CNppExecConsole::CNppExecConsole()
{
    m_hParent = NULL;
    m_nOutputEnabled = 1;

    m_colorTextNorm = 0;
    m_colorTextMsg = 0;
    m_colorTextErr = 0;
    m_colorBkgnd = 0;
}

COLORREF CNppExecConsole::getCurrentColorTextNorm() const
{
    return m_colorTextNorm ? m_colorTextNorm : g_colorTextNorm;
}

COLORREF CNppExecConsole::getCurrentColorTextMsg() const
{
    return m_colorTextMsg ? m_colorTextMsg : g_colorTextMsg;
}

COLORREF CNppExecConsole::getCurrentColorTextErr() const
{
    return m_colorTextErr ? m_colorTextErr : g_colorTextErr;
}

COLORREF CNppExecConsole::getCurrentColorBkgnd() const
{
    return m_colorBkgnd ? m_colorBkgnd : g_colorBkgnd;
}

void CNppExecConsole::setCurrentColorTextNorm(COLORREF colorTextNorm)
{
    m_colorTextNorm = colorTextNorm;
}

void CNppExecConsole::setCurrentColorTextMsg(COLORREF colorTextMsg)
{
    m_colorTextMsg = colorTextMsg;
}

void CNppExecConsole::setCurrentColorTextErr(COLORREF colorTextErr)
{
    m_colorTextErr = colorTextErr;
}

void CNppExecConsole::setCurrentColorBkgnd(COLORREF colorBkgnd)
{
    m_colorBkgnd = colorBkgnd;
}

static inline void Logger_Add_WithoutConsoleOutput(const tstr& S)
{
    const bool bOutputMode = Runtime::GetLogger().IsOutputMode();
    if ( bOutputMode )
    {
        Runtime::GetLogger().SetOutputMode(false);
    }
    Runtime::GetLogger().Add( S.c_str() );
    if ( bOutputMode )
    {
        Runtime::GetLogger().SetOutputMode(true, CNppExec::printScriptString);
    }
}

void CNppExecConsole::PrintError(LPCTSTR cszMessage, bool bLogThisMsg )
{
    if ( !IsOutputEnabled() )
        return;

    m_reConsole.AddStr( cszMessage, FALSE, getCurrentColorTextErr() ); 
    m_reConsole.AddLine( _T(""), IsScrollToEnd(), getCurrentColorTextNorm() );
    LockConsoleEndPos();

    if ( bLogThisMsg && Runtime::GetLogger().IsLogFileOpen() )
    {
        tstr S = _T("<ERR> ");
        S += cszMessage;
        Logger_Add_WithoutConsoleOutput(S);
    }
}

void CNppExecConsole::PrintMessage(LPCTSTR cszMessage, bool bIsInternalMsg , bool bLogThisMsg )
{
    if ( !IsOutputEnabled() )
        return;

    if ( bIsInternalMsg && Runtime::GetOptions().GetBool(OPTB_CONSOLE_NOINTMSGS) )
        return;

    m_reConsole.AddStr( cszMessage, FALSE, getCurrentColorTextMsg() ); 
    m_reConsole.AddLine( _T(""), IsScrollToEnd(), getCurrentColorTextNorm() );
    LockConsoleEndPos();

    if ( bLogThisMsg && Runtime::GetLogger().IsLogFileOpen() )
    {
        tstr S = _T("<MSG> ");
        S += cszMessage;
        Logger_Add_WithoutConsoleOutput(S);
    }
}

void CNppExecConsole::PrintOutput(LPCTSTR cszMessage, bool bNewLine , bool bLogThisMsg )
{
    if ( !IsOutputEnabled() )
        return;

    long color = getCurrentColorTextNorm();
    int  style = 0;

    if ( Runtime::GetWarningAnalyzer().match( cszMessage ) )
    {
        color = Runtime::GetWarningAnalyzer().GetColor();
        style = Runtime::GetWarningAnalyzer().GetStyle();
    }

    if ( bNewLine )
    {
        m_reConsole.AddLine( cszMessage, IsScrollToEnd(), color, CFM_EFFECTS, style );
    }
    else
    {
        m_reConsole.AddStr( cszMessage, IsScrollToEnd(), color, CFM_EFFECTS, style );
    }

    LockConsoleEndPos();

    if ( bLogThisMsg && Runtime::GetLogger().IsLogFileOpen() )
    {
        tstr S = _T("<OUT> ");
        S += cszMessage;
        Logger_Add_WithoutConsoleOutput(S);
    }
}

void CNppExecConsole::PrintStr(LPCTSTR cszStr, bool bNewLine, bool bLogThisMsg )
{
    if ( !IsOutputEnabled() )
        return;

    if ( bNewLine )
        m_reConsole.AddLine( cszStr, IsScrollToEnd(), getCurrentColorTextNorm(), CFM_EFFECTS, 0 );
    else
        m_reConsole.AddStr( cszStr, IsScrollToEnd(), getCurrentColorTextNorm(), CFM_EFFECTS, 0 );

    if ( bLogThisMsg && Runtime::GetLogger().IsLogFileOpen() )
    {
        tstr S = _T("<STR> ");
        S += cszStr;
        Logger_Add_WithoutConsoleOutput(S);
    }
}

void CNppExecConsole::PrintSysError(LPCTSTR cszFunctionName, DWORD dwErrorCode, bool bLogThisMsg )
{
    if ( !IsOutputEnabled() )
        return;

    if ( cszFunctionName && cszFunctionName[0] )
    {
        LPTSTR lpMsgBuf;
        TCHAR  szText[100];
  
        ::wsprintf( szText, _T("%s failed with error code %lu:"), cszFunctionName, dwErrorCode );
        ::FormatMessage(
            FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL,
            dwErrorCode,
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            (LPTSTR) &lpMsgBuf,
            0, 
            NULL );

        PrintError( szText, bLogThisMsg );
        PrintError( lpMsgBuf, bLogThisMsg );

        ::LocalFree(lpMsgBuf);
    }
}

void CNppExecConsole::LockConsolePos(INT nPos, bool bForce )
{
    if ( (!bForce) && (!IsOutputEnabled()) )
        return;

    ::SendMessage(m_hParent, WM_LOCKCONSOLEPOS, 0, (LPARAM) nPos);
}

void CNppExecConsole::LockConsoleEndPos(bool bForce )
{
    if ( (!bForce) && (!IsOutputEnabled()) )
        return;

    ::SendMessage(m_hParent, WM_LOCKCONSOLEPOS, 0, (LPARAM) m_reConsole.GetTextLengthEx() );
}

CAnyRichEdit& CNppExecConsole::GetConsoleEdit()
{
    return m_reConsole;
}

HWND CNppExecConsole::GetConsoleWnd() const
{
    return m_reConsole.m_hWnd;
}

void CNppExecConsole::SetConsoleWnd(HWND hWndRichEdit)
{
    m_reConsole.m_hWnd = hWndRichEdit;
}

HWND CNppExecConsole::GetParentWnd() const
{
    return m_hParent;
}

void CNppExecConsole::SetParentWnd(HWND hParent)
{
    m_hParent = hParent;
}

bool CNppExecConsole::IsOutputEnabled() const
{
    return (m_nOutputEnabled != 0);
}

int CNppExecConsole::IsOutputEnabledN() const
{
    return m_nOutputEnabled;
}

void CNppExecConsole::SetOutputEnabled(bool bEnabled)
{
    if ( bEnabled )
    {
        if ( m_nOutputEnabled == 0 )
            m_nOutputEnabled = 1; // enabled after was disabled
        else
            m_nOutputEnabled = 2; // enabled after was already enabled
    }
    else
        m_nOutputEnabled = 0;
}

BOOL CNppExecConsole::IsScrollToEnd() const
{
#ifdef _SCROLL_TO_LATEST  
    if ( !Runtime::GetOptions().GetBool(OPTB_CONSOLE_SCROLL2LATEST) )
    {
        return FALSE;
    }
#endif

    return TRUE;
}

void CNppExecConsole::ClearText(bool bForce )
{
    if ( (!bForce) && !IsOutputEnabled() )
        return;

    m_reConsole.SetText( _T("") );
    LockConsolePos( 0, bForce );
}

void CNppExecConsole::RestoreDefaultTextStyle(bool bLockPos)
{
    if ( !IsOutputEnabled() )
        return;

    m_reConsole.AddStr( _T(" "), FALSE, getCurrentColorTextNorm(), CFM_EFFECTS, 0 );
    int iPos = m_reConsole.GetTextLengthEx();
    m_reConsole.ExSetSel(iPos - 1, -1);
    m_reConsole.ReplaceSelText( _T("") );
    if ( bLockPos )
        LockConsolePos( iPos - 1 );
}

void CNppExecConsole::UpdateColours()
{
    // background color
    if ( getCurrentColorBkgnd() != 0xFFFFFFFF )
        m_reConsole.SendMsg(EM_SETBKGNDCOLOR, 0, getCurrentColorBkgnd());
    else
        m_reConsole.SendMsg(EM_SETBKGNDCOLOR, 0, GetSysColor(COLOR_WINDOW));

//#ifdef UNICODE
    DWORD dwLangOpt = (DWORD) m_reConsole.SendMsg( EM_GETLANGOPTIONS, 0, 0 );
    if (dwLangOpt & IMF_AUTOFONT)
    {
        dwLangOpt -= IMF_AUTOFONT;
        m_reConsole.SendMsg( EM_SETLANGOPTIONS, 0, dwLangOpt );
    }
//#endif

    // normal text color
    CHARFORMAT cf;
    cf.cbSize = sizeof(CHARFORMAT);
    cf.dwMask = CFM_COLOR | CFM_FACE;
    cf.dwEffects = 0;
    // default character format
    m_reConsole.SendMsg( EM_GETCHARFORMAT, SCF_ALL, (LPARAM) &cf );
    if ( cf.dwEffects & CFE_AUTOCOLOR )  
        cf.dwEffects -= CFE_AUTOCOLOR; // removing CFE_AUTOCOLOR
    cf.dwMask = CFM_COLOR | CFM_FACE;
    cf.crTextColor = getCurrentColorTextNorm();  // set our color
    m_reConsole.SendMsg( EM_SETCHARFORMAT, SCF_ALL, (LPARAM) &cf );
    // set character format explicitly
    RestoreDefaultTextStyle(false);
}

//-------------------------------------------------------------------------
