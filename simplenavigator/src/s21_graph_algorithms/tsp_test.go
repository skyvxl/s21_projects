package s21_graph_algorithms

import (
	"errors"
	"math"
	"math/rand/v2"
	"reflect"
	"slices"
	"testing"

	"simple_navigator/src/s21_graph"
)

func TestSolveTravelingSalesmanProblem_DistanceOverflow(t *testing.T) {
	defer func() {
		if value := recover(); value != nil {
			t.Fatalf("TSP panicked on distance overflow: %v", value)
		}
	}()
	graph := mustGraph(t, [][]int{{0, math.MaxInt, 0}, {0, 0, 1}, {1, 0, 0}})
	if _, err := (&GraphAlgorithms{}).SolveTravelingSalesmanProblem(graph); !errors.Is(err, ErrDistanceOverflow) {
		t.Fatalf("TSP error = %v, want distance overflow", err)
	}
}

func TestShortestSteps(t *testing.T) {
	graph := mustGraph(t, [][]int{
		{0, 1, 0},
		{1, 0, 2},
		{0, 2, 0},
	})
	distances, err := (&GraphAlgorithms{}).GetShortestPathsBetweenAllVertices(graph)
	if err != nil {
		t.Fatalf("Floyd error: %v", err)
	}

	tests := []struct {
		name string
		from int
		to   int
		want shortestStepsResult
	}{
		{name: "indirect path", from: 0, to: 2, want: shortestStepsResult{steps: []int{2, 3}, distance: 3, found: true}},
		{name: "reverse path", from: 2, to: 0, want: shortestStepsResult{steps: []int{2, 1}, distance: 3, found: true}},
		{name: "same vertex", from: 1, to: 1, want: shortestStepsResult{distance: 0, found: true}},
	}
	for _, tc := range tests {
		t.Run(tc.name, func(t *testing.T) {
			got := shortestSteps(graph, distances, tc.from, tc.to)
			if got.found != tc.want.found || got.distance != tc.want.distance || !slices.Equal(got.steps, tc.want.steps) {
				t.Fatalf("shortestSteps(%d, %d) = %+v, want %+v", tc.from, tc.to, got, tc.want)
			}
		})
	}

	directed := mustGraph(t, [][]int{
		{0, 1, 0},
		{0, 0, 2},
		{0, 0, 0},
	})
	directedDistances, err := (&GraphAlgorithms{}).GetShortestPathsBetweenAllVertices(directed)
	if err != nil {
		t.Fatalf("directed Floyd error: %v", err)
	}
	got := shortestSteps(directed, directedDistances, 2, 0)
	if got.found || got.distance != 0 || len(got.steps) != 0 {
		t.Fatalf("shortestSteps() for unreachable path = %+v, want no path", got)
	}
}

func TestSolveTravelingSalesmanProblem_ValidTours(t *testing.T) {
	tests := []struct {
		name   string
		matrix [][]int
	}{
		{
			name: "connected path requires repeated vertex",
			matrix: [][]int{
				{0, 1, 0},
				{1, 0, 2},
				{0, 2, 0},
			},
		},
		{
			name: "directed cycle",
			matrix: [][]int{
				{0, 1, 0},
				{0, 0, 2},
				{3, 0, 0},
			},
		},
		{
			name: "weighted complete graph",
			matrix: [][]int{
				{0, 2, 9, 5},
				{2, 0, 1, 8},
				{9, 1, 0, 3},
				{5, 8, 3, 0},
			},
		},
	}

	for _, tc := range tests {
		t.Run(tc.name, func(t *testing.T) {
			graph := mustGraph(t, tc.matrix)
			rng := rand.New(rand.NewPCG(1, 2))
			result, err := (&GraphAlgorithms{}).solveTravelingSalesmanProblem(graph, rng)
			if err != nil {
				t.Fatalf("SolveTravelingSalesmanProblem() error = %v", err)
			}
			assertValidTsmResult(t, graph, result)
			if tc.name == "connected path requires repeated vertex" {
				if result.Distance != 6 {
					t.Fatalf("path tour distance = %v, want 6", result.Distance)
				}
				if len(result.Vertices) != 5 {
					t.Fatalf("path tour = %v, want 5 vertices including a repeated one", result.Vertices)
				}
			}
			if tc.name == "directed cycle" && result.Distance != 6 {
				t.Fatalf("directed cycle distance = %v, want 6", result.Distance)
			}
		})
	}
}

func TestSolveTravelingSalesmanProblem_Reproducible(t *testing.T) {
	graph := mustGraph(t, [][]int{{0, 2, 9, 5}, {2, 0, 1, 8}, {9, 1, 0, 3}, {5, 8, 3, 0}})
	algo := &GraphAlgorithms{}
	first, err := algo.solveTravelingSalesmanProblem(graph, rand.New(rand.NewPCG(1, 2)))
	if err != nil {
		t.Fatal(err)
	}
	second, err := algo.solveTravelingSalesmanProblem(graph, rand.New(rand.NewPCG(1, 2)))
	if err != nil || !reflect.DeepEqual(first, second) {
		t.Fatalf("same seed gave different tours: %v and %v, error=%v", first, second, err)
	}
	assertValidTsmResult(t, graph, first)
	if first.Distance > 11 {
		t.Fatalf("small complete graph tour distance = %v, want at most 11", first.Distance)
	}
}

func TestSolveTravelingSalesmanProblem_SingleVertex(t *testing.T) {
	graph := mustGraph(t, [][]int{{0}})
	got, err := (&GraphAlgorithms{}).SolveTravelingSalesmanProblem(graph)
	if err != nil {
		t.Fatalf("SolveTravelingSalesmanProblem() error = %v", err)
	}
	if !slices.Equal(got.Vertices, []int{1, 1}) || got.Distance != 0 {
		t.Fatalf("SolveTravelingSalesmanProblem() = %+v, want route [1 1] with distance 0", got)
	}
}

func TestSolveTravelingSalesmanProblem_Errors(t *testing.T) {
	tests := []struct {
		name  string
		graph *s21_graph.Graph
		want  error
	}{
		{name: "nil graph", want: ErrNilGraph},
		{name: "empty graph", graph: s21_graph.NewGraph(), want: ErrNilGraph},
		{name: "no return to start", graph: mustGraph(t, [][]int{
			{0, 1, 0},
			{0, 0, 2},
			{0, 0, 0},
		}), want: ErrNoTour},
	}

	for _, tc := range tests {
		t.Run(tc.name, func(t *testing.T) {
			got, err := (&GraphAlgorithms{}).SolveTravelingSalesmanProblem(tc.graph)
			if !errors.Is(err, tc.want) {
				t.Fatalf("SolveTravelingSalesmanProblem() error = %v, want %v", err, tc.want)
			}
			if len(got.Vertices) != 0 || got.Distance != 0 {
				t.Fatalf("SolveTravelingSalesmanProblem() = %+v, want empty result on error", got)
			}
		})
	}
}

func assertValidTsmResult(t *testing.T, graph *s21_graph.Graph, result TsmResult) {
	t.Helper()
	if len(result.Vertices) < graph.VertexCount()+1 {
		t.Fatalf("tour %v does not contain enough steps", result.Vertices)
	}
	if result.Vertices[0] != result.Vertices[len(result.Vertices)-1] {
		t.Fatalf("tour %v does not return to its start", result.Vertices)
	}

	visited := make([]bool, graph.VertexCount())
	distance := 0.0
	for i, vertex := range result.Vertices {
		if vertex < 1 || vertex > graph.VertexCount() {
			t.Fatalf("tour %v contains invalid vertex %d", result.Vertices, vertex)
		}
		visited[vertex-1] = true
		if i == 0 {
			continue
		}
		weight, err := graph.Weight(result.Vertices[i-1], vertex)
		if err != nil || weight <= 0 {
			t.Fatalf("tour %v uses missing edge %d -> %d", result.Vertices, result.Vertices[i-1], vertex)
		}
		distance += float64(weight)
	}
	for vertex, seen := range visited {
		if !seen {
			t.Fatalf("tour %v did not visit vertex %d", result.Vertices, vertex+1)
		}
	}
	if result.Distance != distance {
		t.Fatalf("tour %v has distance %v, edge weights sum to %v", result.Vertices, result.Distance, distance)
	}
}
