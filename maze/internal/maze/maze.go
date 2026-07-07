package maze

import "errors"

const MaxSize = 50

var (
	ErrNilMaze       = errors.New("maze is nil")
	ErrInvalidPoint  = errors.New("point is outside maze")
	ErrNoPath        = errors.New("path not found")
	ErrInvalidSize   = errors.New("maze size must be from 1 to 50")
	ErrInvalidMatrix = errors.New("maze matrix is invalid")
)

type Cell struct {
	RightWall  bool
	BottomWall bool
}

type Point struct {
	Row int
	Col int
}

type Maze struct {
	Rows int
	Cols int

	cells [][]Cell
}

func New(rows, cols int) (*Maze, error) {
	if err := ValidateSize(rows, cols); err != nil {
		return nil, err
	}

	cells := make([][]Cell, rows)
	for row := range cells {
		cells[row] = make([]Cell, cols)
	}

	return &Maze{
		Rows:  rows,
		Cols:  cols,
		cells: cells,
	}, nil
}

func ValidateSize(rows, cols int) error {
	if rows <= 0 || cols <= 0 || rows > MaxSize || cols > MaxSize {
		return ErrInvalidSize
	}
	return nil
}

func (m *Maze) Cell(row, col int) Cell {
	return m.cells[row][col]
}

func (m *Maze) SetRightWall(row, col int, wall bool) {
	m.cells[row][col].RightWall = wall
}

func (m *Maze) SetBottomWall(row, col int, wall bool) {
	m.cells[row][col].BottomWall = wall
}

func (m *Maze) InBounds(point Point) bool {
	return m != nil &&
		point.Row >= 0 &&
		point.Row < m.Rows &&
		point.Col >= 0 &&
		point.Col < m.Cols
}

func (m *Maze) CanMove(from, to Point) bool {
	if !m.InBounds(from) || !m.InBounds(to) {
		return false
	}

	rowDiff := to.Row - from.Row
	colDiff := to.Col - from.Col
	if abs(rowDiff)+abs(colDiff) != 1 {
		return false
	}

	switch {
	case colDiff == 1:
		return !m.Cell(from.Row, from.Col).RightWall
	case colDiff == -1:
		return !m.Cell(to.Row, to.Col).RightWall
	case rowDiff == 1:
		return !m.Cell(from.Row, from.Col).BottomWall
	case rowDiff == -1:
		return !m.Cell(to.Row, to.Col).BottomWall
	default:
		return false
	}
}

func (m *Maze) Neighbors(point Point) []Point {
	candidates := []Point{
		{Row: point.Row, Col: point.Col + 1},
		{Row: point.Row + 1, Col: point.Col},
		{Row: point.Row, Col: point.Col - 1},
		{Row: point.Row - 1, Col: point.Col},
	}

	neighbors := make([]Point, 0, 4)
	for _, candidate := range candidates {
		if m.CanMove(point, candidate) {
			neighbors = append(neighbors, candidate)
		}
	}
	return neighbors
}

func (m *Maze) IsConnected() bool {
	if m == nil || m.Rows == 0 || m.Cols == 0 {
		return false
	}

	start := Point{}
	visited := map[Point]struct{}{start: {}}
	queue := []Point{start}

	for len(queue) > 0 {
		current := queue[0]
		queue = queue[1:]

		for _, next := range m.Neighbors(current) {
			if _, ok := visited[next]; ok {
				continue
			}
			visited[next] = struct{}{}
			queue = append(queue, next)
		}
	}

	return len(visited) == m.Rows*m.Cols
}

func (m *Maze) IsPerfect() bool {
	if m == nil {
		return false
	}
	return m.IsConnected() && m.openEdges() == m.Rows*m.Cols-1
}

func (m *Maze) openEdges() int {
	edges := 0
	for row := 0; row < m.Rows; row++ {
		for col := 0; col < m.Cols; col++ {
			cell := m.Cell(row, col)
			if col+1 < m.Cols && !cell.RightWall {
				edges++
			}
			if row+1 < m.Rows && !cell.BottomWall {
				edges++
			}
		}
	}
	return edges
}

func abs(value int) int {
	if value < 0 {
		return -value
	}
	return value
}
