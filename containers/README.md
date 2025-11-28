# containers

Язык: C++

Кратко: реализация стандартных контейнеров (vector, list, map, set, stack, queue, tree) в виде шаблонных классов.

Зависимости: стандартная библиотека C++

Сборка (Makefile/CMake):

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
├─ inc/                # заголовочные файлы (s21_list.h, s21_vector.h, ...)
├─ s21_containers/     # шаблонные реализации
└─ tests/              # unit tests
```
