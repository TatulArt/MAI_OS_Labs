Для запуска 1-й программы с 1-й реализацией:\
gcc program1.c lib1_impl.c -lm -o program1 && ./program1\
\
Для запуска 2-й программы:\
gcc -c -fPIC lib1_impl.c -o lib1.o\
gcc -shared lib1.o -o ../libraries/lib1.so -lm\
gcc -c -fPIC lib2_impl.c -o lib2.o\
gcc -shared lib2.o -o ../libraries/lib2.so -lm\
gcc program2.c -ldl -o program2\
./program2