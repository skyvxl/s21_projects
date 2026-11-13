package tsp_comparison

import (
	"bufio"
	"bytes"
	"errors"
	"io"
	"os"
	"path/filepath"
	"strings"
	"testing"

	"simple_navigator/src/s21_graph"
	"simple_navigator/src/s21_graph_algorithms"
)

func TestMeasureTSP_RunCount(t *testing.T) {
	graph := s21_graph.NewGraph()
	calls := 0
	solver := func(received *s21_graph.Graph) (s21_graph_algorithms.TsmResult, error) {
		if received != graph {
			t.Fatal("solver received another graph")
		}
		calls++
		return s21_graph_algorithms.TsmResult{}, nil
	}
	timing, err := measureTSP(graph, 3, "test solver", solver)
	if err != nil || calls != 3 || timing.runs != 3 || timing.name != "test solver" || timing.elapsed < 0 {
		t.Fatalf("timing=%+v, calls=%d, err=%v", timing, calls, err)
	}
	for _, runs := range []int{0, -1} {
		calls = 0
		if _, err := measureTSP(graph, runs, "test solver", solver); err == nil || calls != 0 {
			t.Fatalf("invalid N=%d: calls=%d, err=%v", runs, calls, err)
		}
	}
}

func TestMeasureTSP_SolverError(t *testing.T) {
	want := errors.New("solver failed")
	calls := 0
	_, err := measureTSP(nil, 5, "broken solver", func(*s21_graph.Graph) (s21_graph_algorithms.TsmResult, error) {
		calls++
		return s21_graph_algorithms.TsmResult{}, want
	})
	if !errors.Is(err, want) || calls != 1 || !strings.Contains(err.Error(), "broken solver") {
		t.Fatalf("calls=%d, err=%v", calls, err)
	}
}

func TestRun_TSPComparison(t *testing.T) {
	path := filepath.Join(t.TempDir(), "graph.txt")
	if err := os.WriteFile(path, []byte("3\n0 1 0\n1 0 2\n0 2 0\n"), 0o600); err != nil {
		t.Fatal(err)
	}
	graph := s21_graph.NewGraph()
	if err := graph.LoadGraphFromFile(path); err != nil {
		t.Fatal(err)
	}
	var output bytes.Buffer
	algo := &s21_graph_algorithms.GraphAlgorithms{}
	if err := Run(bufio.NewScanner(strings.NewReader("2\n")), algo, graph, &output); err != nil {
		t.Fatal(err)
	}
	for _, expected := range []string{"Ant colony", "Nearest neighbor", "Brute force", "Total", "Average"} {
		if !strings.Contains(output.String(), expected) {
			t.Errorf("output missing %q:\n%s", expected, output.String())
		}
	}
	for _, line := range strings.Split(output.String(), "\n") {
		if strings.HasPrefix(line, "Ant colony") || strings.HasPrefix(line, "Nearest neighbor") || strings.HasPrefix(line, "Brute force") {
			fields := strings.Fields(line)
			if len(fields) != 5 || fields[2] != "2" {
				t.Errorf("wrong run count in row %q", line)
			}
		}
	}
}

type failingComparisonIO struct {
	err error
}

func (f failingComparisonIO) Read([]byte) (int, error) { return 0, f.err }

func (f failingComparisonIO) Write([]byte) (int, error) { return 0, f.err }

type comparisonLimitedWriter struct {
	writes int
	err    error
}

func (f *comparisonLimitedWriter) Write(data []byte) (int, error) {
	if f.writes > 0 {
		f.writes--
		return len(data), nil
	}
	return 0, f.err
}

func TestRun_IOErrors(t *testing.T) {
	graph, err := s21_graph.NewFromMatrix([][]int{{0}})
	if err != nil {
		t.Fatal(err)
	}
	algo := &s21_graph_algorithms.GraphAlgorithms{}
	want := errors.New("I/O failure")
	if err := Run(bufio.NewScanner(failingComparisonIO{want}), algo, graph, io.Discard); !errors.Is(err, want) {
		t.Fatalf("reader error=%v, want %v", err, want)
	}
	if err := Run(bufio.NewScanner(strings.NewReader("1\n")), algo, graph, failingComparisonIO{want}); !errors.Is(err, want) {
		t.Fatalf("writer error=%v, want %v", err, want)
	}
	writer := &comparisonLimitedWriter{writes: 1, err: want}
	if err := Run(bufio.NewScanner(strings.NewReader("1\n")), algo, graph, writer); !errors.Is(err, want) {
		t.Fatalf("timing table writer error=%v, want %v", err, want)
	}
}

func TestRun_Input(t *testing.T) {
	graph, err := s21_graph.NewFromMatrix([][]int{{0, 1}, {1, 0}})
	if err != nil {
		t.Fatal(err)
	}
	algo := &s21_graph_algorithms.GraphAlgorithms{}
	for _, input := range []string{"0\n", "-2\n", "abc\n", "\n", "99999999999999999999999999\n"} {
		var output bytes.Buffer
		err := Run(bufio.NewScanner(strings.NewReader(input)), algo, graph, &output)
		if err == nil || strings.Contains(output.String(), "Ant colony") {
			t.Fatalf("input %q: err=%v, output=%s", input, err, output.String())
		}
	}
	if err := Run(bufio.NewScanner(strings.NewReader("")), algo, graph, io.Discard); !errors.Is(err, io.EOF) {
		t.Fatalf("empty input: %v", err)
	}
	if err := Run(bufio.NewScanner(strings.NewReader("1\n")), algo, s21_graph.NewGraph(), io.Discard); err == nil {
		t.Fatal("unloaded graph accepted")
	}
	large := make([][]int, s21_graph_algorithms.MaxBruteForceVertices+1)
	for i := range large {
		large[i] = make([]int, len(large))
	}
	largeGraph, err := s21_graph.NewFromMatrix(large)
	if err != nil {
		t.Fatal(err)
	}
	if err := Run(bufio.NewScanner(strings.NewReader("1\n")), algo, largeGraph, io.Discard); !errors.Is(err, s21_graph_algorithms.ErrBruteForceTooLarge) {
		t.Fatalf("large graph: %v", err)
	}
}

func TestRun_SolverError(t *testing.T) {
	graph, err := s21_graph.NewFromMatrix([][]int{
		{0, 1, 0},
		{1, 0, 0},
		{0, 0, 0},
	})
	if err != nil {
		t.Fatal(err)
	}
	algo := &s21_graph_algorithms.GraphAlgorithms{}
	err = Run(bufio.NewScanner(strings.NewReader("1\n")), algo, graph, io.Discard)
	if !errors.Is(err, s21_graph_algorithms.ErrNoTour) {
		t.Fatalf("Run() error = %v, want ErrNoTour", err)
	}
}

func TestRun_TimingRowWriteError(t *testing.T) {
	graph, err := s21_graph.NewFromMatrix([][]int{{0, 1}, {1, 0}})
	if err != nil {
		t.Fatal(err)
	}
	algo := &s21_graph_algorithms.GraphAlgorithms{}
	want := errors.New("I/O failure")
	writer := &comparisonLimitedWriter{writes: 2, err: want}
	if err := Run(bufio.NewScanner(strings.NewReader("1\n")), algo, graph, writer); !errors.Is(err, want) {
		t.Fatalf("row write error = %v, want %v", err, want)
	}
}
