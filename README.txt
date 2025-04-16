LICENSE
 Este trabajo está bajo la licencia CC BY 4.0 
 This work is licensed under CC BY 4.0

COMPILE 

For compilation, it depends on:
-Irrklang (audio)
-c++11

Compiled with MinGW64 g++:
(WINDRES) -i "resourceicon.rc" -o "resourceicon.o" --use-temp-file --output-format=coff --target=pe-i386 &
(G++) -m32 -std=c++11 main.cpp resourceicon.o -o "build/Plants vs Zombies Cmd.exe" -I (Irrklang include folder) -L  (IrrKlang Library folder Win32 GCC) -lirrKlang

Para la compilacion depende de :
-Irrklang (audio)
-c++11

Compilado con MinGW64 g++:
(WINDRES) -i "resourceicon.rc" -o "resourceicon.o" --use-temp-file --output-format=coff --target=pe-i386 &
(G++) -m32 -std=c++11 main.cpp resourceicon.o -o "build/Plants vs Zombies Cmd.exe" -I (Irrklang include folder) -L  (IrrKlang Library folder Win32 GCC) -lirrKlang

