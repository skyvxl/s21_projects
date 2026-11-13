package s21_graph_algorithms

import (
	"errors"
	"fmt"
	"slices"
	"testing"

	"simple_navigator/src/s21_graph"
)

func mustGraph(t *testing.T, matrix [][]int) *s21_graph.Graph {
	t.Helper()
	graph, err := s21_graph.NewFromMatrix(matrix)
	if err != nil {
		t.Fatalf("NewFromMatrix() error: %v", err)
	}
	return graph
}

func TestDepthFirstSearch_Valid(t *testing.T) {
	tests := []struct {
		name   string
		matrix [][]int
		start  int
		want   []int
	}{
		{
			name: "branching undirected graph",
			// 1-2, 2-3, 2-4, 1-5
			matrix: [][]int{
				{0, 1, 0, 0, 1},
				{1, 0, 1, 1, 0},
				{0, 1, 0, 0, 0},
				{0, 1, 0, 0, 0},
				{1, 0, 0, 0, 0},
			},
			start: 1,
			want:  []int{1, 2, 3, 4, 5},
		},
		{
			name: "path graph from middle",
			matrix: [][]int{
				{0, 1, 0, 0},
				{1, 0, 1, 0},
				{0, 1, 0, 1},
				{0, 0, 1, 0},
			},
			start: 2,
			want:  []int{2, 1, 3, 4},
		},
		{
			name: "star graph",
			matrix: [][]int{
				{0, 1, 1, 1},
				{1, 0, 0, 0},
				{1, 0, 0, 0},
				{1, 0, 0, 0},
			},
			start: 1,
			want:  []int{1, 2, 3, 4},
		},
		{
			name: "self loop does not duplicate vertex",
			matrix: [][]int{
				{1, 1},
				{1, 0},
			},
			start: 1,
			want:  []int{1, 2},
		},
		{
			name: "directed graph",
			// 1 -> 2 -> 3
			matrix: [][]int{
				{0, 1, 0},
				{0, 0, 1},
				{0, 0, 0},
			},
			start: 1,
			want:  []int{1, 2, 3},
		},
		{
			name: "disconnected graph returns reachable component",
			matrix: [][]int{
				{0, 1, 0, 0},
				{1, 0, 0, 0},
				{0, 0, 0, 1},
				{0, 0, 1, 0},
			},
			start: 1,
			want:  []int{1, 2},
		},
		{
			name: "cycle pushes duplicate vertex",
			matrix: [][]int{
				{0, 1, 1},
				{1, 0, 1},
				{1, 1, 0},
			},
			start: 1,
			want:  []int{1, 2, 3},
		},
	}

	for _, tc := range tests {
		t.Run(tc.name, func(t *testing.T) {
			got, err := (&GraphAlgorithms{}).DepthFirstSearch(mustGraph(t, tc.matrix), tc.start)
			if err != nil {
				t.Fatalf("DepthFirstSearch() unexpected error: %v", err)
			}
			if !slices.Equal(got, tc.want) {
				t.Fatalf("DepthFirstSearch() = %v, want %v", got, tc.want)
			}
		})
	}
}

func TestBreadthFirstSearch_Valid(t *testing.T) {
	tests := []struct {
		name   string
		matrix [][]int
		start  int
		want   []int
	}{
		{
			name: "branching undirected graph",
			matrix: [][]int{
				{0, 1, 0, 0, 1},
				{1, 0, 1, 1, 0},
				{0, 1, 0, 0, 0},
				{0, 1, 0, 0, 0},
				{1, 0, 0, 0, 0},
			},
			start: 1,
			want:  []int{1, 2, 5, 3, 4}, // по слоям
		},
		{
			name: "path graph from middle",
			matrix: [][]int{
				{0, 1, 0, 0},
				{1, 0, 1, 0},
				{0, 1, 0, 1},
				{0, 0, 1, 0},
			},
			start: 2,
			want:  []int{2, 1, 3, 4},
		},
		{
			name: "star graph",
			matrix: [][]int{
				{0, 1, 1, 1},
				{1, 0, 0, 0},
				{1, 0, 0, 0},
				{1, 0, 0, 0},
			},
			start: 1,
			want:  []int{1, 2, 3, 4},
		},
		{
			name: "self loop does not duplicate vertex",
			matrix: [][]int{
				{1, 1},
				{1, 0},
			},
			start: 1,
			want:  []int{1, 2},
		},
		{
			name: "directed graph",
			matrix: [][]int{
				{0, 1, 0},
				{0, 0, 1},
				{0, 0, 0},
			},
			start: 1,
			want:  []int{1, 2, 3},
		},
		{
			name: "directed graph unreachable start",
			matrix: [][]int{
				{0, 1, 0},
				{0, 0, 1},
				{0, 0, 0},
			},
			start: 3,
			want:  []int{3},
		},
		{
			name: "disconnected graph returns reachable component",
			matrix: [][]int{
				{0, 1, 0, 0},
				{1, 0, 0, 0},
				{0, 0, 0, 1},
				{0, 0, 1, 0},
			},
			start: 1,
			want:  []int{1, 2},
		},
	}

	for _, tc := range tests {
		t.Run(tc.name, func(t *testing.T) {
			got, err := (&GraphAlgorithms{}).BreadthFirstSearch(mustGraph(t, tc.matrix), tc.start)
			if err != nil {
				t.Fatalf("BreadthFirstSearch() unexpected error: %v", err)
			}
			if !slices.Equal(got, tc.want) {
				t.Fatalf("BreadthFirstSearch() = %v, want %v", got, tc.want)
			}
		})
	}
}

func traversals() map[string]func(*s21_graph.Graph, int) ([]int, error) {
	algo := &GraphAlgorithms{}
	return map[string]func(*s21_graph.Graph, int) ([]int, error){
		"DFS": algo.DepthFirstSearch,
		"BFS": algo.BreadthFirstSearch,
	}
}

func TestTraversals_InvalidStart(t *testing.T) {
	graph := mustGraph(t, [][]int{{0, 1}, {1, 0}})

	for name, traverse := range traversals() {
		for _, start := range []int{-1, 0, 3} {
			t.Run(fmt.Sprintf("%s/start=%d", name, start), func(t *testing.T) {
				_, err := traverse(graph, start)
				if !errors.Is(err, s21_graph.ErrInvalidVertex) {
					t.Fatalf("error=%v, want errors.Is(_, %v)", err, s21_graph.ErrInvalidVertex)
				}
			})
		}
	}
}

func TestTraversals_EmptyGraph(t *testing.T) {
	for name, traverse := range traversals() {
		t.Run(name, func(t *testing.T) {
			_, err := traverse(s21_graph.NewGraph(), 1)
			if !errors.Is(err, ErrNilGraph) {
				t.Fatalf("error=%v, want errors.Is(_, %v)", err, ErrNilGraph)
			}
		})
	}
}

// DFS и BFS из одной вершины обязаны посетить один и тот же набор вершин —
// это одна и та же компонента связности.
func TestTraversals_VisitSameComponent(t *testing.T) {
	graph := mustGraph(t, [][]int{
		{0, 1, 0, 0, 1},
		{1, 0, 1, 1, 0},
		{0, 1, 0, 0, 0},
		{0, 1, 0, 0, 0},
		{1, 0, 0, 0, 0},
	})

	algo := &GraphAlgorithms{}
	dfs, err := algo.DepthFirstSearch(graph, 1)
	if err != nil {
		t.Fatalf("DepthFirstSearch() error: %v", err)
	}
	bfs, err := algo.BreadthFirstSearch(graph, 1)
	if err != nil {
		t.Fatalf("BreadthFirstSearch() error: %v", err)
	}

	slices.Sort(dfs)
	slices.Sort(bfs)
	if !slices.Equal(dfs, bfs) {
		t.Fatalf("visited sets differ: DFS=%v, BFS=%v", dfs, bfs)
	}
}
