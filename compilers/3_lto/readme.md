# то такое lto и зачем оно нужно

## gcc
1. lto на примере .cpp шников
```sh
# обычная компиляция с -O3
g++ -O3 main.cpp foo.cpp -o main
# смотрим есть ли inline функция из foo.cpp
objdump -d main > main.bin

# обычная компиляция с -O3 + -flot
g++ -O3 -flto main.cpp foo.cpp -o main
# смотрим есть ли inline функция из foo.cpp
objdump -d main > main.bin
```
2. lto на примере .a
```sh
# можно заметить что для static lib lto работает
g++ -c -fverbose-asm -O3 -flto foo.cpp -o foo.o
ar rcs libfoo.a foo.o
```
3. lto на примере .so
```sh
# а для динамических уже нет
g++ -fPIC -flto -shared foo.cpp -o libfoo.so
g++ main.cpp -o main -L. -lfoo -Wl,-rpath,. 
```

## clang 
аналогично как с gcc
```sh
# обычная компиляция с -O3
clang++ -O3 main.cpp foo.cpp -o main
# смотрим есть ли inline функция из foo.cpp
objdump -d main > main.bin

# обычная компиляция с -O3 + -flot
clang++ -O3 -flto main.cpp foo.cpp -o main
# смотрим есть ли inline функция из foo.cpp
objdump -d main > main.bin
```

литература
https://gcc.gnu.org/wiki/LinkTimeOptimization
https://stackoverflow.com/questions/24733986/gcc-lto-shared-library-am-i-right 
https://gcc.gnu.org/onlinedocs/gccint/LTO.html
