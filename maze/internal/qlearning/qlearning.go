// Package qlearning trains a Q-learning agent to find the shortest route
// through a fixed maze to a fixed end point, then lets it be queried for a
// route from any start point.
package qlearning

import (
	"errors"
	"fmt"
	"math/rand"
	"maze/internal/maze"
	"time"
)

const actionCount = 4

var actionDeltas = [actionCount]maze.Point{
	{Row: 0, Col: 1},
	{Row: 1, Col: 0},
	{Row: 0, Col: -1},
	{Row: -1, Col: 0},
}

var ErrInvalidParams = errors.New("qlearning params are invalid")

// Params controls the Q-learning training process.
type Params struct {
	Episodes     int
	MaxSteps     int
	Alpha        float64
	Gamma        float64
	Epsilon      float64
	EpsilonDecay float64
	MinEpsilon   float64
	StepPenalty  float64
	GoalReward   float64
}

// DefaultParams returns hyperparameters that reliably solve mazes up to
// a few hundred cells within a fraction of a second.
func DefaultParams() Params {
	return Params{
		Episodes:     4000,
		MaxSteps:     500,
		Alpha:        0.5,
		Gamma:        0.9,
		Epsilon:      1,
		EpsilonDecay: 0.995,
		MinEpsilon:   0.05,
		StepPenalty:  -1,
		GoalReward:   100,
	}
}

// Agent holds the Q-table learned for a single maze and end point.
type Agent struct {
	m   *maze.Maze
	end maze.Point
	q   [][actionCount]float64
}

// Train runs tabular Q-learning on m for the fixed end point and returns the
// resulting agent. The maze and end point never change during training.
func Train(m *maze.Maze, end maze.Point, params Params, rng *rand.Rand) (*Agent, error) {
	if m == nil {
		return nil, maze.ErrNilMaze
	}
	if !m.InBounds(end) {
		return nil, fmt.Errorf("end point: %w", maze.ErrInvalidPoint)
	}
	if params.Episodes <= 0 || params.MaxSteps <= 0 {
		return nil, ErrInvalidParams
	}
	if rng == nil {
		rng = rand.New(rand.NewSource(time.Now().UnixNano()))
	}

	q := make([][actionCount]float64, m.Rows*m.Cols)
	epsilon := params.Epsilon

	for episode := 0; episode < params.Episodes; episode++ {
		runEpisode(m, end, q, params, epsilon, rng)

		epsilon *= params.EpsilonDecay
		if epsilon < params.MinEpsilon {
			epsilon = params.MinEpsilon
		}
	}

	return &Agent{m: m, end: end, q: q}, nil
}

func runEpisode(m *maze.Maze, end maze.Point, q [][actionCount]float64, params Params, epsilon float64, rng *rand.Rand) {
	current := randomStart(m, end, rng)

	for step := 0; step < params.MaxSteps && current != end; step++ {
		actions := validActions(m, current)
		if len(actions) == 0 {
			return
		}
		action := chooseAction(q, m, current, actions, epsilon, rng)
		next := move(current, action)

		done := next == end
		reward := params.StepPenalty
		if done {
			reward = params.GoalReward
		}

		best := 0.0
		if !done {
			best = maxQ(q, m, next)
		}

		idx := stateIndex(m, current)
		q[idx][action] += params.Alpha * (reward + params.Gamma*best - q[idx][action])

		current = next
	}
}

// Path returns the route the trained agent follows from start to the maze's
// end point, greedily picking the highest-valued legal move at each cell.
func (a *Agent) Path(start maze.Point) ([]maze.Point, error) {
	if !a.m.InBounds(start) {
		return nil, fmt.Errorf("start point: %w", maze.ErrInvalidPoint)
	}

	maxSteps := a.m.Rows*a.m.Cols + 1
	path := []maze.Point{start}
	visited := map[maze.Point]bool{start: true}

	for current := start; current != a.end; {
		if len(path) > maxSteps {
			return nil, maze.ErrNoPath
		}

		actions := validActions(a.m, current)
		if len(actions) == 0 {
			return nil, maze.ErrNoPath
		}

		next := move(current, bestAction(a.q, a.m, current, actions))
		if visited[next] {
			return nil, maze.ErrNoPath
		}

		visited[next] = true
		path = append(path, next)
		current = next
	}

	return path, nil
}

// End returns the fixed end point the agent was trained for.
func (a *Agent) End() maze.Point {
	return a.end
}

func randomStart(m *maze.Maze, end maze.Point, rng *rand.Rand) maze.Point {
	if m.Rows*m.Cols <= 1 {
		return end
	}
	for {
		p := maze.Point{Row: rng.Intn(m.Rows), Col: rng.Intn(m.Cols)}
		if p != end {
			return p
		}
	}
}

func chooseAction(q [][actionCount]float64, m *maze.Maze, p maze.Point, actions []int, epsilon float64, rng *rand.Rand) int {
	if rng.Float64() < epsilon {
		return actions[rng.Intn(len(actions))]
	}
	return bestAction(q, m, p, actions)
}

func bestAction(q [][actionCount]float64, m *maze.Maze, p maze.Point, actions []int) int {
	idx := stateIndex(m, p)
	best := actions[0]
	for _, action := range actions[1:] {
		if q[idx][action] > q[idx][best] {
			best = action
		}
	}
	return best
}

func maxQ(q [][actionCount]float64, m *maze.Maze, p maze.Point) float64 {
	actions := validActions(m, p)
	if len(actions) == 0 {
		return 0
	}
	idx := stateIndex(m, p)
	best := q[idx][actions[0]]
	for _, action := range actions[1:] {
		if q[idx][action] > best {
			best = q[idx][action]
		}
	}
	return best
}

func validActions(m *maze.Maze, p maze.Point) []int {
	actions := make([]int, 0, actionCount)
	for action := 0; action < actionCount; action++ {
		if m.CanMove(p, move(p, action)) {
			actions = append(actions, action)
		}
	}
	return actions
}

func move(p maze.Point, action int) maze.Point {
	delta := actionDeltas[action]
	return maze.Point{Row: p.Row + delta.Row, Col: p.Col + delta.Col}
}

func stateIndex(m *maze.Maze, p maze.Point) int {
	return p.Row*m.Cols + p.Col
}
