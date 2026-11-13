package s21_graph_algorithms

import (
	"math/rand/v2"

	"simple_navigator/src/s21_graph"
)

type TsmResult struct {
	Vertices []int
	Distance float64
}

type antTour struct {
	vertices []int
	distance float64
	legs     [][2]int
}

type shortestStepsResult struct {
	steps    []int
	distance float64
	found    bool
}

func chooseTarget(current int, visited []bool, distances [][]int, pheromone [][]float64, rng *rand.Rand) int {
	var candidates []int
	var weights []float64
	total := 0.0
	for target := range visited {
		if visited[target] || distances[current][target] <= 0 {
			continue
		}
		distance := float64(distances[current][target])
		weight := pheromone[current][target] / (distance * distance)
		candidates = append(candidates, target)
		weights = append(weights, weight)
		total += weight
	}
	if len(candidates) == 0 || total == 0 {
		return -1
	}
	//nolint:gosec // G404: math/rand is safe
	draw := rng.Float64() * total
	for i, weight := range weights {
		if draw < weight {
			return candidates[i]
		}
		draw -= weight
	}
	return candidates[len(candidates)-1]
}

func shortestSteps(graph *s21_graph.Graph, distances [][]int, from, to int) shortestStepsResult {
	if distances[from][to] < 0 {
		return shortestStepsResult{
			nil,
			0,
			false,
		}
	}
	current := from
	var steps []int
	length := 0.0
	for current != to {
		remaining := distances[current][to]
		neighbors, err := graph.Neighbors(current + 1)
		if err != nil {
			return shortestStepsResult{
				nil,
				0,
				false,
			}
		}
		found := false
		for _, neighbor := range neighbors {
			next := neighbor.Vertex - 1
			rest := distances[next][to]
			if rest >= 0 &&
				neighbor.Weight <= remaining &&
				rest == remaining-neighbor.Weight {
				steps = append(steps, neighbor.Vertex)
				length += float64(neighbor.Weight)
				current = next
				found = true
				break
			}
		}
		if !found {
			return shortestStepsResult{
				nil,
				0,
				false,
			}
		}
	}
	return shortestStepsResult{
		steps:    steps,
		distance: length,
		found:    true,
	}
}

func buildAntTour(graph *s21_graph.Graph, distances [][]int, pheromone [][]float64, start int, rng *rand.Rand) (antTour, bool) {
	n := graph.VertexCount()
	visited := make([]bool, n)
	visited[start] = true
	visitedCount := 1
	current := start
	vertices := []int{start + 1}
	legs := make([][2]int, 0, n)
	length := 0.0
	for visitedCount < n {
		target := chooseTarget(current, visited, distances, pheromone, rng)
		if target == -1 {
			return antTour{}, false
		}
		shortStepResult := shortestSteps(graph, distances, current, target)
		steps, stepLength, ok := shortStepResult.steps, shortStepResult.distance, shortStepResult.found
		if !ok {
			return antTour{}, false
		}
		for _, vertex := range steps {
			vertices = append(vertices, vertex)

			if !visited[vertex-1] {
				visited[vertex-1] = true
				visitedCount++
			}
		}
		legs = append(legs, [2]int{current, target})
		length += stepLength
		current = target
	}
	shortStepResult := shortestSteps(graph, distances, current, start)
	steps, stepLength, ok := shortStepResult.steps, shortStepResult.distance, shortStepResult.found
	if !ok {
		return antTour{}, false
	}
	vertices = append(vertices, steps...)
	legs = append(legs, [2]int{current, start})
	length += stepLength
	return antTour{
		vertices: vertices,
		distance: length,
		legs:     legs,
	}, true
}

func (a *GraphAlgorithms) SolveTravelingSalesmanProblem(graph *s21_graph.Graph) (TsmResult, error) {
	//nolint:gosec // Route selection does not require cryptographic randomness.
	rng := rand.New(rand.NewPCG(rand.Uint64(), rand.Uint64()))
	return a.solveTravelingSalesmanProblem(graph, rng)
}

func (a *GraphAlgorithms) solveTravelingSalesmanProblem(graph *s21_graph.Graph, rng *rand.Rand) (TsmResult, error) {
	if graph == nil || graph.VertexCount() == 0 {
		return TsmResult{}, ErrNilGraph
	}
	n := graph.VertexCount()
	if n == 1 {
		return TsmResult{
			Vertices: []int{1, 1},
			Distance: 0,
		}, nil
	}
	distances, err := a.GetShortestPathsBetweenAllVertices(graph)
	if err != nil {
		return TsmResult{}, err
	}
	pheromone := make([][]float64, n)
	for i := range pheromone {
		pheromone[i] = make([]float64, n)
		for j := range pheromone[i] {
			pheromone[i][j] = 1
		}
	}
	const iterations = 100
	const evaporation = 0.5
	const q = 100.0
	var best TsmResult
	found := false
	for range iterations {
		tours := make([]antTour, 0, n)
		for start := range n {
			tour, ok := buildAntTour(graph, distances, pheromone, start, rng)
			if !ok {
				continue
			}
			tours = append(tours, tour)
			if !found || tour.distance < best.Distance {
				best = TsmResult{
					Vertices: append([]int(nil), tour.vertices...),
					Distance: tour.distance,
				}
				found = true
			}
		}
		for i := range pheromone {
			for j := range pheromone[i] {
				pheromone[i][j] *= 1 - evaporation
				if pheromone[i][j] < 1e-6 {
					pheromone[i][j] = 1e-6
				}
			}
		}
		for _, tour := range tours {
			amount := q / tour.distance
			for _, leg := range tour.legs {
				pheromone[leg[0]][leg[1]] += amount
			}
		}
	}
	if !found {
		return TsmResult{}, ErrNoTour
	}
	return best, nil
}
