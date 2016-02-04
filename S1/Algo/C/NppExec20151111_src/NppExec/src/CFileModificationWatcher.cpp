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

#include "CFileModificationWatcher.h"

CFileModificationWatcher::CFileModificationWatcher() : 
  m_bTimeIsValid(FALSE), m_bSizeIsValid(FALSE)
{
}

CFileModificationWatcher::~CFileModificationWatcher()
{
}

BOOL CFileModificationWatcher::getFileTime(HANDLE hFile, FILETIME* lpLastWriteTime)
{
    return ::GetFileTime(hFile, NULL, NULL, lpLastWriteTime);
}

DWORD CFileModificationWatcher::getFileSize(HANDLE hFile)
{
    return ::GetFileSize(hFile, NULL);
}

HANDLE CFileModificationWatcher::openTheFile(void)
{
    HANDLE hFile = ::CreateFile( m_path.c_str(), GENERIC_READ, 
                       FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL );
    return ( (hFile != INVALID_HANDLE_VALUE) ? hFile : NULL );
}

BOOL CFileModificationWatcher::AssignFile(const TCHAR* cszFilePathName)
{
    m_path = cszFilePathName;
    return UpdateFileInfo();
}

long CFileModificationWatcher::IsFileSizeChanged(DWORD dwFileSize)
{
    if ( !m_bSizeIsValid )  
        m_dwFileSize = 0;
    return ((long) m_dwFileSize) - ((long) dwFileSize);
}

long CFileModificationWatcher::IsFileTimeChanged(const FILETIME* lpFileTime)
{
    if ( !m_bTimeIsValid )  
        return -1;
    return ::CompareFileTime( &m_modificationTime, lpFileTime );
}

BOOL CFileModificationWatcher::RequestFileTime(FILETIME* lpLastWriteTime)
{
    BOOL bResult = FALSE;
    if ( lpLastWriteTime )
    {
        HANDLE hFile = openTheFile();
        if ( hFile )
        {
            bResult = getFileTime(hFile, lpLastWriteTime);
            ::CloseHandle(hFile);
        }
    }
    return bResult;
}

BOOL CFileModificationWatcher::RequestFileSize(DWORD* lpdwSize)
{
    if ( lpdwSize )
    {
        HANDLE hFile = openTheFile();
        if ( hFile )
        {
            *lpdwSize = getFileSize(hFile);
            ::CloseHandle(hFile);
            return TRUE;
        }
    }
    return FALSE;
}

void CFileModificationWatcher::SetTime(const FILETIME* lpFileTime)
{
    ::CopyMemory( &m_modificationTime, lpFileTime, sizeof(FILETIME) );
    m_bTimeIsValid = TRUE;
}

void CFileModificationWatcher::SetSize(DWORD dwFileSize)
{
    m_dwFileSize = dwFileSize;
    m_bSizeIsValid = TRUE;
}

BOOL CFileModificationWatcher::UpdateFileInfo(void)
{
    HANDLE hFile = openTheFile();
    if ( hFile )
    {
        m_bTimeIsValid = getFileTime(hFile, &m_modificationTime);
        m_dwFileSize = getFileSize(hFile);
        m_bSizeIsValid = TRUE;
        ::CloseHandle(hFile);
        return m_bTimeIsValid;
    }
    return FALSE;
}

