# Как компилируется С++ код 

Препроцессинг
```sh
g++ -E main.cpp -o main-gcc.ii
clang++ -E main.cpp -o main-clang.ii

# -E - флаг, который позволяет увидеть результат препроцессинг
```

Компиляция
```sh
g++ -fverbose-asm -masm=intel -S main.cpp -o main-gcc.s
clang++ -fverbose-asm -masm=intel -S main.cpp -o main-clang.s

# -S - флаг, который позволяет увидеть результат компиляции (ассемблерный код)
# -fverbose-asm - говорит компилятору добавить комментарии к полученному ассемблеру, чтобы лучше в нем ориентироваться
# -masm=intel - указывает на нужный диалект языка ассемблера. В данном случае intel
```

Accемблирование
```sh
g++ -c main.cpp -o main-gcc.o
clang++ -c main.cpp -o main-clang.o

# -с - флаг, который позволяет увидеть результат ассемблирования (объектный файл)

objdump -d main-gcc.o > main-gcc.dump
objdump -d main-clang.o > main-clang.dump

# objdump - утилита, которая позволяет открыть и расшифровать объектный файл
# после флага -d идет путь к объектному файлу
```

Линковка
```sh
g++ main.cpp -o main-gcc
clang++ main.cpp -o main-clang

```
