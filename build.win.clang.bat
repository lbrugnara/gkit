::@echo off

set WINDOW_HANDLING=""
set WINDOW_HANDLING_OBJ=""
set WINDOW_HANDLING_LIB=""

if "%~1"=="glfw" (
    set WINDOW_HANDLING="glfw"
    set WINDOW_HANDLING_LIB=deps\lib\glfw3.lib
)

if "%~1"=="glut" (
    set WINDOW_HANDLING="glut"
    set WINDOW_HANDLING_LIB=deps\lib\freeglut_static.lib
)

if %WINDOW_HANDLING%=="" (
    echo You must specify the window handling library: [glfw^|glut]
    goto :EOF
)

rd /s /q "obj\"
rd /s /q "build\"

md .\lib
md .\obj\debug\%WINDOW_HANDLING%
md .\build\debug

set FLAGS=-I"../fllib/include" ^
            -I"./deps/include" ^
            -Wall ^
            -Wextra ^
            -Wno-unused-parameter ^
            -Wno-unused-variable ^
            -Wno-unused-function ^
            -std=c99 ^
            -pedantic ^
            -fstrict-aliasing ^
            -finput-charset=UTF-8 ^
            -fexec-charset=UTF-8 ^
            -ggdb

if "%~1"=="glut" (
    set FLAGS=%FLAGS% -DFREEGLUT_STATIC=1 -DFREEGLUT_LIB_PRAGMAS=0
)

clang.exe %FLAGS% -c .\deps\glad.c -o obj\debug\glad.o
clang.exe %FLAGS% -c .\src\core.c -o obj\debug\core.o
clang.exe %FLAGS% -c .\src\window.c -o obj\debug\window.o
clang.exe %FLAGS% -c .\src\shader.c -o obj\debug\shader.o
clang.exe %FLAGS% -c .\src\element.c -o obj\debug\element.o
clang.exe %FLAGS% -c .\src\main.c -o obj\debug\main.o

if %WINDOW_HANDLING%=="glfw" (
    clang.exe %FLAGS% -c .\src\glfw\core.c -o obj\debug\glfw\core.o
    clang.exe %FLAGS% -c .\src\glfw\window.c -o obj\debug\glfw\window.o
    
    set WINDOW_HANDLING_OBJ=obj\debug\glfw\core.o obj\debug\glfw\window.o
)

if %WINDOW_HANDLING%=="glut" (
    clang.exe %FLAGS% -c .\src\glut\core.c -o obj\debug\glut\core.o
    clang.exe %FLAGS% -c .\src\glut\window.c -o obj\debug\glut\window.o
    
    set WINDOW_HANDLING_OBJ=obj\debug\glut\core.o obj\debug\glut\window.o
)

lld-link.exe ^
    /OUT:build\debug\gkit.exe ^
    /DEFAULTLIB:libcmt ^
    /DEFAULTLIB:opengl32 ^
    /DEFAULTLIB:kernel32 ^
    /DEFAULTLIB:user32 ^
    /DEFAULTLIB:gdi32 ^
    /DEFAULTLIB:shell32 ^
    /LIBPATH:D:\dev\LLVM\x86\lib\clang\8.0.0\lib\windows ^
    /NOLOGO ^
    /DEBUG:FULL ^
    deps\lib\fl.lib ^
    %WINDOW_HANDLING_LIB% ^
    obj\debug\glad.o ^
    obj\debug\core.o ^
    obj\debug\window.o ^
    obj\debug\shader.o ^
    obj\debug\element.o ^
    obj\debug\main.o ^
    %WINDOW_HANDLING_OBJ%