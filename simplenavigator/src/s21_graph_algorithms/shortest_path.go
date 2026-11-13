package s21_graph_algorithms

import "simple_navigator/src/s21_graph"

func (a *GraphAlgorithms) GetShortestPathBetweenVertices(graph *s21_graph.Graph, vertex1, vertex2 int) (int, error) {
	if graph == nil || graph.VertexCount() == 0 {
		return 0, ErrNilGraph
	}
	n := graph.VertexCount()
	if vertex1 < 1 || vertex1 > n || vertex2 < 1 || vertex2 > n {
		return 0, s21_graph.ErrInvalidVertex
	}
	if vertex1 == vertex2 {
		return 0, nil
	}
	dist := make([]int, n+1)
	for i := 1; i <= n; i++ {
		if i == vertex1 {
			dist[i] = 0
		} else {
			dist[i] = unreachableDistance
		}
	}
	visited := make([]bool, n+1)
	for range n {
		current := -1
		for v := 1; v <= n; v++ {
			if !visited[v] && dist[v] != unreachableDistance && (current == -1 || shorterDistance(dist[v], dist[current])) {
				current = v
			}
		}
		if current == -1 {
			break
		}
		visited[current] = true
		if current == vertex2 {
			break
		}
		neighbors, err := graph.Neighbors(current)
		if err != nil {
			return 0, err
		}
		for _, neighbor := range neighbors {
			if !visited[neighbor.Vertex] {
				candidate := addDistances(dist[current], neighbor.Weight)
				if shorterDistance(candidate, dist[neighbor.Vertex]) {
					dist[neighbor.Vertex] = candidate
				}
			}
		}
	}
	if dist[vertex2] == unreachableDistance {
		return 0, ErrNoPath
	}
	if dist[vertex2] == overflowDistance {
		return 0, ErrDistanceOverflow
	}
	return dist[vertex2], nil
}
