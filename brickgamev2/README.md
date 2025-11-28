# BrickGame v2.0

Язык: C/C++

Кратко: расширенная версия аркадной игры с несколькими режимами (snake, tetris), GUI и тестами.

Зависимости: ncurses (CLI), дополнительные библиотеки для десктопной части (см. CMakeLists.txt)

Сборка:

```sh
cd src
make
```

Для десктопной сборки используйте CMake:

```sh
cd src/desktop
mkdir build && cd build
cmake .. && make
```

Тесты:

```sh
cd src
make test
```

Структура проекта:

```text
src/
├─ brick_game/        # snake, tetris, игры
├─ gui/               # cli и desktop
├─ tests/             # unit tests
└─ desktop/           # десктопная часть (CMake)
```
