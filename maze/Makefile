APP_NAME := Maze
SRC_DIR := $(patsubst %/,%,$(dir $(abspath $(lastword $(MAKEFILE_LIST)))))
ROOT_DIR := $(SRC_DIR)
MAIN := ./cmd/maze
INSTALL_DIR ?= $(ROOT_DIR)/build
DIST_DIR := $(ROOT_DIR)/dist
DIST_ROOT := $(DIST_DIR)/$(APP_NAME)
ARCHIVE := $(ROOT_DIR)/$(APP_NAME).zip
DOC_FILE := $(SRC_DIR)/dvi.txt
BIN := $(INSTALL_DIR)/$(APP_NAME).exe
PS := powershell -NoProfile -ExecutionPolicy Bypass -Command

.PHONY: all install uninstall clean dvi dist tests

all: install

install:
	$(PS) "New-Item -ItemType Directory -Force -Path '$(INSTALL_DIR)' | Out-Null"
	$(PS) "Set-Location -LiteralPath '$(SRC_DIR)'; go build -o '$(BIN)' '$(MAIN)'"

uninstall:
	$(PS) "if (Test-Path -LiteralPath '$(BIN)') { Remove-Item -LiteralPath '$(BIN)' -Force }"

clean:
	$(PS) "if (Test-Path -LiteralPath '$(INSTALL_DIR)') { Remove-Item -LiteralPath '$(INSTALL_DIR)' -Recurse -Force }; if (Test-Path -LiteralPath '$(DIST_DIR)') { Remove-Item -LiteralPath '$(DIST_DIR)' -Recurse -Force }; if (Test-Path -LiteralPath '$(ARCHIVE)') { Remove-Item -LiteralPath '$(ARCHIVE)' -Recurse -Force }; if (Test-Path -LiteralPath '$(DOC_FILE)') { Remove-Item -LiteralPath '$(DOC_FILE)' -Recurse -Force }"

dvi:
	$(PS) "Set-Content -Path '$(DOC_FILE)' -Encoding UTF8 -Value @('Maze Go desktop application', 'Build: make install or task install.', 'Test: make tests or task tests.', 'The GUI can load, generate, save, solve mazes, and simulate caves.')"

dist: clean
	$(PS) "New-Item -ItemType Directory -Force -Path '$(DIST_ROOT)' | Out-Null; Copy-Item -LiteralPath '$(SRC_DIR)/README.md', '$(SRC_DIR)/go.mod', '$(SRC_DIR)/go.sum', '$(SRC_DIR)/Makefile', '$(SRC_DIR)/Taskfile.yml', '$(SRC_DIR)/golangci.yaml', '$(SRC_DIR)/cmd', '$(SRC_DIR)/internal', '$(SRC_DIR)/examples' -Destination '$(DIST_ROOT)' -Recurse -Force; Compress-Archive -Force -Path '$(DIST_ROOT)' -DestinationPath '$(ARCHIVE)'"

tests:
	$(PS) "Set-Location -LiteralPath '$(SRC_DIR)'; go test -cover ./..."
