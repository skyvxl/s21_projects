package maze

import (
	"bufio"
	"fmt"
	"io"
	"os"
	"strconv"
	"strings"
)

func Parse(reader io.Reader) (*Maze, error) {
	scanner := bufio.NewScanner(reader)

	sizeLine, ok := readNonEmptyLine(scanner)
	if !ok {
		return nil, fmt.Errorf("read maze size: %w", ErrInvalidMatrix)
	}

	sizeFields := strings.Fields(sizeLine)
	if len(sizeFields) != 2 {
		return nil, fmt.Errorf("read maze size: %w", ErrInvalidMatrix)
	}

	rows, err := strconv.Atoi(sizeFields[0])
	if err != nil {
		return nil, fmt.Errorf("parse maze rows: %w", err)
	}
	cols, err := strconv.Atoi(sizeFields[1])
	if err != nil {
		return nil, fmt.Errorf("parse maze cols: %w", err)
	}

	m, err := New(rows, cols)
	if err != nil {
		return nil, err
	}

	if err := readWallMatrix(scanner, rows, cols, m.SetRightWall); err != nil {
		return nil, fmt.Errorf("read right walls: %w", err)
	}
	if err := readWallMatrix(scanner, rows, cols, m.SetBottomWall); err != nil {
		return nil, fmt.Errorf("read bottom walls: %w", err)
	}
	if line, ok := readNonEmptyLine(scanner); ok {
		return nil, fmt.Errorf("unexpected maze data %q: %w", line, ErrInvalidMatrix)
	}
	if err := scanner.Err(); err != nil {
		return nil, fmt.Errorf("scan maze: %w", err)
	}

	return m, nil
}

func Load(path string) (*Maze, error) {
	file, err := os.Open(path)
	if err != nil {
		return nil, fmt.Errorf("open maze file: %w", err)
	}
	defer file.Close()

	return Parse(file)
}

func (m *Maze) Save(path string) error {
	file, err := os.OpenFile(path, os.O_WRONLY|os.O_CREATE|os.O_TRUNC, 0o644)
	if err != nil {
		return fmt.Errorf("create maze file: %w", err)
	}
	defer file.Close()

	if err := m.Write(file); err != nil {
		return fmt.Errorf("write maze file: %w", err)
	}
	return nil
}

func (m *Maze) Write(writer io.Writer) error {
	if m == nil {
		return ErrNilMaze
	}

	buffered := bufio.NewWriter(writer)
	if _, err := fmt.Fprintf(buffered, "%d %d\n", m.Rows, m.Cols); err != nil {
		return fmt.Errorf("write maze size: %w", err)
	}
	if err := writeWallMatrix(buffered, m, func(cell Cell) bool { return cell.RightWall }); err != nil {
		return fmt.Errorf("write right walls: %w", err)
	}
	if _, err := fmt.Fprintln(buffered); err != nil {
		return fmt.Errorf("write maze separator: %w", err)
	}
	if err := writeWallMatrix(buffered, m, func(cell Cell) bool { return cell.BottomWall }); err != nil {
		return fmt.Errorf("write bottom walls: %w", err)
	}
	if err := buffered.Flush(); err != nil {
		return fmt.Errorf("flush maze: %w", err)
	}
	return nil
}

func readWallMatrix(
	scanner *bufio.Scanner,
	rows int,
	cols int,
	setWall func(row int, col int, wall bool),
) error {
	for row := 0; row < rows; row++ {
		line, ok := readNonEmptyLine(scanner)
		if !ok {
			return fmt.Errorf("missing row %d: %w", row+1, ErrInvalidMatrix)
		}

		fields := strings.Fields(line)
		if len(fields) != cols {
			return fmt.Errorf("row %d width is %d, want %d: %w", row+1, len(fields), cols, ErrInvalidMatrix)
		}

		for col, field := range fields {
			value, err := parseBinary(field)
			if err != nil {
				return fmt.Errorf("row %d col %d: %w", row+1, col+1, err)
			}
			setWall(row, col, value)
		}
	}
	return nil
}

func writeWallMatrix(writer io.Writer, m *Maze, isWall func(Cell) bool) error {
	for row := 0; row < m.Rows; row++ {
		for col := 0; col < m.Cols; col++ {
			if col > 0 {
				if _, err := fmt.Fprint(writer, " "); err != nil {
					return err
				}
			}

			value := 0
			if isWall(m.Cell(row, col)) {
				value = 1
			}
			if _, err := fmt.Fprint(writer, value); err != nil {
				return err
			}
		}
		if _, err := fmt.Fprintln(writer); err != nil {
			return err
		}
	}
	return nil
}

func readNonEmptyLine(scanner *bufio.Scanner) (string, bool) {
	for scanner.Scan() {
		line := strings.TrimSpace(scanner.Text())
		if line != "" {
			return line, true
		}
	}
	return "", false
}

func parseBinary(token string) (bool, error) {
	switch token {
	case "0":
		return false, nil
	case "1":
		return true, nil
	default:
		return false, fmt.Errorf("expected 0 or 1, got %q: %w", token, ErrInvalidMatrix)
	}
}
