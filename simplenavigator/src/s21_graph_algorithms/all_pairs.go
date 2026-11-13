package s21_graph_algorithms

import "simple_navigator/src/s21_graph"

// GetShortestPathsBetweenAllVertices запускает алгоритм Флойда-Уоршелла.
// В матрице результатов -1 означает, что путь между вершинами отсутствует.
// Возвращает nil и ErrNilGraph для пустого графа либо nil и ErrDistanceOverflow,
// если кратчайшее расстояние не может быть представлено в виде int.
func (a *GraphAlgorithms) GetShortestPathsBetweenAllVertices(graph *s21_graph.Graph) ([][]int, error) {
	if graph == nil || graph.VertexCount() == 0 {
		return nil, ErrNilGraph
	}
	n := graph.VertexCount()
	dist := make([][]int, n)
	for i := range dist {
		dist[i] = make([]int, n)
	}
	for i := range n {
		for j := range n {
			if i != j {
				// Обе вершины находятся в допустимом диапазоне графа.
				dist[i][j], _ = graph.Weight(i+1, j+1)
				if dist[i][j] == 0 {
					dist[i][j] = unreachableDistance
				}
			}
		}
	}
	for k := range n {
		for i := range n {
			for j := range n {
				candidate := addDistances(dist[i][k], dist[k][j])
				if shorterDistance(candidate, dist[i][j]) {
					dist[i][j] = candidate
				}
			}
		}
	}
	for i := range n {
		for j := range n {
			if dist[i][j] == overflowDistance {
				return nil, ErrDistanceOverflow
			}
		}
	}
	return dist, nil
}
