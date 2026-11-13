package s21_graph_algorithms

import (
	"errors"
	"math"
	"reflect"
	"testing"

	"simple_navigator/src/s21_graph"
)

func TestShortestPaths_LargeWeights(t *testing.T) {
	for _, weight := range []int{math.MaxInt / 4, math.MaxInt} {
		graph := mustGraph(t, [][]int{{0, weight}, {weight, 0}})
		algo := &GraphAlgorithms{}
		got, err := algo.GetShortestPathBetweenVertices(graph, 1, 2)
		if err != nil || got != weight {
			t.Errorf("Dijkstra with weight %d = %d, %v", weight, got, err)
		}
		want := [][]int{{0, weight}, {weight, 0}}
		if got, err := algo.GetShortestPathsBetweenAllVertices(graph); err != nil || !reflect.DeepEqual(got, want) {
			t.Errorf("Floyd with weight %d = %v, %v, want %v", weight, got, err, want)
		}
	}
}

func TestShortestPaths_OverflowIsDifferentFromNoPath(t *testing.T) {
	graph := mustGraph(t, [][]int{
		{0, math.MaxInt, 0, 0},
		{0, 0, 1, 0},
		{0, 0, 0, 0},
		{0, 0, 0, 0},
	})
	algo := &GraphAlgorithms{}
	for _, tc := range []struct {
		to   int
		want error
	}{{3, ErrDistanceOverflow}, {4, ErrNoPath}} {
		if _, err := algo.GetShortestPathBetweenVertices(graph, 1, tc.to); !errors.Is(err, tc.want) {
			t.Fatalf("Dijkstra 1 -> %d error = %v, want %v", tc.to, err, tc.want)
		}
	}
	if matrix, err := algo.GetShortestPathsBetweenAllVertices(graph); !errors.Is(err, ErrDistanceOverflow) || matrix != nil {
		t.Fatalf("Floyd = %v, %v, want overflow error", matrix, err)
	}
}

func TestShortestPaths_EmptyAndDisconnectedGraph(t *testing.T) {
	algo := &GraphAlgorithms{}
	for _, graph := range []*s21_graph.Graph{nil, s21_graph.NewGraph()} {
		if matrix, err := algo.GetShortestPathsBetweenAllVertices(graph); !errors.Is(err, ErrNilGraph) || matrix != nil {
			t.Fatalf("empty Floyd = %v, %v", matrix, err)
		}
	}
	graph := mustGraph(t, [][]int{{0, 2, 0}, {0, 0, 0}, {0, 0, 0}})
	want := [][]int{{0, 2, -1}, {-1, 0, -1}, {-1, -1, 0}}
	if got, err := algo.GetShortestPathsBetweenAllVertices(graph); err != nil || !reflect.DeepEqual(got, want) {
		t.Fatalf("disconnected Floyd = %v, %v, want %v", got, err, want)
	}
}

func TestShortestPaths_IgnoreOverflowingDetour(t *testing.T) {
	graph := mustGraph(t, [][]int{{0, 1, 4}, {1, 0, math.MaxInt}, {4, math.MaxInt, 0}})
	algo := &GraphAlgorithms{}
	if got, err := algo.GetShortestPathBetweenVertices(graph, 1, 3); err != nil || got != 4 {
		t.Fatalf("Dijkstra = %d, %v, want 4", got, err)
	}
	want := [][]int{{0, 1, 4}, {1, 0, 5}, {4, 5, 0}}
	if got, err := algo.GetShortestPathsBetweenAllVertices(graph); err != nil || !reflect.DeepEqual(got, want) {
		t.Fatalf("Floyd = %v, %v, want %v", got, err, want)
	}
}

func TestShortestPaths_ExactMaxIntDistance(t *testing.T) {
	graph := mustGraph(t, [][]int{{0, math.MaxInt - 1, 0}, {0, 0, 1}, {0, 0, 0}})
	algo := &GraphAlgorithms{}
	if got, err := algo.GetShortestPathBetweenVertices(graph, 1, 3); err != nil || got != math.MaxInt {
		t.Fatalf("Dijkstra = %d, %v, want MaxInt", got, err)
	}
	want := [][]int{{0, math.MaxInt - 1, math.MaxInt}, {-1, 0, 1}, {-1, -1, 0}}
	if got, err := algo.GetShortestPathsBetweenAllVertices(graph); err != nil || !reflect.DeepEqual(got, want) {
		t.Fatalf("Floyd = %v, %v, want %v", got, err, want)
	}
}

func TestShortestPaths_AllPairsOverflowDoesNotReturnCorruptMatrix(t *testing.T) {
	graph := mustGraph(t, [][]int{{0, math.MaxInt, 0}, {0, 0, 1}, {0, 0, 0}})
	if got, err := (&GraphAlgorithms{}).GetShortestPathsBetweenAllVertices(graph); !errors.Is(err, ErrDistanceOverflow) || got != nil {
		t.Fatalf("overflowing Floyd result = %v, %v, want nil and overflow error", got, err)
	}
}
