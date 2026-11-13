package s21_graph

import (
	"errors"
	"fmt"
	"math"
	"os"
	"path/filepath"
	"testing"
)

func TestLoadGraphFromFile_PreservesGraphOnInvalidInput(t *testing.T) {
	for _, tc := range []struct {
		name, content string
		want          error
	}{
		{"empty file", "", ErrInvalidMatrix},
		{"truncated matrix", "2\n0 1\n1", ErrInvalidMatrix},
		{"extra number", "2\n0 1\n1 0\n999", ErrInvalidMatrix},
		{"extra text", "2\n0 1\n1 0\ngarbage", ErrInvalidMatrix},
		{"negative weight", "2\n0 -1\n1 0", ErrNegativeWeight},
		{"zero size", "0", ErrEmptyGraph},
		{"oversized header", fmt.Sprint(math.MaxInt), ErrInvalidMatrix},
	} {
		t.Run(tc.name, func(t *testing.T) {
			defer func() {
				if value := recover(); value != nil {
					t.Fatalf("LoadGraphFromFile panicked: %v", value)
				}
			}()
			g, err := NewFromMatrix([][]int{{0, 7}, {7, 0}})
			if err != nil {
				t.Fatal(err)
			}
			filename := filepath.Join(t.TempDir(), "graph.txt")
			if err := os.WriteFile(filename, []byte(tc.content), 0o600); err != nil {
				t.Fatal(err)
			}
			if err := g.LoadGraphFromFile(filename); !errors.Is(err, tc.want) {
				t.Fatalf("error = %v, want %v", err, tc.want)
			}
			weight, err := g.Weight(1, 2)
			if g.VertexCount() != 2 || err != nil || weight != 7 {
				t.Fatalf("failed load changed graph: count=%d weight=%d error=%v", g.VertexCount(), weight, err)
			}
		})
	}
}

func TestLoadGraphFromFile_WhitespaceAndDirectedWeights(t *testing.T) {
	filename := filepath.Join(t.TempDir(), "graph.txt")
	if err := os.WriteFile(filename, []byte(" \n2\t0 5\n0 3 \t\n"), 0o600); err != nil {
		t.Fatal(err)
	}
	g := NewGraph()
	if err := g.LoadGraphFromFile(filename); err != nil {
		t.Fatal(err)
	}
	weight, err := g.Weight(1, 2)
	loop, loopErr := g.Weight(2, 2)
	if g.VertexCount() != 2 || !g.IsDirected() || weight != 5 || loop != 3 || err != nil || loopErr != nil {
		t.Fatalf("loaded graph: count=%d directed=%v weight=%d loop=%d errors=(%v, %v)", g.VertexCount(), g.IsDirected(), weight, loop, err, loopErr)
	}
}

func TestLoadGraphFromFile_MissingFilePreservesGraph(t *testing.T) {
	g, err := NewFromMatrix([][]int{{9}})
	if err != nil {
		t.Fatal(err)
	}
	if err := g.LoadGraphFromFile(filepath.Join(t.TempDir(), "missing.txt")); !errors.Is(err, os.ErrNotExist) {
		t.Fatalf("error = %v, want file not found", err)
	}
	if weight, err := g.Weight(1, 1); err != nil || weight != 9 {
		t.Fatalf("failed load changed graph: weight=%d error=%v", weight, err)
	}
}
