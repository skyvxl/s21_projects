package solver

import (
	"maze/internal/maze"
	"fmt"
)

func Solve(m *maze.Maze, start, end maze.Point) ([]maze.Point, error) {
	if m == nil {
		return nil, maze.ErrNilMaze
	}
	if !m.InBounds(start) {
		return nil, fmt.Errorf("start point: %w", maze.ErrInvalidPoint)
	}
	if !m.InBounds(end) {
		return nil, fmt.Errorf("end point: %w", maze.ErrInvalidPoint)
	}

	parent := map[maze.Point]maze.Point{start: start}
	queue := []maze.Point{start}

	for len(queue) > 0 {
		current := queue[0]
		queue = queue[1:]

		if current == end {
			return buildPath(parent, start, end), nil
		}

		for _, next := range m.Neighbors(current) {
			if _, seen := parent[next]; seen {
				continue
			}
			parent[next] = current
			queue = append(queue, next)
		}
	}

	return nil, maze.ErrNoPath
}

func buildPath(parent map[maze.Point]maze.Point, start, end maze.Point) []maze.Point {
	reversed := []maze.Point{end}
	for current := end; current != start; {
		current = parent[current]
		reversed = append(reversed, current)
	}

	path := make([]maze.Point, len(reversed))
	for i := range reversed {
		path[i] = reversed[len(reversed)-1-i]
	}
	return path
}
