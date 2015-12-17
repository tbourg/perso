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

#ifndef _simple_logger_h_
#define _simple_logger_h_
//---------------------------------------------------------------------------
#include "base.h"
#include <TCHAR.h>
#include "cpp/CStrT.h"
#include "cpp/CListT.h"

class LogFileWriter;

class CSimpleLogger
{
public:
    typedef CStrT<TCHAR> tstr; 
    typedef void (*OUTPUTFUNC)(const TCHAR* str, int len);

protected:
    CListT<tstr>   m_StrList;
    tstr           m_IndentStr;
    tstr           m_strIndentLevel;
    int            m_nIndentLevel;
    bool           m_bCanUpdate;
    OUTPUTFUNC     m_pOutputStrFunc;
    LogFileWriter* m_pFileWriter;
    
    void applyIndentLevel(int nIndentLevel);
    void writeFileLogStart();
    void writeFileLogEnd();

public:
    CSimpleLogger();
    ~CSimpleLogger();

    void Activate(bool bActivate)  { m_bCanUpdate = bActivate; }
    void Add(const TCHAR* str);
    void AddEx(const TCHAR* fmt, ...);
    void Clear();
    void DecIndentLevel();
    const TCHAR* GetIndentStr() const { return m_IndentStr.c_str(); }
    void IncIndentLevel();
    void OutputAll(OUTPUTFUNC pOutputAllFunc);
    void SetIndentStr(const TCHAR* str);
    bool IsOutputMode() const { return (m_pOutputStrFunc ? true: false); }
    void SetOutputMode(bool bDirectOutput, OUTPUTFUNC pOutputStrFunc = NULL);

    const TCHAR* GetLogFile() const;
    bool IsLogFileOpen() const;
    bool SetLogFile(const TCHAR* logFilePathName);
};

//---------------------------------------------------------------------------
#endif

