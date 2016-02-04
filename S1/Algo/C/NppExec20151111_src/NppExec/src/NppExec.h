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
 * NppExec History:
 ****************************************************************************


 v0.5.9.9 dev
 -------------
 dev version - see "NppExec.txt"


 v0.5.3 - February 2015
 ----------------------
 + new menu items "Go to next error" and "Go to previous error" (based on
   the HighLight masks), thanks to mrmixer
 + now %FILE% is treated as both %ABSFILE% and %FILE%, so there's no need
   to duplicate the masks with %ABSFILE% and %FILE%
 + maximum number of user menu items increased to 100 (was 40)
 + when npes_saved.txt is saved, its previous version is kept as .bak
 + TAB navigation works in Filter/Replace/HighLight dialog
 + now the INPUTBOX command can accept 3 parameters
 + NppExec Manual updated
 + other small improvements


 v0.5.2.2 - December 2014
 ------------------------
 + now the IF-GOTO statement supports floating-point comparison as well
   (e.g. IF 2.5 < 10.1 GOTO Label2)
 + new advanced option "Console_NulChar" (enhancing v0.5.2.1)
 * 8 years since NppExec was born! :)


 v0.5.2.1 - November 2014
 ------------------------
 + Now NppExec is able to deal with NUL ('\0') character when external 
   application outputs it to the Console:
   (Unicode) 0x0000 "NUL" is replaced with 0x25E6 "White Bullet";
   (ANSI) 0x00 "NUL" is replaced with 0x17 "End of Text Block".
 + NppExec Manual updated a little bit.
 * The "Donate..." button has been removed. Since I did not receive any 
   donation - either due to ukrainian politics/restrictions as for PayPal
   or due to lack of donations itself - and since this situation is unlikely
   to change, the button has been removed.
   You can always donate to Notepad++ itself.


 v0.5.2 - January 2014
 ---------------------
 + set local <var> = <value>
 + NppExec's options are now automatically saved on NPPN_SHUTDOWN
 + Ctrl+Z sends ^Z (end-of-file) to current child process
 + new advanced option "Key_Enter", by default its value is "\n"
 + new advanced option "LogsDir" to create NppExec's log files
 + NPE_CONSOLE k0..3 to enable/disable NppExec's shortcut keys in the Console
 + NppExec Manual updated
 + different internal improvements & refactoring
 + fparser updated to v4.5.1
 * 7th anniversary of NppExec :)


 v0.5 - April 2013
 -----------------
 1. Scripting abilities enhanced:
    + new commands: IF, GOTO
    + set <var> ~ strupper/strlower/substr <string>
    + set <var> ~ strfind/strrfind <string>
    + set <var> ~ strreplace <string>
 2. Highlighting abilities enhanced:
    + now all the Console Filters support run-time $(vars)
    + new commands: CON_COLOUR, CON_FILTER
 3. New/enhanced commands:
    + new command: NPP_MENUCOMMAND
    + now sel_settext+ supports '\r'
 4. Fixed:
    - fixed: could not get/modify Notepad++'s main menu when it was hidden
 5. Other:
    + new Console's commands: MANUAL, ABOUT
    * different internal improvements


 v0.4.3.2 - November 2012
 ------------------------
 - fixed: partial file name matching in npp_exec, npp_close, npp_save and
   npp_switch (CNppExec::nppConvertToFullPathName, nppSwitchToDocument)
 + HELP ALL to show all available help (all the commands in detail)
 + @[] in CScriptEngine::doSendMsg


 v0.4.3.1 - September 2012
 -------------------------
 - fixed: "Follow $(CURRENT_DIRECTORY)" did not work on file save
 + NPE_CONSOLE f+/f- r+/r- to enable/disable the console output filters


 v0.4.3 - May 2012
 -----------------
 + now NppExec's Console keeps its shown/hidden state after Notepad++ restarts
   (* thanks to ufo-pu55y for help)
 + NPP_CONSOLE +/- to enable/disable output to the Console
 + some refactoring of NppExec's Console functions


 v0.4.2.1 - March 2012
 ---------------------
 - fixed: incorrect check of Notepad++ version


 v0.4.2 - November 2011
 ----------------------
 - fixed: $("var") was processed incorrectly
 + now npe_cmdalias uses run-time $(vars)
 + set <var> ~ strlen <string>
 + set <var> ~ strlenutf8 <string>
 + $(EXITCODE)
 + npp_sendmsgex, [] in CScriptEngine::doSendMsg
 + %CHAR% in WarningAnalyzer, thanks to Carlos (cabanela)
 + fparser updated to v4.4.3
 + NppExec Manual updated (one step closer to enlightenment :))


 v0.4.1 - July 2010
 ------------------
 - fix for NPP_SENDMSG/SCI_SENDMSG with parameters containing dbl-quote(s)
 + fparser updated to v4.2
 + NppExec Manual updated


 v0.4 - April 2010
 -----------------
 + new commands: NPP_SENDMSG, SCI_SENDMSG
 + SET command extension: set <var> ~ <math expression>
   (using Function Parser by Juha Nieminen, Joel Yliluoma)
 + several small improvements


 v0.3.3.1 - February 2010
 ------------------------
 - fixed: "WS_VSCROLL" was omitted for combo-boxes in IDD_ADVANCEDOPTIONS

 
 v0.3.3 - January 2010
 ---------------------
 - fix1: now "\r\n" (Enter key emulation) is sent to the pipe separately
 - fix2: n++'es unsubclassing on exit removed (might be dangerous)
 - fix3: now the Console should keep its font always (IMF_AUTOFONT turned off)
 + Help/Manual updated (sections 4.6.3 and 4.6.4 added)


 v0.3.2 - September 2009
 -----------------------
 + new feature: TAB autocompletes file paths (in the Console)
   (* thanks to ILme for initial implementation)
 + new feature: Shift+Tab (in the Console, the same as TAB but backwards)
 - fixed: 'cd \path', 'dir \path' did not jump to the root of current drive
 - fixed: on-exit script was not executed
 + new command: NPE_CMDALIAS
 + new menu item: Disable command aliases
 + new option in NPE_CONSOLE: q+/q-


 v0.3.1 - June 2009
 ------------------
 + new command: SEL_LOADFROM
 + new command: SEL_SETTEXT, SEL_SETTEXT+
 + new variable: $(OUTPUT), $(OUTPUTL), $(OUTPUT1), see NPE_CONSOLE V+
 + several small improvements


 v0.3 final - April 2009
 -----------------------
 * thanks to Greg Webb for the .chm help file (manual)
 + several small fixes
 + new command: NPE_CONSOLE


 v0.3 RC1 - January 2009
 -----------------------
 * thanks to John LaCasse for useful suggestions
 + new environment variable: $(CWD)
 + new commands: ENV_SET/ENV_UNSET
 + new menu item: Toggle Console Dialog (Ctrl+~)
 + now NPE_DEBUGLOG reports on Console double-clicked line's Highlight Filter
 + NppExecPluginMsgTester has been re-designed and updated
 + both Unicode and ANSI builds are supported
 + now the "Execute..." dialog is resizable
 + now NppExec supports remapped shortcut keys (requires Notepad++ >= 5.1.4)
 + other improvements
 + now NppExec has the manual (yes!)

 
 v0.3 beta1 - November 2008
 --------------------------
 + full Unicode compatibility (for Unicode version of Notepad++)
 + additional functionality
 + improvements in the script engine (architecture re-design)
 + improvements in the warning analyzer (spaces/tabs can be omitted)
 + other improvements and fixes
 + now NppExec gets its configuration directory from Notepad++
 + now you can exit a console process or interrupt current script at any time
 + now both Console Output and Input can be ANSI, OEM or UTF-8
 + now each copy of NppExec.dll has its own Console History
 + new option: Follow $(CURRENT_DIRECTORY)
 + new command: NPP_CONSOLE
 + new environment variable: $(PLUGINS_CONFIG_DIR)

 
 v0.2.5 release 2 - September 2008
 ---------------------------------
 - fixed: (potential) bug in CNppExec::DoExecScript()
 - fixed: several (potential) bugs, thanks to Patrick Corwin
 
 
 v0.2.5 - August 2008
 --------------------
 + added: word wrap in the Console dialog (Ctrl+W)
 * changed: Advanced Options dialog - SaveCmdHistory added, 
   RichEdit_MaxTextLength removed
 - fixed: script with spaces in its name was not started using
   associated menu item
 + some improvements
 + new command: NPP_CLOSE
 + new command: INPUTBOX
 * now NppExec uses Rich Edit 2.0 (instead of 1.0) in its Console window
 + added Notepad++'s variables: $(CURRENT_LINE), $(CURRENT_COLUMN)
 + external plugin interface messages:
   NPEM_GETVERDWORD [returns version as DWORD]
   NPEM_GETVERSTR [returns version as string]
   NPEM_GETSTATE [returns state: ready/busy]
   NPEM_PRINT [prints text which can be highlighted by the Highlight Filters]
   NPEM_EXECUTE [executes given commands]
   NPEM_NPPEXEC [executes script/file]
   NPEN_RESULT [as a result of NPEM_EXECUTE or NPEM_NPPEXEC]
 
 
 v0.2.4 - June 2008
 ------------------
 + added: "Replace" tab in the Console Filters dialog
 * improved: the code of the DlgConsoleOutputFilter has been rewritten
 + added: Console text and background colours can be changed
 + new command: SEL_SAVETO - saves the selected text to a file
 - fixed: CON_SAVETO did not work under Windows 9x
 + added: ability to execute specified script when Notepad++ starts/exits
 + added: ability to assign a menu item to specified script
   (then you can assign a shortcut key to this menu item in Notepad++)
 + added: Advanced Options dialog
 + other improvements

 
 v0.2 RC3.2 - February 2008
 --------------------------
 - fixed: NPP_RUN (ShellExecute) did not work with several arguments


 v0.2 RC3.1 - February 2008
 --------------------------
 - fixed: buffer overflow in the Warning Analyzer
 * changed: now NPP_RUN calls ShellExecute() instead of WinExec()

 
 v0.2 RC3 - January 2008
 -----------------------
 + search in the Console
 + history in the Console Filters dialog
 + several issues fixed
 + several cosmetic improvements

 
 v0.2 RC2 - December 2007
 ------------------------
 + speeding up the console output
 + watching for changes in "npes_saved.txt"
 + TAB-key in the console auto-completes current command
 + several cosmetic improvements


 v0.2 RC1 - December 2007
 ------------------------
 1. A lot of internal changes/improvements.
 In particular, now all plugin's options are stored, read and written by the
 Options Manager component.
 The Warning Analyzer component, developed by Nicolas Babled, allows to filter
 and recognize different compiler/interpretter error and warning messages - and
 extract useful information from these messages (such as file name and line 
 number).
 The Simple Logger component stores detailed information about last executed
 command (can be usefull for "debugging").
 * The console redirecting engine has been improved.
 * Now the plugin identifies itself by its dll-file name. Each copy of the 
 plugin (each dll-file) stores its options in corresponding ini-file.
 * Now plugin's environment variables (e.g. "$(FILE_NAME)") are available for
 child console process. For example, start "cmd.exe" and type "echo $(#1)".
 * Now system's environment variables are available in a form of "$(SYS.<var>)"
 e.g. "$(SYS.WINDIR)", "$(SYS.TEMP)" etc.
 * Now the NPP_EXEC command can pass arguments to the script. These arguments 
 can be accessed in the script through the following macro-variables:
 $(ARGC), $(ARGV), $(ARGV[n]).
 Example:  npp_exec "script name" param_1 "param 2".
 This example sets the following values for the script:
 $(ARGC) = 3;  $(ARGV) = param_1 "param 2";  $(ARGV[0]) = script name;
 $(ARGV[1]) = param_1;  $(ARGV[2]) = param 2.
 * Each command now have detailed help information. Type "help" or "help help" 
 in the NppExec's Console.
 * Now the console commands history is restored when you restart Notepad++.

 2. New menu items:
 (Thanks to Nicolas Babled & Geert Vancompernolle)
 Direct Execute Previous - directly executes previous script.
 No internal messages - do not show such messages as "Process started >>>", etc.
 Save all files on execute - saves all files before executing a script.
 Also NppExec's toolbar icon has been added.

 3. New dialog:  Console Output Filter -> Highlight.
 (Thanks to Nicolas Babled & Geert Vancompernolle)
 This dialog allows you to specify a string mask for compiler/interpretter error 
 and warning messages with such keywords as %FILE%, %ABSFILE% and %LINE%.
 These messages can be highlighted with different colours and allow you to go
 to certain line in certain file when double-clicked in the Console window.

 4. New messages:
 NPP_SAVEALL - saves all modified files.
 CON_LOADFROM - loads a file's content to the Console.
 CON_SAVETO - saves the Console's content to a file.
 ECHO - prints a text in the Console (supports internal env. variables)
 SET - sets the value of user's variable
 UNSET - removes user's variable


 v0.2 beta4 - July 2007
 ----------------------
 - fixed: buffer size was not checked before calling OemToChar() 
 - fixed/added: '\b' and '\r' handling
 + a lot of changes in plugin's internals (basic cpp-classes and NppExecEngine)
 + new command: cd, cd <path>, cd <drive:\path>
 + new command: dir, dir <mask>, dir <path\mask>
 + new feature: npp_open <mask>, npp_open <path\mask>
 + new environment variables: $(LEFT_VIEW_FILE) and $(RIGHT_VIEW_FILE)
 

 v0.2 beta3 - April 2007
 -----------------------
 - fixed: Delete button (Script name dialog) was not enabled sometimes
 + Menu: Show Console Dialog
 + Console Output Filter: Exclude all empty lines
 + Comment delimiter can be user-defined, // in :// is no more comment

 
 v0.2 beta2 - March 2007
 -----------------------
 + Menu: Console Commands History.
     When enabled, previous commands can be scrolled by pressing 
     arrow keys Up and Down.
 + Menu: Console Output Filter.
     When enabled, output messages from running console process
     can be filtered (i.e. particular lines can be excluded)
 - ConsoleDlg: Ctrl+A is unlocked
 - ConsoleDlg: Quotes "" are no more added automatically for paths
     with spaces (because of a bug with executables w/o extension
     such as "cmd /c calc.exe")
 - Several internal fixes
 * Thanks to Jim Granville for his suggestions

    
 v0.1 - March 2007
 -----------------
 * initial version
 

 ****************************************************************************
 */

#ifndef _npp_exec_h_
#define _npp_exec_h_
//--------------------------------------------------------------------
#include "base.h"

#include "PluginInterface.h"
#include "CAnyWindow.h"
#include "CAnyRichEdit.h"
#include "cpp/CListT.h"
#include "cpp/CStrT.h"
#include "cpp/CBufT.h"
#include "cpp/StrSplitT.h"
#include "NppScriptList.h"
#include "CSimpleLogger.h"
#include "DlgConsoleOutputFilter.h"
#include "CStaticOptionsManager.h"
#include "WarningAnalyzer.h"
#include "PluginCommunication/NppExecPluginInterface.h"
#include <map>

#define NPPEXEC_VER_DWORD 0x05F9
#define NPPEXEC_VER_STR   _T("0.5.9.9")

#define FILEPATH_BUFSIZE       500
#define CONSOLECOMMAND_BUFSIZE 2000
#define OUTPUTFILTER_BUFSIZE   200
#define CONSOLEPIPE_BUFSIZE    8000
#define MAX_USERMENU_ITEMS     100    // OPTS_USERMENU_ITEMxx, pUserMenuFuncs

#define DEFAULT_ALIAS_CMD_NPPEXEC _T("\\")
#define DEFAULT_ALIAS_CMD_LABEL   _T(':')
#define DEFAULT_KEY_ENTER         _T("\\n")

const COLORREF COLOR_CON_TEXTNORM = RGB(0x00, 0x00, 0x00); // black
const COLORREF COLOR_CON_TEXTERR  = RGB(0xA0, 0x10, 0x10); // ~red
const COLORREF COLOR_CON_TEXTMSG  = RGB(0x20, 0x80, 0x20); // ~green
const COLORREF COLOR_CON_BKGND    = 0xFFFFFFFF; // means system default

typedef CStrT<TCHAR> tstr;

enum enumNFuncItems {
  N_DO_EXEC = 0,
  //#ifdef _NIB
  N_DIRECT_EXEC,
  //#endif
  N_SHOWCONSOLE,
  N_TOGGLECONSOLE,
  N_GOTO_NEXT_ERROR,
  N_GOTO_PREV_ERROR,
  N_SEPARATOR_1,
  N_CMDHISTORY,
  N_CONSOLE_ENC,

#ifdef _SCROLL_TO_LATEST  
  N_SCROLL2LATEST,
#endif

  N_NOINTMSGS,
  //#ifdef _GVC
  N_SAVEONEXECUTE,
  //#endif
  N_CDCURDIR,
  N_NOCMDALIASES,
  N_SEPARATOR_2,
  N_OUTPUT_FILTER,
  N_ADV_OPTIONS,
  N_CONSOLE_FONT,
  N_SEPARATOR_3,
  N_HELP_MANUAL,
  N_HELP_DOCS,
  N_HELP_ABOUT,
  nbFunc
};

enum EPluginOptions {
    OPTS_PLUGIN_HOTKEY = 0,
    OPTU_PLUGIN_HOTKEY,
    OPTB_NPE_DEBUGLOG,
    OPTI_TOOLBARBTN,
    OPTS_SCRIPT_NPPSTART,
    OPTS_SCRIPT_NPPEXIT,
    OPTU_CHILDP_STARTUPTIMEOUT_MS,
    OPTU_CHILDP_CYCLETIMEOUT_MS,
    OPTU_CHILDP_EXITTIMEOUT_MS,
    OPTB_PATH_AUTODBLQUOTES,
    OPTI_CMDHISTORY_MAXITEMS,
    OPTI_EXEC_MAXCOUNT,
    OPTI_GOTO_MAXCOUNT,
    OPTS_ALIAS_CMD_NPPEXEC,
    OPTS_KEY_ENTER,
    OPTD_CONSOLE_NULCHAR,
    OPTI_RICHEDIT_MAXTEXTLEN,
    OPTI_SENDMSG_MAXBUFLEN,
    OPTS_CALC_PRECISION,
    OPTS_COMMENTDELIMITER,
    OPTI_CONSOLE_VISIBLE,
    OPTB_CONSOLE_SHOWHELP,
    OPTU_CONSOLE_ENCODING,
    OPTB_CONSOLE_WORDWRAP,
    OPTB_CONSOLE_HIDETOGGLED,
    //#ifdef _GVC
    OPTB_CONSOLE_SAVEONEXECUTE,
    //#endif
    OPTB_CONSOLE_CDCURDIR,
    OPTB_CONSOLE_CMDHISTORY,
    OPTB_CONSOLE_SAVECMDHISTORY,

  #ifdef _SCROLL_TO_LATEST  
    OPTB_CONSOLE_SCROLL2LATEST,
  #endif

    OPTB_CONSOLE_NOINTMSGS,
    OPTB_CONSOLE_NOEMPTYVARS,
    OPTB_CONSOLE_NOCMDALIASES,
    OPTD_CONSOLE_FONT,
    OPTB_CONSOLE_APPENDMODE,
    OPTU_CONSOLE_CATCHSHORTCUTKEYS,
    OPTB_CONSOLE_SETOUTPUTVAR,
    OPTB_CONFLTR_ENABLE,
    OPTB_CONFLTR_EXCLALLEMPTY,
    OPTB_CONFLTR_EXCLDUPEMPTY,
    OPTI_CONFLTR_INCLMASK,
    OPTI_CONFLTR_EXCLMASK,
    OPTS_CONFLTR_INCLLINE1,
    OPTS_CONFLTR_INCLLINE2, // = OPTS_CONFLTR_INCLLINE1 + 1,
    OPTS_CONFLTR_INCLLINE3, // = OPTS_CONFLTR_INCLLINE1 + 2,
    OPTS_CONFLTR_INCLLINE4, // = OPTS_CONFLTR_INCLLINE1 + 3,
    OPTS_CONFLTR_INCLLINE5, // = OPTS_CONFLTR_INCLLINE1 + 4,
    OPTS_CONFLTR_INCLLINE6, // = OPTS_CONFLTR_INCLLINE1 + 5,
    OPTS_CONFLTR_INCLLINE7, // = OPTS_CONFLTR_INCLLINE1 + 6,
    OPTS_CONFLTR_INCLLINE8, // = OPTS_CONFLTR_INCLLINE1 + 7,
    OPTS_CONFLTR_EXCLLINE1,  
    OPTS_CONFLTR_EXCLLINE2, // = OPTS_CONFLTR_EXCLLINE1 + 1,
    OPTS_CONFLTR_EXCLLINE3, // = OPTS_CONFLTR_EXCLLINE1 + 2,
    OPTS_CONFLTR_EXCLLINE4, // = OPTS_CONFLTR_EXCLLINE1 + 3,
    OPTS_CONFLTR_EXCLLINE5, // = OPTS_CONFLTR_EXCLLINE1 + 4,
    OPTS_CONFLTR_EXCLLINE6, // = OPTS_CONFLTR_EXCLLINE1 + 5,
    OPTS_CONFLTR_EXCLLINE7, // = OPTS_CONFLTR_EXCLLINE1 + 6,
    OPTS_CONFLTR_EXCLLINE8, // = OPTS_CONFLTR_EXCLLINE1 + 7,
    OPTB_CONFLTR_R_ENABLE,
    OPTB_CONFLTR_R_EXCLEMPTY,
    OPTI_CONFLTR_R_FINDMASK,
    OPTI_CONFLTR_R_CASEMASK,
    OPTS_CONFLTR_R_FIND1,
    OPTS_CONFLTR_R_FIND2,
    OPTS_CONFLTR_R_FIND3,
    OPTS_CONFLTR_R_FIND4,
    OPTS_CONFLTR_R_FIND5,
    OPTS_CONFLTR_R_FIND6,
    OPTS_CONFLTR_R_FIND7,
    OPTS_CONFLTR_R_FIND8,
    OPTS_CONFLTR_R_RPLC1,
    OPTS_CONFLTR_R_RPLC2,
    OPTS_CONFLTR_R_RPLC3,
    OPTS_CONFLTR_R_RPLC4,
    OPTS_CONFLTR_R_RPLC5,
    OPTS_CONFLTR_R_RPLC6,
    OPTS_CONFLTR_R_RPLC7,
    OPTS_CONFLTR_R_RPLC8,
    //#ifdef _NIB    
    OPTS_CONFLTR_RCGNMSK1,
    OPTS_CONFLTR_RCGNMSK2,  // = OPTS_CONFLTR_RCGNMSK1 + 1,
    OPTS_CONFLTR_RCGNMSK3,  // = OPTS_CONFLTR_RCGNMSK1 + 2,
    OPTS_CONFLTR_RCGNMSK4,  // = OPTS_CONFLTR_RCGNMSK1 + 3,
    OPTS_CONFLTR_RCGNMSK5,  // = OPTS_CONFLTR_RCGNMSK1 + 4,
    OPTS_CONFLTR_RCGNMSK6,  // = OPTS_CONFLTR_RCGNMSK1 + 5,
    OPTS_CONFLTR_RCGNMSK7,  // = OPTS_CONFLTR_RCGNMSK1 + 6,
    OPTS_CONFLTR_RCGNMSK8,  // = OPTS_CONFLTR_RCGNMSK1 + 7,
    OPTS_CONFLTR_RCGNMSK9,  // = OPTS_CONFLTR_RCGNMSK1 + 8,
    OPTS_CONFLTR_RCGNMSK10, // = OPTS_CONFLTR_RCGNMSK1 + 9,
    OPTS_CONFLTR_RCGNEFF1,
    OPTS_CONFLTR_RCGNEFF2,  // = OPTS_CONFLTR_RCGNEFF1 + 1,
    OPTS_CONFLTR_RCGNEFF3,  // = OPTS_CONFLTR_RCGNEFF1 + 2,
    OPTS_CONFLTR_RCGNEFF4,  // = OPTS_CONFLTR_RCGNEFF1 + 3,
    OPTS_CONFLTR_RCGNEFF5,  // = OPTS_CONFLTR_RCGNEFF1 + 4,
    OPTS_CONFLTR_RCGNEFF6,  // = OPTS_CONFLTR_RCGNEFF1 + 5,
    OPTS_CONFLTR_RCGNEFF7,  // = OPTS_CONFLTR_RCGNEFF1 + 6,
    OPTS_CONFLTR_RCGNEFF8,  // = OPTS_CONFLTR_RCGNEFF1 + 7,
    OPTS_CONFLTR_RCGNEFF9,  // = OPTS_CONFLTR_RCGNEFF1 + 8,
    OPTS_CONFLTR_RCGNEFF10, // = OPTS_CONFLTR_RCGNEFF1 + 9,
    //#endif
    OPTB_WATCHSCRIPTFILE,

    OPTS_FILTERS_EXCL1,
    OPTS_FILTERS_EXCL2,
    OPTS_FILTERS_EXCL3,
    OPTS_FILTERS_EXCL4,
    OPTS_FILTERS_EXCL5,
    OPTS_FILTERS_EXCL6,
    OPTS_FILTERS_EXCL7,
    OPTS_FILTERS_EXCL8,
    OPTS_FILTERS_EXCL9,
    OPTS_FILTERS_EXCL10,
    OPTS_FILTERS_EXCL11,
    OPTS_FILTERS_EXCL12,
    OPTS_FILTERS_EXCL13,
    OPTS_FILTERS_EXCL14,
    OPTS_FILTERS_EXCL15,
    OPTS_FILTERS_EXCL16,
    OPTS_FILTERS_EXCL17,
    OPTS_FILTERS_EXCL18,
    OPTS_FILTERS_EXCL19,
    OPTS_FILTERS_EXCL20,

    OPTS_FILTERS_INCL1,
    OPTS_FILTERS_INCL2,
    OPTS_FILTERS_INCL3,
    OPTS_FILTERS_INCL4,
    OPTS_FILTERS_INCL5,
    OPTS_FILTERS_INCL6,
    OPTS_FILTERS_INCL7,
    OPTS_FILTERS_INCL8,
    OPTS_FILTERS_INCL9,
    OPTS_FILTERS_INCL10,
    OPTS_FILTERS_INCL11,
    OPTS_FILTERS_INCL12,
    OPTS_FILTERS_INCL13,
    OPTS_FILTERS_INCL14,
    OPTS_FILTERS_INCL15,
    OPTS_FILTERS_INCL16,
    OPTS_FILTERS_INCL17,
    OPTS_FILTERS_INCL18,
    OPTS_FILTERS_INCL19,
    OPTS_FILTERS_INCL20,

    OPTS_FILTERS_R_FIND1,
    OPTS_FILTERS_R_FIND2,
    OPTS_FILTERS_R_FIND3,
    OPTS_FILTERS_R_FIND4,
    OPTS_FILTERS_R_FIND5,
    OPTS_FILTERS_R_FIND6,
    OPTS_FILTERS_R_FIND7,
    OPTS_FILTERS_R_FIND8,
    OPTS_FILTERS_R_FIND9,
    OPTS_FILTERS_R_FIND10,
    OPTS_FILTERS_R_FIND11,
    OPTS_FILTERS_R_FIND12,
    OPTS_FILTERS_R_FIND13,
    OPTS_FILTERS_R_FIND14,
    OPTS_FILTERS_R_FIND15,
    OPTS_FILTERS_R_FIND16,
    OPTS_FILTERS_R_FIND17,
    OPTS_FILTERS_R_FIND18,
    OPTS_FILTERS_R_FIND19,
    OPTS_FILTERS_R_FIND20,
    
    OPTS_FILTERS_R_RPLC1,
    OPTS_FILTERS_R_RPLC2,
    OPTS_FILTERS_R_RPLC3,
    OPTS_FILTERS_R_RPLC4,
    OPTS_FILTERS_R_RPLC5,
    OPTS_FILTERS_R_RPLC6,
    OPTS_FILTERS_R_RPLC7,
    OPTS_FILTERS_R_RPLC8,
    OPTS_FILTERS_R_RPLC9,
    OPTS_FILTERS_R_RPLC10,
    OPTS_FILTERS_R_RPLC11,
    OPTS_FILTERS_R_RPLC12,
    OPTS_FILTERS_R_RPLC13,
    OPTS_FILTERS_R_RPLC14,
    OPTS_FILTERS_R_RPLC15,
    OPTS_FILTERS_R_RPLC16,
    OPTS_FILTERS_R_RPLC17,
    OPTS_FILTERS_R_RPLC18,
    OPTS_FILTERS_R_RPLC19,
    OPTS_FILTERS_R_RPLC20,
    
    OPTS_FILTERS_HGLT1,
    OPTS_FILTERS_HGLT2,
    OPTS_FILTERS_HGLT3,
    OPTS_FILTERS_HGLT4,
    OPTS_FILTERS_HGLT5,
    OPTS_FILTERS_HGLT6,
    OPTS_FILTERS_HGLT7,
    OPTS_FILTERS_HGLT8,
    OPTS_FILTERS_HGLT9,
    OPTS_FILTERS_HGLT10,
    OPTS_FILTERS_HGLT11,
    OPTS_FILTERS_HGLT12,
    OPTS_FILTERS_HGLT13,
    OPTS_FILTERS_HGLT14,
    OPTS_FILTERS_HGLT15,
    OPTS_FILTERS_HGLT16,
    OPTS_FILTERS_HGLT17,
    OPTS_FILTERS_HGLT18,
    OPTS_FILTERS_HGLT19,
    OPTS_FILTERS_HGLT20,

#ifdef _GVCRESTORE
    OPTI_SELECTED_SCRIPT,
#endif

    OPTD_COLOR_TEXTNORM,
    OPTD_COLOR_TEXTERR,
    OPTD_COLOR_TEXTMSG,
    OPTD_COLOR_BKGND,

    OPTB_USERMENU_NPPMACROS,
    OPTS_USERMENU_ITEM01,
    OPTS_USERMENU_ITEM02,
    OPTS_USERMENU_ITEM03,
    OPTS_USERMENU_ITEM04,
    OPTS_USERMENU_ITEM05,
    OPTS_USERMENU_ITEM06,
    OPTS_USERMENU_ITEM07,
    OPTS_USERMENU_ITEM08,
    OPTS_USERMENU_ITEM09,
    OPTS_USERMENU_ITEM10,
    OPTS_USERMENU_ITEM11,
    OPTS_USERMENU_ITEM12,
    OPTS_USERMENU_ITEM13,
    OPTS_USERMENU_ITEM14,
    OPTS_USERMENU_ITEM15,
    OPTS_USERMENU_ITEM16,
    OPTS_USERMENU_ITEM17,
    OPTS_USERMENU_ITEM18,
    OPTS_USERMENU_ITEM19,
    OPTS_USERMENU_ITEM20,
    OPTS_USERMENU_ITEM21,
    OPTS_USERMENU_ITEM22,
    OPTS_USERMENU_ITEM23,
    OPTS_USERMENU_ITEM24,
    OPTS_USERMENU_ITEM25,
    OPTS_USERMENU_ITEM26,
    OPTS_USERMENU_ITEM27,
    OPTS_USERMENU_ITEM28,
    OPTS_USERMENU_ITEM29,
    OPTS_USERMENU_ITEM30,
    OPTS_USERMENU_ITEM31,
    OPTS_USERMENU_ITEM32,
    OPTS_USERMENU_ITEM33,
    OPTS_USERMENU_ITEM34,
    OPTS_USERMENU_ITEM35,
    OPTS_USERMENU_ITEM36,
    OPTS_USERMENU_ITEM37,
    OPTS_USERMENU_ITEM38,
    OPTS_USERMENU_ITEM39,
    OPTS_USERMENU_ITEM40,
    OPTS_USERMENU_ITEM41,
    OPTS_USERMENU_ITEM42,
    OPTS_USERMENU_ITEM43,
    OPTS_USERMENU_ITEM44,
    OPTS_USERMENU_ITEM45,
    OPTS_USERMENU_ITEM46,
    OPTS_USERMENU_ITEM47,
    OPTS_USERMENU_ITEM48,
    OPTS_USERMENU_ITEM49,
    OPTS_USERMENU_ITEM50,
    OPTS_USERMENU_ITEM51,
    OPTS_USERMENU_ITEM52,
    OPTS_USERMENU_ITEM53,
    OPTS_USERMENU_ITEM54,
    OPTS_USERMENU_ITEM55,
    OPTS_USERMENU_ITEM56,
    OPTS_USERMENU_ITEM57,
    OPTS_USERMENU_ITEM58,
    OPTS_USERMENU_ITEM59,
    OPTS_USERMENU_ITEM60,
    OPTS_USERMENU_ITEM61,
    OPTS_USERMENU_ITEM62,
    OPTS_USERMENU_ITEM63,
    OPTS_USERMENU_ITEM64,
    OPTS_USERMENU_ITEM65,
    OPTS_USERMENU_ITEM66,
    OPTS_USERMENU_ITEM67,
    OPTS_USERMENU_ITEM68,
    OPTS_USERMENU_ITEM69,
    OPTS_USERMENU_ITEM70,
    OPTS_USERMENU_ITEM71,
    OPTS_USERMENU_ITEM72,
    OPTS_USERMENU_ITEM73,
    OPTS_USERMENU_ITEM74,
    OPTS_USERMENU_ITEM75,
    OPTS_USERMENU_ITEM76,
    OPTS_USERMENU_ITEM77,
    OPTS_USERMENU_ITEM78,
    OPTS_USERMENU_ITEM79,
    OPTS_USERMENU_ITEM80,
    OPTS_USERMENU_ITEM81,
    OPTS_USERMENU_ITEM82,
    OPTS_USERMENU_ITEM83,
    OPTS_USERMENU_ITEM84,
    OPTS_USERMENU_ITEM85,
    OPTS_USERMENU_ITEM86,
    OPTS_USERMENU_ITEM87,
    OPTS_USERMENU_ITEM88,
    OPTS_USERMENU_ITEM89,
    OPTS_USERMENU_ITEM90,
    OPTS_USERMENU_ITEM91,
    OPTS_USERMENU_ITEM92,
    OPTS_USERMENU_ITEM93,
    OPTS_USERMENU_ITEM94,
    OPTS_USERMENU_ITEM95,
    OPTS_USERMENU_ITEM96,
    OPTS_USERMENU_ITEM97,
    OPTS_USERMENU_ITEM98,
    OPTS_USERMENU_ITEM99,
    OPTS_USERMENU_ITEM100,

    OPTS_INPUTBOX_VALUE1,
    OPTS_INPUTBOX_VALUE2,
    OPTS_INPUTBOX_VALUE3,
    OPTS_INPUTBOX_VALUE4,
    OPTS_INPUTBOX_VALUE5,
    OPTS_INPUTBOX_VALUE6,
    OPTS_INPUTBOX_VALUE7,
    OPTS_INPUTBOX_VALUE8,
    OPTS_INPUTBOX_VALUE9,
    OPTS_INPUTBOX_VALUE10,
    OPTS_INPUTBOX_VALUE11,
    OPTS_INPUTBOX_VALUE12,
    OPTS_INPUTBOX_VALUE13,
    OPTS_INPUTBOX_VALUE14,
    OPTS_INPUTBOX_VALUE15,
    OPTS_INPUTBOX_VALUE16,
    OPTS_INPUTBOX_VALUE17,
    OPTS_INPUTBOX_VALUE18,
    OPTS_INPUTBOX_VALUE19,
    OPTS_INPUTBOX_VALUE20,

    OPTS_EXITBOX_VALUE1,
    OPTS_EXITBOX_VALUE2,
    OPTS_EXITBOX_VALUE3,
    OPTS_EXITBOX_VALUE4,
    OPTS_EXITBOX_VALUE5,
    OPTS_EXITBOX_VALUE6,
    OPTS_EXITBOX_VALUE7,
    OPTS_EXITBOX_VALUE8,
    OPTS_EXITBOX_VALUE9,
    OPTS_EXITBOX_VALUE10,
    OPTS_EXITBOX_VALUE11,
    OPTS_EXITBOX_VALUE12,
    OPTS_EXITBOX_VALUE13,
    OPTS_EXITBOX_VALUE14,
    OPTS_EXITBOX_VALUE15,
    OPTS_EXITBOX_VALUE16,
    OPTS_EXITBOX_VALUE17,
    OPTS_EXITBOX_VALUE18,
    OPTS_EXITBOX_VALUE19,
    OPTS_EXITBOX_VALUE20,

    OPTS_PLUGIN_HELPFILE,
    OPTS_PLUGIN_LOGSDIR,
    OPTS_PLUGIN_SCRIPTSDIR,
    OPTU_PLUGIN_AUTOSAVE_SECONDS,

    OPT_COUNT
};

class CSuspendedScriptAction 
{
    public:
        enum eActionType {
            AT_NONE = 0,
            AT_ONDOEXEC = 1,
            AT_ONDIRECTEXEC,
            AT_DOEXECSCRIPT,
            AT_CLOSECONSOLE
        };

        enum eConsts {
            MAX_TIMEOUT_MS = 4000
        };

        typedef struct tActionParams {
            bool bCanSaveAll;
            tstr ScriptName;
            tstr ScriptArguments;
        } ActionParams;

    protected:
        bool         m_bIsActive;
        int          m_type;
        ActionParams m_params;
        UINT_PTR     m_idTimerEvent;

        static void CALLBACK SuspendedActionTimerProc(HWND, UINT, UINT_PTR, DWORD);

    public:
        CSuspendedScriptAction() : m_bIsActive(false), 
          m_type(AT_NONE), m_idTimerEvent(0) 
        {
        }

        const ActionParams& getParams() const  { return m_params; }
        UINT_PTR getTimerEventID() const  { return m_idTimerEvent; }
        int  getType() const  { return m_type; }  // eActionType
        bool isActive() const  { return m_bIsActive; }
        
        void setActive(bool bActive)
        { 
            m_bIsActive = bActive;
        }
        
        void setAction(eActionType type, bool bCanSaveAll = false, 
               LPCTSTR szScriptName = NULL, LPCTSTR szScriptArguments = NULL)
        {
            stopWaitingTimer();
            
            m_type = type;
            switch ( type )
            {
                case AT_ONDIRECTEXEC:
                    m_params.bCanSaveAll = bCanSaveAll;
                    break;

                case AT_DOEXECSCRIPT:
                    m_params.bCanSaveAll = bCanSaveAll;
                    m_params.ScriptName = szScriptName;
                    m_params.ScriptArguments = szScriptArguments;
                    break;
            }
        }

        void startWaitingTimer(unsigned int uMaxTimeout = MAX_TIMEOUT_MS)
        {
            stopWaitingTimer();
            m_idTimerEvent = ::SetTimer(NULL, 0, uMaxTimeout, 
              CSuspendedScriptAction::SuspendedActionTimerProc);
        }

        void stopWaitingTimer()
        {
            if ( m_idTimerEvent )
            {
                //BOOL bKilled = 
                ::KillTimer(NULL, m_idTimerEvent);
                m_idTimerEvent = 0;
            }
        }

};

class CNppConsoleRichEdit : public CAnyRichEdit
{
public:
    CNppConsoleRichEdit();

//  The following two functions are commented, because NppExec actually
//  expects and deals with null-terminated strings that end with '\0'
//  everywhere. It means that all the strings that come to AddLine() or
//  AddStr() are null-terminated - i.e. there's nothing after the first
//  '\0' in them.
//  And Console_ReadPipesAndOutput() is the only function that deals 
//  with '\0' characters inside of it.

//    void  AddLine(const TCHAR* cszLine, BOOL bScrollText = FALSE, 
//                  COLORREF color = RGB(0,0,0), 
//                  DWORD dwMask = CFM_EFFECTS, DWORD dwEffects = 0);

//    void  AddStr(const TCHAR* cszStr, BOOL bScrollText = FALSE, 
//                 COLORREF color = RGB(0,0,0), 
//                 DWORD dwMask = CFM_EFFECTS, DWORD dwEffects = 0);

    static TCHAR GetNulChar();
};

class CNppExecConsole
{
public:
    CNppExecConsole();

    // colors...
    COLORREF getCurrentColorTextNorm() const;
    COLORREF getCurrentColorTextMsg() const;
    COLORREF getCurrentColorTextErr() const;
    COLORREF getCurrentColorBkgnd() const;

    void setCurrentColorTextNorm(COLORREF colorTextNorm);
    void setCurrentColorTextMsg(COLORREF colorTextMsg);
    void setCurrentColorTextErr(COLORREF colorTextErr);
    void setCurrentColorBkgnd(COLORREF colorBkgnd);

    // print...
    void PrintError(LPCTSTR cszMessage, bool bLogThisMsg = true);
    void PrintMessage(LPCTSTR cszMessage, bool bIsInternalMsg = true, bool bLogThisMsg = true);
    void PrintOutput(LPCTSTR cszMessage, bool bNewLine = true, bool bLogThisMsg = true);
    void PrintStr(LPCTSTR cszStr, bool bNewLine, bool bLogThisMsg = true);
    void PrintSysError(LPCTSTR cszFunctionName, DWORD dwErrorCode, bool bLogThisMsg = true);

    void LockConsolePos(INT nPos, bool bForce = false);
    void LockConsoleEndPos(bool bForce = false);

    // get/set...
    CAnyRichEdit& GetConsoleEdit();

    HWND GetConsoleWnd() const;
    void SetConsoleWnd(HWND hWndRichEdit);

    HWND GetParentWnd() const;
    void SetParentWnd(HWND hParent);
    
    int  IsOutputEnabledN() const;
    bool IsOutputEnabled() const;
    void SetOutputEnabled(bool bEnabled);

    // other...
    void ClearText(bool bForce = false);
    BOOL IsScrollToEnd() const;
    void RestoreDefaultTextStyle(bool bLockPos);
    void UpdateColours();

protected:
    CNppConsoleRichEdit m_reConsole;
    HWND m_hParent;
    int  m_nOutputEnabled;
    COLORREF m_colorTextNorm;
    COLORREF m_colorTextMsg;
    COLORREF m_colorTextErr;
    COLORREF m_colorBkgnd;
};

class CScriptEngine;
class CNppExec;

class CNppExecMacroVars
{
public:
    typedef std::map<tstr, tstr> tMacroVars;

    CNppExecMacroVars();

    // NppExec...
    CNppExec*   GetNppExec() const;
    void        SetNppExec(CNppExec* pNppExec);

    // helpers...
    static bool IsLocalMacroVar(tstr& varName);
    static bool ContainsMacroVar(const tstr& S);
    tMacroVars& GetUserLocalMacroVars();
    tMacroVars& GetUserMacroVars();
    tMacroVars& GetCmdAliases();

    // check macro vars...
    static void CheckCmdArgs(tstr& Cmd, const CStrSplitT<TCHAR>& args);
    void        CheckCmdAliases(tstr& S);
    void        CheckNppMacroVars(tstr& S);
    void        CheckPluginMacroVars(tstr& S);
    void        CheckUserMacroVars(tstr& S, int nCmdType = 0);
    static void CheckEmptyMacroVars(tstr& S, int nCmdType = 0);
    bool        CheckAllMacroVars(tstr& S, bool useLogging);
    bool        SetUserMacroVar(tstr& varName, const tstr& varValue, bool bRemove = false, bool bLocal = false);

protected:
    class StrCalc
    {
    protected:
        enum eCalcType {
            CT_FPARSER = 0,
            CT_STRLEN,
            CT_STRLENUTF8,
            CT_STRUPPER,
            CT_STRLOWER,
            CT_SUBSTR,
            CT_STRFIND,
            CT_STRRFIND,
            CT_STRREPLACE
        };

    public:
        StrCalc(tstr& varValue, CNppExec* pNppExec);

        void Process();
        
    protected:
        void calcFParser();
        void calcStrLen();
        void calcStrCase();
        void calcSubStr();
        void calcStrFind();
        void calcStrRplc();

    protected:
        tstr& m_varValue;
        CNppExec* m_pNppExec;
        int m_calcType;
        const TCHAR* m_pVar;
        tstr m_param;
    };

protected:
    CNppExec*  m_pNppExec;
    tMacroVars m_UserLocalMacroVars0; // <-- just in case, actually the vars are inside ScriptContext
    tMacroVars m_UserMacroVars;
    tMacroVars m_CmdAliases;
};

class CNppExec {
  
private:
  TCHAR   m_szPluginPath[FILEPATH_BUFSIZE];
  TCHAR   m_szPluginDllName[100];
  TCHAR   m_szConfigPath[FILEPATH_BUFSIZE];
  TCHAR   m_szIniFilePathName[FILEPATH_BUFSIZE];

  HMODULE m_hRichEditDll;

  HANDLE  m_hStdInReadPipe;
  HANDLE  m_hStdInWritePipe; 
  HANDLE  m_hStdOutReadPipe;
  HANDLE  m_hStdOutWritePipe;

  HANDLE  m_hScriptThread;

  UINT_PTR m_idTimerAutoSave;
  bool     m_isSavingOptions;
  CRITICAL_SECTION m_csAutoSave;

  static void CALLBACK OnTimer_AutoSaveProc(HWND, UINT, UINT_PTR, DWORD);

  HWND    getCurrentScintilla(INT which);
  void    Console_ClosePipes(void);
  DWORD   Console_ReadPipesAndOutput(CStrT<char>& bufLine, 
             bool& bPrevLineEmpty, int& nPrevState, bool bOutputAll,
             bool& bDoOutputNext);
  int     findFileNameIndexInNppOpenFileNames(const tstr& fileName, 
                                              bool bGetOpenFileNames);

private:
  CNppExecConsole m_Console;
  CNppExecMacroVars m_MacroVars;
  CScriptEngine* m_pRunningScriptEngine;

public:
  toolbarIcons    m_TB_Icon;
    
  HMODULE         m_hDllModule;
  NppData         m_nppData;
  HWND            m_hFocusedWindowBeforeScriptStarted;

  bool            m_bAnotherNppExecDllExists;
      
  CListT<tstr>    m_TempScript;
  bool            m_TempScriptIsModified;

  CNppScriptList  m_ScriptList;

  int             npp_nbFiles;
  CBufT<TCHAR*>   npp_bufFileNames;
  
  CListT<tstr>    m_CmdList;

  bool         _consoleProcessBreak;
  bool         _consoleProcessIsRunning;
  bool         _consoleIsVisible;
  HANDLE       _consoleProcessHandle;
  TCHAR        _consoleCmdLine[CONSOLECOMMAND_BUFSIZE];
  TCHAR        _consoleStrToWrite[CONSOLECOMMAND_BUFSIZE];
  HANDLE       _consoleStdInWritePipe;
  HFONT        _consoleFont;

  tstr         m_strChildProcessOutput;
  int          m_nChildProcessExitCode;

  CSuspendedScriptAction m_SuspendedScriptAction;

public:
  CNppExec();
  ~CNppExec();

  bool isScriptRunning() const  { return m_hScriptThread ? true : false; }
  //HANDLE getScriptThread() const  { return m_hScriptThread; }

  int  conLoadFrom(LPCTSTR cszFile); // returns -1 if can't load
  int  conSaveTo(LPCTSTR cszFile); // returns -1 if can't save
  int  selLoadFrom(LPCTSTR cszFile); // returns -1 if can't load
  int  selSaveTo(LPTSTR szFileAndEncoding); // returns -1 if can't save
  void selSetText(LPCTSTR cszText);
  int  nppConvertToFullPathName(tstr& fileName, bool bGetOpenFileNames);
  int  nppGetMenuItemIdByName(const tstr& menuItemPathName, tstr& parsedPath, tstr& parsedSep);
  int  nppGetOpenFileNames(void);
  int  nppGetOpenFileNamesInView(int nView = PRIMARY_VIEW, int nFiles = -1);
  bool nppSwitchToDocument(const tstr& fileName, bool bGetOpenFileNames);
  void nppSaveAllFiles(void);

  BOOL CreateChildProcess(HWND hParentWnd, LPCTSTR cszCommandLine);
  BOOL CreateScriptThread(void);
  void CloseScriptThreadHandle(void);
  
  void Free(void);
  void Init(void);
  void OnCmdHistory(void);
  void OnDoExec(void);
  //#ifdef _NIB
  void OnDirectExec(bool bCanSaveAll);
  //#endif
  void OnConsoleEncoding(void);
  //#ifdef _GVC
  void OnSaveOnExecute( void );
  //#endif
  void OnCdCurDir(void);
  void OnNoInternalMsgs(void);
  void OnNoCmdAliases(void);
  void OnOutputFilter(void);
  void OnAdvancedOptions(void);
  void OnSelectConsoleFont(void);
  void OnUserMenuItem(int nItemNumber);

  void DoExecScript(LPCTSTR szScriptName, bool bCanSaveAll, LPCTSTR szScriptArguments = NULL);

#ifdef _SCROLL_TO_LATEST  
  void OnScrollToLatest(void);
#endif

  void OnShowConsoleDlg(void);
  void OnToggleConsoleDlg(void);
  void OnHelpManual(void);
  void OnHelpDocs(void);
  void OnHelpAbout(void);
  void ReadOptions(void);
  void SaveOptions(void);
  void StartAutoSaveTimer();
  void StopAutoSaveTimer();
  void SaveScripts(void);
  HWND GetScintillaHandle(void);
  HMENU GetNppMainMenu();
  void SetConsoleFont(HWND hEd, const LOGFONT* plf);
  void ShowError(LPCTSTR szMessage);
  void ShowWarning(LPCTSTR szMessage);
  INT_PTR PluginDialogBox(UINT idDlg, DLGPROC lpDlgProc);

  void ExpandToFullConfigPath(TCHAR* out_szPath, const TCHAR* cszFileName);

  void setConsoleVisible(bool consoleIsVisible);
  void showConsoleDialog(bool bCanHide);
  void verifyConsoleDialogExists();
  bool isConsoleDialogVisible();
  bool CanStartScriptOrCommand();
  void UpdateConsoleEncoding();
  bool ConsoleWriteInput(const TCHAR* szLine, bool bFFlush = false);
  
  void printConsoleHelpInfo();

  void UpdateOutputFilterMenuItem();
  void UpdateGoToErrorMenuItem();

  LPCTSTR getPluginPath() const  { return m_szPluginPath; }
  LPCTSTR getPluginDllName() const  { return m_szPluginDllName; }
  LPCTSTR getConfigPath() const  { return m_szConfigPath; }

  static void printScriptLog(const TCHAR* str, int len);
  static void printScriptString(const TCHAR* str, int len);

  CNppExecConsole& GetConsole() { return m_Console; }
  CNppExecMacroVars& GetMacroVars() { return m_MacroVars; }
  CScriptEngine* GetRunningScriptEngine() { return m_pRunningScriptEngine; }
  void SetRunningScriptEngine(CScriptEngine* pEngine) { m_pRunningScriptEngine = pEngine; }

  LRESULT SendNppMsg(UINT uMsg, WPARAM wParam = 0, LPARAM lParam = 0); // to Notepad++

};

namespace Runtime
{
    CNppExec& GetNppExec();
    CSimpleLogger& GetLogger();
    CStaticOptionsManager& GetOptions();
    CWarningAnalyzer& GetWarningAnalyzer();
};

BOOL  CreateNewThread(LPTHREAD_START_ROUTINE lpFunc, LPVOID lpParam, 
                      HANDLE* lphThread = NULL);

bool isFullPath(const tstr& path);
bool isFullPath(const TCHAR* path);
bool createDirectoryTree(const tstr& dir);
bool createDirectoryTree(const TCHAR* dir);

extern const TCHAR SCRIPTFILE_SAVED[];


//--------------------------------------------------------------------
#endif
