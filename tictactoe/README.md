# TicTacToe

Язык: Go

Кратко: веб-бэкенд для игры в крестики-нолики (3×3). Поддержка мультиплеера, ИИ (minimax), JWT-авторизации, лидерборда и истории игр. PostgreSQL + sqlc.

Зависимости: Go 1.25+, PostgreSQL, sqlc, Task

Сборка и запуск:

```sh
task build
task run
```

Тесты:

```sh
task test
```

Генерация sqlc:

```sh
task generate
```

Структура проекта:

```text
src/
├─ cmd/app/           # main
├─ internal/
│  ├─ domain/         # Game, Board, minimax, сервисы
│  ├─ web/            # HTTP handlers (auth, game)
│  ├─ datasource/     # repositories
│  ├─ db/             # сгенерированный sqlc код
│  ├─ di/             # uber/fx DI
│  └─ jwt/            # JWT провайдер
db/
├─ schema.sql
└─ queries.sql
src/static/           # простой Vue клиент
```
