if "%3" == "" (
echo 5
exit /b )
if not "%5" == "" (
echo 5
exit /b )
if "%1" == "%2" (
if "%3" == "%2" (
echo 0
) else (
echo 3 )
) else (
if "%3" == "%2" (
echo 1
) else (
if "%1" == "%3" (
echo 2
) else (
echo 4 )))