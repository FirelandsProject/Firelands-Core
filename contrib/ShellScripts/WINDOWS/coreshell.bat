@echo off
set nssmService="testrealm"
SET homeDir="C:\FirelandsCata"
SET binDir="%homeDir%\bin"
SET sourceDir="%homeDir%\firelands-cata"

if "%1"=="update" (
    cd %binDir%
    update.bat %homeDir% %nssmService%
)

if "%1"=="restore" (
    nssm %nssmService% stop
    del %binDir%/worldserver.exe
    copy %binDir%/worldserver.backup.exe %binDir%/worldserver.exe
    nssm %nssmService% start
)

if "%1"=="switchbranch" (
    cd %sourceDir%
    git checkout %2
)