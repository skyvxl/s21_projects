package s21_graph_algorithms

import "simple_navigator/src/s21_graph"

func (a *GraphAlgorithms) GetLeastSpanningTree(graph *s21_graph.Graph) ([][]int, error) {
	if graph == nil || graph.VertexCount() == 0 {
		return nil, ErrNilGraph
	}
	if graph.IsDirected() {
		return nil, ErrDirectedGraph
	}
	n := graph.VertexCount()
	tree := make([][]int, n)
	for i := range tree {
		tree[i] = make([]int, n)
	}
	visited := make([]bool, n+1)
	visited[1] = true
	for i := 1; i < n; i++ {
		bestWeight := 0
		bestFrom := 0
		bestTo := 0
		for v := 1; v <= n; v++ {
			if !visited[v] {
				continue
			}
			neighbors, err := graph.Neighbors(v)
			if err != nil {
				return nil, err
			}
			for _, neighbor := range neighbors {
				if visited[neighbor.Vertex] {
					continue
				}
				if bestTo == 0 || neighbor.Weight < bestWeight {
					bestWeight = neighbor.Weight
					bestFrom = v
					bestTo = neighbor.Vertex
				}
			}
		}
		if bestTo == 0 {
			return nil, ErrNoSpanningTree
		}
		tree[bestFrom-1][bestTo-1] = bestWeight
		tree[bestTo-1][bestFrom-1] = bestWeight
		visited[bestTo] = true
	}
	return tree, nil
}
