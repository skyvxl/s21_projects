package s21_graph_algorithms

import (
	"errors"
	"testing"

	"simple_navigator/src/s21_graph"
)

func TestGetShortestPathBetweenVertices_Valid(t *testing.T) {
	tests := []struct {
		name   string
		matrix [][]int
		from   int
		to     int
		want   int
	}{
		{
			name: "simple path",
			matrix: [][]int{
				{0, 1, 0, 0},
				{1, 0, 2, 0},
				{0, 2, 0, 3},
				{0, 0, 3, 0},
			},
			from: 1, to: 4, want: 6,
		},
		{
			name: "backwards path is symmetric (undirected)",
			matrix: [][]int{
				{0, 1, 0, 0},
				{1, 0, 2, 0},
				{0, 2, 0, 3},
				{0, 0, 3, 0},
			},
			from: 4, to: 1, want: 6,
		},
		{
			name: "greedy trap prefers cheaper indirect path",
			matrix: [][]int{
				{0, 1, 10},
				{1, 0, 2},
				{10, 2, 0},
			},
			from: 1, to: 3, want: 3,
		},
		{
			name: "direct edge",
			matrix: [][]int{
				{0, 7},
				{7, 0},
			},
			from: 1, to: 2, want: 7,
		},
		{
			name: "same vertex is zero",
			matrix: [][]int{
				{0, 7},
				{7, 0},
			},
			from: 2, to: 2, want: 0,
		},
		{
			name: "directed forward",
			matrix: [][]int{
				{0, 5, 0},
				{0, 0, 2},
				{1, 0, 0},
			},
			from: 1, to: 3, want: 7,
		},
		{
			name: "directed reverse differs",
			matrix: [][]int{
				{0, 5, 0},
				{0, 0, 2},
				{1, 0, 0},
			},
			from: 2, to: 1, want: 3,
		},
		{
			name: "self loop does not shorten anything",
			matrix: [][]int{
				{5, 1},
				{1, 0},
			},
			from: 1, to: 2, want: 1,
		},
	}

	for _, tc := range tests {
		t.Run(tc.name, func(t *testing.T) {
			got, err := (&GraphAlgorithms{}).
				GetShortestPathBetweenVertices(mustGraph(t, tc.matrix), tc.from, tc.to)
			if err != nil {
				t.Fatalf("unexpected error: %v", err)
			}
			if got != tc.want {
				t.Fatalf("got %d, want %d", got, tc.want)
			}
		})
	}
}

func TestGetShortestPathBetweenVertices_NoPath(t *testing.T) {
	tests := []struct {
		name   string
		matrix [][]int
		from   int
		to     int
	}{
		{
			name: "undirected disconnected",
			matrix: [][]int{
				{0, 1, 0, 0},
				{1, 0, 0, 0},
				{0, 0, 0, 1},
				{0, 0, 1, 0},
			},
			from: 1, to: 3,
		},
		{
			name: "directed unreachable",
			matrix: [][]int{
				{0, 1, 0},
				{0, 0, 0},
				{0, 0, 0},
			},
			from: 1, to: 3,
		},
	}

	for _, tc := range tests {
		t.Run(tc.name, func(t *testing.T) {
			_, err := (&GraphAlgorithms{}).
				GetShortestPathBetweenVertices(mustGraph(t, tc.matrix), tc.from, tc.to)
			if !errors.Is(err, ErrNoPath) {
				t.Fatalf("error=%v, want errors.Is(_, %v)", err, ErrNoPath)
			}
		})
	}
}

func TestGetShortestPathBetweenVertices_Invalid(t *testing.T) {
	graph := mustGraph(t, [][]int{{0, 1}, {1, 0}})
	algo := &GraphAlgorithms{}

	t.Run("nil graph", func(t *testing.T) {
		_, err := algo.GetShortestPathBetweenVertices(nil, 1, 2)
		if !errors.Is(err, ErrNilGraph) {
			t.Fatalf("error=%v, want errors.Is(_, %v)", err, ErrNilGraph)
		}
	})

	t.Run("empty graph", func(t *testing.T) {
		_, err := algo.GetShortestPathBetweenVertices(s21_graph.NewGraph(), 1, 2)
		if !errors.Is(err, ErrNilGraph) {
			t.Fatalf("error=%v, want errors.Is(_, %v)", err, ErrNilGraph)
		}
	})

	for _, tc := range []struct {
		name     string
		from, to int
	}{
		{"from below range", 0, 1},
		{"from above range", 3, 1},
		{"to below range", 1, 0},
		{"to above range", 1, 3},
		{"both invalid", -1, 99},
	} {
		t.Run(tc.name, func(t *testing.T) {
			_, err := algo.GetShortestPathBetweenVertices(graph, tc.from, tc.to)
			if !errors.Is(err, s21_graph.ErrInvalidVertex) {
				t.Fatalf("error=%v, want errors.Is(_, %v)", err, s21_graph.ErrInvalidVertex)
			}
		})
	}
}

func TestGetShortestPathBetweenVertices_Symmetry(t *testing.T) {
	graph := mustGraph(t, [][]int{
		{0, 2, 0, 8},
		{2, 0, 3, 0},
		{0, 3, 0, 1},
		{8, 0, 1, 0},
	})
	algo := &GraphAlgorithms{}
	n := graph.VertexCount()

	for i := 1; i <= n; i++ {
		for j := 1; j <= n; j++ {
			got, err := algo.GetShortestPathBetweenVertices(graph, i, j)
			if err != nil {
				t.Fatalf("path %d->%d: %v", i, j, err)
			}
			rev, err := algo.GetShortestPathBetweenVertices(graph, j, i)
			if err != nil {
				t.Fatalf("path %d->%d: %v", j, i, err)
			}
			if got != rev {
				t.Fatalf("asymmetric for (%d,%d): %d != %d", i, j, got, rev)
			}
		}
	}
}
