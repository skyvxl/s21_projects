// Package web exposes a browser-based interface for the maze application:
// generating, loading, saving, and solving mazes, simulating caves, and
// training and running a Q-learning agent on the currently loaded maze.
package web

import (
	_ "embed"
	"encoding/json"
	"errors"
	"log"
	"math/rand"
	"maze/internal/cave"
	"maze/internal/eller"
	"maze/internal/maze"
	"maze/internal/qlearning"
	"maze/internal/solver"
	"net/http"
	"strings"
	"sync"
	"time"
)

//go:embed templates/index.html
var indexPage []byte

// App holds the in-memory state of the web interface: the current maze, its
// last solved route, the current cave, and the trained Q-learning agent.
type App struct {
	mu sync.Mutex

	maze  *maze.Maze
	cave  *cave.Cave
	agent *qlearning.Agent

	mux *http.ServeMux
}

// NewApp builds a web application with all routes registered.
func NewApp() *App {
	a := &App{mux: http.NewServeMux()}

	a.mux.HandleFunc("GET /{$}", a.handleIndex)

	a.mux.HandleFunc("POST /api/maze/generate", a.handleMazeGenerate)
	a.mux.HandleFunc("POST /api/maze/load", a.handleMazeLoad)
	a.mux.HandleFunc("POST /api/maze/save", a.handleMazeSave)
	a.mux.HandleFunc("POST /api/maze/solve", a.handleMazeSolve)

	a.mux.HandleFunc("POST /api/cave/generate", a.handleCaveGenerate)
	a.mux.HandleFunc("POST /api/cave/load", a.handleCaveLoad)
	a.mux.HandleFunc("POST /api/cave/save", a.handleCaveSave)
	a.mux.HandleFunc("POST /api/cave/simulate", a.handleCaveSimulate)

	a.mux.HandleFunc("POST /api/qlearning/train", a.handleQlearningTrain)
	a.mux.HandleFunc("POST /api/qlearning/path", a.handleQlearningPath)

	return a
}

// Handler returns the HTTP handler serving the web interface.
func (a *App) Handler() http.Handler {
	return a.mux
}

func (a *App) handleIndex(w http.ResponseWriter, r *http.Request) {
	w.Header().Set("Content-Type", "text/html; charset=utf-8")
	if _, err := w.Write(indexPage); err != nil {
		log.Printf("write index: %v", err)
	}
}

type apiResponse struct {
	Status int    `json:"status"`
	Error  string `json:"error,omitempty"`
}

type mazeResponse struct {
	apiResponse
	Rows            int      `json:"rows,omitempty"`
	Cols            int      `json:"cols,omitempty"`
	VerticalWalls   [][]int  `json:"vertical_walls,omitempty"`
	HorizontalWalls [][]int  `json:"horizontal_walls,omitempty"`
	Path            [][2]int `json:"path,omitempty"`
}

type caveResponse struct {
	apiResponse
	Rows  int     `json:"rows,omitempty"`
	Cols  int     `json:"cols,omitempty"`
	Cells [][]int `json:"cells,omitempty"`
	Alive int     `json:"alive"`
}

type qlearningResponse struct {
	apiResponse
	Path [][2]int `json:"path,omitempty"`
}

func writeJSON(w http.ResponseWriter, v any) {
	w.Header().Set("Content-Type", "application/json; charset=utf-8")
	if err := json.NewEncoder(w).Encode(v); err != nil {
		log.Printf("encode json: %v", err)
	}
}

func mazeError(w http.ResponseWriter, err error) {
	writeJSON(w, mazeResponse{apiResponse: apiResponse{Status: 1, Error: err.Error()}})
}

func caveError(w http.ResponseWriter, err error) {
	writeJSON(w, caveResponse{apiResponse: apiResponse{Status: 1, Error: err.Error()}})
}

func qlearningError(w http.ResponseWriter, err error) {
	writeJSON(w, qlearningResponse{apiResponse: apiResponse{Status: 1, Error: err.Error()}})
}

func (a *App) mazeResponseFor(m *maze.Maze, path []maze.Point) mazeResponse {
	vertical, horizontal := wallMatrices(m)
	return mazeResponse{
		apiResponse:     apiResponse{Status: 0},
		Rows:            m.Rows,
		Cols:            m.Cols,
		VerticalWalls:   vertical,
		HorizontalWalls: horizontal,
		Path:            pointsToPairs(path),
	}
}

func (a *App) caveResponseFor(c *cave.Cave) caveResponse {
	cells := make([][]int, c.Rows)
	alive := 0
	for row := 0; row < c.Rows; row++ {
		cells[row] = make([]int, c.Cols)
		for col := 0; col < c.Cols; col++ {
			if c.Alive(row, col) {
				cells[row][col] = 1
				alive++
			}
		}
	}
	return caveResponse{
		apiResponse: apiResponse{Status: 0},
		Rows:        c.Rows,
		Cols:        c.Cols,
		Cells:       cells,
		Alive:       boolToInt(alive > 0),
	}
}

func wallMatrices(m *maze.Maze) (vertical, horizontal [][]int) {
	vertical = make([][]int, m.Rows)
	horizontal = make([][]int, m.Rows)
	for row := 0; row < m.Rows; row++ {
		vertical[row] = make([]int, m.Cols)
		horizontal[row] = make([]int, m.Cols)
		for col := 0; col < m.Cols; col++ {
			cell := m.Cell(row, col)
			vertical[row][col] = boolToInt(cell.RightWall)
			horizontal[row][col] = boolToInt(cell.BottomWall)
		}
	}
	return vertical, horizontal
}

func pointsToPairs(path []maze.Point) [][2]int {
	if len(path) == 0 {
		return nil
	}
	pairs := make([][2]int, len(path))
	for i, p := range path {
		pairs[i] = [2]int{p.Row, p.Col}
	}
	return pairs
}

func boolToInt(b bool) int {
	if b {
		return 1
	}
	return 0
}

type mazeGenerateRequest struct {
	Rows int `json:"rows"`
	Cols int `json:"cols"`
}

func (a *App) handleMazeGenerate(w http.ResponseWriter, r *http.Request) {
	var req mazeGenerateRequest
	if err := json.NewDecoder(r.Body).Decode(&req); err != nil {
		mazeError(w, err)
		return
	}

	generated, err := eller.Generate(req.Rows, req.Cols, seededRand())
	if err != nil {
		mazeError(w, err)
		return
	}

	a.mu.Lock()
	a.maze = generated
	a.agent = nil
	a.mu.Unlock()

	writeJSON(w, a.mazeResponseFor(generated, nil))
}

type mazeLoadRequest struct {
	Content string `json:"content"`
}

func (a *App) handleMazeLoad(w http.ResponseWriter, r *http.Request) {
	var req mazeLoadRequest
	if err := json.NewDecoder(r.Body).Decode(&req); err != nil {
		mazeError(w, err)
		return
	}

	loaded, err := maze.Parse(strings.NewReader(req.Content))
	if err != nil {
		mazeError(w, err)
		return
	}

	a.mu.Lock()
	a.maze = loaded
	a.agent = nil
	a.mu.Unlock()

	writeJSON(w, a.mazeResponseFor(loaded, nil))
}

func (a *App) handleMazeSave(w http.ResponseWriter, r *http.Request) {
	m := a.currentMaze()
	if m == nil {
		mazeError(w, errors.New("there is no maze to save"))
		return
	}

	w.Header().Set("Content-Type", "text/plain; charset=utf-8")
	w.Header().Set("Content-Disposition", `attachment; filename="maze.txt"`)
	if err := m.Write(w); err != nil {
		log.Printf("write maze: %v", err)
	}
}

type mazeSolveRequest struct {
	StartRow int `json:"startRow"`
	StartCol int `json:"startCol"`
	EndRow   int `json:"endRow"`
	EndCol   int `json:"endCol"`
}

func (a *App) handleMazeSolve(w http.ResponseWriter, r *http.Request) {
	m := a.currentMaze()
	if m == nil {
		mazeError(w, errors.New("load or generate a maze first"))
		return
	}

	var req mazeSolveRequest
	if err := json.NewDecoder(r.Body).Decode(&req); err != nil {
		mazeError(w, err)
		return
	}

	start := maze.Point{Row: req.StartRow, Col: req.StartCol}
	end := maze.Point{Row: req.EndRow, Col: req.EndCol}

	path, err := solver.Solve(m, start, end)
	if err != nil {
		mazeError(w, err)
		return
	}

	writeJSON(w, a.mazeResponseFor(m, path))
}

type caveGenerateRequest struct {
	Rows int `json:"rows"`
	Cols int `json:"cols"`
	IC   int `json:"ic"`
}

func (a *App) handleCaveGenerate(w http.ResponseWriter, r *http.Request) {
	var req caveGenerateRequest
	if err := json.NewDecoder(r.Body).Decode(&req); err != nil {
		caveError(w, err)
		return
	}

	generated, err := cave.Generate(req.Rows, req.Cols, req.IC, seededRand())
	if err != nil {
		caveError(w, err)
		return
	}

	a.mu.Lock()
	a.cave = generated
	a.mu.Unlock()

	writeJSON(w, a.caveResponseFor(generated))
}

type caveLoadRequest struct {
	Content string `json:"content"`
}

func (a *App) handleCaveLoad(w http.ResponseWriter, r *http.Request) {
	var req caveLoadRequest
	if err := json.NewDecoder(r.Body).Decode(&req); err != nil {
		caveError(w, err)
		return
	}

	loaded, err := cave.Parse(strings.NewReader(req.Content))
	if err != nil {
		caveError(w, err)
		return
	}

	a.mu.Lock()
	a.cave = loaded
	a.mu.Unlock()

	writeJSON(w, a.caveResponseFor(loaded))
}

func (a *App) handleCaveSave(w http.ResponseWriter, r *http.Request) {
	a.mu.Lock()
	c := a.cave
	a.mu.Unlock()

	if c == nil {
		caveError(w, errors.New("there is no cave to save"))
		return
	}

	w.Header().Set("Content-Type", "text/plain; charset=utf-8")
	w.Header().Set("Content-Disposition", `attachment; filename="cave.txt"`)
	if err := c.Write(w); err != nil {
		log.Printf("write cave: %v", err)
	}
}

type caveSimulateRequest struct {
	BL int `json:"bl"`
	DL int `json:"dl"`
}

func (a *App) handleCaveSimulate(w http.ResponseWriter, r *http.Request) {
	a.mu.Lock()
	c := a.cave
	a.mu.Unlock()

	if c == nil {
		caveError(w, errors.New("generate or load a cave first"))
		return
	}

	var req caveSimulateRequest
	if err := json.NewDecoder(r.Body).Decode(&req); err != nil {
		caveError(w, err)
		return
	}

	next, err := c.Step(req.BL, req.DL)
	if err != nil {
		caveError(w, err)
		return
	}

	a.mu.Lock()
	a.cave = next
	a.mu.Unlock()

	resp := a.caveResponseFor(next)
	if cavesEqual(c, next) {
		resp.Alive = 0
	}
	writeJSON(w, resp)
}

func cavesEqual(a, b *cave.Cave) bool {
	if a.Rows != b.Rows || a.Cols != b.Cols {
		return false
	}
	for row := 0; row < a.Rows; row++ {
		for col := 0; col < a.Cols; col++ {
			if a.Alive(row, col) != b.Alive(row, col) {
				return false
			}
		}
	}
	return true
}

type qlearningTrainRequest struct {
	EndRow int `json:"endRow"`
	EndCol int `json:"endCol"`
}

func (a *App) handleQlearningTrain(w http.ResponseWriter, r *http.Request) {
	m := a.currentMaze()
	if m == nil {
		qlearningError(w, errors.New("load or generate a maze first"))
		return
	}

	var req qlearningTrainRequest
	if err := json.NewDecoder(r.Body).Decode(&req); err != nil {
		qlearningError(w, err)
		return
	}

	end := maze.Point{Row: req.EndRow, Col: req.EndCol}
	agent, err := qlearning.Train(m, end, qlearning.DefaultParams(), seededRand())
	if err != nil {
		qlearningError(w, err)
		return
	}

	a.mu.Lock()
	a.agent = agent
	a.mu.Unlock()

	writeJSON(w, qlearningResponse{apiResponse: apiResponse{Status: 0}})
}

type qlearningPathRequest struct {
	StartRow int `json:"startRow"`
	StartCol int `json:"startCol"`
}

func (a *App) handleQlearningPath(w http.ResponseWriter, r *http.Request) {
	a.mu.Lock()
	agent := a.agent
	a.mu.Unlock()

	if agent == nil {
		qlearningError(w, errors.New("train the agent first"))
		return
	}

	var req qlearningPathRequest
	if err := json.NewDecoder(r.Body).Decode(&req); err != nil {
		qlearningError(w, err)
		return
	}

	path, err := agent.Path(maze.Point{Row: req.StartRow, Col: req.StartCol})
	if err != nil {
		qlearningError(w, err)
		return
	}

	writeJSON(w, qlearningResponse{apiResponse: apiResponse{Status: 0}, Path: pointsToPairs(path)})
}

func (a *App) currentMaze() *maze.Maze {
	a.mu.Lock()
	defer a.mu.Unlock()
	return a.maze
}

func seededRand() *rand.Rand {
	return rand.New(rand.NewSource(time.Now().UnixNano()))
}
