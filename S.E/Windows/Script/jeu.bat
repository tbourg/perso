@echo off
set r=%random%
set j=101
set /a r%%=100
for /l %%i in (1,1,10) do call :compar %j% %%i %r%




:compar
set /p nbjoueur=Votre nombre
set /a rest=10-%2%
if /i %nbjoueur% GTR %3% (
		echo trop grand
		echo %rest% coups restants
)
if /i %nbjoueur% LSS %3% (
		echo trop petit
		echo %rest% coups restants
)
 if /i %nbjoueur% EQU %3% (
		echo bravo
		exit
)