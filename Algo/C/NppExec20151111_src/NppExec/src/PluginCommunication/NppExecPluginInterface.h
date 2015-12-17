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

#ifndef _npp_exec_plugin_interface_h_
#define _npp_exec_plugin_interface_h_
//---------------------------------------------------------------------------
#include "../base.h"
#include "../Notepad_plus_msgs.h"
#include "nppexec_msgs.h"

void ProcessExternalPluginMsg(long nMsg, const TCHAR* srcModuleName, void* pInfo);
void NotifyExternalPluginResult(DWORD dwResultCode);

//---------------------------------------------------------------------------
#endif
