package s21_graph_algorithms

import (
	"errors"
	"math"
	"reflect"
	"testing"

	"simple_navigator/src/s21_graph"
)

func TestGetLeastSpanningTree_MinimumTree(t *testing.T) {
	graph := mustGraph(t, [][]int{
		{0, 1, 2, 9},
		{1, 0, 8, 3},
		{2, 8, 0, 10},
		{9, 3, 10, 0},
	})

	got, err := (&GraphAlgorithms{}).GetLeastSpanningTree(graph)
	if err != nil {
		t.Fatalf("GetLeastSpanningTree() error = %v", err)
	}
	want := [][]int{
		{0, 1, 2, 0},
		{1, 0, 0, 3},
		{2, 0, 0, 0},
		{0, 3, 0, 0},
	}
	if !reflect.DeepEqual(got, want) {
		t.Fatalf("GetLeastSpanningTree() = %v, want %v", got, want)
	}
}

func TestGetLeastSpanningTree_SingleVertex(t *testing.T) {
	got, err := (&GraphAlgorithms{}).GetLeastSpanningTree(mustGraph(t, [][]int{{7}}))
	if err != nil {
		t.Fatalf("GetLeastSpanningTree() error = %v", err)
	}
	if want := [][]int{{0}}; !reflect.DeepEqual(got, want) {
		t.Fatalf("GetLeastSpanningTree() = %v, want %v", got, want)
	}
}

func TestGetLeastSpanningTree_LargeEdgeWeight(t *testing.T) {
	graph := mustGraph(t, [][]int{
		{0, math.MaxInt},
		{math.MaxInt, 0},
	})

	got, err := (&GraphAlgorithms{}).GetLeastSpanningTree(graph)
	if err != nil {
		t.Fatalf("GetLeastSpanningTree() error = %v", err)
	}
	want := [][]int{{0, math.MaxInt}, {math.MaxInt, 0}}
	if !reflect.DeepEqual(got, want) {
		t.Fatalf("GetLeastSpanningTree() = %v, want %v", got, want)
	}
}

func TestGetLeastSpanningTree_Errors(t *testing.T) {
	tests := []struct {
		name  string
		graph *s21_graph.Graph
		want  error
	}{
		{name: "nil graph", want: ErrNilGraph},
		{name: "empty graph", graph: s21_graph.NewGraph(), want: ErrNilGraph},
		{name: "directed graph", graph: mustGraph(t, [][]int{{0, 1}, {0, 0}}), want: ErrDirectedGraph},
		{name: "disconnected graph", graph: mustGraph(t, [][]int{
			{0, 1, 0, 0},
			{1, 0, 0, 0},
			{0, 0, 0, 2},
			{0, 0, 2, 0},
		}), want: ErrNoSpanningTree},
	}

	for _, tc := range tests {
		t.Run(tc.name, func(t *testing.T) {
			got, err := (&GraphAlgorithms{}).GetLeastSpanningTree(tc.graph)
			if !errors.Is(err, tc.want) {
				t.Fatalf("GetLeastSpanningTree() error = %v, want %v", err, tc.want)
			}
			if got != nil {
				t.Fatalf("GetLeastSpanningTree() = %v, want nil on error", got)
			}
		})
	}
}
