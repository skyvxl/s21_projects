package maze_test

import (
	"maze/internal/maze"
	"bytes"
	"strings"
	"testing"
)

const sampleMaze = `4 4
0 0 0 1
1 0 1 1
0 1 0 1
0 0 0 1

1 0 1 0
0 0 1 0
1 1 0 1
1 1 1 1
`

func TestParseAndWriteMaze(t *testing.T) {
	t.Parallel()

	m, err := maze.Parse(strings.NewReader(sampleMaze))
	if err != nil {
		t.Fatalf("parse maze: %v", err)
	}

	if m.Rows != 4 || m.Cols != 4 {
		t.Fatalf("size = %dx%d, want 4x4", m.Rows, m.Cols)
	}
	if m.Cell(0, 3).RightWall != true {
		t.Fatal("top-right cell must have right wall from file")
	}
	if m.Cell(1, 2).BottomWall != true {
		t.Fatal("cell (1,2) must have bottom wall from file")
	}
	if m.Cell(0, 1).BottomWall != false {
		t.Fatal("cell (0,1) must not have bottom wall from file")
	}

	var out bytes.Buffer
	if err := m.Write(&out); err != nil {
		t.Fatalf("write maze: %v", err)
	}

	roundTrip, err := maze.Parse(&out)
	if err != nil {
		t.Fatalf("parse written maze: %v", err)
	}
	if roundTrip.Cell(1, 0).RightWall != true || roundTrip.Cell(3, 3).BottomWall != true {
		t.Fatal("round-trip maze lost wall data")
	}
}

func TestParseMazeRejectsInvalidInput(t *testing.T) {
	t.Parallel()

	cases := []struct {
		name  string
		input string
	}{
		{name: "empty", input: ""},
		{name: "zero rows", input: "0 2\n0 1\n0 1\n\n1 1\n1 1\n"},
		{name: "too large", input: "51 1\n1\n\n1\n"},
		{name: "short row", input: "2 2\n1\n1 1\n\n1 1\n1 1\n"},
		{name: "non binary token", input: "1 1\n2\n\n1\n"},
	}

	for _, tc := range cases {
		t.Run(tc.name, func(t *testing.T) {
			t.Parallel()

			if _, err := maze.Parse(strings.NewReader(tc.input)); err == nil {
				t.Fatal("expected parse error")
			}
		})
	}
}

func TestMazeMovementAndPerfectCheck(t *testing.T) {
	t.Parallel()

	m, err := maze.New(2, 2)
	if err != nil {
		t.Fatalf("new maze: %v", err)
	}
	m.SetRightWall(0, 0, true)
	m.SetBottomWall(0, 0, false)
	m.SetRightWall(0, 1, true)
	m.SetBottomWall(0, 1, false)
	m.SetRightWall(1, 0, false)
	m.SetBottomWall(1, 0, true)
	m.SetRightWall(1, 1, true)
	m.SetBottomWall(1, 1, true)

	if m.CanMove(maze.Point{Row: 0, Col: 0}, maze.Point{Row: 0, Col: 1}) {
		t.Fatal("right wall should block movement")
	}
	if !m.CanMove(maze.Point{Row: 0, Col: 0}, maze.Point{Row: 1, Col: 0}) {
		t.Fatal("missing bottom wall should allow movement")
	}
	if !m.IsPerfect() {
		t.Fatal("2x2 tree-shaped maze should be perfect")
	}

	m.SetBottomWall(0, 0, true)
	if m.IsPerfect() {
		t.Fatal("disconnected maze should not be perfect")
	}
}
