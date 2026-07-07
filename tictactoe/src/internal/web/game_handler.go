package web

import (
	"encoding/json"
	"log/slog"
	"net/http"
	"strconv"
	"strings"
	"time"

	"tictactoe/src/internal/domain"

	"github.com/google/uuid"
)

type gameService interface {
	GetNextMove(game *domain.Game) (int, int, error)
	ValidateBoard(oldBoard, newBoard domain.Board) error
	CheckGameEnd(game *domain.Game)
	GetCompletedGamesByUserID(userID uuid.UUID) ([]*domain.Game, error)
	GetTopPlayers(limit int32) ([]domain.LeaderboardEntry, error)
}

type gameRepo interface {
	Save(game *domain.Game) error
	Get(id string) (*domain.Game, error)
	GetAvailableGames() ([]*domain.Game, error)
}

type GameHandler struct {
	service gameService
	repo    gameRepo
}

func NewGameHandler(service domain.GameService, repo domain.GameRepository) *GameHandler {
	return &GameHandler{service: service, repo: repo}
}

func (h *GameHandler) RegisterRoutes(mux *http.ServeMux) {
	mux.HandleFunc("/game/", h.handleUpdateGame)
}

func (h *GameHandler) Handler() http.HandlerFunc {
	return func(w http.ResponseWriter, r *http.Request) {
		path := strings.TrimPrefix(r.URL.Path, "/game/")
		if strings.HasSuffix(path, "/join") {
			h.handleJoinGame(w, r)
			return
		}
		switch r.Method {
		case http.MethodGet:
			h.handleGetGame(w, r)
		case http.MethodPost:
			h.handleUpdateGame(w, r)
		default:
			writeError(w, "method not allowed", http.StatusMethodNotAllowed)
		}
	}
}

func (h *GameHandler) handleUpdateGame(w http.ResponseWriter, r *http.Request) {
	userID := r.Context().Value(contextKey).(uuid.UUID)

	path := strings.TrimPrefix(r.URL.Path, "/game/")
	if path == "" {
		writeError(w, "game id is required", http.StatusBadRequest)
		return
	}
	gameID, err := uuid.Parse(path)
	if err != nil {
		writeError(w, "invalid game id", http.StatusBadRequest)
		return
	}

	var req GameRequest
	if err := json.NewDecoder(r.Body).Decode(&req); err != nil {
		writeError(w, "invalid request body", http.StatusBadRequest)
		return
	}

	game, err := h.repo.Get(gameID.String())
	if err != nil {
		writeError(w, "game not found", http.StatusNotFound)
		return
	}
	if game.Status != domain.StatusTurn {
		writeError(w, "game is not in progress", http.StatusBadRequest)
		return
	}
	if game.CurrentTurn == nil || *game.CurrentTurn != userID {
		writeError(w, "it is not your turn", http.StatusForbidden)
		return
	}

	newBoard := boardToDomain(req.Board)
	if err := h.service.ValidateBoard(game.Board, newBoard); err != nil {
		writeError(w, err.Error(), http.StatusBadRequest)
		return
	}
	game.Board = newBoard
	h.service.CheckGameEnd(game)

	if game.Status != domain.StatusTurn {
		err := h.repo.Save(game)
		if err != nil {
			slog.Error("failed to save game", "error", err)
			writeError(w, "failed to save game", http.StatusInternalServerError)
			return
		}
		writeJSON(w, gameToResponse(game))
		return
	}

	if game.Player2ID != nil {
		if *game.CurrentTurn == *game.Player2ID {
			game.CurrentTurn = &game.Player1ID
		} else {
			game.CurrentTurn = game.Player2ID
		}
		game.UpdatedAt = time.Now()
		err := h.repo.Save(game)
		if err != nil {
			slog.Error("failed to save game", "error", err)
			writeError(w, "failed to save game", http.StatusInternalServerError)
			return
		}
		writeJSON(w, gameToResponse(game))
		return
	}

	row, col, err := h.service.GetNextMove(game)
	if err != nil {
		writeError(w, err.Error(), http.StatusInternalServerError)
		return
	}
	game.Board[row][col] = domain.PlayerO
	h.service.CheckGameEnd(game)
	err = h.repo.Save(game)
	if err != nil {
		slog.Error("failed to save game", "error", err)
		writeError(w, "failed to save game", http.StatusInternalServerError)
		return
	}
	writeJSON(w, gameToResponse(game))
}

func (h *GameHandler) handleCreateGame(w http.ResponseWriter, r *http.Request) {
	if r.Method != http.MethodPost {
		writeError(w, "method not allowed", http.StatusMethodNotAllowed)
		return
	}
	userID := r.Context().Value(contextKey).(uuid.UUID)

	var req CreateGameRequest
	if err := json.NewDecoder(r.Body).Decode(&req); err != nil {
		writeError(w, "invalid request body", http.StatusBadRequest)
		return
	}

	game := &domain.Game{
		ID:            uuid.New(),
		Player1ID:     userID,
		Board:         domain.Board{},
		Player1Symbol: domain.PlayerX,
		Player2Symbol: domain.PlayerO,
		CreatedAt:     time.Now(),
		UpdatedAt:     time.Now(),
	}
	if req.VsComputer {
		game.Status = domain.StatusTurn
		game.CurrentTurn = &userID
	} else {
		game.Status = domain.StatusWaiting
	}
	err := h.repo.Save(game)
	if err != nil {
		slog.Error("failed to save game", "error", err)
		writeError(w, "failed to save game", http.StatusInternalServerError)
		return
	}
	writeJSON(w, gameToResponse(game))
}

func (h *GameHandler) handleGetAvailableGames(w http.ResponseWriter, r *http.Request) {
	if r.Method != http.MethodGet {
		writeError(w, "method not allowed", http.StatusMethodNotAllowed)
		return
	}
	games, err := h.repo.GetAvailableGames()
	if err != nil {
		writeError(w, err.Error(), http.StatusInternalServerError)
		return
	}
	writeJSON(w, games)
}

func (h *GameHandler) handleGetCompletedGamesByUserID(w http.ResponseWriter, r *http.Request) {
	if r.Method != http.MethodGet {
		writeError(w, "method not allowed", http.StatusMethodNotAllowed)
		return
	}
	userID, ok := userIDFromContext(r.Context())
	if !ok {
		writeError(w, "unauthorized", http.StatusUnauthorized)
		return
	}
	games, err := h.service.GetCompletedGamesByUserID(userID)
	if err != nil {
		writeError(w, err.Error(), http.StatusInternalServerError)
		return
	}
	responseGames := make([]*GameResponse, 0, len(games))
	for _, game := range games {
		responseGames = append(responseGames, gameToResponse(game))
	}
	writeJSON(w, responseGames)
}

func (h *GameHandler) handleGetLeaderboard(w http.ResponseWriter, r *http.Request) {
	if r.Method != http.MethodGet {
		writeError(w, "method not allowed", http.StatusMethodNotAllowed)
		return
	}
	limit := 10
	if raw := r.URL.Query().Get("limit"); raw != "" {
		parsed, err := strconv.Atoi(raw)
		if err != nil {
			writeError(w, "invalid limit", http.StatusBadRequest)
			return
		}
		limit = parsed
	}
	entries, err := h.service.GetTopPlayers(int32(limit))
	if err != nil {
		writeError(w, err.Error(), http.StatusInternalServerError)
		return
	}
	leaders := make([]*LeaderboardEntryResponse, 0, len(entries))
	for _, entry := range entries {
		leaders = append(leaders, &LeaderboardEntryResponse{
			UserID:   entry.UserID.String(),
			Login:    entry.Login,
			WinRatio: entry.WinRatio,
		})
	}
	writeJSON(w, leaders)
}

func (h *GameHandler) handleJoinGame(w http.ResponseWriter, r *http.Request) {
	if r.Method != http.MethodPost {
		writeError(w, "method not allowed", http.StatusMethodNotAllowed)
		return
	}
	path := strings.TrimSuffix(strings.TrimPrefix(r.URL.Path, "/game/"), "/join")
	gameID, err := uuid.Parse(path)
	if err != nil {
		writeError(w, "invalid game id", http.StatusBadRequest)
		return
	}
	game, err := h.repo.Get(gameID.String())
	if err != nil {
		writeError(w, err.Error(), http.StatusInternalServerError)
		return
	}
	userID := r.Context().Value(contextKey).(uuid.UUID)
	if game.Status != domain.StatusWaiting {
		writeError(w, "game is already started", http.StatusBadRequest)
		return
	}
	if game.Player1ID == userID {
		writeError(w, "you are already in the game", http.StatusBadRequest)
		return
	}
	game.Player2ID = &userID
	game.Status = domain.StatusTurn
	game.CurrentTurn = &game.Player1ID
	game.UpdatedAt = time.Now()
	err = h.repo.Save(game)
	if err != nil {
		slog.Error("failed to save game", "error", err)
		writeError(w, "failed to save game", http.StatusInternalServerError)
		return
	}
	writeJSON(w, gameToResponse(game))
}

func (h *GameHandler) handleGetGame(w http.ResponseWriter, r *http.Request) {
	if r.Method != http.MethodGet {
		writeError(w, "method not allowed", http.StatusMethodNotAllowed)
		return
	}
	path := strings.TrimPrefix(r.URL.Path, "/game/")
	gameID, err := uuid.Parse(path)
	if err != nil {
		writeError(w, "invalid game id", http.StatusBadRequest)
		return
	}
	game, err := h.repo.Get(gameID.String())
	if err != nil {
		writeError(w, err.Error(), http.StatusInternalServerError)
		return
	}
	writeJSON(w, gameToResponse(game))
}

func (h *GameHandler) CreateGameHandler() http.HandlerFunc { return h.handleCreateGame }
func (h *GameHandler) JoinGameHandler() http.HandlerFunc   { return h.handleJoinGame }
func (h *GameHandler) GetGameHandler() http.HandlerFunc    { return h.handleGetGame }
func (h *GameHandler) GetAvailableGamesHandler() http.HandlerFunc {
	return h.handleGetAvailableGames
}
func (h *GameHandler) GetCompletedGamesByUserIDHandler() http.HandlerFunc {
	return h.handleGetCompletedGamesByUserID
}
func (h *GameHandler) GetLeaderboardHandler() http.HandlerFunc { return h.handleGetLeaderboard }
