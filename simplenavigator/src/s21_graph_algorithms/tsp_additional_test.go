package s21_graph_algorithms

import (
	"errors"
	"math"
	"slices"
	"testing"

	"simple_navigator/src/s21_graph"
)

func additionalTSPSolvers() map[string]func(*s21_graph.Graph) (TsmResult, error) {
	algo := &GraphAlgorithms{}
	return map[string]func(*s21_graph.Graph) (TsmResult, error){
		"nearest neighbor": algo.SolveTravelingSalesmanProblemNearestNeighbor,
		"brute force":      algo.SolveTravelingSalesmanProblemBruteForce,
	}
}

func TestAdditionalTSP_ValidWalks(t *testing.T) {
	tests := []struct {
		name   string
		matrix [][]int
		want   float64
	}{
		{"path with repeated visits", [][]int{{0, 1, 0}, {1, 0, 2}, {0, 2, 0}}, 6},
		{"directed cycle", [][]int{{0, 1, 0}, {0, 0, 2}, {3, 0, 0}}, 6},
		{"weighted graph", [][]int{{0, 9, 2, 5}, {9, 0, 1, 3}, {2, 1, 0, 8}, {5, 3, 8, 0}}, 11},
		{"large weights", [][]int{{0, math.MaxInt}, {math.MaxInt, 0}}, 2 * float64(math.MaxInt)},
	}
	for name, solve := range additionalTSPSolvers() {
		for _, tc := range tests {
			t.Run(name+"/"+tc.name, func(t *testing.T) {
				graph := mustGraph(t, tc.matrix)
				got, err := solve(graph)
				if err != nil {
					t.Fatal(err)
				}
				assertValidTsmResult(t, graph, got)
				if got.Distance != tc.want {
					t.Fatalf("distance = %g, want %g; route=%v", got.Distance, tc.want, got.Vertices)
				}
			})
		}
	}
}

func TestAdditionalTSP_SingleVertex(t *testing.T) {
	for name, solve := range additionalTSPSolvers() {
		t.Run(name, func(t *testing.T) {
			got, err := solve(mustGraph(t, [][]int{{0}}))
			if err != nil || got.Distance != 0 || !slices.Equal(got.Vertices, []int{1, 1}) {
				t.Fatalf("single vertex = %+v, %v", got, err)
			}
		})
	}
}

func TestAdditionalTSP_Errors(t *testing.T) {
	tests := []struct {
		name  string
		graph *s21_graph.Graph
		want  error
	}{
		{"nil graph", nil, ErrNilGraph},
		{"empty graph", s21_graph.NewGraph(), ErrNilGraph},
		{"no return", mustGraph(t, [][]int{{0, 1}, {0, 0}}), ErrNoTour},
		{"disconnected", mustGraph(t, [][]int{{0, 0}, {0, 0}}), ErrNoTour},
		{"overflow", mustGraph(t, [][]int{{0, math.MaxInt, 0}, {0, 0, 1}, {1, 0, 0}}), ErrDistanceOverflow},
	}
	for name, solve := range additionalTSPSolvers() {
		for _, tc := range tests {
			t.Run(name+"/"+tc.name, func(t *testing.T) {
				got, err := solve(tc.graph)
				if !errors.Is(err, tc.want) || len(got.Vertices) != 0 || got.Distance != 0 {
					t.Fatalf("result=%+v error=%v, want empty result and %v", got, err, tc.want)
				}
			})
		}
	}
}

func TestAdditionalTSP_GreedyIsNotAlwaysOptimal(t *testing.T) {
	graph := mustGraph(t, [][]int{{0, 1, 2, 10}, {1, 0, 3, 9}, {2, 3, 0, 12}, {10, 9, 12, 0}})
	algo := &GraphAlgorithms{}
	for _, tc := range []struct {
		name  string
		solve func(*s21_graph.Graph) (TsmResult, error)
		want  float64
	}{
		{"nearest neighbor", algo.SolveTravelingSalesmanProblemNearestNeighbor, 26},
		{"brute force", algo.SolveTravelingSalesmanProblemBruteForce, 24},
	} {
		t.Run(tc.name, func(t *testing.T) {
			got, err := tc.solve(graph)
			if err != nil {
				t.Fatal(err)
			}
			assertValidTsmResult(t, graph, got)
			if got.Distance != tc.want {
				t.Fatalf("distance=%g, want %g", got.Distance, tc.want)
			}
		})
	}
}

func TestBruteForceTSP_SizeLimit(t *testing.T) {
	matrix := make([][]int, MaxBruteForceVertices+1)
	for i := range matrix {
		matrix[i] = make([]int, len(matrix))
		for j := range matrix[i] {
			if i != j {
				matrix[i][j] = 1
			}
		}
	}
	_, err := (&GraphAlgorithms{}).SolveTravelingSalesmanProblemBruteForce(mustGraph(t, matrix))
	if !errors.Is(err, ErrBruteForceTooLarge) {
		t.Fatalf("error=%v, want %v", err, ErrBruteForceTooLarge)
	}
}

func TestBruteForceTSP_ExactCostComparison(t *testing.T) {
	for _, weight := range []int64{1 << 53, int64(math.MaxInt) - 1} {
		if weight >= int64(math.MaxInt) {
			continue
		}
		w := int(weight)
		graph := mustGraph(t, [][]int{{0, w + 1, w}, {w, 0, w}, {w, w, 0}})
		result, err := (&GraphAlgorithms{}).SolveTravelingSalesmanProblemBruteForce(graph)
		if err != nil {
			t.Fatal(err)
		}
		if !slices.Equal(result.Vertices, []int{1, 3, 2, 1}) {
			t.Fatalf("weight=%d: route=%v, want exact minimum 1-3-2-1", weight, result.Vertices)
		}
		assertValidTsmResult(t, graph, result)
	}
}
