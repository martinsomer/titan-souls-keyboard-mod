cl /LD ^
    /std:c17 ^
    /Fo:obj\ ^
    /I dllproxy /I minhook\include /I sdl2\include ^
    /TC src\version.c minhook\src\*.c minhook\src\hde\*.c ^
    /link ^
    /NOIMPLIB /NOEXP ^
    user32.lib ^
    /DEF:dllproxy\version.def ^
    /OUT:bin\version.dll
