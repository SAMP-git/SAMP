@ECHO OFF

TITLE Update SAMP Includes v1.0
COLOR 9F

SET SETTINGS=%CD%\samp_settings.ini
SET LISTFILE=%CD%\samp_include.lst

ECHO.
ECHO Update SAMP Includes v1.0
ECHO.

IF NOT EXIST "%SETTINGS%" (
	ECHO [SAM] >> "%SETTINGS%"
	ECHO version=1.0 >> "%SETTINGS%" 
	ECHO input=https://raw.githubusercontent.com/AbyssMorgan/SA-MP/master/samp/include >> "%SETTINGS%" 
	ECHO output=%CD%\include >> "%SETTINGS%" 
)

FOR /F "tokens=*" %%s IN (samp_settings.ini) DO CALL :READINI %%s

IF NOT "%VERSION%" == "1.0" ECHO Invalid settings & PAUSE > nul & EXIT
IF NOT DEFINED INPUT ECHO Source is not set & PAUSE > nul & EXIT
IF NOT DEFINED OUTPUT ECHO Destination folder is not set & PAUSE > nul & EXIT

ECHO Update Include List
IF EXIST "%CD%\samp_include.tmp" DEL /Q /A "%CD%\samp_include.tmp"
powershell -command "& { (New-Object Net.WebClient).DownloadFile('%INPUT%/x_include.lst', '%CD%\samp_include.tmp') }"
IF EXIST "%CD%\samp_include.tmp" (
	IF EXIST "%CD%\samp_include.lst" (
		DEL /Q /A "%CD%\samp_include.lst"
	)
	REN "%CD%\samp_include.tmp" "samp_include.lst"
)
ECHO.

IF NOT EXIST "%LISTFILE%" ECHO List file not exist & PAUSE > nul & EXIT

IF NOT EXIST "%OUTPUT%" (
	ECHO Create Output folder: %OUTPUT%
	MD "%OUTPUT%"
	ECHO.
)

FOR /F "tokens=*" %%s IN (samp_include.lst) DO (
	ECHO Download %%s
	CALL :CHECK_FOLDER "%OUTPUT%/%%s"
	powershell -command "& { (New-Object Net.WebClient).DownloadFile('%INPUT%/%%s', '%OUTPUT%/%%s') }"
)

ECHO.
ECHO End
ECHO.
PAUSE > nul
GOTO :eof

:CHECK_FOLDER
SET DR=%~dp1
IF "%DR:~-1,1%" == "\" SET DR=%DR:~0,-1%
IF NOT EXIST "%DR%" (
	ECHO Create Output folder: %DR%
	MD "%DR%"
	ECHO.
)
GOTO :eof

REM Read Ini file
:READINI
SET BUFFER=%*
IF "%BUFFER:~0,1%" == "$" GOTO :eof
IF "%BUFFER:~0,1%" == "#" GOTO :eof
IF "%BUFFER:~0,1%" == ";" GOTO :eof
IF "%BUFFER:~0,1%" == "!" GOTO :eof
IF "%BUFFER:~0,1%%BUFFER:~-1,1%" == "[]" GOTO :eof
SET %*
GOTO :eof
