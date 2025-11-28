# 3DViewer

Язык: C++

Кратко: демонстрация простого 3D-Viewer с рендерингом и архитектурой проекта.

Зависимости: библиотеки для рендеринга (см. CMakeLists.txt)

Сборка (CMake):

```sh
cd src
mkdir build && cd build
cmake .. && make
```

Запуск:

```sh
cd src/build
./3DViewer
```

Тесты:

```sh
cd src
make test
```

Структура проекта:

```text
src/
├─ controllers/       # контроллеры приложения
├─ models/            # модель данных
├─ views/             # представления / UI
├─ resources/         # шейдеры, plist, и т.д.
└─ tests/             # unit tests
```
