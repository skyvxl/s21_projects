package s21_graph_algorithms

import (
	"simple_navigator/src/s21_graph"
	"simple_navigator/src/s21_queue"
	"simple_navigator/src/s21_stack"
)

type GraphAlgorithms struct{}

func (a *GraphAlgorithms) DepthFirstSearch(graph *s21_graph.Graph, startVertex int) ([]int, error) {
	if graph == nil || graph.VertexCount() == 0 {
		return nil, ErrNilGraph
	}
	n := graph.VertexCount()
	if startVertex < 1 || startVertex > n {
		return nil, s21_graph.ErrInvalidVertex
	}
	visited := make([]bool, n+1)
	order := make([]int, 0, n)

	stack := s21_stack.NewStack()
	stack.Push(startVertex)
	for !stack.IsEmpty() {
		vertex, _ := stack.Pop()
		if visited[vertex] {
			continue
		}
		visited[vertex] = true
		order = append(order, vertex)

		neighbors, err := graph.Neighbors(vertex)
		if err != nil {
			return nil, err
		}
		for i := len(neighbors) - 1; i >= 0; i-- {
			if !visited[neighbors[i].Vertex] {
				stack.Push(neighbors[i].Vertex)
			}
		}
	}
	return order, nil
}

func (a *GraphAlgorithms) BreadthFirstSearch(graph *s21_graph.Graph, startVertex int) ([]int, error) {
	if graph == nil || graph.VertexCount() == 0 {
		return nil, ErrNilGraph
	}
	n := graph.VertexCount()
	if startVertex < 1 || startVertex > n {
		return nil, s21_graph.ErrInvalidVertex
	}
	visited := make([]bool, n+1)
	order := make([]int, 0, n)

	queue := s21_queue.NewQueue()
	visited[startVertex] = true
	queue.Push(startVertex)

	for !queue.IsEmpty() {
		vertex, _ := queue.Pop()
		order = append(order, vertex)

		neighbors, err := graph.Neighbors(vertex)
		if err != nil {
			return nil, err
		}
		for _, neighbor := range neighbors {
			if !visited[neighbor.Vertex] {
				visited[neighbor.Vertex] = true
				queue.Push(neighbor.Vertex)
			}
		}
	}
	return order, nil
}
