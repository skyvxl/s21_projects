.PHONY: all build run test cover race fmt lint lint-fix vet clean libraries s21_graph s21_graph_algorithms s21_stack s21_queue

all: test build libraries

build:
	go build -o build/navigator ./src/cmd/navigator

run:
	go run ./src/cmd/navigator

test:
	go test ./src/s21_graph/... ./src/s21_graph_algorithms/... ./src/s21_stack/... ./src/s21_queue/... ./src/tsp_comparison/...

cover:
	go test -cover ./src/s21_graph/... ./src/s21_graph_algorithms/... ./src/s21_stack/... ./src/s21_queue/... ./src/tsp_comparison/...

libraries: s21_graph s21_graph_algorithms s21_stack s21_queue

s21_graph s21_graph_algorithms s21_stack s21_queue:
	go build -o build/$@.a ./src/$@

race:
	go test -race ./...

fmt:
	gofmt -w $$(find . -name '*.go')

lint:
	golangci-lint run

lint-fix:
	golangci-lint run --fix

vet:
	go vet ./...

clean:
	rm -rf build coverage.out coverage.html
