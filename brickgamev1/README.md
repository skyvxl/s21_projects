# BrickGame v1.0

Язык: C

Кратко: реализация игры "Тетрис" с базовой логикой игрового процесса и тестами.

Зависимости: ncurses (для CLI-части)

Установка зависимостей (Debian/Ubuntu):

```sh
sudo apt-get install libncurses5-dev libncursesw5-dev
```

Сборка:

```sh
cd src
make
```

Запуск игры:

```sh
./brick_game/tetris
```

Документация:

```sh
cd src
make dvi
```

Тесты:

```sh
cd src
make test
```

Структура проекта:

```text
src/
├─ brick_game/tetris/    # логика игры
├─ gui/cli/              # фронтенд на ncurses
├─ run/                  # точки входа (main)
└─ test/                 # тесты
```
