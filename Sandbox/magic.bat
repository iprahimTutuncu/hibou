@echo off
setlocal

REM Define source folder
set "SOURCE_FOLDER=.\files"

REM Define destination folders
set "SRC_FOLDER=src"
set "INCLUDE_FOLDER=include"

REM Create the "src" and "include" folders
mkdir "%SRC_FOLDER%"
mkdir "%INCLUDE_FOLDER%"

REM Copy the source folder contents to "src" and "include"
xcopy "%SOURCE_FOLDER%\*" "%SRC_FOLDER%\" /E /I /Y
xcopy "%SOURCE_FOLDER%\*" "%INCLUDE_FOLDER%\" /E /I /Y

REM Delete all .h files in the "src" folder and subfolders
del /s /q "%SRC_FOLDER%\*.h"

REM Delete all .cpp files in the "include" folder and subfolders
del /s /q "%INCLUDE_FOLDER%\*.cpp"

echo Folders created and files deleted successfully.
endlocal
