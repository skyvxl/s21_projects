# decimal

Язык: C

Кратко: реализация собственного типа `decimal` и основных операций (арифметика, сравнения, преобразования).

Зависимости: стандартные библиотеки C

Сборка:

```sh
cd src
make
```

Тесты:

```sh
cd src
make test
```

Структура проекта:

```text
src/
├─ arithmetical/     # add, sub, mul, div
├─ comparison/       # comparisons
├─ conversion/       # from/to int/float
├─ other/            # floor, round, truncate
└─ tests/            # unit tests
```
