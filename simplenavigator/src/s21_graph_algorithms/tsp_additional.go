package s21_graph_algorithms

import (
	"fmt"
	"math/bits"

	"simple_navigator/src/s21_graph"
)

const MaxBruteForceVertices = 11

type tourCost struct {
	high uint64
	low  uint64
}

func (cost tourCost) add(distance int) tourCost {
	//nolint:gosec // G115: tspDistances rejects negative values before search.
	low, carry := bits.Add64(cost.low, uint64(distance), 0)
	return tourCost{high: cost.high + carry, low: low}
}

func (cost tourCost) less(other tourCost) bool {
	return cost.high < other.high || (cost.high == other.high && cost.low < other.low)
}

func (a *GraphAlgorithms) tspDistances(graph *s21_graph.Graph) ([][]int, error) {
	distances, err := a.GetShortestPathsBetweenAllVertices(graph)
	if err != nil {
		return nil, err
	}
	for _, row := range distances {
		for _, distance := range row {
			if distance < 0 {
				return nil, ErrNoTour
			}
		}
	}
	return distances, nil
}

func tspResultFromOrder(graph *s21_graph.Graph, distances [][]int, order []int) (TsmResult, error) {
	result := TsmResult{Vertices: []int{order[0] + 1}}
	if graph.VertexCount() == 1 {
		result.Vertices = append(result.Vertices, 1)
		return result, nil
	}
	for i := 1; i < len(order); i++ {
		leg := shortestSteps(graph, distances, order[i-1], order[i])
		if !leg.found {
			return TsmResult{}, ErrNoTour
		}
		result.Vertices = append(result.Vertices, leg.steps...)
		result.Distance += leg.distance
	}
	return result, nil
}

func (a *GraphAlgorithms) SolveTravelingSalesmanProblemNearestNeighbor(graph *s21_graph.Graph) (TsmResult, error) {
	if graph == nil || graph.VertexCount() == 0 {
		return TsmResult{}, ErrNilGraph
	}
	distances, err := a.tspDistances(graph)
	if err != nil {
		return TsmResult{}, err
	}
	n := graph.VertexCount()
	visited := make([]bool, n)
	visited[0] = true
	order := make([]int, 1, n+1)
	current := 0
	for len(order) < n {
		next := -1
		for candidate := range n {
			if !visited[candidate] && (next == -1 || distances[current][candidate] < distances[current][next]) {
				next = candidate
			}
		}
		visited[next] = true
		order = append(order, next)
		current = next
	}
	order = append(order, 0)
	return tspResultFromOrder(graph, distances, order)
}

func (a *GraphAlgorithms) SolveTravelingSalesmanProblemBruteForce(graph *s21_graph.Graph) (TsmResult, error) {
	if graph == nil || graph.VertexCount() == 0 {
		return TsmResult{}, ErrNilGraph
	}
	n := graph.VertexCount()
	if n > MaxBruteForceVertices {
		return TsmResult{}, fmt.Errorf("%w: %d vertices, maximum %d", ErrBruteForceTooLarge, n, MaxBruteForceVertices)
	}
	distances, err := a.tspDistances(graph)
	if err != nil {
		return TsmResult{}, err
	}
	visited := make([]bool, n)
	visited[0] = true
	order := make([]int, 1, n+1)
	var bestOrder []int
	var bestDistance tourCost
	var search func(current int, distance tourCost)
	search = func(current int, distance tourCost) {
		if len(order) == n {
			total := distance.add(distances[current][0])
			if bestOrder == nil || total.less(bestDistance) {
				bestDistance = total
				bestOrder = append([]int(nil), order...)
			}
			return
		}
		for next := 1; next < n; next++ {
			if visited[next] {
				continue
			}
			visited[next] = true
			order = append(order, next)
			search(next, distance.add(distances[current][next]))
			order = order[:len(order)-1]
			visited[next] = false
		}
	}
	search(0, tourCost{})
	bestOrder = append(bestOrder, 0)
	return tspResultFromOrder(graph, distances, bestOrder)
}
