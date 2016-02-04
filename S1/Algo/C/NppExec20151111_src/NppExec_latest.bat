@echo off

set ARC_EXE=C:\Progs\Far\7z.exe
for /f "tokens=1-3 delims=/.- " %%a in ('DATE /T') do set ARC_DATE=%%c%%b%%a
set ARC_SRC_NAME=NppExec%ARC_DATE%_src
set ARC_DLL_NAME=NppExec%ARC_DATE%_dll

%ARC_EXE% u -tzip %ARC_SRC_NAME%.zip @NppExec_src.txt -mx5
%ARC_EXE% t %ARC_SRC_NAME%.zip

cd NppExec
if not exist Release\NppExec.dll goto no_dll_file_exit
cd Release
%ARC_EXE% u -tzip ..\..\%ARC_DLL_NAME%.zip NppExec.dll NppExec\*.h -mx5
cd ..
%ARC_EXE% u -tzip ..\%ARC_DLL_NAME%.zip doc\NppExec.txt doc\NppExec_TechInfo.txt doc\NppExec*.chm doc\*.html doc\*.css doc\*.png -mx5
cd ..
%ARC_EXE% t %ARC_DLL_NAME%.zip

:no_dll_file_exit