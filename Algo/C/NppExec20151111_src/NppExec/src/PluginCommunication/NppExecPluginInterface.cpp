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

#include "../NppExec.h"
#include "NppExecPluginInterface.h"


const DWORD  PLUGIN_VER_DWORD = NPPEXEC_VER_DWORD;
#ifdef UNICODE
    const TCHAR* PLUGIN_VER_STR = NPPEXEC_VER_STR _T(" Unicode"); // see also: DlgConsole.cpp
#else
    const TCHAR* PLUGIN_VER_STR = NPPEXEC_VER_STR _T(" ANSI"); // see also: DlgConsole.cpp
#endif


CStrT<TCHAR> g_lastExternalPluginName;

extern TCHAR    PLUGIN_NAME_DLL[];


DWORD npemGetState();
void  npemExecuteScript(const TCHAR* szScriptBody);
void  npemPrint(const TCHAR* szText);

void ProcessExternalPluginMsg(long nMsg, const TCHAR* srcModuleName, void* pInfo)
{
    if ( (!pInfo) && (nMsg != NPEM_SUSPENDEDACTION) )
        return;
    
    switch ( nMsg )
    {
        case NPEM_GETVERDWORD:
        {
            Runtime::GetLogger().Add( _T("; NPEM_GETVERDWORD") );

            // pInfo is (DWORD* pdwVersion)
            DWORD* pdwVersion = (DWORD *) pInfo;
            *pdwVersion = PLUGIN_VER_DWORD;
            
            break;
        }
        
        case NPEM_GETVERSTR:
        {
            Runtime::GetLogger().Add( _T("; NPEM_GETVERSTR") );

            // pInfo is (TCHAR* pszVersion)
            TCHAR* pszVersion = (TCHAR *) pInfo;
            const TCHAR* cszVer = PLUGIN_VER_STR;
            while ( *(pszVersion++) = *(cszVer++) )
            {
                if ( (NPE_MAXVERSTR - 1) == (int) (cszVer - PLUGIN_VER_STR) )
                {
                    *pszVersion = 0;
                    break;
                }
            }
            
            break;
        }
        
        case NPEM_GETSTATE:
        {
            Runtime::GetLogger().Add( _T("; NPEM_GETSTATE") );

            // pInfo is (DWORD* pdwState)
            *((DWORD *) pInfo) = npemGetState();

            break;
        }

        case NPEM_PRINT:
        {
            Runtime::GetLogger().Add( _T("; NPEM_PRINT") );

            // pInfo is (const TCHAR* szText)
            const TCHAR* szText = (const TCHAR *) pInfo;
            if ( (npemGetState() == NPE_STATEREADY) && szText )
            {
                npemPrint(szText);
            }

            break;
        }

        case NPEM_EXECUTE:
        {
            Runtime::GetLogger().Add( _T("; NPEM_EXECUTE") );

            // pInfo is (NpeExecuteParam* nep)
            NpeExecuteParam* nep = (NpeExecuteParam *) pInfo;
            if ( (npemGetState() == NPE_STATEREADY) &&
                 nep->szScriptBody && nep->szScriptBody[0] )
            {
                if ( nep->dwResult )
                    g_lastExternalPluginName = srcModuleName;
                else
                    g_lastExternalPluginName.Clear();
                nep->dwResult = NPE_EXECUTE_OK;
                npemExecuteScript( nep->szScriptBody );
            }
            else
            {
                nep->dwResult = NPE_EXECUTE_FAILED;
            }
            
            break;
        }

        case NPEM_NPPEXEC:
        {
            Runtime::GetLogger().Add( _T("; NPEM_NPPEXEC") );

            // pInfo is (NpeNppExecParam* npep)
            NpeNppExecParam* npep = (NpeNppExecParam *) pInfo;
            if ( (npemGetState() == NPE_STATEREADY) &&
                 npep->szScriptName && npep->szScriptName[0] )
            {
                if ( npep->dwResult )
                    g_lastExternalPluginName = srcModuleName;
                else
                    g_lastExternalPluginName.Clear();
                npep->dwResult = NPE_NPPEXEC_OK;
                Runtime::GetNppExec().DoExecScript( npep->szScriptName, true, npep->szScriptArguments );
            }
            else
            {
                npep->dwResult = NPE_NPPEXEC_FAILED;
            }

            break;
        }

        case NPEM_SUSPENDEDACTION:
        {
            const CSuspendedScriptAction::ActionParams& params = Runtime::GetNppExec().m_SuspendedScriptAction.getParams();
            switch ( Runtime::GetNppExec().m_SuspendedScriptAction.getType() )
            {
                case CSuspendedScriptAction::AT_ONDOEXEC :
                    Runtime::GetLogger().Add( _T("; NPEM_SUSPENDEDACTION (AT_ONDOEXEC)") );
                    Runtime::GetNppExec().OnDoExec();
                    break;

                case CSuspendedScriptAction::AT_ONDIRECTEXEC :
                    Runtime::GetLogger().Add( _T("; NPEM_SUSPENDEDACTION (AT_ONDIRECTEXEC)") );
                    Runtime::GetNppExec().OnDirectExec(params.bCanSaveAll);
                    break;

                case CSuspendedScriptAction::AT_DOEXECSCRIPT :
                    Runtime::GetLogger().Add( _T("; NPEM_SUSPENDEDACTION (AT_DOEXECSCRIPT)") );
                    Runtime::GetNppExec().DoExecScript(params.ScriptName.c_str(), params.bCanSaveAll, params.ScriptArguments.c_str());
                    break;

                case CSuspendedScriptAction::AT_CLOSECONSOLE :
                    Runtime::GetLogger().Add( _T("; NPEM_SUSPENDEDACTION (AT_CLOSECONSOLE)") );
                    Runtime::GetNppExec().m_SuspendedScriptAction.setActive(false);
                    Runtime::GetNppExec().showConsoleDialog(true);
                    ::SetFocus( Runtime::GetNppExec().GetScintillaHandle() );
                
                default:
                    Runtime::GetLogger().Add( _T("; NPEM_SUSPENDEDACTION (default: deactivate)") );
                    Runtime::GetNppExec().m_SuspendedScriptAction.setActive(false);
                    break;
            }
            
            break;
        }
        
        default:
            break;
    }
}

void NotifyExternalPluginResult(DWORD dwResultCode)
{
    if ( g_lastExternalPluginName.length() > 0 )
    {
        static DWORD             dwResult;
        static CStrT<TCHAR>      pluginFrom;
        static CStrT<TCHAR>      pluginTo;
        static CommunicationInfo ci;

        dwResult = dwResultCode;
        pluginFrom = PLUGIN_NAME_DLL;  // "NppExec.dll"
        pluginTo = g_lastExternalPluginName;
        ci.internalMsg = NPEN_RESULT;
        ci.srcModuleName = pluginFrom.c_str();
        ci.info = (void *) &dwResult;
        ::PostMessage( 
            Runtime::GetNppExec().m_nppData._nppHandle, 
            NPPM_MSGTOPLUGIN,
            (WPARAM) pluginTo.c_str(),
            (LPARAM) &ci 
        );
        g_lastExternalPluginName.Clear();
    }
}

DWORD npemGetState()
{
    DWORD dwState = NPE_STATEREADY;

    if ( Runtime::GetNppExec()._consoleProcessIsRunning )
        dwState = NPE_STATECHILDPROC;
    else if ( Runtime::GetNppExec().isScriptRunning() )
        dwState = NPE_STATEBUSY;
    
    return dwState;
}

void addScriptCommand(CStrT<TCHAR>& Cmd)
{
    while ( Cmd.GetLastChar() == _T('\r') )
    {
        // remove trailing '\r'
        Cmd.SetSize(Cmd.length() - 1);
    }
    if ( Cmd.length() > 0 )
    {
        Runtime::GetNppExec().m_CmdList.Add( Cmd );
    }
}

void npemExecuteScript(const TCHAR* szScriptBody)
{
    CStrT<TCHAR> Line;

    Runtime::GetNppExec().m_CmdList.DeleteAll();
    while ( *szScriptBody )
    {
        if ( *szScriptBody == _T('\n') )
        {
            addScriptCommand( Line );
            Line.Clear();
        }
        else
        {
            Line += *szScriptBody;
        }
        ++szScriptBody;
    }
    addScriptCommand( Line );
    Runtime::GetNppExec().OnDirectExec(true);
}

void conPrintLine(CStrT<TCHAR>& Line)
{
    while ( Line.GetLastChar() == _T('\r') )
    {
        // remove trailing '\r'
        Line.SetSize(Line.length() - 1);
    }
    Runtime::GetNppExec().GetConsole().PrintOutput( Line.c_str(), true );
}

void npemPrint(const TCHAR* szText)
{
    CStrT<TCHAR> Line;
    
    Runtime::GetNppExec()._consoleProcessIsRunning = true;
    Runtime::GetNppExec()._consoleProcessBreak = false;

    Runtime::GetNppExec().showConsoleDialog(false);

    while ( *szText )
    {
        if ( *szText == _T('\n') )
        {
            conPrintLine( Line );
            Line.Clear();
        }
        else
        {
            Line += *szText;
        }
        ++szText;
    }
    conPrintLine( Line );

    Runtime::GetNppExec()._consoleProcessIsRunning = false;
}
