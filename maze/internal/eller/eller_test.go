package eller_test

import (
	"maze/internal/eller"
	"math/rand"
	"testing"
)

func TestGenerateCreatesPerfectMaze(t *testing.T) {
	t.Parallel()

	cases := []struct {
		name string
		rows int
		cols int
	}{
		{name: "one cell", rows: 1, cols: 1},
		{name: "one row", rows: 1, cols: 8},
		{name: "one column", rows: 8, cols: 1},
		{name: "square", rows: 10, cols: 10},
		{name: "rectangle", rows: 12, cols: 7},
	}

	for _, tc := range cases {
		t.Run(tc.name, func(t *testing.T) {
			t.Parallel()

			m, err := eller.Generate(tc.rows, tc.cols, rand.New(rand.NewSource(42)))
			if err != nil {
				t.Fatalf("generate: %v", err)
			}
			if m.Rows != tc.rows || m.Cols != tc.cols {
				t.Fatalf("size = %dx%d, want %dx%d", m.Rows, m.Cols, tc.rows, tc.cols)
			}
			if !m.IsPerfect() {
				t.Fatal("generated maze must be connected and acyclic")
			}
			for row := 0; row < m.Rows; row++ {
				if !m.Cell(row, m.Cols-1).RightWall {
					t.Fatalf("row %d is missing outside right border", row)
				}
			}
			for col := 0; col < m.Cols; col++ {
				if !m.Cell(m.Rows-1, col).BottomWall {
					t.Fatalf("column %d is missing outside bottom border", col)
				}
			}
		})
	}
}

func TestGenerateRejectsInvalidSizes(t *testing.T) {
	t.Parallel()

	for _, size := range [][2]int{{0, 1}, {1, 0}, {-1, 1}, {51, 1}, {1, 51}} {
		if _, err := eller.Generate(size[0], size[1], rand.New(rand.NewSource(1))); err == nil {
			t.Fatalf("expected error for size %dx%d", size[0], size[1])
		}
	}
}
