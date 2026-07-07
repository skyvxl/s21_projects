package cave

import (
	"errors"
	"fmt"
	"math/rand"
	"time"
)

const MaxSize = 50

var (
	ErrInvalidSize   = errors.New("cave size must be from 1 to 50")
	ErrInvalidMatrix = errors.New("cave matrix is invalid")
	ErrInvalidLimit  = errors.New("cave limits must be from 0 to 7")
	ErrInvalidChance = errors.New("cave chance must be from 0 to 100")
	ErrNilCave       = errors.New("cave is nil")
)

type Cave struct {
	Rows int
	Cols int

	cells [][]bool
}

func New(rows, cols int) (*Cave, error) {
	if err := validateSize(rows, cols); err != nil {
		return nil, err
	}

	cells := make([][]bool, rows)
	for row := range cells {
		cells[row] = make([]bool, cols)
	}

	return &Cave{Rows: rows, Cols: cols, cells: cells}, nil
}

func FromCells(cells [][]bool) (*Cave, error) {
	if len(cells) == 0 {
		return nil, ErrInvalidSize
	}

	rows := len(cells)
	cols := len(cells[0])
	if err := validateSize(rows, cols); err != nil {
		return nil, err
	}

	c, err := New(rows, cols)
	if err != nil {
		return nil, err
	}
	for row := 0; row < rows; row++ {
		if len(cells[row]) != cols {
			return nil, fmt.Errorf("row %d width differs: %w", row+1, ErrInvalidMatrix)
		}
		copy(c.cells[row], cells[row])
	}
	return c, nil
}

func Generate(rows, cols int, aliveChance int, rng *rand.Rand) (*Cave, error) {
	if err := validateChance(aliveChance); err != nil {
		return nil, err
	}
	if rng == nil {
		rng = rand.New(rand.NewSource(time.Now().UnixNano()))
	}

	c, err := New(rows, cols)
	if err != nil {
		return nil, err
	}
	for row := 0; row < rows; row++ {
		for col := 0; col < cols; col++ {
			c.cells[row][col] = rng.Intn(100) < aliveChance
		}
	}
	return c, nil
}

func (c *Cave) Alive(row, col int) bool {
	return c.cells[row][col]
}

func (c *Cave) Set(row, col int, alive bool) {
	c.cells[row][col] = alive
}

func (c *Cave) Step(birthLimit, deathLimit int) (*Cave, error) {
	if c == nil {
		return nil, ErrNilCave
	}
	if err := validateLimit(birthLimit); err != nil {
		return nil, err
	}
	if err := validateLimit(deathLimit); err != nil {
		return nil, err
	}

	next, err := New(c.Rows, c.Cols)
	if err != nil {
		return nil, err
	}

	for row := 0; row < c.Rows; row++ {
		for col := 0; col < c.Cols; col++ {
			aliveNeighbors := c.aliveNeighbors(row, col)
			alive := c.Alive(row, col)
			switch {
			case alive && aliveNeighbors < deathLimit:
				next.Set(row, col, false)
			case !alive && aliveNeighbors > birthLimit:
				next.Set(row, col, true)
			default:
				next.Set(row, col, alive)
			}
		}
	}

	return next, nil
}

func (c *Cave) aliveNeighbors(row, col int) int {
	alive := 0
	for rowDelta := -1; rowDelta <= 1; rowDelta++ {
		for colDelta := -1; colDelta <= 1; colDelta++ {
			if rowDelta == 0 && colDelta == 0 {
				continue
			}

			nextRow := row + rowDelta
			nextCol := col + colDelta
			if nextRow < 0 || nextRow >= c.Rows || nextCol < 0 || nextCol >= c.Cols {
				alive++
				continue
			}
			if c.Alive(nextRow, nextCol) {
				alive++
			}
		}
	}
	return alive
}

func validateSize(rows, cols int) error {
	if rows <= 0 || cols <= 0 || rows > MaxSize || cols > MaxSize {
		return ErrInvalidSize
	}
	return nil
}

func validateLimit(limit int) error {
	if limit < 0 || limit > 7 {
		return ErrInvalidLimit
	}
	return nil
}

func validateChance(chance int) error {
	if chance < 0 || chance > 100 {
		return ErrInvalidChance
	}
	return nil
}
