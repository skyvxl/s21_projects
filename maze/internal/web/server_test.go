package web_test

import (
	"bytes"
	"encoding/json"
	"io"
	"maze/internal/web"
	"net/http"
	"net/http/httptest"
	"strings"
	"testing"
)

const sampleMaze = `2 3
0 1 1
0 0 1

1 0 1
1 1 1
`

const sampleCave = `2 3
0 1 1
0 0 1
`

func newTestServer(t *testing.T) (*httptest.Server, *http.Client) {
	t.Helper()
	server := httptest.NewServer(web.NewApp().Handler())
	t.Cleanup(server.Close)
	return server, server.Client()
}

func postJSON(t *testing.T, client *http.Client, target string, body any) map[string]any {
	t.Helper()

	payload, err := json.Marshal(body)
	if err != nil {
		t.Fatalf("marshal body: %v", err)
	}

	resp, err := client.Post(target, "application/json", bytes.NewReader(payload))
	if err != nil {
		t.Fatalf("post %s: %v", target, err)
	}
	defer resp.Body.Close()

	if resp.StatusCode != http.StatusOK {
		t.Fatalf("post %s status = %d, want %d", target, resp.StatusCode, http.StatusOK)
	}

	var decoded map[string]any
	if err := json.NewDecoder(resp.Body).Decode(&decoded); err != nil {
		t.Fatalf("decode %s response: %v", target, err)
	}
	return decoded
}

func readBody(t *testing.T, resp *http.Response) string {
	t.Helper()
	defer resp.Body.Close()
	body, err := io.ReadAll(resp.Body)
	if err != nil {
		t.Fatalf("read body: %v", err)
	}
	return string(body)
}

func TestIndexServesPage(t *testing.T) {
	t.Parallel()

	server, client := newTestServer(t)
	resp, err := client.Get(server.URL + "/")
	if err != nil {
		t.Fatalf("get /: %v", err)
	}
	body := readBody(t, resp)

	if resp.StatusCode != http.StatusOK {
		t.Fatalf("status = %d, want %d", resp.StatusCode, http.StatusOK)
	}
	if !strings.Contains(body, "<title>Maze / Cave</title>") {
		t.Fatalf("body missing title: %s", body)
	}
}

func TestMazeGenerateLoadSolveAndAgentFlow(t *testing.T) {
	t.Parallel()

	server, client := newTestServer(t)

	generated := postJSON(t, client, server.URL+"/api/maze/generate", map[string]any{"rows": 5, "cols": 5})
	if status := generated["status"]; status != float64(0) {
		t.Fatalf("generate status = %v, want 0: %v", status, generated)
	}
	if rows := generated["rows"]; rows != float64(5) {
		t.Fatalf("generate rows = %v, want 5", rows)
	}

	solved := postJSON(t, client, server.URL+"/api/maze/solve", map[string]any{
		"startRow": 0, "startCol": 0, "endRow": 4, "endCol": 4,
	})
	if status := solved["status"]; status != float64(0) {
		t.Fatalf("solve status = %v, want 0: %v", status, solved)
	}
	if _, ok := solved["path"]; !ok {
		t.Fatalf("solve response missing path: %v", solved)
	}

	trained := postJSON(t, client, server.URL+"/api/qlearning/train", map[string]any{"endRow": 4, "endCol": 4})
	if status := trained["status"]; status != float64(0) {
		t.Fatalf("train status = %v, want 0: %v", status, trained)
	}

	ran := postJSON(t, client, server.URL+"/api/qlearning/path", map[string]any{"startRow": 0, "startCol": 0})
	if status := ran["status"]; status != float64(0) {
		t.Fatalf("path status = %v, want 0: %v", status, ran)
	}
	if _, ok := ran["path"]; !ok {
		t.Fatalf("agent path response missing path: %v", ran)
	}
}

func TestMazeLoadAndSave(t *testing.T) {
	t.Parallel()

	server, client := newTestServer(t)

	loaded := postJSON(t, client, server.URL+"/api/maze/load", map[string]any{"content": sampleMaze})
	if status := loaded["status"]; status != float64(0) {
		t.Fatalf("load status = %v, want 0: %v", status, loaded)
	}

	saveResp, err := client.Post(server.URL+"/api/maze/save", "application/json", strings.NewReader("{}"))
	if err != nil {
		t.Fatalf("save maze: %v", err)
	}
	saveBody := readBody(t, saveResp)
	if saveResp.StatusCode != http.StatusOK {
		t.Fatalf("save status = %d, want %d", saveResp.StatusCode, http.StatusOK)
	}
	if saveBody != sampleMaze {
		t.Fatalf("saved maze = %q, want %q", saveBody, sampleMaze)
	}
}

func TestMazeSolveWithoutMazeReportsError(t *testing.T) {
	t.Parallel()

	server, client := newTestServer(t)

	resp := postJSON(t, client, server.URL+"/api/maze/solve", map[string]any{
		"startRow": 0, "startCol": 0, "endRow": 0, "endCol": 0,
	})
	if status := resp["status"]; status != float64(1) {
		t.Fatalf("status = %v, want 1", status)
	}
	if !strings.Contains(resp["error"].(string), "load or generate a maze first") {
		t.Fatalf("missing error: %v", resp)
	}
}

func TestQlearningPathBeforeTrainReportsError(t *testing.T) {
	t.Parallel()

	server, client := newTestServer(t)

	postJSON(t, client, server.URL+"/api/maze/generate", map[string]any{"rows": 3, "cols": 3})

	resp := postJSON(t, client, server.URL+"/api/qlearning/path", map[string]any{"startRow": 0, "startCol": 0})
	if status := resp["status"]; status != float64(1) {
		t.Fatalf("status = %v, want 1", status)
	}
	if !strings.Contains(resp["error"].(string), "train the agent first") {
		t.Fatalf("missing error: %v", resp)
	}
}

func TestMazeSaveWithoutMazeReportsError(t *testing.T) {
	t.Parallel()

	server, client := newTestServer(t)

	resp, err := client.Post(server.URL+"/api/maze/save", "application/json", strings.NewReader("{}"))
	if err != nil {
		t.Fatalf("save maze: %v", err)
	}
	var decoded map[string]any
	if err := json.NewDecoder(resp.Body).Decode(&decoded); err != nil {
		t.Fatalf("decode response: %v", err)
	}
	if !strings.Contains(decoded["error"].(string), "there is no maze to save") {
		t.Fatalf("missing error: %v", decoded)
	}
}

func TestMazeSolveAndTrainRejectOutOfBoundsPoints(t *testing.T) {
	t.Parallel()

	server, client := newTestServer(t)
	postJSON(t, client, server.URL+"/api/maze/generate", map[string]any{"rows": 3, "cols": 3})

	solved := postJSON(t, client, server.URL+"/api/maze/solve", map[string]any{
		"startRow": 0, "startCol": 0, "endRow": 9, "endCol": 9,
	})
	if status := solved["status"]; status != float64(1) {
		t.Fatalf("solve status = %v, want 1: %v", status, solved)
	}

	trained := postJSON(t, client, server.URL+"/api/qlearning/train", map[string]any{"endRow": 9, "endCol": 9})
	if status := trained["status"]; status != float64(1) {
		t.Fatalf("train status = %v, want 1: %v", status, trained)
	}
}

func TestMazeGenerateRejectsOutOfRangeSize(t *testing.T) {
	t.Parallel()

	server, client := newTestServer(t)

	resp := postJSON(t, client, server.URL+"/api/maze/generate", map[string]any{"rows": 0, "cols": 5})
	if status := resp["status"]; status != float64(1) {
		t.Fatalf("status = %v, want 1: %v", resp, resp)
	}
	if !strings.Contains(resp["error"].(string), "maze size must be from 1 to") {
		t.Fatalf("missing error: %v", resp)
	}
}

func TestCaveGenerateSimulateLoadAndSave(t *testing.T) {
	t.Parallel()

	server, client := newTestServer(t)

	generated := postJSON(t, client, server.URL+"/api/cave/generate", map[string]any{"rows": 10, "cols": 10, "ic": 100})
	if status := generated["status"]; status != float64(0) {
		t.Fatalf("cave generate status = %v, want 0: %v", status, generated)
	}
	if alive := generated["alive"]; alive != float64(1) {
		t.Fatalf("cave generate alive = %v, want 1", alive)
	}

	simulated := postJSON(t, client, server.URL+"/api/cave/simulate", map[string]any{"bl": 3, "dl": 2})
	if status := simulated["status"]; status != float64(0) {
		t.Fatalf("cave simulate status = %v, want 0: %v", status, simulated)
	}

	loaded := postJSON(t, client, server.URL+"/api/cave/load", map[string]any{"content": sampleCave})
	if status := loaded["status"]; status != float64(0) {
		t.Fatalf("cave load status = %v, want 0: %v", status, loaded)
	}

	saveResp, err := client.Post(server.URL+"/api/cave/save", "application/json", strings.NewReader("{}"))
	if err != nil {
		t.Fatalf("save cave: %v", err)
	}
	saveBody := readBody(t, saveResp)
	if saveBody != sampleCave {
		t.Fatalf("saved cave = %q, want %q", saveBody, sampleCave)
	}
}

func TestCaveSimulateWithoutCaveReportsError(t *testing.T) {
	t.Parallel()

	server, client := newTestServer(t)

	resp := postJSON(t, client, server.URL+"/api/cave/simulate", map[string]any{"bl": 3, "dl": 2})
	if status := resp["status"]; status != float64(1) {
		t.Fatalf("status = %v, want 1", status)
	}
	if !strings.Contains(resp["error"].(string), "generate or load a cave first") {
		t.Fatalf("missing error: %v", resp)
	}
}
