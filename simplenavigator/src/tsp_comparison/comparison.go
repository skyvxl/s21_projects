package tsp_comparison

import (
	"bufio"
	"errors"
	"fmt"
	"io"
	"strconv"
	"strings"
	"time"

	"simple_navigator/src/s21_graph"
	"simple_navigator/src/s21_graph_algorithms"
)

type tspSolver func(*s21_graph.Graph) (s21_graph_algorithms.TsmResult, error)

type tspTiming struct {
	name    string
	runs    int
	elapsed time.Duration
}

func measureTSP(graph *s21_graph.Graph, runs int, name string, solve tspSolver) (tspTiming, error) {
	if runs <= 0 {
		return tspTiming{}, errors.New("number of runs N must be a positive integer")
	}
	start := time.Now()
	for range runs {
		if _, err := solve(graph); err != nil {
			return tspTiming{}, fmt.Errorf("%s: %w", name, err)
		}
	}
	return tspTiming{name: name, runs: runs, elapsed: time.Since(start)}, nil
}

func Run(scanner *bufio.Scanner, algo *s21_graph_algorithms.GraphAlgorithms, graph *s21_graph.Graph, output io.Writer) error {
	if graph == nil || graph.VertexCount() == 0 {
		return errors.New("graph is not loaded, use option 1 first")
	}
	if graph.VertexCount() > s21_graph_algorithms.MaxBruteForceVertices {
		return fmt.Errorf("%w: maximum %d vertices", s21_graph_algorithms.ErrBruteForceTooLarge, s21_graph_algorithms.MaxBruteForceVertices)
	}
	if _, err := fmt.Fprintln(output, "Enter number of runs N (positive integer):"); err != nil {
		return err
	}
	if !scanner.Scan() {
		if err := scanner.Err(); err != nil {
			return err
		}
		return io.EOF
	}
	runs, err := strconv.Atoi(strings.TrimSpace(scanner.Text()))
	if err != nil || runs <= 0 {
		return errors.New("number of runs N must be a positive integer")
	}
	solvers := []struct {
		name  string
		solve tspSolver
	}{
		{"Ant colony", algo.SolveTravelingSalesmanProblem},
		{"Nearest neighbor", algo.SolveTravelingSalesmanProblemNearestNeighbor},
		{"Brute force", algo.SolveTravelingSalesmanProblemBruteForce},
	}
	timings := make([]tspTiming, 0, len(solvers))
	for _, solver := range solvers {
		timing, err := measureTSP(graph, runs, solver.name, solver.solve)
		if err != nil {
			return err
		}
		timings = append(timings, timing)
	}
	if _, err := fmt.Fprintf(output, "%-20s %8s %16s %16s\n", "Algorithm", "Runs", "Total", "Average"); err != nil {
		return err
	}
	for _, timing := range timings {
		average := timing.elapsed / time.Duration(timing.runs)
		if _, err := fmt.Fprintf(output, "%-20s %8d %16s %16s\n", timing.name, timing.runs, timing.elapsed, average); err != nil {
			return err
		}
	}
	return nil
}
