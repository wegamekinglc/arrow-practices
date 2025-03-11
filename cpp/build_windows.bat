@echo off

call :set_variable BUILD_TYPE Release %BUILD_TYPE%
call :set_variable WORK_DIR "%CD%" %WORK_DIR%
call :set_variable ADDRESS_MODEL Win64 %ADDRESS_MODEL%
call :set_variable MSVC_RUNTIME static %MSVC_RUNTIME%
call :set_variable MSVC_VERSION "Visual Studio 17 2022" %MSVC_VERSION%

echo BUILD_TYPE:  %BUILD_TYPE%
echo WORK_DIR: %WORK_DIR%
echo ADDRESS_MODEL: %ADDRESS_MODEL%
echo MSVC_RUNTIME: %MSVC_RUNTIME%
echo MSVC_VERSION: %MSVC_VERSION%

git submodule init
git submodule update

cd vcpkg
echo Starting vcpkg install gtest
if exist "./vcpkg.exe" (
    rem "vcpkg executable already exists"
) else (
    .\bootstrap-vcpkg.bat
)

.\vcpkg install arrow:x64-windows

if %errorlevel% neq 0 exit /b 1
cd ..

if "%ADDRESS_MODEL%"=="Win64" (
  set PLATFORM=x64
) else (
  if "%MSVC_VERSION%"=="Visual Studio 16 2019" (
    set PLATFORM=x64
  ) else (
    if "%MSVC_VERSION%"=="Visual Studio 17 2022" (
        set PLATFORM=x64
    ) else (
        set PLATFORM=Win32
    )
  )
)

if "%ADDRESS_MODEL%"=="Win64" (
  if "%MSVC_VERSION%"=="Visual Studio 16 2019" (
    set ADDRESS_MODEL=
  ) else (
    if "%MSVC_VERSION%"=="Visual Studio 17 2022" (
        set ADDRESS_MODEL=
    )
  )
)

if exist build (
  rem build folder already exists.
) else (
  mkdir build
)

cd build
if "%ADDRESS_MODEL%"=="Win64" (
cmake -G "%MSVC_VERSION% %ADDRESS_MODEL%" --preset %BUILD_TYPE% -DMSVC_RUNTIME=%MSVC_RUNTIME% -DVCPKG_TARGET_TRIPLET=x64-windows ..
) else (
cmake -G "%MSVC_VERSION%" --preset %BUILD_TYPE% -DMSVC_RUNTIME=%MSVC_RUNTIME% -DVCPKG_TARGET_TRIPLET=x64-windows ..
)

if %errorlevel% neq 0 exit /b 1

msbuild Arrow-Practices.sln /m /p:Configuration=%BUILD_TYPE% /p:Platform=%PLATFORM%
msbuild INSTALL.vcxproj /m:%NUMBER_OF_PROCESSORS% /p:Configuration=%BUILD_TYPE% /p:Platform=%PLATFORM%

if %errorlevel% neq 0 exit /b 1

cd ..

@echo on

EXIT /B 0

:set_variable
set %~1=%~2