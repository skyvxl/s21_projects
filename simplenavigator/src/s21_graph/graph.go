package s21_graph

import (
	"bufio"
	"errors"
	"fmt"
	"io"
	"math"
	"os"
	"strings"
	"unicode"
)

type Neighbor struct {
	Vertex int
	Weight int
}

type Graph struct {
	matrix [][]int
}

// NewGraph создаёт пустой граф без вершин и рёбер.
// Матрица смежности появится после вызова LoadGraphFromFile или NewFromMatrix.
func NewGraph() *Graph {
	return &Graph{}
}

// LoadGraphFromFile инициализирует граф из его матричного представления в файле.
// Первая строка файла должна содержать число вершин N, а следующие N строк —
// квадратную матрицу смежности размером N x N.
// Возвращает ошибку, если файл не найден или его содержимое имеет неверный формат.
func (g *Graph) LoadGraphFromFile(filename string) error {
	// #nosec G304 -- filename is the caller-selected graph input path.
	file, err := os.Open(filename)
	if err != nil {
		return fmt.Errorf("open graph file %q: %w", filename, err)
	}
	defer func() {
		_ = file.Close()
	}()
	reader := bufio.NewReader(file)
	var n int
	_, err = fmt.Fscan(reader, &n)
	if err != nil {
		return fmt.Errorf("read graph size: %w (%v)", ErrInvalidMatrix, err)
	}
	if n < 0 {
		return fmt.Errorf("graph size %d: %w", n, ErrInvalidMatrix)
	}
	if n == 0 {
		return fmt.Errorf("graph size is zero: %w", ErrEmptyGraph)
	}
	if n > math.MaxInt/n {
		return fmt.Errorf("graph size %d is too large: %w", n, ErrInvalidMatrix)
	}
	var matrix [][]int
	for i := 0; i < n; i++ {
		var row []int
		for j := 0; j < n; j++ {
			var gn int
			_, err := fmt.Fscan(reader, &gn)
			if err != nil {
				return fmt.Errorf("read matrix value at row %d, column %d: %w (%v)", i+1, j+1, ErrInvalidMatrix, err)
			}
			if gn < 0 {
				return fmt.Errorf("matrix value at row %d, column %d is %d: %w", i+1, j+1, gn, ErrNegativeWeight)
			}
			row = append(row, gn)
		}
		matrix = append(matrix, row)
	}
	for {
		r, _, err := reader.ReadRune()
		if errors.Is(err, io.EOF) {
			break
		}
		if err != nil {
			return fmt.Errorf("read trailing data: %w (%v)", ErrInvalidMatrix, err)
		}
		if !unicode.IsSpace(r) {
			return fmt.Errorf("unexpected data after adjacency matrix: %w", ErrInvalidMatrix)
		}
	}
	if err := file.Close(); err != nil {
		return fmt.Errorf("close graph file %q: %w", filename, err)
	}
	g.matrix = matrix
	return nil
}

// ExportGraphToDot записывает текущий граф в файл формата DOT.
// Для ориентированного графа используются digraph и стрелки ->, для
// неориентированного — graph и рёбра --. Вес ребра записывается как label.
// Возвращает ошибку, если граф пуст, имя файла некорректно или запись не удалась.
func (g *Graph) ExportGraphToDot(filename string) (err error) {
	if len(g.matrix) == 0 {
		return ErrEmptyGraph
	}
	// #nosec G304 -- filename is the caller-selected graph output path.
	file, err := os.Create(filename)
	if err != nil {
		return fmt.Errorf("create graph file %q: %w", filename, err)
	}
	defer func() {
		if closeErr := file.Close(); closeErr != nil && err == nil {
			err = fmt.Errorf("close graph file %q: %w", filename, closeErr)
		}
	}()
	var builder strings.Builder
	graphType := "graph"
	edgeOperator := "--"
	directed := g.IsDirected()
	if directed {
		graphType = "digraph"
		edgeOperator = "->"
	}
	fmt.Fprintf(&builder, "%s G {\n", graphType)
	for v := 1; v <= len(g.matrix); v++ {
		fmt.Fprintf(&builder, "    %d;\n", v)
	}
	for i := 0; i < len(g.matrix); i++ {
		j := i
		if directed {
			j = 0
		}
		for ; j < len(g.matrix); j++ {
			weight := g.matrix[i][j]

			if weight == 0 {
				continue
			}
			fmt.Fprintf(&builder, "    %d %s %d [label=\"%d\"];\n", i+1, edgeOperator, j+1, weight)
		}
	}
	builder.WriteString("}\n")
	_, err = file.WriteString(builder.String())
	if err != nil {
		return fmt.Errorf("write graph to file %q: %w", filename, err)
	}
	return nil
}

// VertexCount возвращает количество вершин в графе.
// Для пустого или неинициализированного графа возвращает 0.
func (g *Graph) VertexCount() int {
	if len(g.matrix) == 0 {
		return 0
	}
	return len(g.matrix)
}

// Neighbors возвращает все вершины, в которые существует ребро из vertex,
// вместе с весом каждого ребра. Номера вершин во внешнем API начинаются с 1,
// а соседи возвращаются по возрастанию номера.
// Возвращает ошибку, если граф пуст или vertex находится вне диапазона 1..N.
func (g *Graph) Neighbors(vertex int) ([]Neighbor, error) {
	if len(g.matrix) == 0 {
		return nil, ErrEmptyGraph
	}
	if vertex < 1 || vertex > len(g.matrix) {
		return nil, fmt.Errorf("vertex %d: %w", vertex, ErrInvalidVertex)
	}
	var neighbors []Neighbor
	for j, weight := range g.matrix[vertex-1] {
		if weight > 0 {
			neighbors = append(neighbors, Neighbor{
				Vertex: j + 1,
				Weight: weight,
			})
		}
	}
	return neighbors, nil
}

// Weight возвращает вес ребра из вершины from в вершину to.
// Номера вершин начинаются с 1. Если ребра нет, возвращает 0 без ошибки.
// Возвращает ошибку, если граф пуст или хотя бы одна вершина вне диапазона 1..N.
func (g *Graph) Weight(from, to int) (int, error) {
	if len(g.matrix) == 0 {
		return 0, ErrEmptyGraph
	}
	if from < 1 || from > len(g.matrix) || to < 1 || to > len(g.matrix) {
		return 0, fmt.Errorf("vertex %d or %d: %w", from, to, ErrInvalidVertex)
	}
	return g.matrix[from-1][to-1], nil
}

// IsDirected определяет направленность графа по матрице смежности.
// Если матрица несимметрична, граф считается ориентированным.
// Для пустого графа возвращает false.
func (g *Graph) IsDirected() bool {
	if len(g.matrix) == 0 {
		return false
	}
	for i := 0; i < len(g.matrix); i++ {
		for j := 0; j < len(g.matrix[i]); j++ {
			if g.matrix[i][j] != g.matrix[j][i] {
				return true
			}
		}
	}
	return false
}

// NewFromMatrix создаёт граф из готовой матрицы смежности.
// Матрица должна быть непустой, квадратной и не содержать отрицательных весов.
// Функция делает глубокую копию матрицы, поэтому последующее изменение
// исходного слайса не должно изменять созданный граф.
// Возвращает ошибку, если матрица не удовлетворяет этим условиям.
func NewFromMatrix(matrix [][]int) (*Graph, error) {
	if len(matrix) == 0 {
		return nil, fmt.Errorf("new matrix: %w", ErrInvalidMatrix)
	}
	size := len(matrix)
	for i, row := range matrix {
		if len(row) != size {
			return nil, fmt.Errorf("matrix row %d has length %d, want %d: %w", i+1, len(row), size, ErrInvalidMatrix)
		}
		for j, weight := range row {
			if weight < 0 {
				return nil, fmt.Errorf("matrix value at row %d, column %d is %d: %w", i+1, j+1, weight, ErrNegativeWeight)
			}
		}
	}
	dst := make([][]int, size)
	for v := range matrix {
		dst[v] = make([]int, size)
		copy(dst[v], matrix[v])
	}
	return &Graph{
		matrix: dst,
	}, nil
}
