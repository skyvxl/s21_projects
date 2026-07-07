package cave

import (
	"bufio"
	"fmt"
	"io"
	"os"
	"strconv"
	"strings"
)

func Parse(reader io.Reader) (*Cave, error) {
	scanner := bufio.NewScanner(reader)

	sizeLine, ok := readNonEmptyLine(scanner)
	if !ok {
		return nil, fmt.Errorf("read cave size: %w", ErrInvalidMatrix)
	}

	sizeFields := strings.Fields(sizeLine)
	if len(sizeFields) != 2 {
		return nil, fmt.Errorf("read cave size: %w", ErrInvalidMatrix)
	}

	rows, err := strconv.Atoi(sizeFields[0])
	if err != nil {
		return nil, fmt.Errorf("parse cave rows: %w", err)
	}
	cols, err := strconv.Atoi(sizeFields[1])
	if err != nil {
		return nil, fmt.Errorf("parse cave cols: %w", err)
	}

	c, err := New(rows, cols)
	if err != nil {
		return nil, err
	}

	for row := 0; row < rows; row++ {
		line, ok := readNonEmptyLine(scanner)
		if !ok {
			return nil, fmt.Errorf("missing row %d: %w", row+1, ErrInvalidMatrix)
		}

		fields := strings.Fields(line)
		if len(fields) != cols {
			return nil, fmt.Errorf("row %d width is %d, want %d: %w", row+1, len(fields), cols, ErrInvalidMatrix)
		}

		for col, field := range fields {
			value, err := parseBinary(field)
			if err != nil {
				return nil, fmt.Errorf("row %d col %d: %w", row+1, col+1, err)
			}
			c.Set(row, col, value)
		}
	}

	if line, ok := readNonEmptyLine(scanner); ok {
		return nil, fmt.Errorf("unexpected cave data %q: %w", line, ErrInvalidMatrix)
	}
	if err := scanner.Err(); err != nil {
		return nil, fmt.Errorf("scan cave: %w", err)
	}

	return c, nil
}

func Load(path string) (*Cave, error) {
	file, err := os.Open(path)
	if err != nil {
		return nil, fmt.Errorf("open cave file: %w", err)
	}
	defer file.Close()

	return Parse(file)
}

func (c *Cave) Save(path string) error {
	file, err := os.OpenFile(path, os.O_WRONLY|os.O_CREATE|os.O_TRUNC, 0o644)
	if err != nil {
		return fmt.Errorf("create cave file: %w", err)
	}
	defer file.Close()

	if err := c.Write(file); err != nil {
		return fmt.Errorf("write cave file: %w", err)
	}
	return nil
}

func (c *Cave) Write(writer io.Writer) error {
	if c == nil {
		return ErrNilCave
	}

	buffered := bufio.NewWriter(writer)
	if _, err := fmt.Fprintf(buffered, "%d %d\n", c.Rows, c.Cols); err != nil {
		return fmt.Errorf("write cave size: %w", err)
	}

	for row := 0; row < c.Rows; row++ {
		for col := 0; col < c.Cols; col++ {
			if col > 0 {
				if _, err := fmt.Fprint(buffered, " "); err != nil {
					return fmt.Errorf("write cave separator: %w", err)
				}
			}

			value := 0
			if c.Alive(row, col) {
				value = 1
			}
			if _, err := fmt.Fprint(buffered, value); err != nil {
				return fmt.Errorf("write cave cell: %w", err)
			}
		}
		if _, err := fmt.Fprintln(buffered); err != nil {
			return fmt.Errorf("write cave row: %w", err)
		}
	}

	if err := buffered.Flush(); err != nil {
		return fmt.Errorf("flush cave: %w", err)
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
