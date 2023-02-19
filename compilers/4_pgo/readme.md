# Зачем компилировать код несколько раз

## gcc 
как посмотреть на профиль
```sh
# собираем инструментированную версию
g++ -fprofile-generate --coverage main.cpp -o main

# запуск программы и генерация профиля
./main

# переводим профиль в формат json
gcov -i main.gcda

# распаковываем полученный архив
zcat main.gcov.json.gz > main.json
```
как оптимизировать программу
```sh
# собираем инструментированную версию
g++ -fprofile-generate  -O3 main.cpp -o main

# запуск программы и генерация профиля
./main

# собираем программу с профилем
g++ -fprofile-use  -O3 main.cpp -o main
```

## clang
```sh
# Сборка инструментированной версии
clang++ -O2 -fprofile-generate main.cpp -o main

# Запуск инструментированной версии
./main

# Объединение профилей и конвертирование их в формат, ожидаемый компилятором
llvm-profdata merge -output=code.profdata code-*.profraw

# Сборка версии, оптимизированной с использованием профиля 
clang++ -O2 -fprofile-use=code.profdata main.cc -o main
```
