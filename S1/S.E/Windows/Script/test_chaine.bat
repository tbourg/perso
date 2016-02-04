set /p ch1=Chaine1
set /p ch2=Chaine2
set /p ch3=Chaine3
if /i "%ch1%" EQU "%ch2%" (
if /i "%ch3%" EQU "%ch2%" (
echo 0
) else (
echo 3 )
) else (
if /i "%ch3%" EQU "%ch2%" (
echo 1
) else (
if "%ch1%" EQU "%ch3%" (
echo 2
) else (
echo 4 )))