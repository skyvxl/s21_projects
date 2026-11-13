package s21_graph_algorithms

import (
	"reflect"
	"testing"
)

func TestGetShortestPathsBetweenAllVertices_Valid(t *testing.T) {
	tests := []struct {
		name   string
		matrix [][]int
		want   [][]int
	}{
		{
			name: "undirected path",
			matrix: [][]int{
				{0, 1, 0, 0},
				{1, 0, 2, 0},
				{0, 2, 0, 3},
				{0, 0, 3, 0},
			},
			want: [][]int{
				{0, 1, 3, 6},
				{1, 0, 2, 5},
				{3, 2, 0, 3},
				{6, 5, 3, 0},
			},
		},
		{
			name: "weighted undirected with shortcuts",
			matrix: [][]int{
				{0, 2, 0, 8},
				{2, 0, 3, 0},
				{0, 3, 0, 1},
				{8, 0, 1, 0},
			},
			want: [][]int{
				{0, 2, 5, 6},
				{2, 0, 3, 4},
				{5, 3, 0, 1},
				{6, 4, 1, 0},
			},
		},
		{
			name: "greedy trap uses indirect route",
			matrix: [][]int{
				{0, 1, 10},
				{1, 0, 2},
				{10, 2, 0},
			},
			want: [][]int{
				{0, 1, 3},
				{1, 0, 2},
				{3, 2, 0},
			},
		},
		{
			name: "directed graph is asymmetric",
			matrix: [][]int{
				{0, 5, 0},
				{0, 0, 2},
				{1, 0, 0},
			},
			want: [][]int{
				{0, 5, 7},
				{3, 0, 2},
				{1, 6, 0},
			},
		},
		{
			name: "disconnected graph uses -1",
			matrix: [][]int{
				{0, 1, 0, 0},
				{1, 0, 0, 0},
				{0, 0, 0, 1},
				{0, 0, 1, 0},
			},
			want: [][]int{
				{0, 1, -1, -1},
				{1, 0, -1, -1},
				{-1, -1, 0, 1},
				{-1, -1, 1, 0},
			},
		},
		{
			name: "self loops are ignored, diagonal stays zero",
			matrix: [][]int{
				{5, 1},
				{1, 3},
			},
			want: [][]int{
				{0, 1},
				{1, 0},
			},
		},
		{
			name:   "single vertex",
			matrix: [][]int{{0}},
			want:   [][]int{{0}},
		},
	}

	for _, tc := range tests {
		t.Run(tc.name, func(t *testing.T) {
			got, err := (&GraphAlgorithms{}).GetShortestPathsBetweenAllVertices(mustGraph(t, tc.matrix))
			if err != nil {
				t.Fatalf("Floyd error: %v", err)
			}
			if !reflect.DeepEqual(got, tc.want) {
				t.Fatalf("got  %v\nwant %v", got, tc.want)
			}
		})
	}
}

func TestGetShortestPathsBetweenAllVertices_SymmetricUndirected(t *testing.T) {
	graph := mustGraph(t, [][]int{
		{0, 2, 0, 8},
		{2, 0, 3, 0},
		{0, 3, 0, 1},
		{8, 0, 1, 0},
	})
	dist, err := (&GraphAlgorithms{}).GetShortestPathsBetweenAllVertices(graph)
	if err != nil {
		t.Fatalf("Floyd error: %v", err)
	}
	n := graph.VertexCount()

	for i := range n {
		if dist[i][i] != 0 {
			t.Fatalf("dist[%d][%d]=%d, want 0", i, i, dist[i][i])
		}
		for j := range n {
			if dist[i][j] != dist[j][i] {
				t.Fatalf("asymmetric: dist[%d][%d]=%d, dist[%d][%d]=%d",
					i, j, dist[i][j], j, i, dist[j][i])
			}
		}
	}
}

func TestGetShortestPathsBetweenAllVertices_MatchesDijkstra(t *testing.T) {
	graph := mustGraph(t, [][]int{
		{0, 2, 0, 8, 0},
		{2, 0, 3, 0, 0},
		{0, 3, 0, 1, 6},
		{8, 0, 1, 0, 4},
		{0, 0, 6, 4, 0},
	})
	algo := &GraphAlgorithms{}
	all, err := algo.GetShortestPathsBetweenAllVertices(graph)
	if err != nil {
		t.Fatalf("Floyd error: %v", err)
	}
	n := graph.VertexCount()

	for i := 1; i <= n; i++ {
		for j := 1; j <= n; j++ {
			want, err := algo.GetShortestPathBetweenVertices(graph, i, j)
			if err != nil {
				t.Fatalf("dijkstra %d->%d: %v", i, j, err)
			}
			if got := all[i-1][j-1]; got != want {
				t.Fatalf("floyd[%d][%d]=%d, dijkstra=%d", i, j, got, want)
			}
		}
	}
}
