@echo off
set _d=
set _arch_=/reg:64
if "%~1"=="Debug" (
    set _d=D
)
if "%~1"=="x86" (
    set _arch_=/reg:32
) 
if "%~2"=="Debug" (
    set _d=D
)
if "%~2"=="x86" (
    set _arch_=/reg:32
)
REG IMPORT %CD%\Regentries%_d%.reg %_arch_%
::runas /profile /user:%USERDOMAIN%\kalle "%COMSPEC% /k \"REG IMPORT %CD%\Regentries%_d%.reg %_arch_%\""
set _arch_=
set _d=