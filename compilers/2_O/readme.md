# Этапы компиляции

Начнем с базовых флагов оптимизации, c параметров -O
```
-O0 - без оптимизаций
-O1 - компиляция с небольшими оптимизациями
-O2 - компиляция, не требующие больших оптимизация по времени
-O3 - самое сильные оптимизации
```

## gcc

Парсинг
```sh
g++ -fdump-tree-original -c main.cpp -o main

# -fdump-tree-original - посмотреть дерево парсинга
```

Гимплифткация 
```sh
g++ -fdump-tree-gimple -c main.cpp -o main

# -fdump-tree-gimple - посмотреть дерево с кодом gimple
```

Генерация языка регистров
```sh
g++ -da -c main.cpp -o main.o

#-da - сгенерировать файлы на языке регистров
```

Информация по оптимизациям
```sh
g++ -O2 -fopt-info main.cpp

#-fopt-info - краткий отчет об оптимизациях

g++ -fsave-optimization-record -O3  main.cpp -o main
zcat main.cpp.opt-record.json.gz > main.opt

#-fsave-optimization-record - подробный отчет об оптимизациях
```

## clang

Парсинг
```sh
clang++ -cc1 -ast-dump -c main.cpp -o main

# -cc1 -ast-dump - посмотреть дерево парсинга
```

llvm IR - промежуточное представление 
```sh
clang++ -S -emit-llvm  main.cpp -o main.ll
clang++ -c -emit-llvm  main.cpp -o main.ll

# -emit-llvm - сгенерировать llvm IR
```

Перевод llvm IR в ассемблер
```sh
llc test.bc –o test.s

# test.bc - файл с llvm IR
```

Информация об оптимизациях
```sh
clang++ -fsave-optimization-record=yaml -O3 main.cpp -o main
# -fsave-optimization-record=yaml - подробный отчет об оптимизациях в формате yaml
```