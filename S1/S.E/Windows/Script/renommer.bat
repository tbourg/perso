set name=%1%
set ext=%2%
set cpt=1
set nb=0
for %%a in (*.%ext%) do set /a nb+=1
:chgnom
ren *.%ext% %name%%cpt%.%ext%
set /a cpt=%cpt%+1
if exist *.%ext% (
if not exist %name%%nb%.%ext% (
goto chgnom
)
)
exit /b