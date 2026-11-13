package s21_graph

import (
	"errors"
	"reflect"
	"testing"
)

func TestNewFromMatrix_ValidationAndCopy(t *testing.T) {
	for _, matrix := range [][][]int{nil, {}, {{0, 1}, {1}}, {{0, 1, 2}, {1, 0, 3}}} {
		if _, err := NewFromMatrix(matrix); !errors.Is(err, ErrInvalidMatrix) {
			t.Fatalf("NewFromMatrix(%v) error = %v, want %v", matrix, err, ErrInvalidMatrix)
		}
	}
	matrix := [][]int{{0, 5}, {7, 0}}
	g, err := NewFromMatrix(matrix)
	if err != nil {
		t.Fatal(err)
	}
	matrix[0][1] = 99
	matrix[1] = []int{99, 99}
	if weight, err := g.Weight(1, 2); err != nil || weight != 5 {
		t.Fatalf("source mutation changed graph: weight=%d error=%v", weight, err)
	}
	if weight, err := g.Weight(2, 1); err != nil || weight != 7 {
		t.Fatalf("source row replacement changed graph: weight=%d error=%v", weight, err)
	}
}

func TestGraph_NeighborsAndWeight(t *testing.T) {
	g, err := NewFromMatrix([][]int{{3, 0, 5}, {7, 0, 0}, {0, 0, 0}})
	if err != nil {
		t.Fatal(err)
	}
	neighbors, err := g.Neighbors(1)
	want := []Neighbor{{Vertex: 1, Weight: 3}, {Vertex: 3, Weight: 5}}
	if err != nil || !reflect.DeepEqual(neighbors, want) {
		t.Fatalf("Neighbors(1) = %v, %v, want %v", neighbors, err, want)
	}
	neighbors[0].Weight = 99
	for _, tc := range []struct{ from, to, weight int }{{1, 1, 3}, {1, 2, 0}, {1, 3, 5}, {3, 1, 0}} {
		if weight, err := g.Weight(tc.from, tc.to); err != nil || weight != tc.weight {
			t.Fatalf("Weight(%d, %d) = %d, %v, want %d", tc.from, tc.to, weight, err, tc.weight)
		}
	}
	if neighbors, err := g.Neighbors(3); err != nil || len(neighbors) != 0 {
		t.Fatalf("isolated neighbors = %v, %v", neighbors, err)
	}
	for _, vertex := range []int{-1, 0, 4} {
		if _, err := g.Neighbors(vertex); !errors.Is(err, ErrInvalidVertex) {
			t.Fatalf("Neighbors(%d) error = %v", vertex, err)
		}
		for _, pair := range [][2]int{{vertex, 1}, {1, vertex}} {
			if _, err := g.Weight(pair[0], pair[1]); !errors.Is(err, ErrInvalidVertex) {
				t.Fatalf("Weight(%d, %d) error = %v", pair[0], pair[1], err)
			}
		}
	}
	empty := NewGraph()
	if _, err := empty.Neighbors(1); !errors.Is(err, ErrEmptyGraph) {
		t.Fatalf("empty Neighbors error = %v", err)
	}
	if _, err := empty.Weight(1, 1); !errors.Is(err, ErrEmptyGraph) {
		t.Fatalf("empty Weight error = %v", err)
	}
}
