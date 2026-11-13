package s21_graph_algorithms

import "math"

// Веса рёбер и представимые расстояния неотрицательны. Эти сигналы
// отличают отсутствующий путь от пути, чья длина не помещается в int.
const (
	unreachableDistance = -1
	overflowDistance    = -2
)

func addDistances(left, right int) int {
	if left == unreachableDistance || right == unreachableDistance {
		return unreachableDistance
	}
	if left == overflowDistance || right == overflowDistance || left > math.MaxInt-right {
		return overflowDistance
	}
	return left + right
}

func shorterDistance(candidate, current int) bool {
	if candidate == unreachableDistance {
		return false
	}
	if current == unreachableDistance {
		return true
	}
	if current == overflowDistance {
		return candidate != overflowDistance
	}
	return candidate != overflowDistance && candidate < current
}
