package s21_graph

import (
	"errors"
	"os"
	"path/filepath"
	"testing"
)

func TestGraph_Valid_IsDirected(t *testing.T) {
	tests := []struct {
		name   string
		matrix [][]int
		want   bool
	}{
		{
			name: "empty graph",
			want: false,
		},
		{
			name: "undirected graph",
			matrix: [][]int{
				{0, 4, 0},
				{4, 0, 7},
				{0, 7, 0},
			},
			want: false,
		},
		{
			name: "undirected graph with loop",
			matrix: [][]int{
				{3, 4},
				{4, 0},
			},
			want: false,
		},
		{
			name: "directed graph",
			matrix: [][]int{
				{0, 5, 0},
				{0, 0, 2},
				{1, 0, 0},
			},
			want: true,
		},
	}

	for _, tc := range tests {
		t.Run(tc.name, func(t *testing.T) {
			graph := NewGraph()
			if tc.matrix != nil {
				var err error
				graph, err = NewFromMatrix(tc.matrix)
				if err != nil {
					t.Fatalf("NewFromMatrix() error: %v", err)
				}
			}

			if got := graph.IsDirected(); got != tc.want {
				t.Fatalf("IsDirected()=%v, want %v", got, tc.want)
			}
		})
	}
}

func TestGraph_Undirected_ExportGraphToDot(t *testing.T) {
	graph, err := NewFromMatrix([][]int{
		{9, 4, 0},
		{4, 0, 7},
		{0, 7, 0},
	})
	if err != nil {
		t.Fatalf("NewFromMatrix() error: %v", err)
	}

	filename := filepath.Join(t.TempDir(), "undirected.dot")
	if err := graph.ExportGraphToDot(filename); err != nil {
		t.Fatalf("ExportGraphToDot() error: %v", err)
	}

	got, err := os.ReadFile(filename)
	if err != nil {
		t.Fatalf("read exported DOT: %v", err)
	}
	want := "graph G {\n" +
		"    1;\n" +
		"    2;\n" +
		"    3;\n" +
		"    1 -- 1 [label=\"9\"];\n" +
		"    1 -- 2 [label=\"4\"];\n" +
		"    2 -- 3 [label=\"7\"];\n" +
		"}\n"

	if string(got) != want {
		t.Fatalf("exported DOT:\n%s\nwant:\n%s", got, want)
	}
}

func TestGraph_Directed_ExportGraphToDot(t *testing.T) {
	graph, err := NewFromMatrix([][]int{
		{0, 5, 0},
		{0, 0, 2},
		{1, 0, 0},
	})
	if err != nil {
		t.Fatalf("NewFromMatrix() error: %v", err)
	}

	filename := filepath.Join(t.TempDir(), "directed.dot")
	if err := graph.ExportGraphToDot(filename); err != nil {
		t.Fatalf("ExportGraphToDot() error: %v", err)
	}

	got, err := os.ReadFile(filename)
	if err != nil {
		t.Fatalf("read exported DOT: %v", err)
	}
	want := "digraph G {\n" +
		"    1;\n" +
		"    2;\n" +
		"    3;\n" +
		"    1 -> 2 [label=\"5\"];\n" +
		"    2 -> 3 [label=\"2\"];\n" +
		"    3 -> 1 [label=\"1\"];\n" +
		"}\n"

	if string(got) != want {
		t.Fatalf("exported DOT:\n%s\nwant:\n%s", got, want)
	}
}

func TestGraph_Invalid_ExportGraphToDot(t *testing.T) {
	t.Run("empty graph", func(t *testing.T) {
		err := NewGraph().ExportGraphToDot(filepath.Join(t.TempDir(), "graph.dot"))
		if !errors.Is(err, ErrEmptyGraph) {
			t.Fatalf("ExportGraphToDot() error=%v, want errors.Is(_, %v)", err, ErrEmptyGraph)
		}
	})

	t.Run("output path is directory", func(t *testing.T) {
		graph, err := NewFromMatrix([][]int{{0}})
		if err != nil {
			t.Fatalf("NewFromMatrix() error: %v", err)
		}

		if err := graph.ExportGraphToDot(t.TempDir()); err == nil {
			t.Fatal("ExportGraphToDot() error=nil, want file creation error")
		}
	})
}

func TestGraph_ExportGraphToDot_WriteError(t *testing.T) {
	if _, err := os.Stat("/dev/full"); err != nil {
		t.Skip("/dev/full is not available on this platform")
	}
	graph, err := NewFromMatrix([][]int{{0, 1}, {1, 0}})
	if err != nil {
		t.Fatal(err)
	}
	if err := graph.ExportGraphToDot("/dev/full"); err == nil {
		t.Fatal("ExportGraphToDot(/dev/full) error = nil, want write error")
	}
}
