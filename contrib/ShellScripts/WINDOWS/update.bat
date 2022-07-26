@echo off
call "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat" amd64

rem Settings
SET homeDir=%1
SET cmake="C:\Program Files (x86)\CMake\bin\cmake.exe"

SET binDir="%homeDir%\bin"
SET compileDir="%homeDir%\build"
SET confDir="%homeDir%\etc"
SET sourceDir="%homeDir%\firelands-cata"
SET buildConf=RelWithDebInfo
SET projectFile="FirelandsCata.sln"
SET compiler="Visual Studio 14 2015 Win64"

echo "########################"
echo "# Updating Core:"
echo "########################"
cd %sourceDir%
git pull
echo "########################"
echo "# Compiling Core:"
echo "########################"

cd %compileDir%
del %compileDir%\CMakeCache.txt
del %compileDir%\revision.h
del %binDir%\worldserver.backup.exe
copy -u %binDir%\worldserver.exe %binDir%\worldserver.backup.exe
%cmake% %sourceDir% -DWITH_MMAPS=0 -DAUTHSERVER=0 -DWITH_COREDEBUG=0 -DPREFIX=%homeDir% -DCONF_DIR=%confDir% -DCMAKE_BUILD_TYPE=%buildConf% -G %compiler%

rem %1 == nssmService
nssm %2 stop

msbuild %projectFile% /p:configuration=%buildConf% /p:Platform=x64

cd %binDir%
nssm %2 start