package eller

import (
	"maze/internal/maze"
	"fmt"
	"math/rand"
	"time"
)

func Generate(rows, cols int, rng *rand.Rand) (*maze.Maze, error) {
	if err := maze.ValidateSize(rows, cols); err != nil {
		return nil, err
	}
	if rng == nil {
		rng = rand.New(rand.NewSource(time.Now().UnixNano()))
	}

	m, err := maze.New(rows, cols)
	if err != nil {
		return nil, fmt.Errorf("create maze: %w", err)
	}

	sets := make([]int, cols)
	nextSet := 1

	for row := 0; row < rows; row++ {
		for col := 0; col < cols; col++ {
			if sets[col] == 0 {
				sets[col] = nextSet
				nextSet++
			}
		}

		for col := 0; col < cols-1; col++ {
			if sets[col] == sets[col+1] {
				m.SetRightWall(row, col, true)
				continue
			}

			joinSets := row == rows-1 || rng.Intn(2) == 0
			if !joinSets {
				m.SetRightWall(row, col, true)
				continue
			}

			m.SetRightWall(row, col, false)
			mergeSets(sets, sets[col+1], sets[col])
		}
		m.SetRightWall(row, cols-1, true)

		if row == rows-1 {
			for col := 0; col < cols; col++ {
				m.SetBottomWall(row, col, true)
			}
			continue
		}

		sets = createNextRow(m, row, sets, rng)
	}

	return m, nil
}

func mergeSets(sets []int, from int, to int) {
	for col, set := range sets {
		if set == from {
			sets[col] = to
		}
	}
}

func createNextRow(m *maze.Maze, row int, sets []int, rng *rand.Rand) []int {
	groups := make(map[int][]int, len(sets))
	for col, set := range sets {
		groups[set] = append(groups[set], col)
	}

	next := make([]int, len(sets))
	for set, cols := range groups {
		open := chooseVerticalExits(cols, rng)
		for _, col := range cols {
			if open[col] {
				m.SetBottomWall(row, col, false)
				next[col] = set
				continue
			}
			m.SetBottomWall(row, col, true)
		}
	}
	return next
}

func chooseVerticalExits(cols []int, rng *rand.Rand) map[int]bool {
	open := make(map[int]bool, len(cols))
	for _, col := range cols {
		if rng.Intn(2) == 0 {
			open[col] = true
		}
	}
	if len(open) == 0 {
		open[cols[rng.Intn(len(cols))]] = true
	}
	return open
}
