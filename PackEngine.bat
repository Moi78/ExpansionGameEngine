@echo off

set /p PROJ_NAME="Project Name: "
if %PROJ_NAME% equ "" (
    echo Bad project name, abort...
    goto EndScript
)

echo Project name set to : %PROJ_NAME%

echo Creating project folder...
mkdir %PROJ_NAME%

echo Entering .\%PROJ_NAME%
cd %PROJ_NAME%
echo Making folder tree...
mkdir Bins
mkdir Lib

cd Bins
mkdir Win64
mkdir Win64\Debug
mkdir Win64\Release

cd ..\Lib
mkdir Debug
mkdir Release
mkdir 3rdParty

cd ..
mkdir Include
mkdir Include\EXPGE
mkdir Include\3rdParty

echo Leaving project dir
cd ..

echo Checking if all files are present...

echo Searching for Debug bins...
cd .\src\x64\Debug
CALL :CheckEngineBins

echo Searching for Release bins...
cd ..\Release
CALL :CheckEngineBins

echo Searching for libs bins...
cd ..\..\libs
CALL :CheckLibBins

echo Searching for libs include folder
CALL :CheckLibIncFolders

echo All files are here, returning to root folder...
cd ..\..\

echo Copying engine files...
CALL :CopyEngineBins
CALL :CopyEngineInclude
CALL :CopyEngineContentFolder

echo Copying libs files...
CALL :CopyLibsBins
CALL :CopyLibsInc

goto EndScript

rem -----------------------------------------------------------------------------------

:CopyLibsInc

echo Copying Libs Include Files
cd src/libs
set INC_PATH=..\..\%PROJ_NAME%\Include\3rdParty\

robocopy AL\include %INC_PATH% /is /it /ndl /njh /nc /ns /njs /e
robocopy PHYSX\PhysX\include %INC_PATH% /is /it /ndl /njh /nc /ns /njs /e
robocopy PHYSX\PxShared\include %INC_PATH% /is /it /ndl /njh /nc /ns /njs /e
robocopy GL\include %INC_PATH% /is /it /ndl /njh /nc /ns /njs /e
robocopy JSONCPP\include %INC_PATH% /is /it /ndl /njh /nc /ns /njs /e
robocopy libsndfile\include %INC_PATH% /is /it /ndl /njh /nc /ns /njs /e

cd ..\..

exit /B 0

rem -----------------------------------------------------------------------------------

:CopyLibsBins
set DEBUG_PATH=..\..\%PROJ_NAME%\Bins\Win64\Debug\
set RELEASE_PATH=..\..\%PROJ_NAME%\Bins\Win64\Release\

set DEBUGLIB_PATH=..\..\%PROJ_NAME%\Lib\3rdParty\

echo Copying Debug bins...
cd src/libs

copy /Y AL\bin\Debug\OpenAL32.dll %DEBUG_PATH%\OpenAL32.dll

copy /Y GL\dll\Debug\glew32.dll %DEBUG_PATH%\glew32.dll
copy /Y GL\dll\Debug\glfw3.dll %DEBUG_PATH%\glfw3.dll

copy /Y libsndfile\bin\libsndfile-1.dll %DEBUG_PATH%\libsndfile-1.dll

copy /Y PHYSX\PhysX\bin\Debug\PhysX_64.dll %DEBUG_PATH%\PhysX_64.dll
copy /Y PHYSX\PhysX\bin\Debug\PhysXCommon_64.dll %DEBUG_PATH%\PhysXCommon_64.dll
copy /Y PHYSX\PhysX\bin\Debug\PhysXCooking_64.dll %DEBUG_PATH%\PhysXCooking_64.dll
copy /Y PHYSX\PhysX\bin\Debug\PhysXFoundation_64.dll %DEBUG_PATH%\PhysXFoundation_64.dll

echo Copying Release bins

copy /Y AL\bin\Release\OpenAL32.dll %RELEASE_PATH%\OpenAL32.dll

copy /Y GL\dll\Release\glew32.dll %RELEASE_PATH%\glew32.dll
copy /Y GL\dll\Release\glfw3.dll %RELEASE_PATH%\glfw3.dll

copy /Y libsndfile\bin\libsndfile-1.dll %RELEASE_PATH%\libsndfile-1.dll

copy /Y PHYSX\PhysX\bin\Release\PhysX_64.dll %RELEASE_PATH%\PhysX_64.dll
copy /Y PHYSX\PhysX\bin\Release\PhysXCommon_64.dll %RELEASE_PATH%\PhysXCommon_64.dll
copy /Y PHYSX\PhysX\bin\Release\PhysXCooking_64.dll %RELEASE_PATH%\PhysXCooking_64.dll
copy /Y PHYSX\PhysX\bin\Release\PhysXFoundation_64.dll %RELEASE_PATH%\PhysXFoundation_64.dll

echo Copying Libs lib files...

copy /Y AL\lib\OpenAL32.lib %DEBUGLIB_PATH%\OpenAL32.lib

copy /Y GL\lib\glew32.lib %DEBUGLIB_PATH%\glew32.lib
copy /Y GL\lib\glfw3dll.lib %DEBUGLIB_PATH%\glfw3dll.lib

copy /Y libsndfile\lib\libsndfile-1.lib %DEBUGLIB_PATH%\libsndfile-1.lib

copy /Y PHYSX\PhysX\bin\Release\PhysX_64.lib %DEBUGLIB_PATH%\PhysX_64.lib
copy /Y PHYSX\PhysX\bin\Release\PhysXCommon_64.lib %DEBUGLIB_PATH%\PhysXCommon_64.lib
copy /Y PHYSX\PhysX\bin\Release\PhysXCooking_64.lib %DEBUGLIB_PATH%\PhysXCooking_64.lib
copy /Y PHYSX\PhysX\bin\Release\PhysXFoundation_64.lib %DEBUGLIB_PATH%\PhysXFoundation_64.lib

echo All files copied, returning to root directory...
cd ../..

exit /B 0

rem -----------------------------------------------------------------------------------

:CopyEngineContentFolder
echo Copying Engine content folder...
xcopy src\ExGame\Engine\ %PROJ_NAME%\Engine /s /i

exit /B 0

rem -----------------------------------------------------------------------------------

:CopyEngineInclude
echo Copying Engine include files...
cd src/
set INC_EXPGE=..\%PROJ_NAME%\Include\EXPGE

robocopy .\BulldozerFileManager %INC_EXPGE% *.h /is /it /ndl /njh /nc /ns /njs
robocopy .\ExpansionGameEngineCore %INC_EXPGE% *.h /is /it /ndl /njh /nc /ns /njs
robocopy .\ExpansionMath %INC_EXPGE% *.h /is /it /ndl /njh /nc /ns /njs
robocopy .\PhysicaSound %INC_EXPGE% *.h /is /it /ndl /njh /nc /ns /njs
robocopy .\RaindropRenderer %INC_EXPGE% *.h /is /it /ndl /njh /nc /ns /njs
robocopy .\RaindropRenderer\stb %INC_EXPGE%\stb *.h /is /it /ndl /njh /nc /ns /njs

echo Every files copied, returning to root folder...
cd ../

exit /B 0

rem -----------------------------------------------------------------------------------

:CopyEngineBins
set DEBUG_PATH=..\..\..\%PROJ_NAME%\Bins\Win64\Debug\
set RELEASE_PATH=..\..\..\%PROJ_NAME%\Bins\Win64\Release\

set DEBUGLIB_PATH=..\..\..\%PROJ_NAME%\Lib\Debug\
set RELEASELIB_PATH=..\..\..\%PROJ_NAME%\Lib\Release\

echo Copying Debug Bins...
cd src/x64/Debug
copy /Y .\BulldozerFileManager.dll %DEBUG_PATH%\BulldozerFileManager.dll
copy /Y .\ExGame.exe %DEBUG_PATH%\ExGame.exe
copy /Y .\ExpansionGameEngineCore.dll %DEBUG_PATH%\ExpansionGameEngineCore.dll
copy /Y .\ExpansionMath.dll %DEBUG_PATH%\ExpansionMath.dll
copy /Y .\PhysicaSound.dll %DEBUG_PATH%\PhysicaSound.dll
copy /Y .\RaindropRenderer.dll %DEBUG_PATH%\RaindropRenderer.dll

echo Copying Release Bins...
cd ../Release
copy /Y .\BulldozerFileManager.dll %RELEASE_PATH%\BulldozerFileManager.dll
copy /Y .\ExGame.exe %RELEASE_PATH%\ExGame.exe
copy /Y .\ExpansionGameEngineCore.dll %RELEASE_PATH%\ExpansionGameEngineCore.dll
copy /Y .\ExpansionMath.dll %RELEASE_PATH%\ExpansionMath.dll
copy /Y .\PhysicaSound.dll %RELEASE_PATH%\PhysicaSound.dll
copy /Y .\RaindropRenderer.dll %RELEASE_PATH%\RaindropRenderer.dll

echo Copying Debug Lib Files...
copy /Y .\BulldozerFileManager.lib %DEBUGLIB_PATH%\BulldozerFileManager.lib
copy /Y .\ExpansionGameEngineCore.lib %DEBUGLIB_PATH%\ExpansionGameEngineCore.lib
copy /Y .\ExpansionMath.lib %DEBUGLIB_PATH%\ExpansionMath.lib
copy /Y .\PhysicaSound.lib %DEBUGLIB_PATH%\PhysicaSound.lib
copy /Y .\RaindropRenderer.lib %DEBUGLIB_PATH%\RaindropRenderer.lib

echo Copying Release Lib Files...
copy /Y .\BulldozerFileManager.lib %RELEASELIB_PATH%\BulldozerFileManager.lib
copy /Y .\ExpansionGameEngineCore.lib %RELEASELIB_PATH%\ExpansionGameEngineCore.lib
copy /Y .\ExpansionMath.lib %RELEASELIB_PATH%\ExpansionMath.lib
copy /Y .\PhysicaSound.lib %RELEASELIB_PATH%\PhysicaSound.lib
copy /Y .\RaindropRenderer.lib %RELEASELIB_PATH%\RaindropRenderer.lib

echo Copied all Engine bins, returning to root dir
cd ../../../

exit /B 0

rem --------------------------------------------------------------------------------

:CheckLibBins
CALL :CheckFileExistence AL\bin\Debug\OpenAL32.dll
CALL :CheckFileExistence AL\bin\Release\OpenAL32.dll
CALL :CheckFileExistence AL\lib\OpenAL32.lib

CALL :CheckFileExistence GL\dll\Debug\glew32.dll
CALL :CheckFileExistence GL\dll\Debug\glfw3.dll
CALL :CheckFileExistence GL\dll\Release\glew32.dll
CALL :CheckFileExistence GL\dll\Release\glfw3.dll

CALL :CheckFileExistence GL\lib\glew32.lib
CALL :CheckFileExistence GL\lib\glfw3dll.lib

CALL :CheckFileExistence libsndfile\bin\libsndfile-1.dll
CALL :CheckFileExistence libsndfile\lib\libsndfile-1.lib

CALL :CheckFileExistence PHYSX\PhysX\bin\Debug\PhysX_64.dll
CALL :CheckFileExistence PHYSX\PhysX\bin\Debug\PhysX_64.lib
CALL :CheckFileExistence PHYSX\PhysX\bin\Debug\PhysXCommon_64.dll
CALL :CheckFileExistence PHYSX\PhysX\bin\Debug\PhysXCommon_64.lib
CALL :CheckFileExistence PHYSX\PhysX\bin\Debug\PhysXCooking_64.dll
CALL :CheckFileExistence PHYSX\PhysX\bin\Debug\PhysXCooking_64.lib
CALL :CheckFileExistence PHYSX\PhysX\bin\Debug\PhysXFoundation_64.dll
CALL :CheckFileExistence PHYSX\PhysX\bin\Debug\PhysXFoundation_64.lib

CALL :CheckFileExistence PHYSX\PhysX\bin\Release\PhysX_64.dll
CALL :CheckFileExistence PHYSX\PhysX\bin\Release\PhysX_64.lib
CALL :CheckFileExistence PHYSX\PhysX\bin\Release\PhysXCommon_64.dll
CALL :CheckFileExistence PHYSX\PhysX\bin\Release\PhysXCommon_64.lib
CALL :CheckFileExistence PHYSX\PhysX\bin\Release\PhysXCooking_64.dll
CALL :CheckFileExistence PHYSX\PhysX\bin\Release\PhysXCooking_64.lib
CALL :CheckFileExistence PHYSX\PhysX\bin\Release\PhysXFoundation_64.dll
CALL :CheckFileExistence PHYSX\PhysX\bin\Release\PhysXFoundation_64.lib

exit /B 0

rem ----------------------------------------------------------------

:CheckLibIncFolders
CALL :CheckFileExistence AL\include
CALL :CheckFileExistence PHYSX\PhysX\include
CALL :CheckFileExistence PHYSX\PxShared\include
CALL :CheckFileExistence GL\include
CALL :CheckFileExistence JSONCPP\include
CALL :CheckFileExistence libsndfile\include

exit /B 0

rem ------------------------------------------------------------

:CheckEngineBins
CALL :CheckFileExistence BulldozerFileManager.dll
CALL :CheckFileExistence BulldozerFileManager.lib

CALL :CheckFileExistence ExGame.exe

CALL :CheckFileExistence ExpansionGameEngineCore.dll
CALL :CheckFileExistence ExpansionGameEngineCore.lib

CALL :CheckFileExistence ExpansionMath.dll
CALL :CheckFileExistence ExpansionMath.lib

CALL :CheckFileExistence PhysicaSound.dll
CALL :CheckFileExistence PhysicaSound.lib

CALL :CheckFileExistence RaindropRenderer.dll
CALL :CheckFileExistence RaindropRenderer.lib
exit /B 0

rem ---------------------------------------------------

:CheckFileExistence
if exist %~1 (
    echo Found %~1

) else (
    echo Couln't find %~1, aborting

    goto EndScript
)
exit /B 0

:EndScript
pause
exit