## Дополнение к Makefile для работы с Docker
# Подключайте через include src/docker/docker_makefile.mk

DOCKER_IMAGE = s21_containers_linux
# Корень проекта — на директорию выше, если запускать make из src
PROJECT_ROOT := $(abspath ..)
# Папка с Dockerfile всегда src/docker относительно корня
DOCKERFILE_DIR := $(PROJECT_ROOT)/src/docker

# Собрать docker-образ
.PHONY: docker-build
docker-build:
	docker build -t $(DOCKER_IMAGE) $(DOCKERFILE_DIR)

# Запустить тесты в контейнере (монтируется весь проект)
.PHONY: docker-test
docker-test: docker-build
	docker run --rm -it \
	  -v $(PROJECT_ROOT):/workspace \
	  -w /workspace/src \
	  $(DOCKER_IMAGE) make test

# Запустить valgrind в контейнере
.PHONY: docker-valgrind
docker-valgrind: docker-build
	docker run --rm -it \
	  -v $(PROJECT_ROOT):/workspace \
	  -w /workspace/src \
	  $(DOCKER_IMAGE) make valgrind

# Запустить bash в контейнере (для ручной работы)
.PHONY: docker-shell
docker-shell: docker-build
	docker run --rm -it \
	  -v $(PROJECT_ROOT):/workspace \
	  -w /workspace/src \
	  $(DOCKER_IMAGE) bash
