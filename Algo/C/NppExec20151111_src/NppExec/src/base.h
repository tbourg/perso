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

#ifndef _base_h_
#define _base_h_
//-------------------------------------------------------------------------
//#define _NIB - now part of the sources
//#define _GVC - now part of the sources
#define _GVCRESTORE
//#define _SCROLL_TO_LATEST

#include <windows.h>
#include <TCHAR.h>

// Rich Edit's End-Of-Line
//#define _T_RE_EOL _T("\r\n")  // RichEdit 1.0
//#define _T_RE_EOL_LEN 2       // RichEdit 1.0
#define _T_RE_EOL _T("\r")    // RichEdit 2.0
#define _T_RE_EOL_LEN 1       // RichEdit 2.0

//-------------------------------------------------------------------------
#endif
