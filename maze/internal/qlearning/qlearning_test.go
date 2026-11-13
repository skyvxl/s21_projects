package qlearning_test

import (
	"errors"
	"math/rand"
	"maze/internal/eller"
	"maze/internal/maze"
	"maze/internal/qlearning"
	"maze/internal/solver"
	"strings"
	"testing"
)

func fastParams() qlearning.Params {
	p := qlearning.DefaultParams()
	p.Episodes = 1500
	p.MaxSteps = 200
	return p
}

func TestTrainRejectsInvalidInput(t *testing.T) {
	t.Parallel()

	m, err := maze.New(2, 2)
	if err != nil {
		t.Fatalf("new maze: %v", err)
	}

	if _, err := qlearning.Train(nil, maze.Point{}, fastParams(), nil); !errors.Is(err, maze.ErrNilMaze) {
		t.Fatalf("nil maze: got %v, want %v", err, maze.ErrNilMaze)
	}
	if _, err := qlearning.Train(m, maze.Point{Row: 5, Col: 5}, fastParams(), nil); !errors.Is(err, maze.ErrInvalidPoint) {
		t.Fatalf("invalid end: got %v, want %v", err, maze.ErrInvalidPoint)
	}

	badEpisodes := fastParams()
	badEpisodes.Episodes = 0
	if _, err := qlearning.Train(m, maze.Point{}, badEpisodes, nil); !errors.Is(err, qlearning.ErrInvalidParams) {
		t.Fatalf("zero episodes: got %v, want %v", err, qlearning.ErrInvalidParams)
	}

	badSteps := fastParams()
	badSteps.MaxSteps = 0
	if _, err := qlearning.Train(m, maze.Point{}, badSteps, nil); !errors.Is(err, qlearning.ErrInvalidParams) {
		t.Fatalf("zero max steps: got %v, want %v", err, qlearning.ErrInvalidParams)
	}
}

func TestPathRejectsOutOfBoundsStart(t *testing.T) {
	t.Parallel()

	m, err := maze.New(2, 2)
	if err != nil {
		t.Fatalf("new maze: %v", err)
	}
	agent, err := qlearning.Train(m, maze.Point{Row: 1, Col: 1}, fastParams(), rand.New(rand.NewSource(1)))
	if err != nil {
		t.Fatalf("train: %v", err)
	}

	if _, err := agent.Path(maze.Point{Row: -1, Col: 0}); !errors.Is(err, maze.ErrInvalidPoint) {
		t.Fatalf("out of bounds start: got %v, want %v", err, maze.ErrInvalidPoint)
	}
}

func TestPathHandlesSingleCellMaze(t *testing.T) {
	t.Parallel()

	m, err := maze.New(1, 1)
	if err != nil {
		t.Fatalf("new maze: %v", err)
	}
	end := maze.Point{Row: 0, Col: 0}

	agent, err := qlearning.Train(m, end, fastParams(), rand.New(rand.NewSource(1)))
	if err != nil {
		t.Fatalf("train: %v", err)
	}

	path, err := agent.Path(end)
	if err != nil {
		t.Fatalf("path: %v", err)
	}
	if len(path) != 1 || path[0] != end {
		t.Fatalf("path = %#v, want [%#v]", path, end)
	}
}

func TestAgentFindsShortestPathOnOpenCorridor(t *testing.T) {
	t.Parallel()

	m, err := maze.New(1, 5)
	if err != nil {
		t.Fatalf("new maze: %v", err)
	}
	for col := 0; col < m.Cols-1; col++ {
		m.SetRightWall(0, col, false)
	}
	end := maze.Point{Row: 0, Col: 4}

	agent, err := qlearning.Train(m, end, fastParams(), rand.New(rand.NewSource(7)))
	if err != nil {
		t.Fatalf("train: %v", err)
	}

	path, err := agent.Path(maze.Point{Row: 0, Col: 0})
	if err != nil {
		t.Fatalf("path: %v", err)
	}
	if len(path) != 5 {
		t.Fatalf("path length = %d, want 5: %#v", len(path), path)
	}
	for i := 1; i < len(path); i++ {
		if !m.CanMove(path[i-1], path[i]) {
			t.Fatalf("path crosses a wall at step %d: %#v -> %#v", i, path[i-1], path[i])
		}
	}
}

func TestAgentMatchesSolverOnMazeWithWalls(t *testing.T) {
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

	want, err := solver.Solve(m, start, end)
	if err != nil {
		t.Fatalf("solve: %v", err)
	}

	agent, err := qlearning.Train(m, end, fastParams(), rand.New(rand.NewSource(42)))
	if err != nil {
		t.Fatalf("train: %v", err)
	}

	got, err := agent.Path(start)
	if err != nil {
		t.Fatalf("agent path: %v", err)
	}

	if len(got) != len(want) {
		t.Fatalf("path length = %d, want %d: got %#v, want %#v", len(got), len(want), got, want)
	}
	for i := range want {
		if got[i] != want[i] {
			t.Fatalf("path[%d] = %#v, want %#v", i, got[i], want[i])
		}
	}
}

func TestAgentReportsErrorForUnreachableEnd(t *testing.T) {
	t.Parallel()

	m, err := maze.New(1, 2)
	if err != nil {
		t.Fatalf("new maze: %v", err)
	}
	m.SetRightWall(0, 0, true)

	agent, err := qlearning.Train(m, maze.Point{Row: 0, Col: 1}, fastParams(), rand.New(rand.NewSource(3)))
	if err != nil {
		t.Fatalf("train: %v", err)
	}

	if _, err := agent.Path(maze.Point{Row: 0, Col: 0}); !errors.Is(err, maze.ErrNoPath) {
		t.Fatalf("unreachable end: got %v, want %v", err, maze.ErrNoPath)
	}
}

func TestAgentDetectsLoopInUnreachablePocket(t *testing.T) {
	t.Parallel()

	m, err := maze.New(1, 4)
	if err != nil {
		t.Fatalf("new maze: %v", err)
	}
	m.SetRightWall(0, 0, false)
	m.SetRightWall(0, 1, true)
	m.SetRightWall(0, 2, false)

	agent, err := qlearning.Train(m, maze.Point{Row: 0, Col: 3}, fastParams(), rand.New(rand.NewSource(4)))
	if err != nil {
		t.Fatalf("train: %v", err)
	}

	if _, err := agent.Path(maze.Point{Row: 0, Col: 0}); !errors.Is(err, maze.ErrNoPath) {
		t.Fatalf("unreachable pocket: got %v, want %v", err, maze.ErrNoPath)
	}
}

func TestAgentMatchesShortestPathOnGeneratedMaze(t *testing.T) {
	t.Parallel()

	m, err := eller.Generate(6, 6, rand.New(rand.NewSource(11)))
	if err != nil {
		t.Fatalf("generate maze: %v", err)
	}
	end := maze.Point{Row: 5, Col: 5}

	agent, err := qlearning.Train(m, end, fastParams(), rand.New(rand.NewSource(99)))
	if err != nil {
		t.Fatalf("train: %v", err)
	}

	starts := []maze.Point{
		{Row: 0, Col: 0},
		{Row: 0, Col: 5},
		{Row: 5, Col: 0},
		{Row: 2, Col: 3},
	}

	for _, start := range starts {
		want, err := solver.Solve(m, start, end)
		if err != nil {
			t.Fatalf("solve from %#v: %v", start, err)
		}

		got, err := agent.Path(start)
		if err != nil {
			t.Fatalf("agent path from %#v: %v", start, err)
		}

		if len(got) != len(want) {
			t.Fatalf("from %#v: path length = %d, want %d (perfect maze has a unique route): got %#v, want %#v",
				start, len(got), len(want), got, want)
		}
	}
}

func TestEndReturnsTrainedEndPoint(t *testing.T) {
	t.Parallel()

	m, err := maze.New(2, 2)
	if err != nil {
		t.Fatalf("new maze: %v", err)
	}
	end := maze.Point{Row: 1, Col: 0}

	agent, err := qlearning.Train(m, end, fastParams(), rand.New(rand.NewSource(5)))
	if err != nil {
		t.Fatalf("train: %v", err)
	}
	if agent.End() != end {
		t.Fatalf("End() = %#v, want %#v", agent.End(), end)
	}
}
