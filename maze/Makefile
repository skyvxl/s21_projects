APP_NAME := Maze
SRC_DIR := $(patsubst %/,%,$(dir $(abspath $(lastword $(MAKEFILE_LIST)))))
ROOT_DIR := $(abspath $(SRC_DIR)/..)
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
	$(PS) "New-Item -ItemType Directory -Force -Path '$(DIST_ROOT)/src' | Out-Null; Copy-Item -LiteralPath '$(ROOT_DIR)/README_RUS.md', '$(ROOT_DIR)/go.mod', '$(ROOT_DIR)/go.sum' -Destination '$(DIST_ROOT)' -Force; Get-ChildItem -LiteralPath '$(SRC_DIR)' -Force | Copy-Item -Destination '$(DIST_ROOT)/src' -Recurse -Force; Compress-Archive -Force -Path '$(DIST_ROOT)' -DestinationPath '$(ARCHIVE)'"

tests:
	$(PS) "Set-Location -LiteralPath '$(SRC_DIR)'; go test -cover ./..."
