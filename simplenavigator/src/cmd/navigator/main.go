package main

import (
	"bufio"
	"errors"
	"fmt"
	"io"
	"os"
	"path/filepath"
	"strconv"
	"strings"

	"simple_navigator/src/s21_graph"
	"simple_navigator/src/s21_graph_algorithms"
	"simple_navigator/src/tsp_comparison"
)

func main() {
	if err := run(os.Stdin, os.Stdout); err != nil {
		_, _ = fmt.Fprintln(os.Stderr, err)
		os.Exit(1)
	}
}

type checkedOutput struct {
	writer io.Writer
	err    error
}

func (w *checkedOutput) Write(data []byte) (int, error) {
	if w.err != nil {
		return 0, w.err
	}
	n, err := w.writer.Write(data)
	if err == nil && n != len(data) {
		err = io.ErrShortWrite
	}
	w.err = err
	return n, err
}

func run(input io.Reader, destination io.Writer) error {
	output := &checkedOutput{writer: destination}
	scanner := bufio.NewScanner(input)
	graph := s21_graph.NewGraph()
	algo := &s21_graph_algorithms.GraphAlgorithms{}
	for {
		const menu = "1. Load graph from file\n" +
			"2. Export graph to DOT\n" +
			"3. Breadth-first search\n" +
			"4. Depth-first search\n" +
			"5. Shortest path between two vertices\n" +
			"6. Shortest paths between all vertices\n" +
			"7. Minimum spanning tree\n" +
			"8. Traveling salesman problem\n" +
			"9. Compare TSP algorithms\n" +
			"0. Exit"
		if _, err := fmt.Fprintln(output, menu); err != nil {
			return err
		}

		if !scanner.Scan() {
			if err := scanner.Err(); err != nil {
				return fmt.Errorf("input error: %w", err)
			}
			return nil
		}

		num := strings.TrimSpace(scanner.Text())

		var err error
		switch num {
		case "1":
			err = handleLoad(scanner, graph, output)
		case "2":
			err = handleExport(graph, output)
		case "3", "4":
			err = handleDFSBFS(scanner, num, algo, graph, output)
		case "5":
			err = handleShortestPath(scanner, algo, graph, output)
		case "6":
			err = handleAllPairs(graph, algo, output)
		case "7":
			err = handleSpanningTree(graph, algo, output)
		case "8":
			err = handleTSP(graph, algo, output)
		case "9":
			err = tsp_comparison.Run(scanner, algo, graph, output)
		case "0":
			return nil
		default:
			err = errors.New("unknown option")
		}
		if output.err != nil {
			return output.err
		}
		if scanErr := scanner.Err(); scanErr != nil {
			return fmt.Errorf("input error: %w", scanErr)
		}
		if errors.Is(err, io.EOF) {
			return nil
		}
		if err != nil {
			if _, writeErr := fmt.Fprintln(output, err); writeErr != nil {
				return writeErr
			}
		}
	}
}

func printMatrix(output io.Writer, matr [][]int) error {
	if len(matr) == 0 {
		_, err := fmt.Fprintln(output, "(empty matrix)")
		return err
	}
	for _, row := range matr {
		for _, v := range row {
			if _, err := fmt.Fprintf(output, "%6d", v); err != nil {
				return err
			}
		}
		if _, err := fmt.Fprintln(output); err != nil {
			return err
		}
	}
	return nil
}

func graphLoaded(g *s21_graph.Graph) error {
	if g == nil || g.VertexCount() == 0 {
		return errors.New("graph is not loaded, use option 1 first")
	}
	return nil
}

func handleLoad(scanner *bufio.Scanner, graph *s21_graph.Graph, output io.Writer) error {
	if _, err := fmt.Fprint(output, "Path to graph file (Enter for data-samples/graph.txt): "); err != nil {
		return err
	}
	if !scanner.Scan() {
		if err := scanner.Err(); err != nil {
			return err
		}
		return io.EOF
	}
	path := strings.TrimSpace(scanner.Text())
	if path == "" {
		path = filepath.Join("data-samples", "graph.txt")
	}
	if err := graph.LoadGraphFromFile(path); err != nil {
		return fmt.Errorf("load error: %w", err)
	}
	_, err := fmt.Fprintln(output, "Graph loaded:", graph.VertexCount(), "vertices")
	return err
}

func handleExport(graph *s21_graph.Graph, output io.Writer) error {
	if err := graphLoaded(graph); err != nil {
		return err
	}
	if err := os.MkdirAll("build", 0o750); err != nil {
		return fmt.Errorf("create build dir: %w", err)
	}
	if err := graph.ExportGraphToDot("build/graph.dot"); err != nil {
		return fmt.Errorf("export graph: %w", err)
	}
	_, err := fmt.Fprintln(output, "Exported to build/graph.dot")
	return err
}

func handleDFSBFS(scanner *bufio.Scanner, num string, algo *s21_graph_algorithms.GraphAlgorithms, graph *s21_graph.Graph, output io.Writer) error {
	if err := graphLoaded(graph); err != nil {
		return err
	}
	if _, err := fmt.Fprintln(output, "Enter start vertex:"); err != nil {
		return err
	}
	if !scanner.Scan() {
		if err := scanner.Err(); err != nil {
			return err
		}
		return io.EOF
	}
	startVertex, err := strconv.Atoi(strings.TrimSpace(scanner.Text()))
	if err != nil {
		return fmt.Errorf("input error: %w", err)
	}
	var result []int
	if num == "3" {
		result, err = algo.BreadthFirstSearch(graph, startVertex)
	} else {
		result, err = algo.DepthFirstSearch(graph, startVertex)
	}
	if err != nil {
		return err
	}
	_, err = fmt.Fprintln(output, strings.Trim(fmt.Sprint(result), "[]"))
	return err
}

func handleShortestPath(scanner *bufio.Scanner, algo *s21_graph_algorithms.GraphAlgorithms, graph *s21_graph.Graph, output io.Writer) error {
	if err := graphLoaded(graph); err != nil {
		return err
	}
	if _, err := fmt.Fprintln(output, "Enter two vertices (e.g. 1 4):"); err != nil {
		return err
	}
	if !scanner.Scan() {
		if err := scanner.Err(); err != nil {
			return err
		}
		return io.EOF
	}
	vertex := strings.Fields(scanner.Text())
	if len(vertex) != 2 {
		return errors.New("enter exactly two vertices, e.g. '1 4'")
	}
	startVertex, err := strconv.Atoi(vertex[0])
	if err != nil {
		return fmt.Errorf("input error: %w", err)
	}
	endVertex, err := strconv.Atoi(vertex[1])
	if err != nil {
		return fmt.Errorf("input error: %w", err)
	}
	result, err := algo.GetShortestPathBetweenVertices(graph, startVertex, endVertex)
	if err != nil {
		return err
	}
	_, err = fmt.Fprintln(output, "Shortest distance:", result)
	return err
}

func handleAllPairs(graph *s21_graph.Graph, algo *s21_graph_algorithms.GraphAlgorithms, output io.Writer) error {
	if err := graphLoaded(graph); err != nil {
		return err
	}
	result, err := algo.GetShortestPathsBetweenAllVertices(graph)
	if err != nil {
		return fmt.Errorf("shortest paths error: %w", err)
	}
	return printMatrix(output, result)
}

func handleSpanningTree(graph *s21_graph.Graph, algo *s21_graph_algorithms.GraphAlgorithms, output io.Writer) error {
	if err := graphLoaded(graph); err != nil {
		return err
	}
	tree, err := algo.GetLeastSpanningTree(graph)
	if err != nil {
		return fmt.Errorf("MST error: %w", err)
	}
	return printMatrix(output, tree)
}

func handleTSP(graph *s21_graph.Graph, algo *s21_graph_algorithms.GraphAlgorithms, output io.Writer) error {
	if err := graphLoaded(graph); err != nil {
		return err
	}
	res, err := algo.SolveTravelingSalesmanProblem(graph)
	if err != nil {
		return fmt.Errorf("TSP error: %w", err)
	}
	if _, err := fmt.Fprintln(output, "Route:", strings.Trim(fmt.Sprint(res.Vertices), "[]")); err != nil {
		return err
	}
	_, err = fmt.Fprintf(output, "Distance: %g\n", res.Distance)
	return err
}
