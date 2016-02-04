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

#include "DlgConsole.h"
#include "NppExec.h"
#include "NppExecEngine.h"
#include "PluginInterface.h"  // docking feature
#include "Docking.h"          // docking feature
#include "WarningAnalyzer.h"
#include "CSimpleLogger.h"
#include "DirFileLister.h"
#include "DlgAdvancedOptions.h"
#include <commctrl.h>

#ifndef IMF_AUTOFONT
  #define IMF_AUTOFONT 0x0002
#endif

extern int                   g_nUserMenuItems;
extern FuncItem              g_funcItem[nbFunc + MAX_USERMENU_ITEMS + 1];
extern ShortcutKey           g_funcShortcut[nbFunc + MAX_USERMENU_ITEMS + 1];

extern TCHAR PLUGIN_NAME[];
extern TCHAR PLUGIN_NAME_DLL[];
extern TCHAR CMDHISTORY_FILENAME[];

extern COLORREF g_colorBkgnd;
extern COLORREF g_colorTextNorm;

static INT  nConsoleFirstUnlockedPos = 0;

const TCHAR CONSOLE_CMD_HELP[]   = _T("HELP");   // show console commands
const TCHAR CONSOLE_CMD_VER[]    = _T("VER");    // show plugin's version
const TCHAR CONSOLE_CMD_MANUAL[] = _T("MANUAL"); // show plugin's manual
const TCHAR CONSOLE_CMD_ABOUT[]  = _T("ABOUT");  // show plugin's about

#ifdef UNICODE
const TCHAR PLUGIN_CURRENT_VER[] = NPPEXEC_VER_STR _T(" Unicode"); // see also: NppExecPluginInterface.cpp
#else
const TCHAR PLUGIN_CURRENT_VER[] = NPPEXEC_VER_STR _T(" ANSI"); // see also: NppExecPluginInterface.cpp
#endif


const TCHAR CONSOLE_COMMANDS_INFO[] = _T_RE_EOL \
  _T("-------- Console keys --------") _T_RE_EOL \
  _T("Enter  -  executes entered command") _T_RE_EOL \
  _T("Tab  -  auto-completes current command/looks through the commands history") _T_RE_EOL \
  _T("Shift+Tab  -  the same as Tab but backwards") _T_RE_EOL \
  _T("Arrow Up  -  previous command (when Console Commands History is enabled)") _T_RE_EOL \
  _T("Arrow Down  -  next command (when Console Commands History is enabled)") _T_RE_EOL \
  _T("Ctrl+C  -  copy selected text to the clipboard") _T_RE_EOL \
  _T("Ctrl+V  -  paste from the clipboard") _T_RE_EOL \
  _T("Ctrl+W  -  word wrap on/off") _T_RE_EOL \
  _T("Ctrl+~  -  toggle Console/editing window") _T_RE_EOL \
  _T("Ctrl+T  -  hide toggled Console on/off") _T_RE_EOL \
  _T("Ctrl+F, F3, F7  -  shows Console\'s Find dialog") _T_RE_EOL \
  _T("Esc  -  hides Console\'s Find dialog") _T_RE_EOL \
  _T("Enter (in Console\'s Find dialog)  -  find next") _T_RE_EOL \
  _T("-------- Console commands --------") _T_RE_EOL \
  _T("help  -  show available commands") _T_RE_EOL \
  _T("help <command>  -  show detailed information about specified command") _T_RE_EOL \
  _T("help all  -  show all available help (all the commands in detail)") _T_RE_EOL \
  _T("ver  -  show plugin\'s version") _T_RE_EOL \
  _T("manual  -  show NppExec User Guide") _T_RE_EOL \
  _T("about  - show NppExec Help/About") _T_RE_EOL \
  _T("CTRL+C  -  terminate current child process") _T_RE_EOL \
  _T("CTRL+BREAK  -  terminate current child process") _T_RE_EOL \
  _T("CTRL+Z  -  send ^Z to current child process") _T_RE_EOL \
  _T("-------- General commands --------") _T_RE_EOL \
  _T("cls  -  clear Console screen") _T_RE_EOL \
  _T("cd  -  shows current path") _T_RE_EOL \
  _T("cd <absolute_path or relative_path>  -  changes current path") _T_RE_EOL \
  _T("dir  -  lists files and subdirs") _T_RE_EOL \
  _T("dir <mask or path\\mask>  -  lists files/subdirs matched the mask") _T_RE_EOL \
  _T("echo <text>  -  prints a text in the Console") _T_RE_EOL \
  _T("if <condition> goto <label>  -  jumps to the label if the condition is true") _T_RE_EOL \
  _T("goto <label>  -  jumps to the label") _T_RE_EOL \
  _T("set  -  shows all user\'s variables") _T_RE_EOL \
  _T("set <var>  -  shows the value of user\'s variable <var>") _T_RE_EOL \
  _T("set <var> = <value>  -  sets the value of user\'s variable <var>") _T_RE_EOL \
  _T("set <var> ~ <math expression>  -  calculates the math expression") _T_RE_EOL \
  _T("set <var> ~ strlen <string>  -  calculates the string length") _T_RE_EOL \
  _T("set <var> ~ strlenutf8 <string>  -  calculates the UTF-8 string length") _T_RE_EOL \
  _T("set <var> ~ strupper <string>  -  returns the string in upper case") _T_RE_EOL \
  _T("set <var> ~ strlower <string>  -  returns the string in lower case") _T_RE_EOL \
  _T("set <var> ~ substr <pos> <len> <string>  -  returns the substring") _T_RE_EOL \
  _T("set <var> ~ strfind <s> <t>  -  returns the first position of <t> in <s>") _T_RE_EOL \
  _T("set <var> ~ strrfind <s> <t>  -  returns the last position of <t> in <s>") _T_RE_EOL \
  _T("set <var> ~ strreplace <s> <t0> <t1>  -  replaces all <t0> with <t1>") _T_RE_EOL \
  _T("set local  -  shows all user\'s local variables") _T_RE_EOL \
  _T("set local <var>  -  shows the value of user\'s local variable <var>") _T_RE_EOL \
  _T("set local <var> = ...  -  sets the value of user\'s local variable <var>") _T_RE_EOL \
  _T("set local <var> ~ ...  -  calculates the value of user\'s local variable") _T_RE_EOL \
  _T("unset <var>  -  removes user\'s variable <var>") _T_RE_EOL \
  _T("unset <var> = <value>  -  removes user\'s variable <var>") _T_RE_EOL \
  _T("unset local <var>  -  removes user\'s local variable <var>") _T_RE_EOL \
  _T("env_set <var>  -  shows the value of environment variable <var>") _T_RE_EOL \
  _T("env_set <var> = <value>  -  sets the value of environment variable <var>") _T_RE_EOL \
  _T("env_unset <var>  -  removes/restores the environment variable <var>") _T_RE_EOL \
  _T("set_env <var> = <value>  -  see \"env_set\"") _T_RE_EOL \
  _T("unset_env <var>  -  see \"env_unset\"") _T_RE_EOL \
  _T("inputbox \"message\"  -  shows InputBox, sets $(INPUT)") _T_RE_EOL \
  _T("inputbox \"message\" : initial_value  -  shows InputBox, sets $(INPUT)") _T_RE_EOL \
  _T("inputbox \"message\" : \"value_name\" : initial_value  -  InputBox customization") _T_RE_EOL \
  _T("con_colour <colours>  -  sets the Console\'s colours") _T_RE_EOL \
  _T("con_filter <filters>  -  enables/disables the Console\'s output filters") _T_RE_EOL \
  _T("con_loadfrom <file>  -  loads a file\'s content to the Console") _T_RE_EOL \
  _T("con_load <file>  -  see \"con_loadfrom\"") _T_RE_EOL \
  _T("con_saveto <file>  -  saves the Console\'s content to a file") _T_RE_EOL \
  _T("con_save <file>  -  see \"con_saveto\"") _T_RE_EOL \
  _T("sel_loadfrom <file>  -  replace current selection with the file\'s content") _T_RE_EOL \
  _T("sel_load <file>  -  see \"sel_loadfrom\"") _T_RE_EOL \
  _T("sel_saveto <file>  -  save the selected text to a file") _T_RE_EOL \
  _T("sel_saveto <file> : <encoding>  -  save the selected text to a file") _T_RE_EOL \
  _T("sel_save <file> : <encoding>  -  see \"sel_saveto\"") _T_RE_EOL \
  _T("sel_settext <text>  -  replace current selection with the text specified") _T_RE_EOL \
  _T("sel_settext+ <text>  -  replace current selection with the text specified") _T_RE_EOL \
  _T("npp_exec <script>  -  execute commands from specified script") _T_RE_EOL \
  _T("npp_exec <file>  -  execute commands from specified file") _T_RE_EOL \
  _T("npp_exec <script/file> <args>  -  passes additional arguments <args>") _T_RE_EOL \
  _T("npp_close  -  close current file in Notepad++") _T_RE_EOL \
  _T("npp_close <file>  -  close specified file opened in Notepad++") _T_RE_EOL \
  _T("npp_console <on/off/keep>  -  show/hide the Console window") _T_RE_EOL \
  _T("npp_console <enable/disable>  -  enables/disables output to the Console") _T_RE_EOL \
  _T("npp_console <1/0/?>  -  show/hide the Console window") _T_RE_EOL \
  _T("npp_console <+/->  -  enables/disables output to the Console") _T_RE_EOL \
  _T("npp_menucommand <menu\\item\\name>  -  executes (invokes) a menu item") _T_RE_EOL \
  _T("npp_open <file>  -  (re)open specified file in Notepad++") _T_RE_EOL \
  _T("npp_open <mask or path\\mask>  -  opens file(s) matched the mask") _T_RE_EOL \
  _T("npp_run <command>  -  run external process/command") _T_RE_EOL \
  _T("npp_save  -  save current file in Notepad++") _T_RE_EOL \
  _T("npp_save <file>  -  save specified file opened in Notepad++") _T_RE_EOL \
  _T("npp_saveas <file>  -  save current file with a new (path)name") _T_RE_EOL \
  _T("npp_saveall  -  save all modified files") _T_RE_EOL \
  _T("npp_switch <file>  -  switch to specified opened file") _T_RE_EOL \
  _T("npp_sendmsg <msg>  -  send a message (msg) to Notepad++") _T_RE_EOL \
  _T("npp_sendmsg <msg> <wparam>  -  message with parameter (wparam)") _T_RE_EOL \
  _T("npp_sendmsg <msg> <wparam> <lparam>  -  msg to Notepad++") _T_RE_EOL \
  _T("npp_sendmsgex <hwnd> <msg> <wparam> <lparam>  -  msg to hwnd") _T_RE_EOL \
  _T("sci_sendmsg <msg>  -  send a message (msg) to current Scintilla") _T_RE_EOL \
  _T("sci_sendmsg <msg> <wparam>  -  message with parameter (wparam)") _T_RE_EOL \
  _T("sci_sendmsg <msg> <wparam> <lparam>  -  msg to Scintilla") _T_RE_EOL \
  _T("npe_cmdalias  -  show all command aliases") _T_RE_EOL \
  _T("npe_cmdalias <alias>  -  shows the value of command alias") _T_RE_EOL \
  _T("npe_cmdalias <alias> =  -  removes the command alias") _T_RE_EOL \
  _T("npe_cmdalias <alias> = <command>  -  sets the command alias") _T_RE_EOL \
  _T("npe_console <options>  -  set/modify Console options/mode") _T_RE_EOL \
  _T("npe_debuglog <on/off>  -  enable/disable Debug Log") _T_RE_EOL \
  _T("npe_debug <1/0>  -  see \"npe_debuglog\"") _T_RE_EOL \
  _T("npe_noemptyvars <1/0>  -  enable/disable replacement of empty vars") _T_RE_EOL \
  DEFAULT_ALIAS_CMD_NPPEXEC _T("<script/file>  -  the same as  npp_exec <script/file>") _T_RE_EOL \
  DEFAULT_ALIAS_CMD_NPPEXEC _T("<script/file> <args>  -  the same as  npp_exec <script/file> <args>") _T_RE_EOL \
  _T("-------- Environment variables --------") _T_RE_EOL \
  _T("$(FULL_CURRENT_PATH)  :  E:\\my Web\\main\\welcome.html") _T_RE_EOL \
  _T("$(CURRENT_DIRECTORY)  :  E:\\my Web\\main") _T_RE_EOL \
  _T("$(FILE_NAME)  :  welcome.html") _T_RE_EOL \
  _T("$(NAME_PART)  :  welcome") _T_RE_EOL \
  _T("$(EXT_PART)  :  .html") _T_RE_EOL \
  _T("$(NPP_DIRECTORY)  :  the full path of Notepad++\'s directory") _T_RE_EOL \
  _T("$(CURRENT_WORD)  :  word(s) you selected in Notepad++") _T_RE_EOL \
  _T("$(CURRENT_LINE)  :  current line number") _T_RE_EOL \
  _T("$(CURRENT_COLUMN)  :  current column number") _T_RE_EOL \
  _T("$(#N)  :  full path of the Nth opened document (N=1,2,3...)") _T_RE_EOL \
  _T("$(#0)  :  full path to notepad++.exe") _T_RE_EOL \
  _T("$(LEFT_VIEW_FILE)  :  current file path-name in primary (left) view") _T_RE_EOL \
  _T("$(RIGHT_VIEW_FILE)  :  current file path-name in secondary (right) view") _T_RE_EOL \
  _T("$(PLUGINS_CONFIG_DIR)  :  full path of the plugins configuration directory") _T_RE_EOL \
  _T("$(CWD)  :  current working directory of NppExec (use \"cd\" to change it)") _T_RE_EOL \
  _T("$(ARGC)  :  number of arguments passed to the NPP_EXEC command") _T_RE_EOL \
  _T("$(ARGV)  :  all arguments passed to the NPP_EXEC command after the script name") _T_RE_EOL \
  _T("$(ARGV[0])  :  script name - first parameter of the NPP_EXEC command") _T_RE_EOL \
  _T("$(ARGV[N])  :  Nth argument (N=1,2,3...)") _T_RE_EOL \
  _T("$(RARGV)  :  all arguments in reverse order (except the script name)") _T_RE_EOL \
  _T("$(RARGV[N])  :  Nth argument in reverse order (N=1,2,3...)") _T_RE_EOL \
  _T("$(INPUT)  :  this value is set by the \'inputbox\' command") _T_RE_EOL \
  _T("$(INPUT[N])  :  Nth field of the $(INPUT) value (N=1,2,3...)") _T_RE_EOL \
  _T("$(OUTPUT)  :  this value can be set by the child process, see npe_console v+") _T_RE_EOL \
  _T("$(OUTPUT1)  :  first line in $(OUTPUT)") _T_RE_EOL \
  _T("$(OUTPUTL)  :  last line in $(OUTPUT)") _T_RE_EOL \
  _T("$(EXITCODE)  :  exit code of the last executed child process") _T_RE_EOL \
  _T("$(MSG_RESULT)  :  result of \'npp_sendmsg[ex]\' or \'sci_sendmsg\'") _T_RE_EOL \
  _T("$(MSG_WPARAM)  :  wParam (output) of \'npp_sendmsg[ex]\' or \'sci_sendmsg\'") _T_RE_EOL \
  _T("$(MSG_LPARAM)  :  lParam (output) of \'npp_sendmsg[ex]\' or \'sci_sendmsg\'") _T_RE_EOL \
  _T("$(SYS.<var>)  :  system\'s environment variable, e.g. $(SYS.PATH)") _T_RE_EOL \
  _T_RE_EOL;

const int CMD_COUNT = 41;
const TCHAR* CMD_ITEMS[CMD_COUNT] = {
  CONSOLE_CMD_HELP,
  CONSOLE_CMD_VER,
  CONSOLE_CMD_MANUAL,
  CONSOLE_CMD_ABOUT,
  CMD_CLS,
  CMD_CD,
  CMD_DIR,
  CMD_ECHO,
  CMD_CONCOLOUR,
  CMD_CONFILTER,
  CMD_CONLOADFROM,
  CMD_CONSAVETO,
  CMD_NPPCLOSE,
  CMD_NPPCONSOLE,
  CMD_NPPEXEC,
  CMD_NPPMENUCOMMAND,
  CMD_NPPOPEN,
  CMD_NPPRUN,
  CMD_NPPSAVE,
  CMD_NPPSAVEALL,
  CMD_NPPSAVEAS,
  CMD_NPPSWITCH,
  CMD_NPPSENDMSG,
  CMD_NPPSENDMSGEX,
  CMD_SCISENDMSG,
  _T("MASK"),
  CMD_SET,
  CMD_ENVSET,
  CMD_INPUTBOX,
  CMD_NPECMDALIAS,
  CMD_NPECONSOLE,
  CMD_NPEDEBUGLOG,
  CMD_NPENOEMPTYVARS,
  CMD_SELLOADFROM,
  CMD_SELSAVETO,
  CMD_SELSETTEXT,
  CMD_IF,
  CMD_LABEL,
  CMD_GOTO,
  CMD_ELSE,
  CMD_ENDIF
};

#ifdef UNICODE
  #define _T_MENUCOMMAND_CYRILLIC  _T("\x0424\x0430\x0439\x043B|\x041D\x043E\x0432\x044B\x0439")
  #define _T_STRLEN_CYRILLIC       _T("\x041F\x0440\x0438\x0432\x0435\x0442")
#else
  #define _T_MENUCOMMAND_CYRILLIC  _T("\xD4\xE0\xE9\xEB|\xCD\xEE\xE2\xFB\xE9")
  #define _T_STRLEN_CYRILLIC       _T("\xCF\xF0\xE8\xE2\xE5\xF2")
#endif

// in the same order as in CMD_ITEMS
const TCHAR* CONSOLE_CMD_INFO[CMD_COUNT] = {
  // HELP
  _T("COMMAND:  help") _T_RE_EOL \
  _T("USAGE:") _T_RE_EOL \
  _T("  help            (*)") _T_RE_EOL \
  _T("  help <command>  (*)") _T_RE_EOL \
  _T("  help all        (*)") _T_RE_EOL \
  _T("  * available in the Console dialog only") _T_RE_EOL \
  _T("DESCRIPTION:") _T_RE_EOL \
  _T("  Prints help ;-)") _T_RE_EOL \
  _T("EXAMPLES:") _T_RE_EOL \
  _T("  help           // prints general help information") _T_RE_EOL \
  _T("  help npp_open  // prints detailed information about the NPP_OPEN command") _T_RE_EOL \
  _T("  help all       // prints detailed information about all available commands") _T_RE_EOL \
  _T("SEE ALSO:") _T_RE_EOL \
  _T("  manual") _T_RE_EOL,

  // VER
  _T("COMMAND:  ver") _T_RE_EOL \
  _T("USAGE:") _T_RE_EOL \
  _T("  ver             (*)") _T_RE_EOL \
  _T("  * available in the Console dialog only") _T_RE_EOL \
  _T("DESCRIPTION:") _T_RE_EOL \
  _T("  Prints plugin\'s version") _T_RE_EOL,

  // MANUAL
  _T("COMMAND:  manual") _T_RE_EOL \
  _T("USAGE:") _T_RE_EOL \
  _T("  manual          (*)") _T_RE_EOL \
  _T("  * available in the Console dialog only") _T_RE_EOL \
  _T("DESCRIPTION:") _T_RE_EOL \
  _T("  Shows NppExec User Guide") _T_RE_EOL \
  _T("EXAMPLES:") _T_RE_EOL \
  _T("  manual  // shows NppExec User Guide") _T_RE_EOL \
  _T("SEE ALSO:") _T_RE_EOL \
  _T("  help") _T_RE_EOL,

  // ABOUT
  _T("COMMAND:  about") _T_RE_EOL \
  _T("USAGE:") _T_RE_EOL \
  _T("  about           (*)") _T_RE_EOL \
  _T("  * available in the Console dialog only") _T_RE_EOL \
  _T("DESCRIPTION:") _T_RE_EOL \
  _T("  Shows NppExec Help/About") _T_RE_EOL \
  _T("EXAMPLES:") _T_RE_EOL \
  _T("  about  // shows NppExec Help/About") _T_RE_EOL \
  _T("SEE ALSO:") _T_RE_EOL \
  _T("  help, manual") _T_RE_EOL,

  // CLS
  _T("COMMAND:  cls") _T_RE_EOL \
  _T("USAGE:") _T_RE_EOL \
  _T("  cls") _T_RE_EOL \
  _T("DESCRIPTION:") _T_RE_EOL \
  _T("  Clears the Console screen") _T_RE_EOL,

  // CD
  _T("COMMAND:  cd") _T_RE_EOL \
  _T("USAGE:") _T_RE_EOL \
  _T("  cd") _T_RE_EOL \
  _T("  cd <relative_path>") _T_RE_EOL \
  _T("  cd <absolute_path>") _T_RE_EOL \
  _T("DESCRIPTION:") _T_RE_EOL \
  _T("  1. Changes current directory (\"cd\" with parameters)") _T_RE_EOL \
  _T("  2. Shows current directory path (\"cd\" without parameters)") _T_RE_EOL \
  _T("EXAMPLES:") _T_RE_EOL \
  _T("  cd       // shows current directory path") _T_RE_EOL \
  _T("  cd ..    // go to up-directory") _T_RE_EOL \
  _T("  cd \\     // go to root-directory") _T_RE_EOL \
  _T("  cd $(NPP_DIRECTORY)    // go to notepad++\'s directory") _T_RE_EOL \
  _T("  cd e:\\temp            // change drive and directory") _T_RE_EOL,

  // DIR
  _T("COMMAND:  dir") _T_RE_EOL \
  _T("USAGE:") _T_RE_EOL \
  _T("  dir") _T_RE_EOL \
  _T("  dir <path>") _T_RE_EOL \
  _T("  dir <mask>") _T_RE_EOL \
  _T("  dir <path\\mask>") _T_RE_EOL \
  _T("DESCRIPTION:") _T_RE_EOL \
  _T("  1. Lists all subdirs and files in current/specified directory  (*)") _T_RE_EOL \
  _T("  2. Lists subdirs and files matched the mask                   (**)") _T_RE_EOL \
  _T("   * current directory can be set by the \"cd\" command") _T_RE_EOL \
  _T("  ** type \"help mask\" for more information about file masks") _T_RE_EOL \
  _T("EXAMPLES:") _T_RE_EOL \
  _T("  dir                 // list all subdirs and files in current directory") _T_RE_EOL \
  _T("  dir *               // the same") _T_RE_EOL \
  _T("  dir d:\\docs\\*.txt   // list all *.txt files in the folder d:\\docs") _T_RE_EOL,

  // ECHO
  _T("COMMAND:  echo") _T_RE_EOL \
  _T("USAGE:") _T_RE_EOL \
  _T("  echo <text>") _T_RE_EOL \
  _T("DESCRIPTION:") _T_RE_EOL \
  _T("  Prints a text in the Console (the environment variables are supported)") _T_RE_EOL \
  _T("EXAMPLES:") _T_RE_EOL \
  _T("  echo Notepad++\'s directory is: $(NPP_DIRECTORY)") _T_RE_EOL,

  // CON_COLOUR
  _T("COMMAND:  con_colour") _T_RE_EOL \
  _T("USAGE:") _T_RE_EOL \
  _T("  con_colour") _T_RE_EOL \
  _T("  con_colour fg = <RR GG BB>") _T_RE_EOL \
  _T("  con_colour bg = <RR GG BB>") _T_RE_EOL \
  _T("  con_colour fg = <RR GG BB> bg = <RR GG BB>") _T_RE_EOL \
  _T("  con_colour fg = 0") _T_RE_EOL \
  _T("  con_colour bg = 0") _T_RE_EOL \
  _T("  con_colour fg = 0 bg = 0") _T_RE_EOL \
  _T("DESCRIPTION:") _T_RE_EOL \
  _T("  fg - sets foreground (text) colour of the Console;") _T_RE_EOL \
  _T("  bg - sets background colour of the Console;") _T_RE_EOL \
  _T("  without parameters - shows current values of the colours;") _T_RE_EOL \
  _T("  the value of 0 restores the original colour(s).") _T_RE_EOL \
  _T("EXAMPLES:") _T_RE_EOL \
  _T("  con_colour bg=303030 fg=d0d0d0     // white text on dark-grey") _T_RE_EOL \
  _T("  con_colour fg = 303030 bg = D0D0D0 // dark text on light-grey") _T_RE_EOL \
  _T("  con_colour FG = 20 20 90           // dark-blue text") _T_RE_EOL \
  _T("  con_colour fg = 0                  // restore original text colour") _T_RE_EOL \
  _T("REMARKS:") _T_RE_EOL \
  _T("  The foreground (text) colour is applied to new text only.") _T_RE_EOL \
  _T("  The background colour is applied to the whole Console\'s background. And") _T_RE_EOL \
  _T("  when it is being applied, all the text is being re-coloured to the current") _T_RE_EOL \
  _T("  foreground colour.") _T_RE_EOL \
  _T("  <RR GG BB> are hex values of Right, Green and Blue components of a colour") _T_RE_EOL \
  _T("  to be used. Each colour component can have a value from 00 to FF. The value") _T_RE_EOL \
  _T("  of 00 means absence of the colour component, FF means maximum. Thus, ") _T_RE_EOL \
  _T("  00 00 00 means \"dark\" (absence of all colours), 00 FF 00 means \"maximum of") _T_RE_EOL \
  _T("  green\", FF FF FF means \"maximum white\".") _T_RE_EOL \
  _T("  These colours are run-time only, they are not saved when Notepad++ exits.") _T_RE_EOL \
  _T("  (Use the Advanced Options to set and save the colours.)") _T_RE_EOL \
  _T("SEE ALSO:") _T_RE_EOL \
  _T("  con_filter, npe_console") _T_RE_EOL,

  // CON_FILTER
  _T("COMMAND:  con_filter") _T_RE_EOL \
  _T("USAGE:") _T_RE_EOL \
  _T("  con_filter +x5/-x5 +i1/-i1 +fr4/-fr4 +frc1/-frc1 +h10/-h10") _T_RE_EOL \
  _T("DESCRIPTION:") _T_RE_EOL \
  _T("  Enables or disables the eXclude/Include/Replace/Highlight Filters") _T_RE_EOL \
  _T("  +x<N>/-x<N>     - enables/disables the Nth eXclude Mask (N = 1..5)") _T_RE_EOL \
  _T("  +i<N>/-i<N>     - enables/disables the Nth Include Mask (N = 1..5)") _T_RE_EOL \
  _T("  +fr<N>/-fr<N>   - enables/disables the Nth Replacing Filter and") _T_RE_EOL \
  _T("                    deactivates \"Match case\" (N = 1..4)") _T_RE_EOL \
  _T("  +frc<N>/-frc<N> - enables/disables the Nth Replacing Filter and") _T_RE_EOL \
  _T("                    activates \"Match case\" (N = 1..4)") _T_RE_EOL \
  _T("  +h<N>/-h<N>     - enables/disables the Nth Highlight Mask (N = 1..10)") _T_RE_EOL \
  _T("EXAMPLES:") _T_RE_EOL \
  _T("  con_filter +frc3 +fr2 -i5 +x2 +h1") _T_RE_EOL \
  _T("  con_filter -x4 +h2 +i1 -fr3 +i2 -h7 +x1 +fr1 +frc2") _T_RE_EOL \
  _T("REMARKS:") _T_RE_EOL \
  _T("  This command allows to enable and disable the Console Output Filters from") _T_RE_EOL \
  _T("  your script.") _T_RE_EOL \
  _T("  When any eXclude or Include mask is enabled, the \"Enable Console Output Filter\"") _T_RE_EOL \
  _T("  check-box becomes checked.") _T_RE_EOL \
  _T("  When any Replacing Filter is enabled, the \"Enable Replacing Filters\" check-box") _T_RE_EOL \
  _T("  becomes checked.") _T_RE_EOL \
  _T("  While using CON_FILTER to enable or disable the Console Output Filters and having") _T_RE_EOL \
  _T("  macro-variables as the values of these Filters, you are able to apply different") _T_RE_EOL \
  _T("  filters during execution of your script - thus having the full control over the") _T_RE_EOL \
  _T("  filters at runtime.") _T_RE_EOL \
  _T("  To illustrate the last sentence, let\'s assume you have specified the following") _T_RE_EOL \
  _T("  variables as filters in the Console Output Filters:") _T_RE_EOL \
  _T("    $(Exclude5) as the 5th Exclude mask;") _T_RE_EOL \
  _T("    $(Include5) as the 5th Include mask;") _T_RE_EOL \
  _T("    $(Find4) and $(Replace4) as the 4th Replacing Filter;") _T_RE_EOL \
  _T("    $(Highlight10) as the 10th Highlight mask.") _T_RE_EOL \
  _T("  Now you can write the following script:") _T_RE_EOL \
  _T("    // set the filters dynamically") _T_RE_EOL \
  _T("    set Exclude5 = *(C)*     // exclude any line that contains \"(C)\"") _T_RE_EOL \
  _T("    set Include5 = *         // include any line") _T_RE_EOL \
  _T("    set Find4 = /            // find all \'/\'...") _T_RE_EOL \
  _T("    set Replace4 = \\         // ...and replace with \'\\\'") _T_RE_EOL \
  _T("    set Highlight10 = error* // highlight any line that starts with \"error\"") _T_RE_EOL \
  _T("    con_filter +x5 +i5 +fr4 +h10  // enable the above filters") _T_RE_EOL \
  _T("SEE ALSO:") _T_RE_EOL \
  _T("  con_colour, npe_console, npp_console") _T_RE_EOL,

  // CON_LOADFROM
  _T("COMMAND:  con_loadfrom") _T_RE_EOL \
  _T("USAGE:") _T_RE_EOL \
  _T("  con_loadfrom <file>") _T_RE_EOL \
  _T("DESCRIPTION:") _T_RE_EOL \
  _T("  Loads a file\'s content to the Console") _T_RE_EOL \
  _T("EXAMPLES:") _T_RE_EOL \
  _T("  con_loadfrom c:\\temp\\output.txt") _T_RE_EOL \
  _T("REMARKS:") _T_RE_EOL \
  _T("  Unicode version of NppExec can read ANSI, UTF-8, UCS-2 LE and") _T_RE_EOL \
  _T("  UCS-2 BE text files") _T_RE_EOL \
  _T("SEE ALSO:") _T_RE_EOL \
  _T("  con_saveto") _T_RE_EOL, 

  // CON_SAVETO
  _T("COMMAND:  con_saveto") _T_RE_EOL \
  _T("USAGE:") _T_RE_EOL \
  _T("  con_saveto <file>") _T_RE_EOL \
  _T("DESCRIPTION:") _T_RE_EOL \
  _T("  Saves the Console\'s content to a file") _T_RE_EOL \
  _T("EXAMPLES:") _T_RE_EOL \
  _T("  con_saveto c:\\temp\\output.txt") _T_RE_EOL \
  _T("REMARKS:") _T_RE_EOL \
  _T("  Unicode version of NppExec saves the Console\'s content as") _T_RE_EOL \
  _T("  an Unicode text file (UCS-2 LE)") _T_RE_EOL \
  _T("SEE ALSO:") _T_RE_EOL \
  _T("  con_loadfrom") _T_RE_EOL,
                   
  // NPP_CLOSE
  _T("COMMAND:  npp_close") _T_RE_EOL \
  _T("USAGE:") _T_RE_EOL \
  _T("  npp_close") _T_RE_EOL \
  _T("  npp_close <file>") _T_RE_EOL \
  _T("DESCRIPTION:") _T_RE_EOL \
  _T("  1. Closes current file (\"npp_close\" without parameters)") _T_RE_EOL \
  _T("  2. Closes specified opened file") _T_RE_EOL \
  _T("     (supports a partial file path/name)") _T_RE_EOL \
  _T("EXAMPLES:") _T_RE_EOL \
  _T("  npp_close $(#3)  // closes 3rd file opened in Notepad++") _T_RE_EOL \
  _T("  npp_close awk    // closes a first file which contains \"awk\" in its name") _T_RE_EOL \
  _T("SEE ALSO:") _T_RE_EOL \
  _T("  npp_open, npp_switch") _T_RE_EOL,

  // NPP_CONSOLE
  _T("COMMAND:  npp_console") _T_RE_EOL \
  _T("USAGE:") _T_RE_EOL \
  _T("  npp_console <on/off/keep>") _T_RE_EOL \
  _T("  npp_console <enable/disable>") _T_RE_EOL \
  _T("  npp_console <1/0/?>") _T_RE_EOL \
  _T("  npp_console <+/->") _T_RE_EOL \
  _T("DESCRIPTION:") _T_RE_EOL \
  _T("  Shows/hides the Console window.") _T_RE_EOL \
  _T("  Enables/disables output to the Console.") _T_RE_EOL \
  _T("  Usually the Console window is automatically opened each time you execute") _T_RE_EOL \
  _T("  some command or script, so the purpose of this command is to change the") _T_RE_EOL \
  _T("  default behaviour.") _T_RE_EOL \
  _T("EXAMPLES:") _T_RE_EOL \
  _T("  npp_console ?  // keep the Console\'s state: hidden Console is not shown") _T_RE_EOL \
  _T("  npp_console 1  // open (show) the Console") _T_RE_EOL \
  _T("  npp_console 0  // close (hide) the Console") _T_RE_EOL \
  _T("  npp_console -  // disable output to the Console") _T_RE_EOL \
  _T("  npp_console +  // enable output to the Console") _T_RE_EOL \
  _T("REMARKS:") _T_RE_EOL \
  _T("  You can use \'NPP_CONSOLE ?\' as a first command of your script in order to") _T_RE_EOL \
  _T("  keep the Console\'s state: don\'t open hidden Console or don\'t hide opened one.") _T_RE_EOL \
  _T("  If you want to apply \'NPP_CONSOLE ?\' to every script, you don\'t have to add") _T_RE_EOL \
  _T("  this command to the beginning of each script. An auxiliary script can be created") _T_RE_EOL \
  _T("  in order to start every script using this auxiliary script:") _T_RE_EOL \
  _T("  ") _T_RE_EOL \
  _T("    // auxiliary script") _T_RE_EOL \
  _T("    NPP_CONSOLE ?  // keep the Console\'s state") _T_RE_EOL \
  _T("    NPP_EXEC $(ARGV)  // execute a script passed as the argument") _T_RE_EOL \
  _T("  ") _T_RE_EOL \
  _T("  Now you can start any script using") _T_RE_EOL \
  _T("    NPP_EXEC \"auxiliary script\" \"script to be executed\"") _T_RE_EOL \
  _T("  ") _T_RE_EOL \
  _T("  If you disable output to the Console via \'NPP_CONSOLE -\', you will not see any") _T_RE_EOL \
  _T("  output neither from internal nor from external commands until you enable it") _T_RE_EOL \
  _T("  via \'NPP_CONSOLE +\'.") _T_RE_EOL \
  _T("  If you call \'NPP_CONSOLE -\' before running a child process, you will not see") _T_RE_EOL \
  _T("  any prompt even if the proccess waits for user input. And \'NPP_CONSOLE +\'") _T_RE_EOL \
  _T("  will not work until the child process finishes.") _T_RE_EOL,
  
  // NPP_EXEC
  _T("COMMAND:  npp_exec") _T_RE_EOL \
  _T("USAGE:") _T_RE_EOL \
  _T("  npp_exec <script>") _T_RE_EOL \
  _T("  npp_exec <file>") _T_RE_EOL \
  _T("  npp_exec <script/file> <args>") _T_RE_EOL \
  _T("DESCRIPTION:") _T_RE_EOL \
  _T("  1. Executes commands from specified script") _T_RE_EOL \
  _T("  2. Executes commands from specified file") _T_RE_EOL \
  _T("     (supports a partial file path/name)") _T_RE_EOL \
  _T("  3. Executes commands from specified script/file") _T_RE_EOL \
  _T("     and passes additional arguments to this script.") _T_RE_EOL \
  _T("     These arguments can be accessed in the script") _T_RE_EOL \
  _T("     through the following macro-variables: $(ARGC),") _T_RE_EOL \
  _T("     $(ARGV), $(ARGV[n]), $(RARGV), $(RARGV[n])") _T_RE_EOL \
  _T("EXAMPLES:") _T_RE_EOL \
  _T("  npp_exec \"awk-run\"         // executes a script named \"awk-run\"") _T_RE_EOL \
  _T("  npp_exec \"..\\script.txt\"   // executes a file \"..\\script.txt\"") _T_RE_EOL \
  _T("  npp_exec \"..\\script\"       // the same (if there is no other files with similar name)") _T_RE_EOL \
  _T("  npp_exec \"script\" p1 \"p 2\" // in the script: $(ARGV[1]) = p1, $(ARGV[2]) = p 2") _T_RE_EOL,

  // NPP_MENUCOMMAND
  _T("COMMAND:  npp_menucommand") _T_RE_EOL \
  _T("USAGE:") _T_RE_EOL \
  _T("  npp_menucommand <full name of menu item>") _T_RE_EOL \
  _T("DESCRIPTION:") _T_RE_EOL \
  _T("  Executes (invokes) a menu item by its full name.") _T_RE_EOL \
  _T("EXAMPLES:") _T_RE_EOL \
  _T("  npp_menucommand File\\New") _T_RE_EOL \
  _T("  npp_menucommand Edit|Blank Operations|Trim Leading and Trailing Space") _T_RE_EOL \
  _T("  npp_menucommand Plugins/Plugin Manager/Show Plugin Manager") _T_RE_EOL \
  _T("  npp_menucommand Plugins\\\\NppExec\\\\Help/Manual") _T_RE_EOL \
  _T("REMARKS:") _T_RE_EOL \
  _T("  In general case, names of menu items are language-specific. If you have a") _T_RE_EOL \
  _T("  translated menu in Notepad++, you should use the menu item names you see.") _T_RE_EOL \
  _T("  (For example, \"npp_menucommand ") _T_MENUCOMMAND_CYRILLIC _T("\".)") _T_RE_EOL \
  _T("  The menu item names are processed case-sensitively, so \"file/new\" or") _T_RE_EOL \
  _T("  \"File|new\" does not correspond to the menu item \"File/New\".") _T_RE_EOL \
  _T("  You can use \'\\\', \'/\' and \'|\' as a separator. Moreover, you can double or") _T_RE_EOL \
  _T("  even triple the separator if a menu item already contains \'\\\', \'/\' or \'|\'.") _T_RE_EOL \
  _T("  However, be sure to use the very same separator within a full name of menu") _T_RE_EOL \
  _T("  item. For example: \"Plugins||NppExec||Help/Manual\" is correct since it uses") _T_RE_EOL \
  _T("  the same separator \"||\". And \"Plugins\\\\NppExec||Help/Manual\" is incorrect") _T_RE_EOL \
  _T("  because the first \"\\\\\" has been detected as a separator, and the remaining") _T_RE_EOL \
  _T("  part \"NppExec||Help/Manual\" would be treated as a name of one sub-item") _T_RE_EOL \
  _T("  since it did not contain the separator \"\\\\\".") _T_RE_EOL \
  _T("  Note: do not forget that \"//\" by default is a start of a comment in NppExec.") _T_RE_EOL \
  _T("  So do not use doubled \"//\" or tripled \"///\" as a separator of menu item name") _T_RE_EOL \
  _T("  because the remaining part of the item name (after \"//\" or \"///\") will be") _T_RE_EOL \
  _T("  treated as a comment and ignored.") _T_RE_EOL \
  _T("SEE ALSO:") _T_RE_EOL \
  _T("  npp_sendmsg") _T_RE_EOL,

  // NPP_OPEN
  _T("COMMAND:  npp_open") _T_RE_EOL \
  _T("USAGE:") _T_RE_EOL \
  _T("  npp_open <file>") _T_RE_EOL \
  _T("  npp_open <mask>") _T_RE_EOL \
  _T("  npp_open <path\\mask>") _T_RE_EOL \
  _T("DESCRIPTION:") _T_RE_EOL \
  _T("  1. Opens or reopens specified file in Notepad++") _T_RE_EOL \
  _T("  2. Opens file(s) matched the mask                (*)") _T_RE_EOL \
  _T("  * type \"help mask\" for more information about file masks") _T_RE_EOL \
  _T("EXAMPLES:") _T_RE_EOL \
  _T("  npp_open *.txt             // opens all *.txt files in current directory") _T_RE_EOL \
  _T("  npp_open e:\\temp\\file.txt  // opens the file \"e:\\temp\\file.txt\"") _T_RE_EOL \
  _T("  npp_open .\\text\\*.*        // opens all files in the folder \".\\text\"") _T_RE_EOL \
  _T("REMARKS:") _T_RE_EOL \
  _T("  To reopen a file, specify the full file path.") _T_RE_EOL \
  _T("SEE ALSO:") _T_RE_EOL \
  _T("  npp_close, npp_switch") _T_RE_EOL,

  // NPP_RUN
  _T("COMMAND:  npp_run") _T_RE_EOL \
  _T("USAGE:") _T_RE_EOL \
  _T("  npp_run <command>") _T_RE_EOL \
  _T("  npp_run <file>") _T_RE_EOL \
  _T("DESCRIPTION:") _T_RE_EOL \
  _T("  1. The same as Notepad++\'s Run command - runs an external process") _T_RE_EOL \
  _T("  2. Opens specified file in its associated program (requires NppExec v0.2 RC3.1 or higher)") _T_RE_EOL \
  _T("EXAMPLES:") _T_RE_EOL \
  _T("  npp_run calc.exe    (*)") _T_RE_EOL \
  _T("  * if you type just \"calc.exe\" without \"npp_run\", then NppExec runs") _T_RE_EOL \
  _T("    \"calc.exe\" as a child process and waits until it returns (i.e. until") _T_RE_EOL \
  _T("    you close the calc\'s window)") _T_RE_EOL \
  _T("  npp_run \"index.html\" // opens \"index.html\" in your default web-browser") _T_RE_EOL \
  _T("  npp_run \"C:\\Program Files\\Mozilla Firefox\\firefox.exe\" \"index.html\" // in Firefox") _T_RE_EOL,

  // NPP_SAVE
  _T("COMMAND:  npp_save") _T_RE_EOL \
  _T("USAGE:") _T_RE_EOL \
  _T("  npp_save") _T_RE_EOL \
  _T("  npp_save <file>") _T_RE_EOL \
  _T("DESCRIPTION:") _T_RE_EOL \
  _T("  1. Saves current file (\"npp_save\" without parameters)") _T_RE_EOL \
  _T("  2. Saves specified file if it\'s opened in Notepad++") _T_RE_EOL \
  _T("     (supports a partial file path/name)") _T_RE_EOL \
  _T("EXAMPLES:") _T_RE_EOL \
  _T("  npp_save         // saves current file") _T_RE_EOL \
  _T("  npp_save $(#2)   // saves 2nd file opened in Notepad++") _T_RE_EOL \
  _T("  npp_save txt     // saves a first file which contains \"txt\" in its name") _T_RE_EOL \
  _T("SEE ALSO:") _T_RE_EOL \
  _T("  npp_saveas, npp_saveall") _T_RE_EOL,

  // NPP_SAVEALL
  _T("COMMAND:  npp_saveall") _T_RE_EOL \
  _T("USAGE:") _T_RE_EOL \
  _T("  npp_saveall") _T_RE_EOL \
  _T("DESCRIPTION:") _T_RE_EOL \
  _T("  Saves all modified files") _T_RE_EOL \
  _T("EXAMPLES:") _T_RE_EOL \
  _T("  npp_saveall") _T_RE_EOL \
  _T("SEE ALSO:") _T_RE_EOL \
  _T("  npp_save, npp_saveas") _T_RE_EOL,

  // NPP_SAVEAS
  _T("COMMAND:  npp_saveas") _T_RE_EOL \
  _T("USAGE:") _T_RE_EOL \
  _T("  npp_saveas <file>") _T_RE_EOL \
  _T("DESCRIPTION:") _T_RE_EOL \
  _T("  Saves current file with a new (path)name") _T_RE_EOL \
  _T("EXAMPLES:") _T_RE_EOL \
  _T("  npp_saveas $(SYS.TEMP)\\newname.txt") _T_RE_EOL \
  _T("  npp_saveas anothername.txt") _T_RE_EOL \
  _T("SEE ALSO:") _T_RE_EOL \
  _T("  npp_save, npp_saveall") _T_RE_EOL,

  // NPP_SWITCH
  _T("COMMAND:  npp_switch") _T_RE_EOL \
  _T("USAGE:") _T_RE_EOL \
  _T("  npp_switch <file>") _T_RE_EOL \
  _T("DESCRIPTION:") _T_RE_EOL \
  _T("  Switches to specified opened file") _T_RE_EOL \
  _T("  (supports a partial file path/name)") _T_RE_EOL \
  _T("EXAMPLES:") _T_RE_EOL \
  _T("  npp_switch $(#3)  // activates 3rd file opened in Notepad++") _T_RE_EOL \
  _T("  npp_switch awk    // activates a first file which contains \"awk\" in its name") _T_RE_EOL \
  _T("SEE ALSO:") _T_RE_EOL \
  _T("  npp_open, npp_close") _T_RE_EOL,

  // NPP_SENDMSG
  _T("COMMAND:  npp_sendmsg") _T_RE_EOL \
  _T("USAGE:") _T_RE_EOL \
  _T("  npp_sendmsg <msg>") _T_RE_EOL \
  _T("  npp_sendmsg <msg> <wparam>") _T_RE_EOL \
  _T("  npp_sendmsg <msg> <wparam> <lparam>") _T_RE_EOL \
  _T("   * Warning: incorrect usage of this command may crash Notepad++ !!!") _T_RE_EOL \
  _T("DESCRIPTION:") _T_RE_EOL \
  _T("  Sends a message to Notepad++ (to its main window).") _T_RE_EOL \
  _T("  This command invokes Win API function SendMessage:") _T_RE_EOL \
  _T("    LRESULT SendMessage(hNppWnd, Msg, wParam, lParam)") _T_RE_EOL \
  _T("  where lParam or both lParam and wParam can be omitted.") _T_RE_EOL \
  _T("  <wparam> and <lparam> can be the following:") _T_RE_EOL \
  _T("    <int>    - integer value (int or DWORD)") _T_RE_EOL \
  _T("    @<int>   - pointer to integer (LPDWORD -> signed int)") _T_RE_EOL \
  _T("    \"<str>\"  - const string (LPCTSTR)") _T_RE_EOL \
  _T("    @\"<str>\" - pointer to string (LPTSTR)") _T_RE_EOL \
  _T("  Use <int> and \"<str>\" to pass integer or string to Notepad++:") _T_RE_EOL \
  _T("  e.g. 10, 0x1E, \"some text\".") _T_RE_EOL \
  _T("  Use @<int> and @\"str\" to get integer or string from Notepad++:") _T_RE_EOL \
  _T("  e.g. @0, @0x7FF, @\"\", @\"initial string\".") _T_RE_EOL \
  _T("  When using @<int> and @\"<str>\", the values specified are") _T_RE_EOL \
  _T("  initial values of the parameters.") _T_RE_EOL \
  _T("  NppExec reserves a buffer of (4 MB)*sizeof(TCHAR) for @\"<str>\".") _T_RE_EOL \
  _T("  When omitting <wparam> or <lparam>, NppExec treats the omitted") _T_RE_EOL \
  _T("  parameter(s) as 0 (0 as int).") _T_RE_EOL \
  _T("  This command sets the following variables:") _T_RE_EOL \
  _T("    $(MSG_RESULT) - LRESULT as int") _T_RE_EOL \
  _T("    $(MSG_WPARAM) - int or string for @<int> or @\"<str>\", otherwise empty") _T_RE_EOL \
  _T("    $(MSG_LPARAM) - int or string for @<int> or @\"<str>\", otherwise empty") _T_RE_EOL \
  _T("  See MSDN for more details about SendMessage.") _T_RE_EOL \
  _T("  See Notepad++\'es sources and documentation for more details") _T_RE_EOL \
  _T("  about messages supported by Notepad++.") _T_RE_EOL \
  _T("EXAMPLES:") _T_RE_EOL \
  _T("  // !!! All NPPM_* messages must be specified in upper case !!!") _T_RE_EOL \
  _T("  // !!! (These messages are read from NppExec\\Notepad_plus_msgs.h) !!!") _T_RE_EOL \
  _T("  // NPPM_GETNPPVERSION example:") _T_RE_EOL \
  _T("  npp_sendmsg NPPM_GETNPPVERSION  // returns ver as LRESULT") _T_RE_EOL \
  _T("  set hver ~ trunc($(MSG_RESULT)/65536)  // HIWORD(ver)") _T_RE_EOL \
  _T("  set lver ~ $(MSG_RESULT) - $(hver)*65536  // LOWORD(ver)") _T_RE_EOL \
  _T("  echo $(hver).$(lver)  // prints version as string") _T_RE_EOL \
  _T("  // NPPM_GETPLUGINSCONFIGDIR example:") _T_RE_EOL \
  _T("  npp_sendmsg NPPM_GETPLUGINSCONFIGDIR 4000000 @\"\"  // send a message") _T_RE_EOL \
  _T("  echo $(MSG_LPARAM)  // prints the string pointed by lParam") _T_RE_EOL \
  _T("  // NPPM_GETFULLPATHFROMBUFFERID _advanced_ example:") _T_RE_EOL \
  _T("  npp_sendmsg NPPM_GETCURRENTBUFFERID  // returns buffer Id as result") _T_RE_EOL \
  _T("  set bufferId = $(MSG_RESULT)") _T_RE_EOL \
  _T("  npp_sendmsg NPPM_GETFULLPATHFROMBUFFERID $(bufferId) @\"\"") _T_RE_EOL \
  _T("  echo $(MSG_LPARAM)  // file path as string") _T_RE_EOL \
  _T("  npp_sendmsg NPPM_GETFULLPATHFROMBUFFERID $(bufferId) @[]") _T_RE_EOL \
  _T("  echo $(MSG_LPARAM)  // file path as hex string") _T_RE_EOL \
  _T("  // (this last example uses undocumented ability of") _T_RE_EOL \
  _T("  //  handling of hex-string parameters, e.g.:") _T_RE_EOL \
  _T("  //  [00 7F EA 00] - const buffer of 4 bytes") _T_RE_EOL \
  _T("  //  @[FF 02 0C]   - pointer to buffer") _T_RE_EOL \
  _T("  //  Use it with care!)") _T_RE_EOL \
  _T("SEE ALSO:") _T_RE_EOL \
  _T("  sci_sendmsg, set") _T_RE_EOL,

  // NPP_SENDMSGEX
  _T("COMMAND:  npp_sendmsgex") _T_RE_EOL \
  _T("USAGE:") _T_RE_EOL \
  _T("  npp_sendmsgex <hwnd> <msg>") _T_RE_EOL \
  _T("  npp_sendmsgex <hwnd> <msg> <wparam>") _T_RE_EOL \
  _T("  npp_sendmsgex <hwnd> <msg> <wparam> <lparam>") _T_RE_EOL \
  _T("   * Warning: incorrect usage of this command may crash something !!!") _T_RE_EOL \
  _T("DESCRIPTION:") _T_RE_EOL \
  _T("  Sends a message to hwnd.") _T_RE_EOL \
  _T("  This command invokes Win API function SendMessage:") _T_RE_EOL \
  _T("    LRESULT SendMessage(hWnd, Msg, wParam, lParam)") _T_RE_EOL \
  _T("  where lParam or both lParam and wParam can be omitted.") _T_RE_EOL \
  _T("  See NPP_SENDMSG and SCI_SENDMSG for more details.") _T_RE_EOL \
  _T("EXAMPLES:") _T_RE_EOL \
  _T("  npp_sendmsg NPPM_DMMGETPLUGINHWNDBYNAME \" Console \" \"NppExec.dll\"") _T_RE_EOL \
  _T("  set hwnd = $(MSG_RESULT)  // hwnd of NppExec\'s Console") _T_RE_EOL \
  _T("  npp_sendmsgex $(hwnd) WM_COMMAND 1154 0  // Word-Wrap checkbox") _T_RE_EOL \
  _T("SEE ALSO:") _T_RE_EOL \
  _T("  npp_sendmsg, sci_sendmsg, set") _T_RE_EOL,

  // SCI_SENDMSG
  _T("COMMAND:  sci_sendmsg") _T_RE_EOL \
  _T("USAGE:") _T_RE_EOL \
  _T("  sci_sendmsg <msg>") _T_RE_EOL \
  _T("  sci_sendmsg <msg> <wparam>") _T_RE_EOL \
  _T("  sci_sendmsg <msg> <wparam> <lparam>") _T_RE_EOL \
  _T("   * Warning: incorrect usage of this command may crash Notepad++ !!!") _T_RE_EOL \
  _T("DESCRIPTION:") _T_RE_EOL \
  _T("  Sends a message to current Scintilla (to its window).") _T_RE_EOL \
  _T("  This command invokes Win API function SendMessage:") _T_RE_EOL \
  _T("    LRESULT SendMessage(hSciWnd, Msg, wParam, lParam)") _T_RE_EOL \
  _T("  where lParam or both lParam and wParam can be omitted.") _T_RE_EOL \
  _T("  <wparam> and <lparam> can be the following:") _T_RE_EOL \
  _T("    <int>    - integer value (int or DWORD)") _T_RE_EOL \
  _T("    @<int>   - pointer to integer (LPDWORD -> signed int)") _T_RE_EOL \
  _T("    \"<str>\"  - const string (LPCTSTR)") _T_RE_EOL \
  _T("    @\"<str>\" - pointer to string (LPTSTR)") _T_RE_EOL \
  _T("  Use <int> and \"<str>\" to pass integer or string to Scintilla:") _T_RE_EOL \
  _T("  e.g. 10, 0x1E, \"some text\".") _T_RE_EOL \
  _T("  Use @<int> and @\"str\" to get integer or string from Scintilla:") _T_RE_EOL \
  _T("  e.g. @0, @0x7FF, @\"\", @\"initial string\".") _T_RE_EOL \
  _T("  When using @<int> and @\"<str>\", the values specified are") _T_RE_EOL \
  _T("  initial values of the parameters.") _T_RE_EOL \
  _T("  NppExec reserves a buffer of (4 MB)*sizeof(char) for @\"<str>\".") _T_RE_EOL \
  _T("  NppExec performs conversion from/to ANSI or UTF-8 automatically.") _T_RE_EOL \
  _T("  When omitting <wparam> or <lparam>, NppExec treats the omitted") _T_RE_EOL \
  _T("  parameter(s) as 0 (0 as int).") _T_RE_EOL \
  _T("  This command sets the following variables:") _T_RE_EOL \
  _T("    $(MSG_RESULT) - LRESULT as int") _T_RE_EOL \
  _T("    $(MSG_WPARAM) - int or string for @<int> or @\"<str>\", otherwise empty") _T_RE_EOL \
  _T("    $(MSG_LPARAM) - int or string for @<int> or @\"<str>\", otherwise empty") _T_RE_EOL \
  _T("  See MSDN for more details about SendMessage.") _T_RE_EOL \
  _T("  See Scintilla\'s documentation for more details about messages") _T_RE_EOL \
  _T("  supported by Scintilla.") _T_RE_EOL \
  _T("EXAMPLES:") _T_RE_EOL \
  _T("  // !!! All SCI_* messages must be specified in upper case !!!") _T_RE_EOL \
  _T("  // !!! (These messages are read from NppExec\\Scintilla.h) !!!") _T_RE_EOL \
  _T("  // get the text of the first visible line:") _T_RE_EOL \
  _T("  sci_sendmsg SCI_GETFIRSTVISIBLELINE  // returns line number as LRESULT") _T_RE_EOL \
  _T("  sci_sendmsg SCI_GETLINE $(MSG_RESULT) @\"\"") _T_RE_EOL \
  _T("  echo $(MSG_LPARAM)  // prints the string pointed by lParam") _T_RE_EOL \
  _T("SEE ALSO:") _T_RE_EOL \
  _T("  npp_sendmsg, set") _T_RE_EOL,

  // MASK
  _T("DESCRIPTION:  file mask") _T_RE_EOL \
  _T("USAGE:") _T_RE_EOL \
  _T("  File masks are used by such commands as DIR and NPP_OPEN") _T_RE_EOL \
  _T("DESCRIPTION:") _T_RE_EOL \
  _T("  The file mask in NppExec is an orthographical construction where") _T_RE_EOL \
  _T("    ? means any single character and") _T_RE_EOL \
  _T("    * means any 0 or more characters") _T_RE_EOL \
  _T("  regardless of such concepts as file name or file extension.") _T_RE_EOL \
  _T("  Thus, *.* in NppExec DOES NOT match any file or directory name -") _T_RE_EOL \
  _T("  it matches any string which contains \'.\'") _T_RE_EOL \
  _T("  Also, *tx*.* does not match \"file.txt\" because \"tx\" must be") _T_RE_EOL \
  _T("  located BEFORE \'.\' and not AFTER it.") _T_RE_EOL \
  _T("EXAMPLES:") _T_RE_EOL \
  _T("  dir *.cpp         // prints all *.cpp files in current directory") _T_RE_EOL \
  _T("  npp_open *dlg*.*  // opens all files which contain \"dlg\" and \'.\' in its name") _T_RE_EOL \
  _T("SEE ALSO:") _T_RE_EOL \
  _T("  dir, npp_open") _T_RE_EOL,

  // SET
  _T("COMMAND:  set/unset") _T_RE_EOL \
  _T("USAGE:") _T_RE_EOL \
  _T("  set") _T_RE_EOL \
  _T("  set <var>") _T_RE_EOL \
  _T("  set <var> = <value>") _T_RE_EOL \
  _T("  set $(<var>) = <value>") _T_RE_EOL \
  _T("  set <var> ~ <math expression>") _T_RE_EOL \
  _T("  set <var> ~ strlen <string>") _T_RE_EOL \
  _T("  set <var> ~ strlenutf8 <string>") _T_RE_EOL \
  _T("  set <var> ~ strupper <string>") _T_RE_EOL \
  _T("  set <var> ~ strlower <string>") _T_RE_EOL \
  _T("  set <var> ~ substr <pos> <len> <string>") _T_RE_EOL \
  _T("  set <var> ~ strfind <string> <sfind>") _T_RE_EOL \
  _T("  set <var> ~ strrfind <string> <sfind>") _T_RE_EOL \
  _T("  set <var> ~ strreplace <string> <sfind> <sreplace>") _T_RE_EOL \
  _T("  set local") _T_RE_EOL \
  _T("  set local <var>") _T_RE_EOL \
  _T("  set local <var> = ...") _T_RE_EOL \
  _T("  set local <var> ~ ...") _T_RE_EOL \
  _T("  unset <var>") _T_RE_EOL \
  _T("  unset <var> = <value>") _T_RE_EOL \
  _T("  unset local <var>") _T_RE_EOL \
  _T("DESCRIPTION:") _T_RE_EOL \
  _T("  1. Shows all user\'s variables (\"set\" without parameters)") _T_RE_EOL \
  _T("  2. Shows the value of user\'s variable (\"set\" without \"=\")") _T_RE_EOL \
  _T("  3. Sets the value of user\'s variable (\"set <var> = <value>\")") _T_RE_EOL \
  _T("  4. Calculates the math expression (\"set <var> ~ <math expr>\")") _T_RE_EOL \
  _T("  5a. Calculates the string length (\"set <var> ~ strlen <string>\")") _T_RE_EOL \
  _T("  5b. Calculates the UTF-8 string length (\"set <var> ~ strlenutf8 <s>\")") _T_RE_EOL \
  _T("  5c. Returns the string in upper case (\"set <var> ~ strupper <s>\")") _T_RE_EOL \
  _T("  5d. Returns the string in lower case (\"set <var> ~ strlower <s>\")") _T_RE_EOL \
  _T("  5e. Returns the substring (\"substr <pos> <len> <s>\")") _T_RE_EOL \
  _T("  5f. Returns the position of first <sfind> in <string>") _T_RE_EOL \
  _T("  5g. Returns the position of last <sfind> in <string>") _T_RE_EOL \
  _T("  5h. Replaces all <sfind> with <sreplace> in <string>") _T_RE_EOL \
  _T("  6.  Shows/sets the value of local variable (\"set local <var> ...\")") _T_RE_EOL \
  _T("  7.  Removes the variable <var> (\"unset <var>\")") _T_RE_EOL \
  _T("  8.  Removes the local variable <var> (\"unset local <var>\")") _T_RE_EOL \
  _T("EXAMPLES:") _T_RE_EOL \
  _T("  set p = C:\\Program Files") _T_RE_EOL \
  _T("  dir $(p)\\*  // the same as dir C:\\Program Files\\*") _T_RE_EOL \
  _T("  unset p     // removes the variable p") _T_RE_EOL \
  _T("  // set vs. set local:") _T_RE_EOL \
  _T("  set x = 10       // 10: global x") _T_RE_EOL \
  _T("  set local x = 20 // 20: local x") _T_RE_EOL \
  _T("  echo $(x)        // 20: substitutes the overridden (local) x") _T_RE_EOL \
  _T("  set x            // 10: prints global x") _T_RE_EOL \
  _T("  set local x      // 20: prints local x") _T_RE_EOL \
  _T("  unset local x    // 20: deletes local x") _T_RE_EOL \
  _T("  echo $(x)        // 10: substitutes the global x") _T_RE_EOL \
  _T("  // calculations:") _T_RE_EOL \
  _T("  set ans ~ 1 + 2*(3 + 4) - 0x5  // calculates the expression") _T_RE_EOL \
  _T("  set n ~ strlen   ABC D   // sets n=5 (skipping leading & trailing spaces)") _T_RE_EOL \
  _T("  set n ~ strlen \"  ABC \"  // sets n=8 (including spaces & double quotes)") _T_RE_EOL \
  _T("  // in case of non-Latin characters, strlenutf8 may differ from strlen:") _T_RE_EOL \
  _T("  set n ~ strlenutf8 ") _T_STRLEN_CYRILLIC _T("  // sets n=12 (number of UTF-8 bytes)") _T_RE_EOL \
  _T("  // strupper/strlower:") _T_RE_EOL \
  _T("  set s = Text") _T_RE_EOL \
  _T("  set t = $(s)           // t = Text") _T_RE_EOL \
  _T("  set t ~ strupper $(s)  // t = TEXT") _T_RE_EOL \
  _T("  set t ~ strlower $(s)  // t = text") _T_RE_EOL \
  _T("  // substr:") _T_RE_EOL \
  _T("  set s ~ substr 0 3  abcde  // abc (3 characters from position 0)") _T_RE_EOL \
  _T("  set s ~ substr 2 2  abcde  // cd (2 characters from position 2)") _T_RE_EOL \
  _T("  set s ~ substr 3 -  abcde  // de (substring from position 3 to the end)") _T_RE_EOL \
  _T("  set s ~ substr 1 -2 abcde  // bc (substring from position 1 to length-2)") _T_RE_EOL \
  _T("  set s ~ substr -3 - abcde  // cde (substring from position length-3 to the end)") _T_RE_EOL \
  _T("  set s ~ substr -2 1 abcde  // d (1 character from position length-2)") _T_RE_EOL \
  _T("  set s ~ substr -3 -2 abcde // c (substring from position length-3 to length-2)") _T_RE_EOL \
  _T("  set s = abcde") _T_RE_EOL \
  _T("  set t ~ substr 1 3 $(s)    // bcd (3 characters from position 1)") _T_RE_EOL \
  _T("  // strfind/strrfind") _T_RE_EOL \
  _T("  // * quotes are not treated as a part of a string itself") _T_RE_EOL \
  _T("  set n ~ strfind \"Hello world\" Hello  // returns 0") _T_RE_EOL \
  _T("  set n ~ strfind \"Hello world\" \"o\"    // returns 4 - \"o\" in \"Hello\"") _T_RE_EOL \
  _T("  set n ~ strrfind \"Hello world\" o     // returns 7 - \"o\" in \"world\"") _T_RE_EOL \
  _T("  set n ~ strrfind \"Hello world\" x     // returns -1 - \"x\" not found") _T_RE_EOL \
  _T("  set n ~ strfind \"Hello world\" \"o y\"  // returns -1 - \"o y\" not found") _T_RE_EOL \
  _T("  set s = abcd") _T_RE_EOL \
  _T("  set n ~ strfind \"$(s)\" cd            // returns 2") _T_RE_EOL \
  _T("  // strreplace") _T_RE_EOL \
  _T("  // * quotes are not treated as a part of a string itself") _T_RE_EOL \
  _T("  set s ~ strreplace \"Hello world\" \"o\" \"0\" // Hell0 w0rld    (\"o\" -> \"0\")") _T_RE_EOL \
  _T("  set s ~ strreplace \"$(s)\" l 1            // He110 w0r1d    (\"l\" -> \"1\")") _T_RE_EOL \
  _T("  set s ~ strreplace \"$(s)\" 1 \"y \"         // Hey y 0 w0ry d (\"1\" -> \"y \")") _T_RE_EOL \
  _T("  set s ~ strreplace \"queen-bee\" ee \"\"     // qun-b          (\"ee\" -> \"\")") _T_RE_EOL \
  _T("REMARKS:") _T_RE_EOL \
  _T("  User\'s variables have the lowest priority, so they can\'t override") _T_RE_EOL \
  _T("  other (predefined) variables. Thus, you can set your own variables") _T_RE_EOL \
  _T("  $(FILE_NAME), $(ARGV) and so on, but their values will not be") _T_RE_EOL \
  _T("  available because of predefined variables with the same name.") _T_RE_EOL \
  _T("  These variables are internal variables of NppExec, unlike") _T_RE_EOL \
  _T("  environment variables which can be set using ENV_SET.") _T_RE_EOL \
  _T("  NppExec uses Function Parser (fparser) to calculate the math") _T_RE_EOL \
  _T("  expression given as \"set <var> ~ <math expression>\".") _T_RE_EOL \
  _T("  The math expression can contain hex numbers with leading \"0x\"") _T_RE_EOL \
  _T("  (e.g. set ans ~ 0x0F + 0x10A - 0xB).") _T_RE_EOL \
  _T("  The following constants are hardcoded: WM_USER, NPPMSG.") _T_RE_EOL \
  _T("  For more details about supported functions, see \"fparser.html\".") _T_RE_EOL \
  _T("  You can use NPE_CMDALIAS to define a short alias to the built-in") _T_RE_EOL \
  _T("  calculator:") _T_RE_EOL \
  _T("    npe_cmdalias = = set ans ~  // \"=\" -> \"set ans ~\"") _T_RE_EOL \
  _T("    = 1 + 2/3 + 4/5  // using the alias for calculations") _T_RE_EOL \
  _T("  Local variables (\"set local <var>\") live just within a script") _T_RE_EOL \
  _T("  where they were declared. When the script ends, all its local") _T_RE_EOL \
  _T("  variables are automatically deleted.") _T_RE_EOL \
  _T("  Within a script, a local variable overrides a global variable") _T_RE_EOL \
  _T("  with the same name. Each script may have its own local variables") _T_RE_EOL \
  _T("  that are not inherited by NPP_EXEC-ed scripts.") _T_RE_EOL \
  _T("SEE ALSO:") _T_RE_EOL \
  _T("  env_set/env_unset, if") _T_RE_EOL,
  
  // ENV_SET
  _T("COMMAND:  env_set/env_unset") _T_RE_EOL \
  _T("USAGE:") _T_RE_EOL \
  _T("  env_set <var>") _T_RE_EOL \
  _T("  env_set <var> = <value>") _T_RE_EOL \
  _T("  env_unset <var>") _T_RE_EOL \
  _T("DESCRIPTION:") _T_RE_EOL \
  _T("  1. Shows the value of environment variable (\"env_set\" without \"=\")") _T_RE_EOL \
  _T("  2. Sets the value of environment variable (\"env_set <var> = <value>\")") _T_RE_EOL \
  _T("  3. Removes/restores the environment variable <var> (\"env_unset <var>\")") _T_RE_EOL \
  _T("EXAMPLES:") _T_RE_EOL \
  _T("  env_set NPPHOME = $(NPP_DIRECTORY)  // new environment variable: NPPHOME") _T_RE_EOL \
  _T("  env_set PATH = $(SYS.NPPHOME);$(SYS.PATH)  // modifying the PATH variable") _T_RE_EOL \
  _T("  npp_run readme.txt  // opening \"readme.txt\" from Notepad++\'es folder") _T_RE_EOL \
  _T("                      // (NPPHOME is the first path in the PATH variable)") _T_RE_EOL \
  _T("  env_unset NPPHOME  // removing the environment variable NPPHOME") _T_RE_EOL \
  _T("  env_unset PATH     // restoring initial value of PATH") _T_RE_EOL \
  _T("REMARKS:") _T_RE_EOL \
  _T("  Unlike set/unset, these commands deal with Notepad++\'es environment vars") _T_RE_EOL \
  _T("  which are inherited by child processes (programs, tools) started from") _T_RE_EOL \
  _T("  NppExec or Notepad++\'es \"Run...\" menu.") _T_RE_EOL \
  _T("  NppExec can use the values of environment variables in a form of $(SYS.<var>).") _T_RE_EOL \
  _T("  For example:") _T_RE_EOL \
  _T("    env_set NPPHOME = $(NPP_DIRECTORY)  // new environment variable: NPPHOME") _T_RE_EOL \
  _T("    echo $(SYS.NPPHOME)  // print the value of NPPHOME") _T_RE_EOL \
  _T("  NppExec does not understand such form of environment variable as %<var>%.") _T_RE_EOL \
  _T("  I.e. you can type  \"env_set PATH = C:\\tools;$(SYS.PATH)\",") _T_RE_EOL \
  _T("  but not  \"env_set PATH = C:\\tools;%PATH%\"") _T_RE_EOL \
  _T("  because NppExec does not support such declaration as %PATH%.") _T_RE_EOL \
  _T("  You can use a value of other variable as an environment variable name:") _T_RE_EOL \
  _T("    set a = PATH  // value of \'a\' is a string \"PATH\"") _T_RE_EOL \
  _T("    env_set $(a) = C:\\tools  // env. variable PATH becomes \"C:\\tools\"") _T_RE_EOL \
  _T("    echo $(SYS.PATH)  // prints the value of env. var PATH i.e. \"C:\\tools\"") _T_RE_EOL \
  _T("    env_unset $(a)  // restore the initial value of PATH") _T_RE_EOL \
  _T("  The ENV_UNSET command removes only those environment variables which") _T_RE_EOL \
  _T("  have been created by the ENV_SET command (i.e. did not exist before).") _T_RE_EOL \
  _T("  Other environment variables (such as PATH, TEMP etc.) are not removed") _T_RE_EOL \
  _T("  by ENV_UNSET because these variables were not created by ENV_SET.") _T_RE_EOL \
  _T("  Instead, ENV_UNSET restores initial values of these variables.") _T_RE_EOL \
  _T("SEE ALSO:") _T_RE_EOL \
  _T("  set/unset") _T_RE_EOL,
  
  // INPUTBOX
  _T("COMMAND:  inputbox") _T_RE_EOL \
  _T("USAGE:") _T_RE_EOL \
  _T("  inputbox \"message\"") _T_RE_EOL \
  _T("  inputbox \"message\" : initial_value") _T_RE_EOL \
  _T("  inputbox \"message\" :: initial_value") _T_RE_EOL \
  _T("  inputbox \"message\" : \"value_name\" : initial_value") _T_RE_EOL \
  _T("  inputbox \"message\" : \" \" : initial_value") _T_RE_EOL \
  _T("DESCRIPTION:") _T_RE_EOL \
  _T("  1. Shows the InputBox with a message \'message\';") _T_RE_EOL \
  _T("     the input value is stored in $(INPUT), $(INPUT[1]) etc.") _T_RE_EOL \
  _T("  2. Shows the InputBox with a message \'message\';") _T_RE_EOL \
  _T("     the initial input value is set to \'initial_value\';") _T_RE_EOL \
  _T("     the input value is stored in $(INPUT), $(INPUT[1]) etc.") _T_RE_EOL \
  _T("  3. Shows the InputBox with a message \'message\';") _T_RE_EOL \
  _T("     the text \"$(INPUT) =\" is replaced with \'value_name\'") _T_RE_EOL \
  _T("     (use \"\" to keep \"$(INPUT) =\"; use \" \" to empty it);") _T_RE_EOL \
  _T("     the initial input value is set to \'initial_value\';") _T_RE_EOL \
  _T("     the input value is stored in $(INPUT), $(INPUT[1]) etc.") _T_RE_EOL \
  _T("EXAMPLES:") _T_RE_EOL \
  _T("  // show the InputBox...") _T_RE_EOL \
  _T("  inputbox \"Input values of A, B and C separated by spaces:\"") _T_RE_EOL \
  _T("  // update the values of our variables...") _T_RE_EOL \
  _T("  set a = $(input[1])  // e.g. \'A\' in \'A \"B B\" C\'") _T_RE_EOL \
  _T("  set b = $(input[2])  // e.g. \'B B\' in \'A \"B B\" C\'") _T_RE_EOL \
  _T("  set c = $(input[3])  // e.g. \'C\' in \'A \"B B\" C\'") _T_RE_EOL \
  _T("  // show the InputBox with specified initial value...") _T_RE_EOL \
  _T("  inputbox \"Please confirm the value of C:\" : $(c)") _T_RE_EOL \
  _T("  // update the value of c...") _T_RE_EOL \
  _T("  set c = $(input)") _T_RE_EOL \
  _T("  // show the InputBox with the text \"Input a:\", \"a =\"...") _T_RE_EOL \
  _T("  inputbox \"Input a:\" : \"a =\" : ") _T_RE_EOL \
  _T("  // show the InputBox with empty value name...") _T_RE_EOL \
  _T("  inputbox \"Input a:\" : \" \" :   // notice the  : \" \" :  part!") _T_RE_EOL \
  _T("  // show the initial value with a colon...") _T_RE_EOL \
  _T("  inputbox \"Input a:\" : : 1 : 2   // notice the  : :  part!") _T_RE_EOL \
  _T("  // the same...") _T_RE_EOL \
  _T("  inputbox \"Input a:\" :: 1 : 2   // notice the  ::  part!") _T_RE_EOL \
  _T("REMARKS:") _T_RE_EOL \
  _T("  You can use any environment variable inside the input value,") _T_RE_EOL \
  _T("  except $(ARGC), $(ARGV), $(ARGV[N]), $(RARGV) and $(RARGV[N]).") _T_RE_EOL \
  _T("  To use values of $(ARGV) or $(RARGV) inside the input value,") _T_RE_EOL \
  _T("  you can SET some variable and use this variable in the InputBox e.g.") _T_RE_EOL \
  _T("      SET arg1 = $(ARGV[1])") _T_RE_EOL \
  _T("      INPUTBOX \"Input A:\"") _T_RE_EOL \
  _T("      // ... the InputBox is shown, you can type: $(arg1)") _T_RE_EOL \
  _T("      SET a = $(INPUT)") _T_RE_EOL \
  _T("      // ... $(a) becomes $(arg1) i.e. $(ARGV[1])") _T_RE_EOL \
  _T("SEE ALSO:") _T_RE_EOL \
  _T("  set, unset, if") _T_RE_EOL,
  
  // NPE_CMDALIAS
  _T("COMMAND:  npe_cmdalias") _T_RE_EOL \
  _T("USAGE:") _T_RE_EOL \
  _T("  npe_cmdalias") _T_RE_EOL \
  _T("  npe_cmdalias <alias>") _T_RE_EOL \
  _T("  npe_cmdalias <alias> =") _T_RE_EOL \
  _T("  npe_cmdalias <alias> = <command>") _T_RE_EOL \
  _T("DESCRIPTION:") _T_RE_EOL \
  _T("  1. Shows all command aliases (\"npe_cmdalias\" without parameters)") _T_RE_EOL \
  _T("  2. Shows the value of command alias (\"npe_cmdalias\" without \"=\")") _T_RE_EOL \
  _T("  3. Removes the command alias (\"npe_cmdalias\" with empty \"=\")") _T_RE_EOL \
  _T("  4. Sets the value of command alias (\"npe_cmdalias <alias> = <command>\")") _T_RE_EOL \
  _T("EXAMPLES:") _T_RE_EOL \
  _T("  npe_cmdalias = = calc            // type just \"=\" instead of \"calc\"") _T_RE_EOL \
  _T("  npe_cmdalias copy = cmd /c copy  // \"copy\" instead of \"cmd /c copy\"") _T_RE_EOL \
  _T("  npe_cmdalias /c = cmd /c         // \"/c\" instead of \"cmd /c\"") _T_RE_EOL \
  _T("  npe_cmdalias /c                  // show the value of command alias") _T_RE_EOL \
  _T("  npe_cmdalias /c =                // remove the command alias") _T_RE_EOL \
  _T("REMARKS:") _T_RE_EOL \
  _T("  To use the command alias, type it in the beginning of the input string") _T_RE_EOL \
  _T("  (e.g. \"/c copy /?\" for the command alias \"/c -> cmd /c\"), otherwise") _T_RE_EOL \
  _T("  it will NOT be treated as a command alias. For example, \"/c\" inside") _T_RE_EOL \
  _T("  \"cmd /c move /?\" is not treated as a command alias because it is located") _T_RE_EOL \
  _T("  in the middle of the string, so this string remains unmodified.") _T_RE_EOL \
  _T("SEE ALSO:") _T_RE_EOL \
  _T("  npe_console q+/q-") _T_RE_EOL,
  
  // NPE_CONSOLE
  _T("COMMAND:  npe_console") _T_RE_EOL \
  _T("USAGE:") _T_RE_EOL \
  _T("  npe_console") _T_RE_EOL \
  _T("  npe_console a+/a- d+/d- h+/h- m+/m- q+/q- v+/v- f+/f- r+/r- k0..3") _T_RE_EOL \
  _T("  npe_console o0/o1/o2 i0/i1/i2") _T_RE_EOL \
  _T("  npe_console <options> --") _T_RE_EOL \
  _T("DESCRIPTION:") _T_RE_EOL \
  _T("  1. Without parameter - shows current Console options/mode") _T_RE_EOL \
  _T("  2. X+/X-  enables/disables the option/mode X:") _T_RE_EOL \
  _T("       a+/a-  append mode (don\'t clear Console) on/off") _T_RE_EOL \
  _T("       d+/d-  follow $(CURRENT_DIRECTORY) on/off") _T_RE_EOL \
  _T("       h+/h-  console commands history on/off") _T_RE_EOL \
  _T("       m+/m-  console internal messages on/off") _T_RE_EOL \
  _T("       q+/q-  command aliases on/off") _T_RE_EOL \
  _T("       v+/v-  set the $(OUTPUT) variable on/off") _T_RE_EOL \
  _T("       f+/f-  console output filter on/off") _T_RE_EOL \
  _T("       r+/r-  console output replace filter on/off") _T_RE_EOL \
  _T("       k0..3  catch NppExec\'s shortcut keys on/off") _T_RE_EOL \
  _T("  3. Y0/Y1/Y2  sets the value of the option/mode Y:") _T_RE_EOL \
  _T("       o0/o1/o2  console output encoding: ANSI/OEM/UTF8") _T_RE_EOL \
  _T("       i0/i1/i2  console input encoding: ANSI/OEM/UTF8") _T_RE_EOL \
  _T("  4. --  silent (don\'t print Console mode info)") _T_RE_EOL \
  _T("EXAMPLES:") _T_RE_EOL \
  _T("  npe_console o1 i1 a+ --") _T_RE_EOL \
  _T("    //// OEM/OEM, append mode, silent") _T_RE_EOL \
  _T("  npe_console h+ m+ a- i0 o0") _T_RE_EOL \
  _T("    //// history on, int. msgs on, don\'t append, ANSI/ANSI") _T_RE_EOL \
  _T("  npe_console d+ k0 q-") _T_RE_EOL \
  _T("    //// follow curr. dir, don\'t catch shortcut keys, cmd aliases off") _T_RE_EOL \
  _T("  npe_console -- v+ m- // silent, enable the $(OUTPUT) var, no int.msgs") _T_RE_EOL \
  _T("  cmd /c time /t       // cmd prints current time; $(OUTPUT) is set") _T_RE_EOL \
  _T("  echo $(OUTPUT)       // NppExec prints $(OUTPUT) i.e. current time") _T_RE_EOL \
  _T("  npe_console -- v- m+ // don\'t forget to disable the $(OUTPUT) var") _T_RE_EOL \
  _T("REMARKS:") _T_RE_EOL \
  _T("  a+/a-  Console append mode on/off.") _T_RE_EOL \
  _T("         There is no corresponding menu item.") _T_RE_EOL \
  _T("         This option is not saved when you close Notepad++.") _T_RE_EOL \
  _T("         Default value: off") _T_RE_EOL \
  _T("         (Console is automatically cleared when new script starts).") _T_RE_EOL \
  _T("  d+/d-  Follow $(CURRENT_DIRECTORY) on/off.") _T_RE_EOL \
  _T("         Corresponding menu item: Follow $(CURRENT_DIRECTORY).") _T_RE_EOL \
  _T("         If On, NppExec follows the directory of current file.") _T_RE_EOL \
  _T("  h+/h-  Console commands history on/off.") _T_RE_EOL \
  _T("         Corresponding menu item: Console Commands History.") _T_RE_EOL \
  _T("         If On, NppExec\'s Console remembers the commands you typed.") _T_RE_EOL \
  _T("  m+/m-  Console internal messages on/off.") _T_RE_EOL \
  _T("         Corresponding menu item (inverse): No internal messages.") _T_RE_EOL \
  _T("         If On, NppExec\'s Console prints all internal (green) messages.") _T_RE_EOL \
  _T("  q+/q-  Command aliases on/off.") _T_RE_EOL \
  _T("         Corresponding menu item (inverse): Disable command aliases.") _T_RE_EOL \
  _T("         This option is not saved when you close Notepad++.") _T_RE_EOL \
  _T("         Default value: on") _T_RE_EOL \
  _T("         (command aliases created with NPE_CMDALIAS are active).") _T_RE_EOL \
  _T("  v+/v-  Set the $(OUTPUT) variable on/off.") _T_RE_EOL \
  _T("         (this variable will store the child process\'es output).") _T_RE_EOL \
  _T("         There is no corresponding menu item.") _T_RE_EOL \
  _T("         This option is not saved when you close Notepad++.") _T_RE_EOL \
  _T("         Default value: off.") _T_RE_EOL \
  _T("  f+/f-  Console output filter on/off.") _T_RE_EOL \
  _T("         Corresponding menu item: Console Output Filters...") _T_RE_EOL \
  _T("  r+/r-  Console output replace filter on/off.") _T_RE_EOL \
  _T("         Corresponding menu item: Console Output Filters...") _T_RE_EOL \
  _T("  k0..3  Catch NppExec\'s shortcut keys on/off") _T_RE_EOL \
  _T("         Controls if NppExec\'s Console catches shortcut keys related") _T_RE_EOL \
  _T("         to NppExec\'s menu items or scripts. This allows to execute") _T_RE_EOL \
  _T("         the corresponding commands from NppExec\'s Console when the") _T_RE_EOL \
  _T("         associated shortcut keys are pressed.") _T_RE_EOL \
  _T("         k0: Do not catch NppExec\'s shortcut keys;") _T_RE_EOL \
  _T("         k1: Catch standard NppExec\'s shortcut keys (Execute... etc.);") _T_RE_EOL \
  _T("         k2: Catch user\'s NppExec\'s shortcut keys (scripts\' menu items);") _T_RE_EOL \
  _T("         k3: Catch all NppExec\'s shortcut keys (standard+scripts).") _T_RE_EOL \
  _T("         There is no corresponding menu item.") _T_RE_EOL \
  _T("         This option is not saved when you close Notepad++.") _T_RE_EOL \
  _T("         Default value: 3.") _T_RE_EOL \
  _T("  o0/o1/o2  Console output encoding: ANSI/OEM/UTF8") _T_RE_EOL \
  _T("            Corresponding menu item: Console Output...") _T_RE_EOL \
  _T("            Sets Console output encoding.") _T_RE_EOL \
  _T("  i0/i1/i2  Console input encoding: ANSI/OEM/UTF8") _T_RE_EOL \
  _T("            Corresponding menu item: Console Output...") _T_RE_EOL \
  _T("            Sets Console input encoding.") _T_RE_EOL \
  _T("SEE ALSO:") _T_RE_EOL \
  _T("  con_filter, npp_console, npe_noemptyvars") _T_RE_EOL,

  // NPE_DEBUGLOG
  _T("COMMAND:  npe_debuglog") _T_RE_EOL \
  _T("USAGE:") _T_RE_EOL \
  _T("  npe_debuglog") _T_RE_EOL \
  _T("  npe_debuglog on") _T_RE_EOL \
  _T("  npe_debuglog 1") _T_RE_EOL \
  _T("  npe_debuglog off") _T_RE_EOL \
  _T("  npe_debuglog 0") _T_RE_EOL \
  _T("DESCRIPTION:") _T_RE_EOL \
  _T("  1. Without parameter - shows current Debug Log state (On/Off)") _T_RE_EOL \
  _T("  2. On or 1 - enables the Debug Log") _T_RE_EOL \
  _T("  3. Off or 0 - disables the Debug Log") _T_RE_EOL,

  // NPE_NOEMPTYVARS
  _T("COMMAND:  npe_noemptyvars") _T_RE_EOL \
  _T("USAGE:") _T_RE_EOL \
  _T("  npe_noemptyvars") _T_RE_EOL \
  _T("  npe_noemptyvars 1") _T_RE_EOL \
  _T("  npe_noemptyvars 0") _T_RE_EOL \
  _T("DESCRIPTION:") _T_RE_EOL \
  _T("  1. Without parameter - shows current option\'s state") _T_RE_EOL \
  _T("  2. 1 or On - enables replacement of empty (uninitialized) vars") _T_RE_EOL \
  _T("  3. 0 or Off - disables replacement of empty (uninitialized) vars") _T_RE_EOL \
  _T("EXAMPLES:") _T_RE_EOL \
  _T("  unset var            // to be sure that $(var) is uninitialized") _T_RE_EOL \
  _T("  npe_noemptyvars 0    // disabled") _T_RE_EOL \
  _T("  echo var = \"$(var)\"  // prints: var = \"$(var)\"") _T_RE_EOL \
  _T("  npe_noemptyvars 1    // enabled") _T_RE_EOL \
  _T("  echo var = \"$(var)\"  // prints: var = \"\"") _T_RE_EOL \
  _T("REMARKS:") _T_RE_EOL \
  _T("  Refer to NppExec.ini, parameter NoEmptyVars in the [Console] section.") _T_RE_EOL \
  _T("  This option is not saved when you close Notepad++.") _T_RE_EOL \
  _T("SEE ALSO:") _T_RE_EOL \
  _T("  npe_console") _T_RE_EOL,

  // SEL_LOADFROM
  _T("COMMAND:  sel_loadfrom") _T_RE_EOL \
  _T("USAGE:") _T_RE_EOL \
  _T("  sel_loadfrom <file>") _T_RE_EOL \
  _T("DESCRIPTION:") _T_RE_EOL \
  _T("  Replaces currently selected text with the file\'s content") _T_RE_EOL \
  _T("EXAMPLES:") _T_RE_EOL \
  _T("  sel_loadfrom c:\\temp\\output.txt") _T_RE_EOL \
  _T("REMARKS:") _T_RE_EOL \
  _T("  Unicode version of NppExec can read ANSI, UTF-8, UCS-2 LE and") _T_RE_EOL \
  _T("  UCS-2 BE text files") _T_RE_EOL \
  _T("SEE ALSO:") _T_RE_EOL \
  _T("  sel_saveto, sel_settext") _T_RE_EOL,
  
  // SEL_SAVETO
  _T("COMMAND:  sel_saveto") _T_RE_EOL \
  _T("USAGE:") _T_RE_EOL \
  _T("  sel_saveto <file>") _T_RE_EOL \
  _T("  sel_saveto <file> : <encoding>") _T_RE_EOL \
  _T("DESCRIPTION:") _T_RE_EOL \
  _T("  1. Saves the selected text (in its current encoding) to a file") _T_RE_EOL \
  _T("  2. Saves the selected text (converted to specified encoding) to a file") _T_RE_EOL \
  _T("     <encoding> may be:  a (ANSI), u (UTF-8), w (UCS-2 LE)") _T_RE_EOL \
  _T("EXAMPLES:") _T_RE_EOL \
  _T("  sel_saveto c:\\temp\\output.txt // may be ANSI, UTF-8 or UCS-2 LE file") _T_RE_EOL \
  _T("  sel_saveto c:\\temp\\outputANSI.txt :a     // save as ANSI file") _T_RE_EOL \
  _T("  sel_saveto c:\\temp\\outputUTF8.txt :u     // save as UTF-8 file") _T_RE_EOL \
  _T("  sel_saveto c:\\temp\\outputUnicode.txt :w  // save as UCS-2 LE file") _T_RE_EOL \
  _T("REMARKS:") _T_RE_EOL \
  _T("  SEL_SAVETO with the <encoding> parameter omitted saves the text in the") _T_RE_EOL \
  _T("  Scintilla\'s current encoding (it can be ANSI, DBCS or UTF-8).") _T_RE_EOL \
  _T("  So, if you want to save the text as UCS-2 LE (Unicode in Windows), you") _T_RE_EOL \
  _T("  must specify the \":w\" parameter.") _T_RE_EOL \
  _T("SEE ALSO:") _T_RE_EOL \
  _T("  sel_loadfrom, sel_settext") _T_RE_EOL,

  // SEL_SETTEXT
  _T("COMMAND:  sel_settext, sel_settext+") _T_RE_EOL \
  _T("USAGE:") _T_RE_EOL \
  _T("  sel_settext <text>") _T_RE_EOL \
  _T("  sel_settext+ <text>") _T_RE_EOL \
  _T("DESCRIPTION:") _T_RE_EOL \
  _T("  Replaces currently selected text with the text specified") _T_RE_EOL \
  _T("EXAMPLES:") _T_RE_EOL \
  _T("  sel_settext This text has replaced the previously selected text") _T_RE_EOL \
  _T("  sel_settext $(INPUT)   // see: inputbox") _T_RE_EOL \
  _T("  sel_settext $(OUTPUT)  // see: npe_console v+") _T_RE_EOL \
  _T("  sel_settext $(SYS.PATH)") _T_RE_EOL \
  _T("  sel_settext+ A text with new line:\\r\\nnew line") _T_RE_EOL \
  _T("REMARKS:") _T_RE_EOL \
  _T("  Use sel_settext+ to insert special characters:") _T_RE_EOL \
  _T("  \\n  - line feed (LF) character;") _T_RE_EOL \
  _T("  \\r  - carriage return (CR) character;") _T_RE_EOL \
  _T("  \\t  - tabulation character;") _T_RE_EOL \
  _T("  \\\\n - \\n;  \\\\t - \\t.") _T_RE_EOL \
  _T("SEE ALSO:") _T_RE_EOL \
  _T("  sel_loadfrom, sel_saveto") _T_RE_EOL,

  // IF
  _T("COMMAND:  if") _T_RE_EOL \
  _T("USAGE:") _T_RE_EOL \
  _T("  if <condition> goto <label>") _T_RE_EOL \
  _T("DESCRIPTION:") _T_RE_EOL \
  _T("  If the condition is true, jumps to the specified label.") _T_RE_EOL \
  _T("  If the condition is false, proceeds to the next line.") _T_RE_EOL \
  _T("  * If the specified label can not be found within the current script,") _T_RE_EOL \
  _T("  this command reports an error and proceeds to the next line.") _T_RE_EOL \
  _T("  ** You should always place \"if\" and \"goto\" on the same line.") _T_RE_EOL \
  _T("  Available conditions:") _T_RE_EOL \
  _T("  a == b  - equal:             1 == 1, \"Ab\" == \"Ab\"") _T_RE_EOL \
  _T("  a = b   - equal:             2 = 2,  \"Aa\" = \"Aa\"") _T_RE_EOL \
  _T("  a != b  - not equal:         1 != 2, \"Ab\" != \"AB\"") _T_RE_EOL \
  _T("  a <> b  - not equal:         1 <> 2, \"Aa\" <> \"A\"") _T_RE_EOL \
  _T("  a > b   - greater:           2 > 1,  \"Ab\" > \"Aa\"") _T_RE_EOL \
  _T("  a < b   - less:             -2 < 1,  \"a\" < \"ab\"") _T_RE_EOL \
  _T("  a >= b  - greater or equal:  0 >= 0, \"AB\" >= \"AA\"") _T_RE_EOL \
  _T("  a <= b  - less or equal:     1 <= 2, \"A\" <= \"AA\"") _T_RE_EOL \
  _T("  a ~= b  - equal no case:     \"AA\" ~= \"aa\"") _T_RE_EOL \
  _T("  * You can use variables and constants as the operands:") _T_RE_EOL \
  _T("    \"if $(var1) == $(var2) goto EqualVars\"") _T_RE_EOL \
  _T("    \"if $(var) != 10 goto NotEqualTo10\"") _T_RE_EOL \
  _T("    \"if ABC < $(var) goto GreaterThanABC\"") _T_RE_EOL \
  _T("  ** Each condition always expects two operands: the left one and the") _T_RE_EOL \
  _T("  right one. Thus, \"if $(var) goto Label\" is not valid - you should") _T_RE_EOL \
  _T("  specify either \"if $(var) != 0 goto Label\" for a number or") _T_RE_EOL \
  _T("  \"if \"$(var)\" != \"\" goto Label\" for a string.") _T_RE_EOL \
  _T("  *** If both operands are decimal numbers, a numeric comparison is") _T_RE_EOL \
  _T("  used. Otherwise the operands are compared as strings.") _T_RE_EOL \
  _T("  Thus, -2 < -1 (decimal numbers) and \"-2\" > \"-1\" (strings).") _T_RE_EOL \
  _T("  **** No additional calculations are performed during the comparison.") _T_RE_EOL \
  _T("  Thus, \"if 3 == 1 + 2 goto Label\" is not a valid numeric comparison") _T_RE_EOL \
  _T("  because \"1 + 2\" is not calculated and remains as is, so the string") _T_RE_EOL \
  _T("  comparison is used in this case (\"3\" is compared with \"1 + 2\").") _T_RE_EOL \
  _T("  All the calculations should be performed in advance.") _T_RE_EOL \
  _T("  ***** Notice the usage of double quotes and spaces between the") _T_RE_EOL \
  _T("  operands while comparing strings that contain '>', '<' and so on:") _T_RE_EOL \
  _T("    if \"1 >\" < \"2\" goto Label1  // \"1 >\" vs. \"2\"") _T_RE_EOL \
  _T("    if \"1\" > \"< 2\" goto Label2  // \"1\" vs. \"< 2\"") _T_RE_EOL \
  _T("EXAMPLES:") _T_RE_EOL \
  _T("  ////////////////////////") _T_RE_EOL \
  _T("  // Example 1:") _T_RE_EOL \
  _T("  // emulating \"for (i = 1; i <= 10; i += 1) { cout << i << endl; }\"") _T_RE_EOL \
  _T("  set i ~ 1                // initialization: \"i = 1\"") _T_RE_EOL \
  _T("  :for_i                   // start of the cycle") _T_RE_EOL \
  _T("  echo $(i)                // the cycle body: \"cout << i << endl;\"") _T_RE_EOL \
  _T("  set i ~ $(i) + 1         // iteration: \"i += 1\"") _T_RE_EOL \
  _T("  if $(i) <= 10 goto for_i // condition: \"i <= 10\"") _T_RE_EOL \
  _T("  // - equivalent to \"i = 1; do { cout << i << endl; i += 1; } while (i <= 10);\"") _T_RE_EOL \
  _T("  ////////////////////////") _T_RE_EOL \
  _T("  // Example 2:") _T_RE_EOL \
  _T("  // compare N leading characters of S1 with N trailing characters of S2") _T_RE_EOL \
  _T("  set S1 = extension  // input: S1") _T_RE_EOL \
  _T("  set S2 = some text  // input: S2") _T_RE_EOL \
  _T("  set N = 3           // initialization: N") _T_RE_EOL \
  _T("  set sub1 ~ substr 0 $(N) $(S1)   // get N leading characters of S1") _T_RE_EOL \
  _T("  set sub2 ~ substr -$(N) - $(S2)  // get N trailing characters of S2") _T_RE_EOL \
  _T("  if \"$(sub1)\" == \"$(sub2)\" goto Equal  // if") _T_RE_EOL \
  _T("  goto NotEqual  // else") _T_RE_EOL \
  _T("  :Equal") _T_RE_EOL \
  _T("  echo substrings are equal: \"$(sub1)\" == \"$(sub2)\"") _T_RE_EOL \
  _T("  goto Done") _T_RE_EOL \
  _T("  :NotEqual") _T_RE_EOL \
  _T("  echo substrings are not equal: \"$(sub1)\" != \"$(sub2)\"") _T_RE_EOL \
  _T("  goto Done") _T_RE_EOL \
  _T("  :Done") _T_RE_EOL \
  _T("  ////////////////////////") _T_RE_EOL \
  _T("  // Example 3:") _T_RE_EOL \
  _T("  // emulating \"IF 1 <= x AND x < 5\"") _T_RE_EOL \
  _T("  if 1 > $(x) goto @If_End  // opposite to 1 <= x") _T_RE_EOL \
  _T("  if $(x) >= 5 goto @If_End // opposite to x < 5") _T_RE_EOL \
  _T("  :@If_True") _T_RE_EOL \
  _T("  // the condition is true, do something...") _T_RE_EOL \
  _T("  echo True: 1 <= x < 5") _T_RE_EOL \
  _T("  :@If_End") _T_RE_EOL \
  _T("  ////////////////////////") _T_RE_EOL \
  _T("  // Example 4:") _T_RE_EOL \
  _T("  // emulating \"IF s == \'ABC\' OR s == \'DEF\' THEN ... ELSE ...\"") _T_RE_EOL \
  _T("  if \"$(s)\" == \"ABC\" goto @If_True") _T_RE_EOL \
  _T("  if \"$(s)\" == \"DEF\" goto @If_True") _T_RE_EOL \
  _T("  goto @If_Else") _T_RE_EOL \
  _T("  :@If_True") _T_RE_EOL \
  _T("  // the condition is true, do something...") _T_RE_EOL \
  _T("  echo True: s is either \"ABC\" or \"DEF\"") _T_RE_EOL \
  _T("  goto @If_End") _T_RE_EOL \
  _T("  :@If_Else") _T_RE_EOL \
  _T("  // the condition is false, do something...") _T_RE_EOL \
  _T("  echo False: s is neither \"ABC\" nor \"DEF\"") _T_RE_EOL \
  _T("  :@If_End") _T_RE_EOL \
  _T("REMARKS:") _T_RE_EOL \
  _T("  As you can see, different IF-GOTO constructions in combination with") _T_RE_EOL \
  _T("  another GOTOs and LABELs can serve as IF-ELSE, FOR and WHILE.") _T_RE_EOL \
  _T("  This allows to apply more-or-less complicated conditional logic") _T_RE_EOL \
  _T("  inside NppExec\'s scripts.") _T_RE_EOL \
  _T("SEE ALSO:") _T_RE_EOL \
  _T("  goto, label, set") _T_RE_EOL,

  // LABEL
  _T("COMMAND:  label, :") _T_RE_EOL \
  _T("USAGE:") _T_RE_EOL \
  _T("  label <LabelName>") _T_RE_EOL \
  _T("  :<LabelName>") _T_RE_EOL \
  _T("DESCRIPTION:") _T_RE_EOL \
  _T("  Declares a label within the current script.") _T_RE_EOL \
  _T("  You can jump to a label using the GOTO command.") _T_RE_EOL \
  _T("EXAMPLES:") _T_RE_EOL \
  _T("  :Label1") _T_RE_EOL \
  _T("  label MyLabelB") _T_RE_EOL \
  _T("  :C") _T_RE_EOL \
  _T("  label 4") _T_RE_EOL \
  _T("REMARKS:") _T_RE_EOL \
  _T("  You can give any name to your label. Leading and trailing spaces are") _T_RE_EOL \
  _T("  ignored.") _T_RE_EOL \
  _T("  Each label \"lives\" within a script where it is declared. You can not") _T_RE_EOL \
  _T("  jump to a label located inside another script - either outer or inner") _T_RE_EOL \
  _T("  one. (And this is the difference between labels and variables: each") _T_RE_EOL \
  _T("  script has its own set of labels which are not shared between scripts,") _T_RE_EOL \
  _T("  whereas any declared variable is visible and exists everywhere.)") _T_RE_EOL \
  _T("  Thus, you can declare labels with the same names inside different") _T_RE_EOL \
  _T("  scripts (but not within one script). E.g. you may have labels named") _T_RE_EOL \
  _T("  A, B and C inside SomeScript1 and the same label names A, B and C") _T_RE_EOL \
  _T("  inside SomeScript2 - and these labels are different because they") _T_RE_EOL \
  _T("  \"exist\" in different scripts, even if SomeScript1 then uses") _T_RE_EOL \
  _T("  NPP_EXEC to execute SomeScript2 or vice versa.") _T_RE_EOL \
  _T("SEE ALSO:") _T_RE_EOL \
  _T("  goto, if") _T_RE_EOL,

  // GOTO
  _T("COMMAND:  goto") _T_RE_EOL \
  _T("USAGE:") _T_RE_EOL \
  _T("  goto <LabelName>") _T_RE_EOL \
  _T("DESCRIPTION:") _T_RE_EOL \
  _T("  Jumps to the label") _T_RE_EOL \
  _T("EXAMPLES:") _T_RE_EOL \
  _T("  ////////////////////////") _T_RE_EOL \
  _T("  // Theoretical example:") _T_RE_EOL \
  _T("  // Pure GOTOs with short label names") _T_RE_EOL \
  _T("  goto 1") _T_RE_EOL \
  _T("  :2") _T_RE_EOL \
  _T("  goto 3") _T_RE_EOL \
  _T("  :1") _T_RE_EOL \
  _T("  goto 2") _T_RE_EOL \
  _T("  :3") _T_RE_EOL \
  _T("  ////////////////////////") _T_RE_EOL \
  _T("  // Theoretical example:") _T_RE_EOL \
  _T("  // Endless cycle with neat label name...") _T_RE_EOL \
  _T("  :Neat Label Name") _T_RE_EOL \
  _T("  goto Neat Label Name") _T_RE_EOL \
  _T("  // ...now you can either wait until the number of executed GOTOs") _T_RE_EOL \
  _T("  // exceeds the value of GoTo_MaxCount - or just close the Console.") _T_RE_EOL \
  _T("  // When the Console is closed, the script execution is stopped.") _T_RE_EOL \
  _T("  ////////////////////////") _T_RE_EOL \
  _T("  // Practical example:") _T_RE_EOL \
  _T("  // IF-GOTO") _T_RE_EOL \
  _T("  if \"$(a)\" == \"\" goto @a_is_empty") _T_RE_EOL \
  _T("  echo a is not empty: a=$(a)") _T_RE_EOL \
  _T("  goto done") _T_RE_EOL \
  _T("  :@a_is_empty") _T_RE_EOL \
  _T("  echo a is empty") _T_RE_EOL \
  _T("  :done") _T_RE_EOL \
  _T("REMARKS:") _T_RE_EOL \
  _T("  You can give any name to your label. Leading and trailing spaces are") _T_RE_EOL \
  _T("  ignored.") _T_RE_EOL \
  _T("  Each label \"lives\" within a script where it is declared. You can not") _T_RE_EOL \
  _T("  jump to a label located inside another script - either outer or inner") _T_RE_EOL \
  _T("  one. (And this is the difference between labels and variables: each") _T_RE_EOL \
  _T("  script has its own set of labels which are not shared between scripts,") _T_RE_EOL \
  _T("  whereas any declared variable is visible and exists everywhere.)") _T_RE_EOL \
  _T("SEE ALSO:") _T_RE_EOL \
  _T("  label, if") _T_RE_EOL,

  // ELSE
  _T("COMMAND:  else") _T_RE_EOL \
  _T("USAGE:") _T_RE_EOL \
  _T("  if <condition>") _T_RE_EOL \
  _T("    ...") _T_RE_EOL \
  _T("  else if <condition>") _T_RE_EOL \
  _T("    ...") _T_RE_EOL \
  _T("  else") _T_RE_EOL \
  _T("    ...") _T_RE_EOL \
  _T("  endif") _T_RE_EOL \
  _T("DESCRIPTION:") _T_RE_EOL \
  _T("  Implements IF ... ELSE ... ENDIF") _T_RE_EOL \
  _T("  or         IF ... ELSE IF ... ENDIF") _T_RE_EOL \
  _T("  or         IF ... ELSE IF ... ELSE ... ENDIF") _T_RE_EOL \
  _T("  or         IF ... ELSE IF ... ELSE IF ... ENDIF") _T_RE_EOL \
  _T("  and similar constructions.") _T_RE_EOL \
  _T("EXAMPLES:") _T_RE_EOL \
  _T("  see: if") _T_RE_EOL \
  _T("SEE ALSO:") _T_RE_EOL \
  _T("  if, endif") _T_RE_EOL,

  // ENDIF
  _T("COMMAND:  endif") _T_RE_EOL \
  _T("USAGE:") _T_RE_EOL \
  _T("  if <condition>") _T_RE_EOL \
  _T("    ...") _T_RE_EOL \
  _T("  endif") _T_RE_EOL \
  _T("DESCRIPTION:") _T_RE_EOL \
  _T("  endif") _T_RE_EOL \
  _T("EXAMPLES:") _T_RE_EOL \
  _T("  see: else") _T_RE_EOL \
  _T("SEE ALSO:") _T_RE_EOL \
  _T("  if, else") _T_RE_EOL

};

extern DWORD WINAPI dwCreateConsoleProcess(LPVOID);

typedef BOOL (WINAPI *TRANSPARENCYFUNC)(HWND, COLORREF, BYTE, DWORD);

namespace ConsoleDlg 
{
  void    AddCommandToHistoryList(const tstr& S);
  
  void    OnClose(HWND hDlg);
  void    OnDestroy(HWND hDlg);
  void    OnInitDialog(HWND hDlg);
  INT_PTR OnNotify(HWND hDlg, LPARAM lParam);
  void    OnShowWindow(HWND hDlg);
  void    OnSize(HWND hDlg);
  bool    IsConsoleHelpCommand(const tstr& S);

  void    loadCmdVarsList(void);
  void    loadCmdHistory(void);
  void    saveCmdHistory(void);

  void    enableFindControls(bool bEnable);

  void    doWordWrap(HWND hDlg);
  void    doHideToggled(HWND hDlg);

  bool    GoToLineIfWarningAnalyzerMatch(CAnyRichEdit& Edit, const int nLine);

//#undef _consoledlg_re_subclass_
#define _consoledlg_re_subclass_

#ifdef _consoledlg_re_subclass_
  
  LRESULT CALLBACK RichEditWndProc(
    HWND   hEd, 
    UINT   uMessage, 
    WPARAM wParam, 
    LPARAM lParam);

  WNDPROC OriginalRichEditProc = NULL;

#endif

#undef _consoledlg_keys_log_
//#define _consoledlg_keys_log_

#ifdef _consoledlg_keys_log_
  
  #include <stdio.h>
  
  FILE* fLog = NULL;

#endif

  LRESULT CALLBACK findEditWndProc(HWND, UINT, WPARAM, LPARAM);
  WNDPROC findEditWndProc0 = NULL;

  HICON   hTabIcon = NULL;
  HFONT   hFont = NULL;
  LOGFONT LogFont = { 15, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET,
                                   0, 0, 0, 0, _T("Courier New") };
  tTbData dockData;

  CListT<tstr>      CmdHistoryList;
  CListT<tstr>      CmdVarsList;
  CListItemT<tstr>* pCmdHistoryItemPtr = NULL;
  bool              bCmdHistoryUpdated = false;

  bool              bWordWrap = false;
  bool              bHideToggled = false;
  bool              bIsDocked = false;

  bool              bFindControls = false;
  CAnyCheckBox      chFindCase;
  CAnyWindow        edFind;
  CAnyWindow        btFindClose;
  CAnyCheckBox      chWordWrap;
  CAnyCheckBox      chHideToggled;

  CDirFileLister    tabDirFileLister;

  int               GoToError_nCurrentLine = -1;

}


INT_PTR CALLBACK ConsoleDlgProc(
  HWND   hDlg, 
  UINT   uMessage, 
  WPARAM wParam, 
  LPARAM lParam)
{
  
  /*
  if (uMessage == WM_ACTIVATE)
  {
    MessageBox(NULL, "WM_ACTIVATE", "ConsoleDlgProc", MB_OK);
  }
  else
  */
  
  if (uMessage == WM_COMMAND)
  {
    switch (LOWORD(wParam)) 
    {
      case IDOK: // Save
      {
        if (ConsoleDlg::bFindControls)
        {
          ConsoleDlg::findEditWndProc(ConsoleDlg::edFind.m_hWnd, WM_KEYDOWN, VK_RETURN, 0);
        }
        else
        {
          ConsoleDlg::OnClose(hDlg);
        }
        return 1;
      }
      case IDCANCEL: // Cancel
      case IDC_BT_FINDCLOSE:
      {
        if (ConsoleDlg::bFindControls)
        {
          ConsoleDlg::enableFindControls(false);
          ConsoleDlg::OnSize(hDlg);
        }
        else
        {
          ConsoleDlg::OnClose(hDlg);
        }
        return 1;
      }
      case IDC_CH_CASE:
      {
        if (HIWORD(wParam) == BN_CLICKED)
        {
          if (ConsoleDlg::bFindControls)
          {
            ConsoleDlg::edFind.SetFocus();
          }
          else
          {
            Runtime::GetNppExec().GetConsole().GetConsoleEdit().SetFocus();
          }
          return 1;
        }
        break;
      }
      case IDC_CH_WORDWRAP:
      {
        if (HIWORD(wParam) == BN_CLICKED)
        {
          ConsoleDlg::doWordWrap(hDlg);
          return 1;
        }
        break;
      }
      case IDC_CH_HIDETOGGLED:
      {
        if (HIWORD(wParam) == BN_CLICKED)
        {
          ConsoleDlg::doHideToggled(hDlg);
          return 1;
        }
        break;
      }
      default:
        break;
    }
  }

  else if (uMessage == WM_SYSCOMMAND)
  {
    if (wParam == SC_CLOSE)
    {
      ConsoleDlg::OnClose(hDlg);
      return 1;
    }
  }

  else if (uMessage == WM_NOTIFY)
  {
    ConsoleDlg::OnNotify(hDlg, lParam);
  }

  else if (uMessage == WM_SIZE)
  {
    ConsoleDlg::OnSize(hDlg);
  }

  /*
  else if (uMessage == WM_ACTIVATE)
  {
      // we don't get here...
      if ( LOWORD(wParam) == WA_INACTIVE )
          ConsoleDlg::EnableTransparency(true);
      else
          ConsoleDlg::EnableTransparency(false);
  }
  */

  else if (uMessage == WM_INITDIALOG)
  {
    ConsoleDlg::OnInitDialog(hDlg);
  }

  else if (uMessage == WM_DESTROY)
  {
    ConsoleDlg::OnDestroy(hDlg);
  }

  else if (uMessage == WM_SHOWWINDOW)
  {
    if ( wParam ) // the window is being shown
      ConsoleDlg::OnShowWindow(hDlg);
  }

  else if (uMessage == WM_LOCKCONSOLEPOS)
  {
    nConsoleFirstUnlockedPos = (INT) lParam;
    if (Runtime::GetNppExec().GetConsole().IsScrollToEnd())
    {
      CAnyRichEdit Edit;
      Edit.m_hWnd = GetDlgItem(hDlg, IDC_RE_CONSOLE);
      Edit.ExSetSel(nConsoleFirstUnlockedPos, nConsoleFirstUnlockedPos);
    }
  }

  /*
  // This doesn't work.
  // It seems these messages are intercepted
  //   before they can be processed here.
  else if (uMessage == WM_NCLBUTTONDOWN || uMessage == WM_LBUTTONDOWN)
  {
    MessageBox(NULL, "left mouse btn pressed", "ConsoleDlgProc", MB_OK);
  }
  */
  
  return 0;
}


void ConsoleDlg::AddCommandToHistoryList(const tstr& S)
{
  if (S.length() > 0)
  {
    if (Runtime::GetOptions().GetBool(OPTB_CONSOLE_CMDHISTORY))
    {
      CListItemT<tstr>* p = CmdHistoryList.FindExact(S);
      if (p)
      {
        if (pCmdHistoryItemPtr == p)
        {
          pCmdHistoryItemPtr = NULL;
        }
        CmdHistoryList.Delete(p);
      }
      if (CmdHistoryList.GetCount() >= Runtime::GetOptions().GetInt(OPTI_CMDHISTORY_MAXITEMS))
      {
        if (pCmdHistoryItemPtr == CmdHistoryList.GetFirst())
        {
          pCmdHistoryItemPtr = NULL;
        }
        CmdHistoryList.DeleteFirst();
      }
      CmdHistoryList.Add(S);
      bCmdHistoryUpdated = true;
      pCmdHistoryItemPtr = NULL;
    }
  }
}


void ConsoleDlg::OnClose(HWND hDlg)
{
//  if ( !IsWindowVisible(hDlg) )
//    return;
    
  bool bClose = true;

  if (Runtime::GetNppExec()._consoleProcessIsRunning)
  {
    //bClose = (MessageBox(hDlg, _T("Child process is still active.\nTerminate child process?"), 
    //    _T("NppExec::CloseConsole"), MB_YESNO | MB_ICONWARNING) == IDYES);

    Runtime::GetNppExec().m_SuspendedScriptAction.setActive(false);
    Runtime::GetNppExec().m_SuspendedScriptAction.setAction(CSuspendedScriptAction::AT_CLOSECONSOLE);

    bClose = Runtime::GetNppExec().CanStartScriptOrCommand();
  }
  
  if (bClose)
  {
    Runtime::GetNppExec().setConsoleVisible(false);
    // When _consoleIsVisible is set to false, the child process 
    // is terminated within CNppExec::CreateChildProcess.
    // Therefore it's very important to process
    // closing of this dialog here.
    /*
    Runtime::GetNppExec().SendNppMsg(WM_MODELESSDIALOG, 
        (WPARAM) MODELESSDIALOGREMOVE, (LPARAM) hDlg); 
    ::DestroyWindow(hDlg);
    */

    /*
    HMENU hMenu = Runtime::GetNppExec().GetNppMainMenu();
    if (hMenu)
    {
      EnableMenuItem(hMenu, g_funcItem[N_CONSOLE_FONT]._cmdID,
        MF_BYCOMMAND | (Runtime::GetNppExec()._consoleIsVisible ? MF_ENABLED : MF_GRAYED) );
    }
    */

    // ... and finally

    /*
    // It does not work.
    // But it's not my fault, IMHO.
    ::SendMessage(Runtime::GetNppExec().GetScintillaHandle(), SCI_SETFOCUS, (WPARAM) TRUE, 0);
    ::SendMessage(Runtime::GetNppExec().GetScintillaHandle(), SCI_GRABFOCUS, (WPARAM) TRUE, 0);
    */

    ::SetFocus( Runtime::GetNppExec().GetScintillaHandle() );
  }
  else
  {
    ::SetWindowLong(hDlg, DWL_MSGRESULT, 1); // 1 means "don't close"
    //Runtime::GetNppExec().SendNppMsg(WM_DMM_SHOW, 0, (LPARAM) hDlg);
    //::ShowWindow(Runtime::GetNppExec().GetConsole().GetParentWnd(), SW_SHOW);
  }
}

void ConsoleDlg::OnDestroy(HWND hDlg)
{
  //MessageBox(NULL, "WM_DESTROY", "ConsoleDlg", MB_OK);
   
  /*saveCmdHistory();*/

  if (hTabIcon)
  {
    ::DestroyIcon(hTabIcon);
    hTabIcon = NULL;
  }

  HWND hDlgItem;

// get rid of the "conversion from 'LONG_PTR' to 'LONG'" shit
// (Microsoft itself does not know what is 'LONG_PTR'?)
#pragma warning(push)
#pragma warning(disable: 4244)

#ifdef _consoledlg_re_subclass_
  hDlgItem = GetDlgItem(hDlg, IDC_RE_CONSOLE);
  if (hDlgItem)
    SetWindowLongPtr(hDlgItem, GWLP_WNDPROC, (LONG_PTR) OriginalRichEditProc);
#endif

  hDlgItem = GetDlgItem(hDlg, IDC_ED_FIND);
  if (hDlgItem)
    SetWindowLongPtr(hDlgItem, GWLP_WNDPROC, (LONG_PTR) findEditWndProc0);

#pragma warning(pop)

#ifdef _consoledlg_keys_log_
  if (fLog)
  {
    fclose(fLog);
    fLog = NULL;
  }
#endif

  Runtime::GetNppExec().GetConsole().SetParentWnd(NULL);
}

void ConsoleDlg::OnInitDialog(HWND hDlg)
{
  CAnyRichEdit Edit;
  DWORD        dwEventMask;

  edFind.m_hWnd = GetDlgItem(hDlg, IDC_ED_FIND);
  chFindCase.m_hWnd = GetDlgItem(hDlg, IDC_CH_CASE);
  btFindClose.m_hWnd = GetDlgItem(hDlg, IDC_BT_FINDCLOSE);
  chWordWrap.m_hWnd = GetDlgItem(hDlg, IDC_CH_WORDWRAP);
  chHideToggled.m_hWnd = GetDlgItem(hDlg, IDC_CH_HIDETOGGLED);
  
  enableFindControls(false);
  
  Edit.m_hWnd = GetDlgItem(hDlg, IDC_RE_CONSOLE);
  Runtime::GetNppExec().GetConsole().SetConsoleWnd( Edit.m_hWnd );
  Runtime::GetNppExec().GetConsole().SetParentWnd( hDlg );

// get rid of the "conversion from 'LONG_PTR' to 'LONG'" shit
// get rid of the "conversion from 'LONG' to 'WNDPROC'" shit
// (Microsoft itself does not know what is 'LONG_PTR'?)
#pragma warning(push)
#pragma warning(disable: 4244)
#pragma warning(disable: 4312)

#ifdef _consoledlg_re_subclass_
  OriginalRichEditProc = (WNDPROC) SetWindowLongPtr(Edit.m_hWnd, 
    GWLP_WNDPROC, (LONG_PTR) RichEditWndProc);
#endif

  findEditWndProc0 = (WNDPROC) SetWindowLongPtr(edFind.m_hWnd,
    GWLP_WNDPROC, (LONG_PTR) findEditWndProc);

#pragma warning(pop)

#ifdef _consoledlg_keys_log_
  char test_str[128];
  
  fLog = fopen("Keys.log", "w");

  if (fLog)
  {
    lstrcpyA(test_str, "ConsoleDlg::OnInitDialog ...\r\n");
    fwrite(test_str, sizeof(char), lstrlenA(test_str), fLog);
  }
#endif

  dwEventMask = Edit.GetEventMask();

#ifdef _consoledlg_keys_log_
  if (fLog)
  {
    wsprintfA(test_str, "Original EventMask: %08X\r\n", dwEventMask);
    fwrite(test_str, sizeof(char), lstrlenA(test_str), fLog);
  }
#endif

  if (!(dwEventMask & (ENM_KEYEVENTS | ENM_MOUSEEVENTS)))
  {
    dwEventMask |= (ENM_KEYEVENTS | ENM_MOUSEEVENTS);
    Edit.SetEventMask(dwEventMask);

#ifdef _consoledlg_keys_log_
    if (fLog)
    {
      wsprintfA(test_str, "Desired EventMask: %08X\r\n", dwEventMask);
      fwrite(test_str, sizeof(char), lstrlenA(test_str), fLog);
      wsprintfA(test_str, "Set EventMask: %08X\r\n", Edit.GetEventMask());
      fwrite(test_str, sizeof(char), lstrlenA(test_str), fLog);
    }
#endif
  
  }

  if (Runtime::GetNppExec()._consoleFont)
  {
    int dataSize = 0;
    const LOGFONT* pLogFont = (const LOGFONT *) Runtime::GetOptions().GetData(OPTD_CONSOLE_FONT, &dataSize);
    Runtime::GetNppExec().GetConsole().GetConsoleEdit().SetFont(pLogFont);
  }

  Runtime::GetNppExec().GetConsole().UpdateColours();

  Edit.ExLimitText(Runtime::GetOptions().GetInt(OPTI_RICHEDIT_MAXTEXTLEN));

  nConsoleFirstUnlockedPos = 0;

  loadCmdVarsList();
  loadCmdHistory();
    
  bWordWrap = !Runtime::GetOptions().GetBool(OPTB_CONSOLE_WORDWRAP);
  // following doWordWrap() inverts the value of bWordWrap :)
  doWordWrap(hDlg);

  bHideToggled = !Runtime::GetOptions().GetBool(OPTB_CONSOLE_HIDETOGGLED);
  // following doHideToggled() inverts the value of bHideToggled :)
  doHideToggled(hDlg);
  
  // docking

  static tstr conDlgTitle = _T(" Console ");
  tstr name = PLUGIN_NAME;
  if (name.length() > 0)  ::CharLower( name.c_str() );
  int pos = name.Find( _T("nppexec") );
  if (pos >= 0)
  {
    conDlgTitle.Insert(1, PLUGIN_NAME, pos);
    conDlgTitle.Insert(conDlgTitle.length() - 1, PLUGIN_NAME + pos + 7);
  }

  RECT           rect;
  
  rect.left   = 0;
  rect.top    = 0;
  rect.right  = 0;
  rect.bottom = 0;

  hTabIcon = (HICON) ::LoadImage( (HINSTANCE) Runtime::GetNppExec().m_hDllModule, 
    MAKEINTRESOURCE(IDI_CONSOLEICON), IMAGE_ICON, 0, 0, 
    LR_LOADMAP3DCOLORS | LR_LOADTRANSPARENT );

  int dlgId = N_SHOWCONSOLE;
  if ( Runtime::GetOptions().GetInt(OPTI_CONSOLE_VISIBLE) != CAdvOptDlg::CON_AUTO )
      dlgId = -1;

  dockData.hClient       = hDlg;
  dockData.pszName       = conDlgTitle.c_str();
  dockData.dlgID         = dlgId;
  dockData.uMask         = DWS_DF_CONT_BOTTOM | DWS_ICONTAB;
  dockData.hIconTab      = hTabIcon;
  dockData.pszAddInfo    = NULL;
  dockData.rcFloat       = rect;
  dockData.iPrevCont     = -1; // Don said: "this initialization is necessary"
  dockData.pszModuleName = PLUGIN_NAME_DLL;

  
}

void ConsoleDlg::DockDialog()
{
  static bool bIsInitializedDocked = false;

  if ( !bIsInitializedDocked )
  {
    Runtime::GetNppExec().SendNppMsg( NPPM_DMMREGASDCKDLG, 0, (LPARAM) &dockData );
  
    Runtime::GetNppExec().SendNppMsg( NPPM_MODELESSDIALOG, 
        (WPARAM) MODELESSDIALOGADD, (LPARAM) Runtime::GetNppExec().GetConsole().GetParentWnd() );
  
    bIsInitializedDocked = true;
  }
}

bool ConsoleDlg::IsDocked()
{
  return bIsDocked;
}

const TCHAR* ConsoleDlg::GetTitle()
{
  return dockData.pszName;
}

bool ConsoleDlg::EnableTransparency(bool bEnable)
{
    static bool bTransparencyFuncInitialized = false;
    static TRANSPARENCYFUNC lpTransparencyFunc = NULL;

    if ( !bTransparencyFuncInitialized )
    {
        HMODULE hUser32 = ::GetModuleHandle(_T("user32"));
        if ( hUser32 )
            lpTransparencyFunc = (TRANSPARENCYFUNC) ::GetProcAddress(hUser32, "SetLayeredWindowAttributes");
        
        bTransparencyFuncInitialized = true;
    }
    
    if ( lpTransparencyFunc && !IsDocked() )
    {
        HWND hDlg = NULL;
        while ( hDlg = ::FindWindowEx(NULL, hDlg, _T("#32770"), GetTitle()) )
        {
            if ( ::GetParent(hDlg) == Runtime::GetNppExec().m_nppData._nppHandle )
                break;
        }

        if ( hDlg )
        {
            DWORD dwExStyle = ::GetWindowLong(hDlg, GWL_EXSTYLE);
            if ( bEnable && bHideToggled )
            {
                if ( !(dwExStyle & 0x00080000) )
                {
                    dwExStyle |= 0x00080000; // WS_EX_LAYERED
                    ::SetWindowLong(hDlg, GWL_EXSTYLE, dwExStyle);
                    lpTransparencyFunc(hDlg, 0, 48, 0x00000002); 
                    return true;
                }
            }
            else
            {
                if ( dwExStyle & 0x00080000 )
                {
                    dwExStyle -= 0x00080000; // WS_EX_LAYERED
                    ::SetWindowLong(hDlg, GWL_EXSTYLE, dwExStyle);
                    return true;
                }
            }
        }
    }

    return false;
}

void ConsoleDlg::GoToError(int direction)
{
    CAnyRichEdit& Edit = Runtime::GetNppExec().GetConsole().GetConsoleEdit();
    int nLineCount = Edit.GetLineCount();
    int nCheckLine = GoToError_nCurrentLine;
    nCheckLine += direction;

    if ( nCheckLine >= nLineCount )
    {
        nCheckLine = nLineCount - 1;
    }
    else if ( nCheckLine < 0 )
    {
        nCheckLine = 0;
    }

    while( (nCheckLine >= 0) && (nCheckLine < nLineCount) )
    {
        if ( GoToLineIfWarningAnalyzerMatch(Edit, nCheckLine) )
        {
            GoToError_nCurrentLine = nCheckLine;
            break;
        }
        nCheckLine += direction;
    }
}

bool ConsoleDlg::GoToLineIfWarningAnalyzerMatch(CAnyRichEdit& Edit, const int nLine)
{
    //#ifdef _NIB            
    TCHAR ch[CONSOLECOMMAND_BUFSIZE]; 
    int   nLineLength = Edit.GetLine( nLine, ch, CONSOLECOMMAND_BUFSIZE-1 );
    ch[CONSOLECOMMAND_BUFSIZE-1] = 0; // just in case
        
    // RichEdit returns a string with "\r\n" at the end. Stupid M$ !!!
    int len = lstrlen(ch);
    while ( (len > 0) && (ch[len-1] == '\n') )
    {
        --len;
        ch[len] = 0;
    }
    while ( (len > 0) && (ch[len-1] == '\r') )
    {
        --len;
        ch[len] = 0;
    }

    if ( Runtime::GetOptions().GetBool(OPTB_NPE_DEBUGLOG) )
    {
        Runtime::GetLogger().Clear();
        Runtime::GetLogger().Add(   _T("") );
        Runtime::GetLogger().Add(   _T("; Console\'s line double-clicked") );
        Runtime::GetLogger().Add(   _T("; WarningAnalyzer info") );
        Runtime::GetLogger().Add(   _T("; {") );
        Runtime::GetLogger().AddEx( _T(";    Input line: \"%s\""), ch );
    }

    //::CharLower( ch );  -  seems it's useless here...
    if ( Runtime::GetWarningAnalyzer().match( ch ) )
    {
        
        if ( Runtime::GetOptions().GetBool(OPTB_NPE_DEBUGLOG) )
        {
            TCHAR szRcgnMask[OUTPUTFILTER_BUFSIZE];

            Runtime::GetLogger().Add(   _T(";    Match result: true") );
            Runtime::GetWarningAnalyzer().GetMask( Runtime::GetWarningAnalyzer().GetLastFoundIndex(), szRcgnMask, OUTPUTFILTER_BUFSIZE - 1 );
            Runtime::GetLogger().AddEx( _T(";    Active filter: \"%s\""), szRcgnMask );
            tstr sMask = szRcgnMask;
            if ( Runtime::GetNppExec().GetMacroVars().CheckAllMacroVars(sMask, false) )
            {
                Runtime::GetLogger().AddEx( _T(";    Active filter -> \"%s\""), sMask.c_str() );
            }
            if ( Runtime::GetWarningAnalyzer().GetFileName()[0] )
                Runtime::GetLogger().AddEx( _T(";    * Parsed File Name: \"%s\""), Runtime::GetWarningAnalyzer().GetFileName() );
            else
                Runtime::GetLogger().Add(   _T(";    * Parsed File Name: <none>") );
            if ( Runtime::GetWarningAnalyzer().GetLineNumber() )
                Runtime::GetLogger().AddEx( _T(";    * Parsed Line Number: %d"), Runtime::GetWarningAnalyzer().GetLineNumber() );
            else
                Runtime::GetLogger().Add(   _T(";    * Parsed Line Number: <none>") );
            if ( Runtime::GetWarningAnalyzer().GetCharNumber() )
                Runtime::GetLogger().AddEx( _T(";    * Parsed Char Position: %d"), Runtime::GetWarningAnalyzer().GetCharNumber() );
            else
                Runtime::GetLogger().Add(   _T(";    * Parsed Char Position: <none>") );
            Runtime::GetLogger().Add(   _T("; }") );
        }
        
        /*
        tstr S = "\"";
        S += Runtime::GetWarningAnalyzer().GetFileName();
        S += "\"";
        Runtime::GetNppExec().ShowWarning(S.c_str());
        */
        if ( Runtime::GetWarningAnalyzer().GetLineNumber() || Runtime::GetWarningAnalyzer().GetFileName()[0] )
        {
            int nLineStartIndex = Edit.LineIndex(nLine);
            Edit.ExSetSel(nLineStartIndex, nLineStartIndex + nLineLength);
            int nLine1st = (int) Edit.SendMsg(EM_GETFIRSTVISIBLELINE, 0, 0);
            if ( nLine < nLine1st )
            {
                Edit.LineScroll(nLine - nLine1st);
            }
            
            if ( Runtime::GetWarningAnalyzer().GetFileName()[0] )
            {
                tstr fileName = Runtime::GetWarningAnalyzer().GetFileName();

                Runtime::GetNppExec().nppConvertToFullPathName(fileName, true);
                
                Runtime::GetNppExec().SendNppMsg( NPPM_DOOPEN
                             , (WPARAM) 0
                             , (LPARAM) fileName.c_str()
                             );
            }

            HWND hSciWnd = Runtime::GetNppExec().GetScintillaHandle();
            ::SendMessage( hSciWnd
                         , SCI_GOTOLINE
                         , (WPARAM) (Runtime::GetWarningAnalyzer().GetLineNumber() - 1)
                         , (LPARAM) 0 
                         );

            if ( Runtime::GetWarningAnalyzer().GetCharNumber() )
            {
                // position of the start of the line
                int pos = (int) ::SendMessage( hSciWnd
                                    , SCI_POSITIONFROMLINE
                                    , (Runtime::GetWarningAnalyzer().GetLineNumber() - 1)
                                    , 0
                                    );

                if ( pos >= 0 )
                {
                    // document's codepage
                    int nSciCodePage = (int) ::SendMessage( hSciWnd
                                                 , SCI_GETCODEPAGE
                                                 , 0
                                                 , 0 
                                                 );
                    if ( nSciCodePage == 0 )
                    {
                        // ANSI: one-byte characters
                        pos += Runtime::GetWarningAnalyzer().GetCharNumber() - 1;
                    }
                    else
                    {
                        // Multi-byte
                        int nChars = Runtime::GetWarningAnalyzer().GetCharNumber();
                        while ( --nChars > 0 )
                        {
                            pos = (int) ::SendMessage( hSciWnd
                                                     , SCI_POSITIONAFTER
                                                     , pos
                                                     , 0
                                                     );
                        }
                    }
                    
                    // set the position
                    ::SendMessage( hSciWnd
                                 , SCI_GOTOPOS
                                 , (WPARAM) pos
                                 , (LPARAM) 0 
                                 );
                }
            }

            /*
            ::SendMessage( Runtime::GetNppExec().GetScintillaHandle()
                         , SCI_SETFOCUS
                         , 1  // <-- bool focus
                         , 0
                         );
            */
            ::SetFocus( Runtime::GetNppExec().GetScintillaHandle() );
            return true;
        }

    }
    else
    {
        
        if ( Runtime::GetOptions().GetBool(OPTB_NPE_DEBUGLOG) )
        {
            Runtime::GetLogger().Add(   _T(";    Match result: false (no matching filter found)") );
            Runtime::GetLogger().Add(   _T("; }") );
        }
        
    }
    //#endif

    return false;
}

INT_PTR ConsoleDlg::OnNotify(HWND hDlg, LPARAM lParam)
{
  NMHDR* pnmh = (NMHDR*) lParam;

  // >>> EN_MSGFILTER
  if (pnmh->code == EN_MSGFILTER)
  {
    static bool  bCommandEntered = false;
    static bool  bDoubleClkEntered = false;

    CAnyRichEdit Edit;
    MSGFILTER*   lpmsgf = (MSGFILTER*) lParam;

    Edit.m_hWnd = GetDlgItem(hDlg, IDC_RE_CONSOLE);

    // All the following code (for EN_MSGFILTER)
    // is never executed under 'wine' in Linux
    // (tested in Mandriva Linux 2007).
    // I don't know why it doesn't work there -
    // so, if you do, let me know :-)

#ifdef _consoledlg_keys_log_
    if (fLog)
    {
      int  log_str_len;
      char log_str[256];

      static bool bfrst = true;
      if (bfrst)
      {
        lstrcpyA(log_str, "Inside EN_MSGFILTER\r\n\r\n");
        fwrite(log_str, sizeof(char), lstrlenA(log_str), fLog);
        bfrst = false;
      }


      if ((lpmsgf->msg == WM_KEYDOWN) || 
          (lpmsgf->msg == WM_KEYUP))
      {
        const char* fmt = (lpmsgf->msg == WM_KEYDOWN) ? 
          "WM_KEYDOWN,  0x%02X  :  " : "WM_KEYUP,  0x%02X  :  ";
        log_str_len = wsprintfA(log_str, fmt, lpmsgf->wParam);
        fwrite(log_str, sizeof(char), log_str_len, fLog);
      }
      else if (lpmsgf->msg == WM_CHAR)
      {
        const char* fmt = "WM_CHAR,  %c  [0x%02X]  :  ";
        log_str_len = wsprintfA(log_str, fmt, lpmsgf->wParam, lpmsgf->wParam);
        fwrite(log_str, sizeof(char), log_str_len, fLog);
      }
    }
#endif

    // >>> VK_ESCAPE
    if ((lpmsgf->wParam == VK_ESCAPE) &&
        ((lpmsgf->msg == WM_KEYDOWN) || 
         (lpmsgf->msg == WM_KEYUP) || 
         (lpmsgf->msg == WM_CHAR)))
    {

#ifdef _consoledlg_keys_log_
        if (fLog)
        {
          const char* logstr = "VK_ESCAPE";
          fwrite(logstr, sizeof(char), lstrlenA(logstr), fLog);
        }
#endif
      
      if (bFindControls)
      {
        enableFindControls(false);
        OnSize(hDlg);
      }
      lpmsgf->wParam = 0;
      return 0;
    }
    // <<< VK_ESCAPE

    /////////////////////////////////////////////////////////////////////////
    // original code by Nicolas Babled, modified by DV
        
    // >>> WM_LBUTTONUP
    if (lpmsgf->msg == WM_LBUTTONUP)
    {
        // >>> bDoubleClkEntered
        if (bDoubleClkEntered)
        {

#ifdef _consoledlg_keys_log_
            if (fLog)
            {
              const char* logstr = "VK_LBUTTONUP && DblClick\r\n";
              fwrite(logstr, sizeof(char), lstrlenA(logstr), fLog);
            }
#endif            
         
            lpmsgf->msg = 0;
            bDoubleClkEntered = false;
            ::SetFocus( Runtime::GetNppExec().GetScintillaHandle() );
            return 0;
        }
        // <<< bDoubleClkEntered
        else
        {
            int posStart = 0;
            int posEnd = 0;
            Edit.ExGetSelPos(&posStart, &posEnd);
            GoToError_nCurrentLine = Edit.ExLineFromChar(posEnd);
        }
    }
    // <<< WM_LBUTTONUP

    // >>> WM_LBUTTONDBLCLK
    if (lpmsgf->msg == WM_LBUTTONDBLCLK) 
    {
            
#ifdef _consoledlg_keys_log_
        if (fLog)
        {
          const char* logstr = "WM_LBUTTONDBLCLK\r\n";
          fwrite(logstr, sizeof(char), lstrlenA(logstr), fLog);
        }
#endif          
          
        int nLine = Edit.ExLineFromChar(Edit.ExGetSelPos());
        bDoubleClkEntered = GoToLineIfWarningAnalyzerMatch(Edit, nLine);
        if ( bDoubleClkEntered )
        {
            GoToError_nCurrentLine = nLine;
            lpmsgf->msg = 0;
        }
        
        return 0;
    }
    // <<< WM_LBUTTONDBLCLK
    
    // >>> WM_LBUTTONDOWN
    if (lpmsgf->msg == WM_LBUTTONDOWN)
    {
        bDoubleClkEntered = false;
        return 0;
    }
    // <<< WM_LBUTTONDOWN

    //
    /////////////////////////////////////////////////////////////////////////

    bool bCtrl  = ((GetKeyState(VK_CONTROL) & 0x80) != 0) ? true : false; // Ctrl
    bool bAlt   = ((GetKeyState(VK_MENU) & 0x80) != 0)    ? true : false; // Alt
    bool bShift = ((GetKeyState(VK_SHIFT) & 0x80) != 0)   ? true : false; // Shift

    // >>> bCtrl
    if (bCtrl) // Ctrl is pressed
    {
      
#ifdef _consoledlg_keys_log_
        if (fLog)
        {
          const char* logstr = "Ctrl + ";
          fwrite(logstr, sizeof(char), lstrlenA(logstr), fLog);
        }
#endif          
        
        // >>> bCtrl : Ctrl+V || Ctrl+X
        if ((lpmsgf->wParam == 0x56) || // Ctrl+V
            (lpmsgf->wParam == 0x58))   // Ctrl+X
        {
        
#ifdef _consoledlg_keys_log_
            if (fLog)
            {
              const char* logstr = "V or X";
              fwrite(logstr, sizeof(char), lstrlenA(logstr), fLog);
            }
#endif                  

            // >>> bCtrl : Ctrl+V || Ctrl+X : WM_KEYDOWN
            if (lpmsgf->msg == WM_KEYDOWN)
            {
                int nCharIndex = Edit.ExGetSelPos();
                if (nCharIndex < nConsoleFirstUnlockedPos)
                {
                    if (lpmsgf->wParam == 0x56)  //Ctrl+V
                    {
                        int len = Edit.GetTextLengthEx();
                        Edit.ExSetSel(len, len); // jump to the last line
                        return 0;
                    }
                    else if (lpmsgf->wParam == 0x58)  // Ctrl+X
                    {
                        lpmsgf->wParam = 0x43;  // Ctrl+C
                    }
                    else
                    {
                        lpmsgf->wParam = 0;
                    }
                }
                else
                {
                }
            }
            // <<< bCtrl : Ctrl+V || Ctrl+X : WM_KEYDOWN
        }
        // <<< bCtrl : Ctrl+V || Ctrl+X
        
        // NOTE:  Ctrl+X can be modified to Ctrl+C
        
        // >>> bCtrl : Ctrl+C
        if (lpmsgf->wParam == 0x43) // Ctrl+C
        {
      
#ifdef _consoledlg_keys_log_
            if (fLog)
            {
              const char* logstr = "C";
              fwrite(logstr, sizeof(char), lstrlenA(logstr), fLog);
            }
#endif                  
        
            // >>> bCtrl : Ctrl+C : WM_KEYDOWN
            if (lpmsgf->msg == WM_KEYDOWN)
            {
                INT nSelStart = 0, nSelEnd = 0;

                Edit.ExGetSelPos(&nSelStart, &nSelEnd);
                if (nSelEnd > nSelStart) 
                {
                    TCHAR ch;
                    INT   nEnd = nSelEnd;

                    while ((nEnd > nSelStart) &&
                           (((ch = Edit.GetCharAt(nEnd-1)) == ' ') || 
                           (ch == '\t') || (ch == '\r') || (ch == '\n')))
                    {
                        nEnd--;
                    }
                    if (nSelEnd != nEnd)
                    {
                        Edit.ExSetSel(nSelStart, nEnd);
                        //Runtime::GetNppExec().ShowWarning("Ctrl+C: modified");
                    }
                    return 0;
                }
                else
                {
                    if (Runtime::GetNppExec()._consoleProcessIsRunning)
                    {
                        Runtime::GetNppExec()._consoleProcessBreak = true;
                        //return 1;
                    }
                    return 0;
                }
            }
            // <<< bCtrl : Ctrl+C : WM_KEYDOWN
        }
        // <<< bCtrl : Ctrl+C

        // >>> bCtrl : Ctrl+Z
        else if (lpmsgf->wParam == 0x5A) // Ctrl+Z
        {

#ifdef _consoledlg_keys_log_
            if (fLog)
            {
              const char* logstr = "Z";
              fwrite(logstr, sizeof(char), lstrlenA(logstr), fLog);
            }
#endif

            if (lpmsgf->msg == WM_KEYDOWN)
            {
                if (Runtime::GetNppExec()._consoleProcessIsRunning)
                {
                    Runtime::GetNppExec().GetConsole().PrintStr( _T("^Z"), true );
                    Runtime::GetNppExec().GetConsole().LockConsoleEndPos();
                    Runtime::GetNppExec().ConsoleWriteInput( _T("\x1A") ); // ^Z
                    Runtime::GetNppExec().ConsoleWriteInput( Runtime::GetOptions().GetStr(OPTS_KEY_ENTER) );
                    return 0;
                }
            }
        }
        // <<< bCtrl : Ctrl+Z

        // >>> bCtrl : WM_KEYDOWN && Ctrl+Break
        else if ((lpmsgf->msg == WM_KEYDOWN) && (lpmsgf->wParam == 0x03)) // Ctrl+Break
        {
        
#ifdef _consoledlg_keys_log_
            if (fLog)
            {
              const char* logstr = "Break";
              fwrite(logstr, sizeof(char), lstrlenA(logstr), fLog);
            }
#endif          
        
            if (Runtime::GetNppExec()._consoleProcessIsRunning)
            {
                Runtime::GetNppExec()._consoleProcessBreak = true;
                //return 1;
            }
            return 0;
        }
        // <<< bCtrl : WM_KEYDOWN && Ctrl+Break

    }
    // <<< bCtrl

    // >>> bAlt
    else if (bAlt)
    {

#ifdef _consoledlg_keys_log_
        if (fLog)
        {
          const char* logstr = "Alt";
          fwrite(logstr, sizeof(char), lstrlenA(logstr), fLog);
        }
#endif          

        SetFocus( GetDlgItem(hDlg, IDC_RE_CONSOLE) );
        return 0;
    }
    // <<< bAlt

    // >>> WM_KEYDOWN (Find Controls)
    if ( lpmsgf->msg == WM_KEYDOWN )
    {
        if ( ((lpmsgf->wParam == VK_F3) && (!bCtrl) && (!bAlt)) || // F3
             ((lpmsgf->wParam == VK_F7) && (!bCtrl) && (!bAlt)) || // F7
             ((lpmsgf->wParam == 0x46) && bCtrl && (!bAlt))  // Ctrl+F
           )
        {
            bFindControls = true;
            OnSize(hDlg);
            enableFindControls(true);
            return 0;
        }

        if ( (lpmsgf->wParam == 0x57) && bCtrl && (!bAlt) )  // Ctrl+W
        {
            doWordWrap(hDlg);
            return 0;
        }

        if ( (lpmsgf->wParam == 0x54) && bCtrl && (!bAlt) )  // Ctrl+T
        {
            doHideToggled(hDlg);
            return 0;
        }
    }
    // <<< WM_KEYDOWN (Find Controls)

    // >>> WM_KEYDOWN && hot-key
    if ( lpmsgf->msg == WM_KEYDOWN )
    {
        const unsigned int k = Runtime::GetOptions().GetUint(OPTU_CONSOLE_CATCHSHORTCUTKEYS) & ConsoleDlg::CSK_ALL;
        if ( k != 0 )
        {
            int i = 0;
            int N = 0;
            if ( k == ConsoleDlg::CSK_STD )
            {
                // standard items: [0 .. (nbFunc-1)]
                i = 0;
                N = nbFunc;
            }
            else if ( k == ConsoleDlg::CSK_USR )
            {
                // user items: [nbFunc .. (nbFunc+g_nUserMenuItems)]
                i = nbFunc;
                N = (g_nUserMenuItems > 0) ? (nbFunc + g_nUserMenuItems + 1) : nbFunc;
            }
            else if ( k == ConsoleDlg::CSK_ALL )
            {
                // all items: [0 .. (nbFunc+g_nUserMenuItems)]
                i = 0;
                N = (g_nUserMenuItems > 0) ? (nbFunc + g_nUserMenuItems + 1) : nbFunc;
            }
            for ( ; i < N; i++ )
            {
                const ShortcutKey& shortcut = g_funcShortcut[i];
                if ( (shortcut._key == lpmsgf->wParam) &&
                     (shortcut._isAlt == bAlt) &&
                     (shortcut._isCtrl == bCtrl) &&
                     (shortcut._isShift == bShift) )
                {
                    #ifdef _consoledlg_keys_log_
                        if (fLog)
                        {
                          char logstr[100];
                          sprintf(logstr, "Hot-key: function[%d]", i);
                          fwrite(logstr, sizeof(char), lstrlenA(logstr), fLog);
                        }
                    #endif

                    Runtime::GetLogger().AddEx( _T("; Hot-key: executing function [%d], \"%s\""), i, g_funcItem[i]._itemName );
                    
                    lpmsgf->wParam = 0;
                    if ( g_funcItem[i]._pFunc )
                        g_funcItem[i]._pFunc();
                    return 0;
                }
            }
        }
    }
    // <<< WM_KEYDOWN && hot-key
    
    // >>> (VK_UP || VK_DOWN) && (WM_KEYDOWN || WM_KEYUP)
    if (Runtime::GetOptions().GetBool(OPTB_CONSOLE_CMDHISTORY) && 
        ((lpmsgf->wParam == VK_UP) || (lpmsgf->wParam == VK_DOWN)) &&
        ((lpmsgf->msg == WM_KEYDOWN) || (lpmsgf->msg == WM_KEYUP)) &&
        (!bCtrl) && (!bAlt)
       )
    {  

#ifdef _consoledlg_keys_log_
        if (fLog)
        {
          const char* logstr = "Console Cmd History\r\n";
          fwrite(logstr, sizeof(char), lstrlenA(logstr), fLog);
        }
#endif          

        Edit.ScrollCaret();
        
        int nPos = Edit.ExGetSelPos();
        int nLine = Edit.ExLineFromChar(nPos);
        if ((nLine >= Edit.ExLineFromChar(nConsoleFirstUnlockedPos)) &&
            (nLine < Edit.GetLineCount()))
        {
            // >>> (VK_UP || VK_DOWN) && (WM_KEYDOWN || WM_KEYUP) : WM_KEYDOWN
            if (lpmsgf->msg == WM_KEYDOWN)
            {
                bool  bPrevItem = (lpmsgf->wParam == VK_UP);
                tstr  S = _T("");

                if (bPrevItem)
                {
                    if (pCmdHistoryItemPtr)
                    {
                        pCmdHistoryItemPtr = pCmdHistoryItemPtr->GetPrev();
                        /*
                        if (!pCmdHistoryItemPtr)
                        {
                            pCmdHistoryItemPtr = CmdHistoryList.GetLast();
                        }
                        */
                    }
                    else
                    {
                        pCmdHistoryItemPtr = CmdHistoryList.GetLast();
                    }
                }
                else
                {
                    if (pCmdHistoryItemPtr)
                    {
                        pCmdHistoryItemPtr = pCmdHistoryItemPtr->GetNext();
                        /*
                        if (!pCmdHistoryItemPtr)
                        {
                            pCmdHistoryItemPtr = CmdHistoryList.GetFirst();
                        }
                        */
                    }
                    else
                    {
                        pCmdHistoryItemPtr = CmdHistoryList.GetFirst();
                    }
                }

                int nFirst = Edit.LineIndex( nLine );
                if (nFirst < nConsoleFirstUnlockedPos)
                    nFirst = nConsoleFirstUnlockedPos;
                int nLength = Edit.LineLength( nFirst );
                if (pCmdHistoryItemPtr)
                {
                    S = pCmdHistoryItemPtr->GetItem();
                }
                Edit.SetRedraw(FALSE);
                Edit.ExSetSel( nFirst, nFirst+nLength );
                Edit.SetRedraw(TRUE);
                Edit.ReplaceSelText( _T("") );
                Edit.ReplaceSelText( S.c_str() );
            }
            // <<< (VK_UP || VK_DOWN) && (WM_KEYDOWN || WM_KEYUP) : WM_KEYDOWN

            lpmsgf->wParam = 0;
            //return 1;
        }
        return 0;
    }
    // <<< (VK_UP || VK_DOWN) && (WM_KEYDOWN || WM_KEYUP)

    // >>> VK_DELETE || VK_BACK
    if ((lpmsgf->wParam == VK_DELETE || lpmsgf->wParam == VK_BACK) &&
        (lpmsgf->msg == WM_KEYDOWN || lpmsgf->msg == WM_KEYUP || lpmsgf->msg == WM_CHAR))
    {

#ifdef _consoledlg_keys_log_
        if (fLog)
        {
          const char* logstr = "VK_DELETE or VK_BACK";
          fwrite(logstr, sizeof(char), lstrlenA(logstr), fLog);
        }
#endif          

        int nCharIndex = Edit.ExGetSelPos();
        if (nCharIndex < nConsoleFirstUnlockedPos)
        {
            lpmsgf->wParam = 0;
            //return 1;
        }
        else if (lpmsgf->msg != WM_CHAR && lpmsgf->wParam == VK_BACK &&
                 nConsoleFirstUnlockedPos == nCharIndex)
        {
            lpmsgf->wParam = 0;
            //return 1;
        }
        return 0;
    }
    // <<< VK_DELETE || VK_BACK

    // >>> WM_CHAR (any character)
    if (lpmsgf->msg == WM_CHAR)
    {
      
#ifdef _consoledlg_keys_log_
        if (fLog)
        {
          const char* logstr = "WM_CHAR or Ctrl+V";
          fwrite(logstr, sizeof(char), lstrlenA(logstr), fLog);
        }
#endif          
      
        if ((!bCtrl) || (lpmsgf->wParam == 0x56)) // not Ctrl, or Ctrl+V
        {
            int nCharIndex = Edit.ExGetSelPos();
            if (nCharIndex < nConsoleFirstUnlockedPos)
            {
                int len = Edit.GetTextLengthEx();
                Edit.ExSetSel(len, len); // jump to the last line
            }
        }

        // DO NOT exit here !!!
    }
    // <<< WM_CHAR (any character)

    // >>> VK_RETURN && WM_KEYDOWN
    if ((lpmsgf->wParam == VK_RETURN) && (lpmsgf->msg == WM_KEYDOWN))
    {
      
#ifdef _consoledlg_keys_log_
        if (fLog)
        {
          const char* logstr = "VK_RETURN";
          fwrite(logstr, sizeof(char), lstrlenA(logstr), fLog);
        }
#endif                

        int nCharIndex = Edit.ExGetSelPos();
        int nLines = Edit.ExLineFromChar(nCharIndex) + 1; //Edit.GetLineCount();

        if (Runtime::GetNppExec()._consoleProcessIsRunning &&
            (nCharIndex >= nConsoleFirstUnlockedPos))
        {
            // input line as a console input
            if (nLines >= 1)
            {
                int nLen = Edit.GetLine(nLines-1, Runtime::GetNppExec()._consoleStrToWrite, CONSOLECOMMAND_BUFSIZE - 1);
                int i = Edit.LineIndex(nLines-1);
                Edit.ExSetSel(i + nLen, i + nLen);
          
                if (Edit.ExLineFromChar(nConsoleFirstUnlockedPos) == nLines-1)
                {
                    i = nConsoleFirstUnlockedPos - i;
                    lstrcpy(Runtime::GetNppExec()._consoleStrToWrite, Runtime::GetNppExec()._consoleStrToWrite + i);
                    nLen -= i;
                }

                /*
                TCHAR sz[512];
                wsprintf(sz, "\"%s\" (%ld)", Runtime::GetNppExec()._consoleStrToWrite, nLen);
                MessageBox(NULL, sz, "", MB_OK);
                */

                tstr s1 = Runtime::GetNppExec()._consoleStrToWrite;
                if (s1.length() > 0)
                {
                    TCHAR ch;
                    while (((ch = s1.GetLastChar()) == _T('\n')) || (ch == _T('\r')))
                    {
                        s1.DeleteLastChar();
                    }
                    AddCommandToHistoryList(s1);
                }
          
                //lstrcat(Runtime::GetNppExec()._consoleStrToWrite, "\n");  nLen += 1;

                //s1 += _T("\r\n");

                Runtime::GetLogger().AddEx( _T_RE_EOL _T("; @Child Process\'es Input: %s"), s1.c_str() );

                Runtime::GetNppExec().GetMacroVars().CheckCmdAliases(s1);
                Runtime::GetNppExec().GetMacroVars().CheckAllMacroVars(s1, true);

                Runtime::GetNppExec().ConsoleWriteInput( s1.c_str() );
                Runtime::GetNppExec().ConsoleWriteInput( Runtime::GetOptions().GetStr(OPTS_KEY_ENTER) );
                  // "\n" must be sent separately in some cases - ask M$ why
                Runtime::GetNppExec()._consoleStrToWrite[0] = 0;

                if ( !Runtime::GetNppExec().GetConsole().IsOutputEnabled() )
                {
                    int nPos = Runtime::GetNppExec().GetConsole().GetConsoleEdit().GetTextLengthEx();
                    nPos += _T_RE_EOL_LEN;
                    Runtime::GetNppExec().GetConsole().LockConsolePos(nPos, true);
                }
            }
        }
        else if (Runtime::GetNppExec()._consoleProcessIsRunning)
        {
            bCommandEntered = true;
            lpmsgf->wParam = 0;
            /*if ( !Runtime::GetNppExec().GetConsole().IsOutputEnabled() )
            {
                int nPos = Runtime::GetNppExec().GetConsole().GetConsoleEdit().GetTextLengthEx();
                nPos += _T_RE_EOL_LEN;
                Runtime::GetNppExec().GetConsole().LockConsolePos(nPos, true);
            }*/
        }
        else if ((!Runtime::GetNppExec().isScriptRunning()) &&
                 (nCharIndex >= nConsoleFirstUnlockedPos))
        {
            // input line as a stand-alone command
            TCHAR ch;
            tstr  S;
          
            int nLen = Edit.GetLine(nLines-1, Runtime::GetNppExec()._consoleStrToWrite, CONSOLECOMMAND_BUFSIZE - 1);
            int i = Edit.LineIndex(nLines-1);
            Edit.ExSetSel(i + nLen, i + nLen);
        
            if (Edit.ExLineFromChar(nConsoleFirstUnlockedPos) == nLines-1)
            {
                i = nConsoleFirstUnlockedPos - i;
                lstrcpy(Runtime::GetNppExec()._consoleStrToWrite, Runtime::GetNppExec()._consoleStrToWrite + i);
                nLen -= i;
            }

            S = Runtime::GetNppExec()._consoleStrToWrite;
            Runtime::GetNppExec()._consoleStrToWrite[0] = 0;
        
            i = 0;
            while ((i < S.length()) && 
                   (((ch = S[i]) == ' ') || (ch == '\t') || (ch == '\r') || (ch == '\n')))
            {
                i++;
            }
            if (i > 0)
                S.Delete(0, i);

            while (((i = S.length() - 1) >= 0) && 
                   (((ch = S[i]) == ' ') || (ch == '\t') || (ch == '\r') || (ch == '\n')))
            {
                S.Delete(i);
            }
        
            if (S.length() > 0)
            {
          
                AddCommandToHistoryList(S);

                Runtime::GetLogger().AddEx( _T_RE_EOL _T("; @Input Command: %s"), S.c_str() );
          
                tstr S1 = S;
                ::CharUpper(S1.c_str());

                if (S1 == CMD_CLS)
                {
                    Runtime::GetNppExec().GetConsole().ClearText(true);
                    bCommandEntered = true;
                    lpmsgf->wParam = 0;
                    return 0; // nothing else to do as ClearText() was forced
                }
                else if (S1 == CONSOLE_CMD_VER)
                {
                    Runtime::GetNppExec().GetConsole().PrintMessage( _T(""), false );
                    Runtime::GetNppExec().GetConsole().PrintMessage( PLUGIN_CURRENT_VER, false );
                    Runtime::GetNppExec().GetConsole().PrintMessage( _T("================ READY ================"), false );
                    bCommandEntered = true;
                    if ( Runtime::GetNppExec().GetConsole().IsOutputEnabled() )
                        lpmsgf->wParam = 0;
                }
                else if (IsConsoleHelpCommand(S1))
                {
                    bCommandEntered = true;
                    if ( Runtime::GetNppExec().GetConsole().IsOutputEnabled() )
                        lpmsgf->wParam = 0;
                }
                else
                {
                    Runtime::GetNppExec().m_CmdList.DeleteAll();
                    Runtime::GetNppExec().m_CmdList.Add(S.c_str());//Runtime::GetNppExec().ShowWarning(S.c_str());
          
                    Runtime::GetNppExec().CreateScriptThread();

                    //lpmsgf->wParam = 0;
                }

                if ( !Runtime::GetNppExec().GetConsole().IsOutputEnabled() )
                {
                    int nPos = Runtime::GetNppExec().GetConsole().GetConsoleEdit().GetTextLengthEx();
                    nPos += _T_RE_EOL_LEN;
                    Runtime::GetNppExec().GetConsole().LockConsolePos(nPos, true);
                }
            }

        }
        else
        {
            lpmsgf->wParam = 0;
        }
        //lpmsgf->wParam = 0;
        return 0;
    }
    // <<< VK_RETURN && WM_KEYDOWN

    else if (bCommandEntered && (lpmsgf->wParam == VK_RETURN) && (lpmsgf->msg == WM_CHAR))
    {
        lpmsgf->wParam = 0;
        return 0;
    }
    else if (bCommandEntered && (lpmsgf->wParam == VK_RETURN) && (lpmsgf->msg == WM_KEYUP))
    {
        lpmsgf->wParam = 0;
        bCommandEntered = false;
        return 0;
    }

    // >>> VK_HOME && WM_KEYDOWN
    if ((lpmsgf->wParam == VK_HOME) && (lpmsgf->msg == WM_KEYDOWN))
    {
        if (!bCtrl)
        {
            int nPos1 = 0, nPos2 = 0;
            Edit.ExGetSelPos(&nPos1, &nPos2);
            int nLine = Edit.ExLineFromChar(nPos2);
            if (nLine == Edit.ExLineFromChar(nConsoleFirstUnlockedPos))
            {
                if (!bShift)
                {
                    Edit.ExSetSel(nConsoleFirstUnlockedPos, nConsoleFirstUnlockedPos);
                }
                else
                {
                    if (nPos2 < nConsoleFirstUnlockedPos)
                    {
                        Edit.ExSetSel(Edit.LineIndex(nLine), nPos2);
                    }
                    else
                    {
                        Edit.ExSetSel(nConsoleFirstUnlockedPos, nPos2);
                    }
                }
                lpmsgf->wParam = 0;
                return 0;
            }
        }
        return 0;
    }
    // <<< VK_HOME && WM_KEYDOWN

    // >>> VK_END && (WM_KEYDOWN || WM_KEYUP)
    if ((lpmsgf->wParam == VK_END) && ((lpmsgf->msg == WM_KEYDOWN) || (lpmsgf->msg == WM_KEYUP)))
    {
        if (!bCtrl)
        {
            int nPos1 = 0, nPos2 = 0;
            Edit.ExGetSelPos(&nPos1, &nPos2);
            if (Edit.LineLength(nPos2) == 0)
            {
                lpmsgf->wParam = 0;
                return 0;
            }
        }
        return 0;
    }
    // <<< VK_END && (WM_KEYDOWN || WM_KEYUP)

    // >>> VK_TAB
    if (lpmsgf->wParam == VK_TAB)
    { 
        if (lpmsgf->msg == WM_KEYDOWN)
        {
            int nPos1 = 0, nPos2 = 0;
            Edit.ExGetSelPos(&nPos1, &nPos2);
            int nLine = Edit.ExLineFromChar(nPos2);
            if (nLine >= Edit.ExLineFromChar(nConsoleFirstUnlockedPos))
            {
                TCHAR szLine[CONSOLECOMMAND_BUFSIZE];
                int   nLen = Edit.GetLine(nLine, szLine, CONSOLECOMMAND_BUFSIZE - 1); // line length
                int   pos = Edit.LineIndex(nLine); // line start pos
                if (nPos1 != nPos2)
                {
                    if (nPos1 < pos)  nPos1 = pos;
                    if (nPos1 - pos < nLen)
                    {
                        nLen = nPos1 - pos;
                        szLine[nLen] = 0;
                    }
                }
                if ((nLen > 0) && (szLine[nLen-1] == _T('\n')))
                {
                    szLine[--nLen] = 0;
                }
                if ((nLen > 0) && (szLine[nLen-1] == _T('\r')))
                {
                    szLine[--nLen] = 0;
                }
                if (nPos1 == nPos2)
                {
                    nPos1 = pos + nLen;
                    nPos2 = nPos1;
                    Edit.ExSetSel(nPos1, nPos1);
                }
                if (nConsoleFirstUnlockedPos > pos)
                {
                    lstrcpy(szLine, szLine + nConsoleFirstUnlockedPos - pos);
                    nLen -= (nConsoleFirstUnlockedPos - pos);
                }
                if (nLen > 0)
                {
                    static CListItemT<tstr>* p = NULL; 
                    static tstr prevLine = _T("");
                    static bool bEnvVar = false;
                    static bool bAllowCyclicSearch = false;
                    tstr S;
                    CListItemT<tstr>* p0 = p;
                    const bool bCmdHistory = 
                        ((CmdHistoryList.GetCount() > 0) && Runtime::GetOptions().GetBool(OPTB_CONSOLE_CMDHISTORY)) ? true : false;
        
                    ::CharUpper(szLine);

                    const TCHAR* pszFileName = NULL;
                    bool         bFullPath = false;
                    int          len2 = 0;
                    CScriptEngine::eCmdType nCmdType = CScriptEngine::CMDTYPE_UNKNOWN;
                    
                    if ( (!Runtime::GetNppExec()._consoleProcessIsRunning) && ((!p) || (prevLine != szLine)) )
                    {
                        UINT uSearchFlags = 0;
                        tstr cmdLine = szLine;

                        /*
                        // disabled because '\' or '/' as alias to "NPP_EXEC"
                        // must be autocompleted in the same way as "NPP_EXEC"
                        switch ( cmdLine.GetAt(0) )
                        {
                            case _T('\\'):
                            case _T('/'):
                                nCmdType = 0;
                                break;
                            default:
                                nCmdType = CScriptEngine::getCmdType(cmdLine);
                                break;
                        }
                        */
                        
                        // disable logging (no output while processing TAB pressed)
                        Runtime::GetLogger().Activate(false);
                        
                        nCmdType = CScriptEngine::getCmdType(cmdLine);

                        switch ( nCmdType )
                        {
                            case CScriptEngine::CMDTYPE_UNKNOWN:
                                uSearchFlags = CDirFileLister::ESF_DIRS | CDirFileLister::ESF_FILES | CDirFileLister::ESF_PLACEFILESFIRST | CDirFileLister::ESF_SORTED;
                                break;
                            case CScriptEngine::CMDTYPE_CD:
                                uSearchFlags = CDirFileLister::ESF_DIRS | CDirFileLister::ESF_SORTED;
                                break;
                            case CScriptEngine::CMDTYPE_DIR:
                                uSearchFlags = CDirFileLister::ESF_DIRS | CDirFileLister::ESF_FILES | CDirFileLister::ESF_PLACEDIRSFIRST | CDirFileLister::ESF_SORTED;
                                break;
                            default:
                                uSearchFlags = CDirFileLister::ESF_DIRS | CDirFileLister::ESF_FILES | CDirFileLister::ESF_PLACEFILESFIRST | CDirFileLister::ESF_SORTED;
                                break;
                        }

                        DelLeadingTabSpaces(cmdLine);

                        Runtime::GetNppExec().GetMacroVars().CheckAllMacroVars(cmdLine, false);
                        
                        // enable logging (don't forget to enable after disabling!)
                        Runtime::GetLogger().Activate(true);
                        
                        if ( cmdLine.length() > 0 )
                        {
                            if ( (cmdLine.length() == 2) && (cmdLine[1] == _T(':')) )
                            {
                                cmdLine += _T('\\');
                                Edit.ReplaceSelText( _T("\\") );
                                ++nPos1;
                                ++nPos2;
                            }
                        
                            bFullPath = isFullPath(cmdLine);
                            if ( !bFullPath )
                            {
                                int   nPathLen;
                                TCHAR szFullPath[FILEPATH_BUFSIZE];
                                
                                szFullPath[0] = 0;
                                nPathLen = (int) ::GetCurrentDirectory(FILEPATH_BUFSIZE - 1, szFullPath);
                                if ( nPathLen > 0 )
                                {
                                    if ( ((cmdLine.GetAt(0) == _T('\\')) && (cmdLine.GetAt(1) != _T('\\'))) || 
                                         ((cmdLine.GetAt(0) == _T('/')) && (cmdLine.GetAt(1) != _T('/'))) )
                                    {
                                        szFullPath[2] = 0; // i.e. 'C:' remaining
                                        cmdLine.Insert(0, szFullPath);
                                    }
                                    else
                                    {
                                        szFullPath[nPathLen++] = _T('\\');
                                        szFullPath[nPathLen] = 0;
                                        cmdLine.Insert(0, szFullPath, nPathLen);
                                    }
                                }
                            }

                            len2 = 0;
                            for ( int h = cmdLine.length() - 1; h >= 0; h-- )
                            {
                                const TCHAR tch = cmdLine[h];
                                if ( (tch != _T('\\')) && (tch != _T('/')) && (tch != _T(':')) )
                                    ++len2;
                                else
                                    break;
                            }
                            
                            if ( !bShift )
                                pszFileName = tabDirFileLister.FindNext(cmdLine.c_str(), uSearchFlags, bAllowCyclicSearch);
                            else
                                pszFileName = tabDirFileLister.FindPrev(cmdLine.c_str(), uSearchFlags, bAllowCyclicSearch);
                            bAllowCyclicSearch = false;
                        }
                    }
                    
                    if ( pszFileName )
                    {
                        UINT itemLen;
                        bool itemIsDir;
                        
                        tabDirFileLister.GetItem(&itemLen, &itemIsDir);
                        
                        Edit.SetRedraw(FALSE);
                        Edit.ExSetSel(nPos1 - len2, nPos2);
                        Edit.SetRedraw(TRUE);
                        if ( itemIsDir /*&& (nCmdType != CScriptEngine::CMDTYPE_CD)*/ )
                        {
                            tstr itemStr = pszFileName;
                            itemStr += _T('\\');
                            ++itemLen;
                            Edit.ReplaceSelText(itemStr.c_str());
                        }
                        else
                        {
                            Edit.ReplaceSelText(pszFileName);
                        }
                        Edit.ExSetSel(nPos1, nPos1 - len2 + itemLen);
                    }
                    else
                    {
                        int nListInd = 0;
                        
                        if ( (!p) || (prevLine != szLine) )
                        {
                            if ( !bShift )
                            {
                                p = bCmdHistory ? CmdHistoryList.GetLast() : CmdVarsList.GetLast();
                            }
                            else
                            {
                                p = NULL;
                                nListInd = 3;
                            }
                            bEnvVar = false;
                        }
                        else
                        {
                            CListItemT<tstr>* p1;
                            
                            if ( !bShift )
                            {
                                p1 = ( (bCmdHistory && (p->GetOwner() == &CmdHistoryList)) ? 
                                          CmdHistoryList.GetLast() : CmdVarsList.GetLast() );
                            }
                            else
                            {
                                p1 = ( (bCmdHistory && (p->GetOwner() == &CmdHistoryList)) ? 
                                          CmdHistoryList.GetFirst() : CmdVarsList.GetFirst() );
                            }

                            while (p1 && (p1 != p))
                            {
                                if ( !bShift )
                                    p1 = p1->GetPrev();
                                else
                                    p1 = p1->GetNext();
                            }
                            if (p1)
                            {
                                nListInd = (p1->GetOwner() == &CmdHistoryList) ? 1 : 2;
                                if ( !bShift )
                                {
                                    p = p1->GetPrev();
                                }
                                else
                                {
                                    p = p1->GetNext();
                                }
                            }
                            else
                            {
                                if ( !bShift )
                                {
                                    p = bCmdHistory ? CmdHistoryList.GetLast() : CmdVarsList.GetLast();
                                }
                                else
                                {
                                    p = bCmdHistory ? CmdHistoryList.GetFirst() : CmdVarsList.GetFirst();
                                }
                                bEnvVar = false;
                            }
                        }

                        prevLine = szLine;
                        
                        if ( nListInd == 0 )
                        {
                            nListInd = ((p && (p->GetOwner() == &CmdVarsList)) || !bCmdHistory) ? 2 : 1;
                        }
                        // 1 - CmdHistoryList
                        // 2 - CmdVarsList
                        // 3 - prepare to look for $(var)
                        // 4 - looking for $(var)
                        while ( nListInd > 0 )
                        {
                            if ( bEnvVar && (nListInd < 3) )
                            {
                                p = NULL;
                                nListInd = 3;
                            }
                            while ( p )
                            {
                                S = p->GetItem();
                                if (S.length() > 0)  ::CharUpper(S.c_str());
                                if (S.StartsWith(szLine))
                                {
                                    if (S.length() > nLen)
                                    {
                                        if ( nListInd /*== 4*/ /*p->GetOwner() == &CmdVarsList*/ )
                                        {
                                            Edit.SetRedraw(FALSE);
                                            Edit.ExSetSel(nPos1 - nLen, nPos2);
                                            Edit.SetRedraw(TRUE);
                                            Edit.ReplaceSelText(p->GetItem().c_str());
                                            //Edit.Redraw();
                                        }
                                        else
                                        {
                                            Edit.ReplaceSelText(p->GetItem().c_str() + nLen);
                                        }
                                        Edit.ExSetSel(nPos1, nPos1 + S.length() - nLen);
                                    }
                                    bEnvVar = (nListInd == 4) ? true : false;
                                    nListInd = 0;
                                    break;
                                }
                                if ( !bShift )
                                    p = p->GetPrev();
                                else
                                    p = p->GetNext();
                            }
                            if ( nListInd == 1 )
                            {
                                if ( !bShift )
                                {
                                    nListInd = 3;
                                    p = CmdVarsList.GetLast();
                                }
                                else
                                {
                                    nListInd = 0;
                                    //p = CmdVarsList.GetFirst();
                                }
                                bEnvVar = false;
                            }
                            else if ( nListInd == 2 )
                            {
                                if ( !bShift )
                                {
                                    nListInd = 3;
                                    //p = bCmdHistory ? CmdHistoryList.GetLast() : NULL;
                                }
                                else
                                {
                                    nListInd = 1;
                                    p = bCmdHistory ? CmdHistoryList.GetFirst() : NULL;
                                }
                                bEnvVar = false;
                            }
                            else if ( nListInd == 3 )
                            {
                                nListInd = 4;
                                int i = nLen - 1;
                                for ( ; i >= 0; i--)
                                {
                                    if ( (szLine[i] == _T(' ')) || (szLine[i] == _T('\t')) )
                                    {
                                        ++i;
                                        if ( szLine[i] == _T('\"') )  ++i;  // skip first '"'
                                        lstrcpy(szLine, szLine + i);        // last word in the string - maybe "$("
                                        nLen -= i;
                                        break;
                                    }
                                }
                                if ( (nLen > 0) && (i < 0) )  // no ' ' or '\t' found
                                {
                                    if ( szLine[0] == _T('\"') )  // skip first '"'
                                    {
                                        lstrcpy(szLine, szLine + 1);
                                        nLen -= 1;
                                    }
                                }
                                if ( (nLen > 0) && (szLine[0] == _T('$')) )
                                {
                                    if ( p0 && (p0->GetOwner() == &CmdVarsList) )
                                    {
                                        if ( !bShift )
                                            p = p0->GetPrev();
                                        else
                                            p = p0->GetNext();
                                    }
                                    else
                                    {
                                        if ( !bShift )
                                            p = CmdVarsList.GetLast();
                                        else
                                            p = CmdVarsList.GetFirst();
                                    }
                                }
                                else    
                                {
                                    if ( !bShift )
                                    {
                                        nListInd = 0;
                                        //p = NULL;
                                    }
                                    else
                                    {
                                        nListInd = 2;
                                        p = CmdVarsList.GetFirst();
                                        lstrcpy( szLine, prevLine.c_str() );
                                        nLen = prevLine.length();
                                    }
                                    bEnvVar = false;
                                }
                            }
                            else
                            {
                                if (nListInd == 4)  bEnvVar = false;
                                if ( !bShift )
                                {
                                    nListInd = 0;
                                }
                                else if ( nListInd != 0 )
                                {
                                    if ( bCmdHistory )
                                    {
                                        nListInd = 1;
                                        p = CmdHistoryList.GetFirst();
                                    }
                                    else
                                    {    
                                        nListInd = 2;
                                        p = CmdVarsList.GetFirst();
                                    }
                                    lstrcpy( szLine, prevLine.c_str() );
                                    nLen = prevLine.length();
                                }
                            }
                        }
                        if ( !p )
                        {
                            Edit.ReplaceSelText( _T("") );
                            Edit.ExSetSel(nPos1, nPos1);
                            bAllowCyclicSearch = true;
                        }
                    }
                }
            }
        }
        lpmsgf->wParam = 0;
        return 0;
    }
    // <<< VK_TAB

#ifdef _consoledlg_keys_log_
      if (fLog)
      {
        if ((lpmsgf->msg == WM_KEYDOWN) ||
            (lpmsgf->msg == WM_KEYUP) ||
            (lpmsgf->msg == WM_CHAR))
        {       
          const char* logstr = "\r\n";
          fwrite(logstr, sizeof(char), lstrlenA(logstr), fLog);
        }
      }
#endif          

  }
  // <<< EN_MSGFILTER

  else if (pnmh->hwndFrom == Runtime::GetNppExec().m_nppData._nppHandle)
  {
      if (LOWORD(pnmh->code) == DMN_CLOSE)
      {
          // closing dlg
          //char str[256]; wsprintf(str, "0x%X", pnmh->idFrom); MessageBox(0,str,"",0);
          ConsoleDlg::OnClose(hDlg);
          return 0;
      }
      else if (LOWORD(pnmh->code) == DMN_FLOAT)
      {
          // floating dlg
          bIsDocked = false;
      }
      else if (LOWORD(pnmh->code) == DMN_DOCK)
      {
          // docking dlg
          bIsDocked = true;
      }
  }
  return 0;
}

void ConsoleDlg::OnShowWindow(HWND hDlg)
{
  HWND hEd = GetDlgItem(hDlg, IDC_RE_CONSOLE);
  ::SetFocus(hEd);

  /*
  if (!Runtime::GetOptions().GetBool(OPTB_CONFLTR_ENABLE))
  {
    ::SendMessage(hDlg, WM_SETTEXT, 0, (LPARAM) " Console ");
  }
  else
  {
    ::SendMessage(hDlg, WM_SETTEXT, 0, (LPARAM) " Console* ");
  }
  */

  //CmdHistoryList.DeleteAll();
  //pCmdHistoryItemPtr = NULL;
}

void ConsoleDlg::OnSize(HWND hDlg)
{
  RECT  rect;
  HWND  hEd;

  //::GetWindowRect(hDlg, &rect);
  ::GetClientRect(hDlg, &rect);
  /*
  rect.left += 1;
  rect.top += 1;
  rect.right -= 2;
  rect.bottom -= 2;
  */
  if (bFindControls)
  {
    rect.bottom -= 24;
  }
  hEd = ::GetDlgItem(hDlg, IDC_RE_CONSOLE);
  ::MoveWindow(hEd, rect.left, rect.top, rect.right, rect.bottom, TRUE);
  ::InvalidateRect(hEd, NULL, TRUE);
  ::UpdateWindow(hEd);
  if (bFindControls)
  {
    int left = 2;
    int top = rect.bottom + 2;
    
    btFindClose.MoveWindow(left, top, TRUE);
    ::GetWindowRect( btFindClose.m_hWnd, &rect );
    left += (6 + rect.right - rect.left);
    edFind.MoveWindow(left, top, TRUE);
    ::GetWindowRect( edFind.m_hWnd, &rect );
    left += (5 + rect.right - rect.left);
    chFindCase.MoveWindow(left, top + 1, TRUE);
    ::GetWindowRect( chFindCase.m_hWnd, &rect );
    left += (4 + rect.right - rect.left);
    chWordWrap.MoveWindow(left, top + 1, TRUE);
    ::GetWindowRect( chWordWrap.m_hWnd, &rect );
    left += (4 + rect.right - rect.left);
    chHideToggled.MoveWindow(left, top + 1, TRUE);
  }
}

bool ConsoleDlg::IsConsoleHelpCommand(const tstr& S)
{
  if (S.StartsWith(CONSOLE_CMD_HELP))
  {
    if (S == CONSOLE_CMD_HELP)
    {
      Runtime::GetNppExec().GetConsole().PrintMessage(CONSOLE_COMMANDS_INFO, false);
      Runtime::GetNppExec().GetConsole().PrintMessage( _T("================ READY ================"), false );
      return true;
    }
    else
    {
      int i = S.Find(_T(' '));
      if (i < 0)  i = S.Find(_T('\t'));
      if (i > 0)
      {
        tstr S1;

        ++i;
        while ((S[i] == _T(' ')) || (S[i] == _T('\t'))) ++i;
        S1.Copy( S.c_str() + i, S.length() - i );
        i = 0;
        while ((i < S1.length()) && (S1[i] != _T(' ')) && (S1[i] != _T('\t'))) ++i;
        if (i < S1.length())
        {
          S1.SetSize(i);
        }
        if (S1.length() > 0)
        {
          ::CharUpper(S1.c_str());
          if (S1 == _T("ALL"))
          {
            Runtime::GetNppExec().GetConsole().PrintMessage( _T(""), false );
            Runtime::GetNppExec().GetConsole().PrintMessage(CONSOLE_COMMANDS_INFO, false);

            i = 0;
            while (i < CMD_COUNT)
            {
              Runtime::GetNppExec().GetConsole().PrintMessage( _T("------------------------------------------------------------------------------"), false );
              Runtime::GetNppExec().GetConsole().PrintMessage( CONSOLE_CMD_INFO[i], false );
              ++i;
            }
            Runtime::GetNppExec().GetConsole().PrintMessage( _T("================ READY ================"), false );
            return true;
          }
          else
          {
            if (S1 == CMD_CONSAVE)
              S1 = CMD_CONSAVETO;
            else if (S1 == CMD_CONLOAD)
              S1 = CMD_CONLOADFROM;
            else if (S1 == CMD_UNSET)
              S1 = CMD_SET;
            else if (S1 == CMD_ENVUNSET || S1 == CMD_SETENV || S1 == CMD_UNSETENV)
              S1 = CMD_ENVSET;
            else if (S1 == CMD_NPEDEBUG)
              S1 = CMD_NPEDEBUGLOG;
            else if (S1 == CMD_SELLOAD)
              S1 = CMD_SELLOADFROM;
            else if (S1 == CMD_SELSAVE)
              S1 = CMD_SELSAVETO;
            else if (S1 == CMD_SELSETTEXTEX)
              S1 = CMD_SELSETTEXT;
            else if (S1 == CMD_CONCOLOR)
              S1 = CMD_CONCOLOUR;
            else if (S1 == _T("STRLENUTF8") || S1 == _T("STRLENU")    ||
                     S1 == _T("STRLENA")    || S1 == _T("STRLEN")     || 
                     S1 == _T("STRUPPER")   || S1 == _T("STRLOWER")   || 
                     S1 == _T("SUBSTR")     || S1 == _T("STRREPLACE") ||
                     S1 == _T("STRFIND")    || S1 == _T("STRRFIND"))
              S1 = CMD_SET;
            else if (S1.GetAt(0) == DEFAULT_ALIAS_CMD_LABEL && S1.GetAt(1) == 0)
              S1 = CMD_LABEL;
            i = 0;
            while (i < CMD_COUNT)
            {
              if (S1 == CMD_ITEMS[i])
              {
                Runtime::GetNppExec().GetConsole().PrintMessage( _T(""), false );
                Runtime::GetNppExec().GetConsole().PrintMessage( CONSOLE_CMD_INFO[i], false );
                Runtime::GetNppExec().GetConsole().PrintMessage( _T("================ READY ================"), false );
                return true;
              }
              ++i;
            }
          }
        }

      }
    }
  }
  else if (S == CONSOLE_CMD_MANUAL || S == CONSOLE_CMD_ABOUT)
  {
    if (S == CONSOLE_CMD_MANUAL)
      Runtime::GetNppExec().OnHelpManual();
    else
      Runtime::GetNppExec().OnHelpAbout();
    Runtime::GetNppExec().GetConsole().PrintMessage( _T(""), false );
    Runtime::GetNppExec().GetConsole().PrintMessage( _T("================ READY ================"), false );
    return true;
  }
  return false;
}

void ConsoleDlg::loadCmdVarsList(void)
{
  // environment variables in reverse order
  CmdVarsList.Add( _T("$(SYS.PATH)") );         //
  CmdVarsList.Add( MACRO_RIGHT_VIEW_FILE );     //  $(RIGHT_VIEW_FILE)
  CmdVarsList.Add( _T("$(RARGV[1])") );         //  $(RARGV[1])
  CmdVarsList.Add( _T("$(RARGV)") );            //  $(RARGV)
  CmdVarsList.Add( MACRO_PLUGINS_CONFIG_DIR );  //  $(PLUGINS_CONFIG_DIR)
  CmdVarsList.Add( MACRO_OUTPUTL );             //  $(OUTPUTL)
  CmdVarsList.Add( MACRO_OUTPUT1 );             //  $(OUTPUT1)
  CmdVarsList.Add( MACRO_OUTPUT );              //  $(OUTPUT)
  CmdVarsList.Add( MACRO_NPP_DIRECTORY );       //  $(NPP_DIRECTORY)
  CmdVarsList.Add( MACRO_FILE_NAMEONLY );       //  $(NAME_PART)
  CmdVarsList.Add( MACRO_MSG_WPARAM );          //  $(MSG_WPARAM)
  CmdVarsList.Add( MACRO_MSG_RESULT );          //  $(MSG_RESULT)
  CmdVarsList.Add( MACRO_MSG_LPARAM );          //  $(MSG_LPARAM)
  CmdVarsList.Add( MACRO_LEFT_VIEW_FILE );      //  $(LEFT_VIEW_FILE)
  CmdVarsList.Add( _T("$(INPUT[1])") );         //  $(INPUT[1])
  CmdVarsList.Add( MACRO_INPUT );               //  $(INPUT)
  CmdVarsList.Add( MACRO_FILE_FULLPATH );       //  $(FULL_CURRENT_PATH)
  CmdVarsList.Add( MACRO_FILE_FULLNAME );       //  $(FILE_NAME)
  CmdVarsList.Add( MACRO_FILE_EXTONLY );        //  $(EXT_PART)
  CmdVarsList.Add( MACRO_EXITCODE );            //  $(EXITCODE)
  CmdVarsList.Add( MACRO_CURRENT_WORKING_DIR ); //  $(CWD)
  CmdVarsList.Add( MACRO_CURRENT_WORD );        //  $(CURRENT_WORD)
  CmdVarsList.Add( MACRO_CURRENT_LINE );        //  $(CURRENT_LINE)
  CmdVarsList.Add( MACRO_FILE_DIRPATH );        //  $(CURRENT_DIRECTORY)
  CmdVarsList.Add( MACRO_CURRENT_COLUMN );      //  $(CURRENT_COLUMN)
  CmdVarsList.Add( _T("$(ARGV[1])") );          //  $(ARGV[1])
  CmdVarsList.Add( _T("$(ARGV)") );             //  $(ARGV)
  CmdVarsList.Add( MACRO_ARGC );                //  $(ARGC)
  CmdVarsList.Add( _T("$(#1)") );               //  $(#1)
  // commands in reverse order
  CmdVarsList.Add( CMD_SELSETTEXTEX );
  CmdVarsList.Add( CMD_SELSETTEXT );
  CmdVarsList.Add( CMD_SELSAVETO );
  CmdVarsList.Add( CMD_SELLOADFROM );
  CmdVarsList.Add( CMD_SCISENDMSG );
  CmdVarsList.Add( CMD_NPPSWITCH );
  CmdVarsList.Add( CMD_NPPSENDMSGEX );
  CmdVarsList.Add( CMD_NPPSENDMSG );
  CmdVarsList.Add( CMD_NPPSAVEAS );
  CmdVarsList.Add( CMD_NPPSAVEALL );
  CmdVarsList.Add( CMD_NPPSAVE );
  CmdVarsList.Add( CMD_NPPRUN );
  CmdVarsList.Add( CMD_NPPOPEN );
  CmdVarsList.Add( CMD_NPPMENUCOMMAND );
  CmdVarsList.Add( CMD_NPPEXEC );
  CmdVarsList.Add( CMD_NPPCONSOLE );
  CmdVarsList.Add( CMD_NPPCLOSE );
  CmdVarsList.Add( CMD_NPENOEMPTYVARS );
  CmdVarsList.Add( CMD_NPEDEBUGLOG );
  CmdVarsList.Add( CMD_NPECONSOLE );
  CmdVarsList.Add( CMD_NPECMDALIAS );
  CmdVarsList.Add( CMD_INPUTBOX );
  CmdVarsList.Add( CMD_ENVUNSET );
  CmdVarsList.Add( CMD_ENVSET );
  CmdVarsList.Add( CMD_CONSAVETO );
  CmdVarsList.Add( CMD_CONLOADFROM );
  CmdVarsList.Add( CMD_CONFILTER );
  CmdVarsList.Add( CMD_CONCOLOUR );
}

void ConsoleDlg::loadCmdHistory(void)
{
  if ( !Runtime::GetOptions().GetBool(OPTB_CONSOLE_SAVECMDHISTORY) )
    return;
  
  TCHAR            path[FILEPATH_BUFSIZE];
  CFileBufT<TCHAR> fbuf;
  tstr             S;

  Runtime::GetNppExec().ExpandToFullConfigPath(path, CMDHISTORY_FILENAME);
  if ( fbuf.LoadFromFile(path) )
  {
    while ( fbuf.GetLine(S) >= 0 )
    {
      CmdHistoryList.Add(S);
    }
  }
}

void ConsoleDlg::saveCmdHistory(void)
{
  if ( !Runtime::GetOptions().GetBool(OPTB_CONSOLE_SAVECMDHISTORY) )
    return;

  if ( !bCmdHistoryUpdated )
    return;
    
  TCHAR            path[FILEPATH_BUFSIZE];
  CFileBufT<TCHAR> fbuf;
  tstr             S;

  Runtime::GetNppExec().ExpandToFullConfigPath(path, CMDHISTORY_FILENAME);
  fbuf.GetBufPtr()->Reserve( CmdHistoryList.GetCount() * 64 );
  CListItemT<tstr> * p = CmdHistoryList.GetFirst();
  while ( p )
  {
    S = p->GetItem();
    S += _T("\r\n");
    fbuf.GetBufPtr()->Append( S.c_str(), S.length() );
    p = p->GetNext();
  }
  if ( fbuf.SaveToFile(path) )
    bCmdHistoryUpdated = false;
}

void ConsoleDlg::SaveCmdHistory()
{
  saveCmdHistory();
}

void ConsoleDlg::enableFindControls(bool bEnable)
{
  bFindControls = bEnable;
  btFindClose.ShowWindow( bEnable ? SW_SHOWNORMAL : SW_HIDE );
  chFindCase.ShowWindow( bEnable ? SW_SHOWNORMAL : SW_HIDE );
  edFind.ShowWindow( bEnable ? SW_SHOWNORMAL : SW_HIDE );
  chWordWrap.ShowWindow( bEnable ? SW_SHOWNORMAL : SW_HIDE );
  chHideToggled.ShowWindow( bEnable ? SW_SHOWNORMAL : SW_HIDE );
  if (bEnable) 
  {
    edFind.SetFocus();
  }
  else
  {
    Runtime::GetNppExec().GetConsole().GetConsoleEdit().SetFocus();
  }
}

void ConsoleDlg::doWordWrap(HWND hDlg)
{
    HWND hEd = ::GetDlgItem(hDlg, IDC_RE_CONSOLE);   
    if ( bWordWrap = !bWordWrap )  // changing the word-wrap value
        SendMessage(hEd, EM_SETTARGETDEVICE, 0, 0);  // enable word-wrap
    else
        SendMessage(hEd, EM_SETTARGETDEVICE, 0, 1);  // disable word-wrap
    
    chWordWrap.SetCheck(bWordWrap ? TRUE : FALSE);
    Runtime::GetOptions().SetBool(OPTB_CONSOLE_WORDWRAP, bWordWrap);
}

void ConsoleDlg::doHideToggled(HWND hDlg)
{
    bHideToggled = !bHideToggled;

    chHideToggled.SetCheck(bHideToggled ? TRUE : FALSE);
    Runtime::GetOptions().SetBool(OPTB_CONSOLE_HIDETOGGLED, bHideToggled);
}

LRESULT CALLBACK ConsoleDlg::findEditWndProc(
  HWND   hEd, 
  UINT   uMsg, 
  WPARAM wParam, 
  LPARAM lParam)
{
  if (uMsg == WM_KEYDOWN)
  {
    if (wParam == VK_RETURN)
    {
      TCHAR szText[500];
      int len = edFind.GetText(szText, 500-1);
      if (len > 0)
      {
        szText[len] = 0;

        WPARAM wFlags = FR_DOWN;
        if (chFindCase.IsChecked())  wFlags |= FR_MATCHCASE;

        int      pos = 0;
        FINDTEXT ft;
        Runtime::GetNppExec().GetConsole().GetConsoleEdit().ExGetSelPos(NULL, &pos);
        ft.chrg.cpMin = pos;
        ft.chrg.cpMax = -1;
        ft.lpstrText = szText;
        
        pos = (int) ::SendMessage(Runtime::GetNppExec().GetConsole().GetConsoleWnd(), 
                        EM_FINDTEXT, wFlags, (LPARAM) &ft);
        if (pos == -1)
        {
          ft.chrg.cpMin = 0;
          ft.chrg.cpMax = -1;
          ft.lpstrText = szText;
          pos = (int) ::SendMessage(Runtime::GetNppExec().GetConsole().GetConsoleWnd(), 
                          EM_FINDTEXT, wFlags, (LPARAM) &ft);
        }
        if (pos >= 0)
        {
          Runtime::GetNppExec().GetConsole().GetConsoleEdit().ExSetSel(pos, pos + len);
        }
      }
      return 1;
    }
    else if (wParam == VK_ESCAPE)
    {
      if (bFindControls)
      {
        enableFindControls(false);
        OnSize( ::GetParent(hEd) );
      }
      return 1;
    }
  }

  return CallWindowProc(findEditWndProc0, hEd, uMsg, wParam, lParam);
}

#ifdef _consoledlg_re_subclass_
  LRESULT CALLBACK ConsoleDlg::RichEditWndProc(
    HWND   hEd, 
    UINT   uMessage, 
    WPARAM wParam, 
    LPARAM lParam)
  {
    //static bool bTrackingMouse = false;
    
    /**/
    if (uMessage == WM_SETFOCUS)
    {
      ConsoleDlg::EnableTransparency(false);
      //return 0;
    }
    /**/
    else if (uMessage == WM_KILLFOCUS)
    {
      if ( wParam && (((HWND) wParam) == Runtime::GetNppExec().GetScintillaHandle()) )
      {
        if ( (GetKeyState(VK_LBUTTON) & 0x80) == 0 ) // left mouse button not pressed
          ConsoleDlg::EnableTransparency(true);
        //return 0;
      }
    }
    /*
    else if (uMessage == WM_MOUSEMOVE)
    {
      if ( (!bTrackingMouse) && (hEd != ::GetFocus()) )
      {
        TRACKMOUSEEVENT tme;

        tme.cbSize = sizeof(TRACKMOUSEEVENT);
        tme.dwFlags = TME_LEAVE;
        tme.hwndTrack = hEd;
        tme.dwHoverTime = HOVER_DEFAULT;
        if ( _TrackMouseEvent(&tme) )
        {
          bTrackingMouse = true;
        }
        ConsoleDlg::EnableTransparency(false);
      }
    }    
    else if (uMessage == WM_MOUSELEAVE)
    {
      HWND hFocused = ::GetFocus();
      
      bTrackingMouse = false;
      if ( (hFocused != hEd) &&
           (::GetParent(hFocused) != Runtime::GetNppExec().GetConsole().GetParentWnd()) )
      {     
        ConsoleDlg::EnableTransparency(true);
      }  
    }
    */

    return CallWindowProc(OriginalRichEditProc, hEd, uMessage, wParam, lParam);
  }
#endif
