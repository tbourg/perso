set oldext=%1%
set newext=%2%
for %%a in (*.%oldext%) do call :changext %%a %newext%
::ren "*.%oldext%" "*.%newext%"
exit /b

:changext
::ren %%1 "*.%2%"
set nom=%~n1
ren %1 %nom%.%2%
::set ext1=%~x1
::set %~x1=.%2%
