cl /LD ^
   /TC src\main.c minhook\src\*.c minhook\src\hde\*.c ^
   /Fo:obj\ ^
   /std:c17 ^
   /I dllproxy /I minhook\include /I sdl2\include ^
   /W3 ^
   /link ^
   user32.lib ^
   /DEF:dllproxy\version.def ^
   /OUT:bin\version.dll ^
   /NOIMPLIB /NOEXP
