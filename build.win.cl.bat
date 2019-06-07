rd /s /q "obj\"
rd /s /q "build\"

md .\lib
md .\obj\debug\core .\obj\debug\window
md .\build\debug

cl.exe /Zi /DEBUG:FULL /W1 -I.\include -I.\..\fllib\include /c .\src\main.c  /Fo.\obj\debug\main.o
cl.exe /Zi /DEBUG:FULL /W1 -I.\include -I.\..\fllib\include /c .\src\glad.c  /Fo.\obj\debug\glad.o
cl.exe /Zi /DEBUG:FULL /W1 -I.\include -I.\..\fllib\include /c .\src\window\glut.c  /Fo.\obj\debug\window\glut.o
cl.exe /Zi /DEBUG:FULL /W1 -I.\include -I.\..\fllib\include /c .\src\core\glut.c  /Fo.\obj\debug\core\glut.o

link.exe ^
    /OUT:build\\debug\\gkit.exe ^
    /DEFAULTLIB:libcmt ^
    /DEFAULTLIB:opengl32 ^
    /DEFAULTLIB:kernel32 ^
    /DEFAULTLIB:user32 ^
    /DEFAULTLIB:gdi32 ^
    /DEFAULTLIB:shell32 ^
    /NOLOGO ^
    /DEBUG:FULL ^
    lib\\fl.lib ^
    lib\\freeglut.lib ^
    obj\\debug\\main.o ^
    obj\\debug\\glad.o ^
    obj\\debug\\window\glut.o ^
    obj\\debug\\core\glut.o 