package solver_test

import (
	"maze/internal/maze"
	"maze/internal/solver"
	"strings"
	"testing"
)

func TestSolveFindsShortestPath(t *testing.T) {
	t.Parallel()

	input := `2 3
0 1 1
0 0 1

1 0 1
1 1 1
`
	m, err := maze.Parse(strings.NewReader(input))
	if err != nil {
		t.Fatalf("parse maze: %v", err)
	}

	start := maze.Point{Row: 0, Col: 0}
	end := maze.Point{Row: 1, Col: 2}
	path, err := solver.Solve(m, start, end)
	if err != nil {
		t.Fatalf("solve: %v", err)
	}

	want := []maze.Point{
		{Row: 0, Col: 0},
		{Row: 0, Col: 1},
		{Row: 1, Col: 1},
		{Row: 1, Col: 2},
	}
	if len(path) != len(want) {
		t.Fatalf("path length = %d, want %d: %#v", len(path), len(want), path)
	}
	for i := range want {
		if path[i] != want[i] {
			t.Fatalf("path[%d] = %#v, want %#v", i, path[i], want[i])
		}
	}
	for i := 1; i < len(path); i++ {
		if !m.CanMove(path[i-1], path[i]) {
			t.Fatalf("path crosses a wall at step %d: %#v -> %#v", i, path[i-1], path[i])
		}
	}
}

func TestSolveReportsInvalidOrMissingPath(t *testing.T) {
	t.Parallel()

	m, err := maze.New(1, 2)
	if err != nil {
		t.Fatalf("new maze: %v", err)
	}
	m.SetRightWall(0, 0, true)
	m.SetRightWall(0, 1, true)
	m.SetBottomWall(0, 0, true)
	m.SetBottomWall(0, 1, true)

	if _, err := solver.Solve(nil, maze.Point{}, maze.Point{}); err == nil {
		t.Fatal("expected nil maze error")
	}
	if _, err := solver.Solve(m, maze.Point{Row: -1, Col: 0}, maze.Point{Row: 0, Col: 1}); err == nil {
		t.Fatal("expected invalid start error")
	}
	if _, err := solver.Solve(m, maze.Point{Row: 0, Col: 0}, maze.Point{Row: 0, Col: 1}); err == nil {
		t.Fatal("expected no path error")
	}
}
