package s21_graph

import (
	"errors"
	"os"
	"path/filepath"
	"testing"
)

func createTempFile(t *testing.T) string {
	tempDir := t.TempDir()
	return tempDir
}

var testData = []byte(`3
0  2  7
2  0  4
7  4  0
`)

func TestGraph_Valid_LoadGraphFromFile(t *testing.T) {
	tempDir := createTempFile(t)
	graphFile := filepath.Join(tempDir, "graph.txt")
	os.WriteFile(graphFile, testData, 0o644)
	graph := NewGraph()
	err := graph.LoadGraphFromFile(graphFile)
	if err != nil {
		t.Fatal(err)
	}
}

func TestGraph_Nil_VertexCount(t *testing.T) {
	graph := NewGraph()
	if graph.VertexCount() != 0 {
		t.Fatal("expected other value")
	}
}

func TestGraph_Valid_VertexCount(t *testing.T) {
	tempDir := createTempFile(t)
	graphFile := filepath.Join(tempDir, "graph.txt")
	os.WriteFile(graphFile, testData, 0o644)
	graph := NewGraph()
	err := graph.LoadGraphFromFile(graphFile)
	if err != nil {
		t.Fatal(err)
	}
	if graph.VertexCount() == 0 {
		t.Fatal("expected other value")
	}
}

func TestGraph_Invalid_LoadGraphFromFile(t *testing.T) {
	tests := []struct {
		name    string
		content string
		wantErr error
	}{
		{name: "invalid vertex count", content: "not-a-number", wantErr: ErrInvalidMatrix},
		{name: "negative vertex count", content: "-1", wantErr: ErrInvalidMatrix},
		{name: "empty graph", content: "0", wantErr: ErrEmptyGraph},
		{name: "missing matrix value", content: "2\n0 1\n1", wantErr: ErrInvalidMatrix},
		{name: "invalid matrix value", content: "2\n0 x\n1 0", wantErr: ErrInvalidMatrix},
		{name: "negative weight", content: "2\n0 -1\n1 0", wantErr: ErrNegativeWeight},
	}

	for _, tc := range tests {
		t.Run(tc.name, func(t *testing.T) {
			filename := filepath.Join(t.TempDir(), "graph.txt")
			if err := os.WriteFile(filename, []byte(tc.content), 0o644); err != nil {
				t.Fatalf("write test graph: %v", err)
			}

			err := NewGraph().LoadGraphFromFile(filename)
			if !errors.Is(err, tc.wantErr) {
				t.Fatalf("LoadGraphFromFile() error=%v, want errors.Is(_, %v)", err, tc.wantErr)
			}
		})
	}
}

func TestGraph_Invalid_NewFromMatrix(t *testing.T) {
	_, err := NewFromMatrix([][]int{{0, -1}, {1, 0}})
	if !errors.Is(err, ErrNegativeWeight) {
		t.Fatalf("NewFromMatrix() error=%v, want errors.Is(_, %v)", err, ErrNegativeWeight)
	}
}
