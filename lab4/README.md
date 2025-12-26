Для запуска 1-й программы с 1-й реализацией:\
gcc program1.c implementation1.c -lm -o program1 && ./program1\
\
Для запуска 2-й программы:\
gcc -c -fPIC implementation1.c -o impl1.o\
gcc -shared impl1.o -o ../libraries/labimpl1.so -lm\
gcc -c -fPIC implementation2.c -o impl2.o\
gcc -shared impl2.o -o ../libraries/labimpl2.so -lm\
gcc program2.c -ldl -o program2\
./program2