package web

import (
	"time"

	"tictactoe/src/internal/domain"
)

type BoardDTO [3][3]int

type GameRequest struct {
	Board BoardDTO `json:"board"`
}

type GameResponse struct {
	ID          string   `json:"id"`
	Board       BoardDTO `json:"board"`
	GameOver    bool     `json:"game_over"`
	Winner      string   `json:"winner"`
	Status      string   `json:"status"`
	CurrentTurn string   `json:"current_turn"`
	Player1ID   string   `json:"player1_id"`
}

type ErrorResponse struct {
	Error string `json:"error"`
}

type SignUpRequest struct {
	Login    string `json:"login"`
	Password string `json:"password"`
}

type UserResponse struct {
	ID        string    `json:"id"`
	Login     string    `json:"login"`
	CreatedAt time.Time `json:"created_at"`
}

type CreateGameRequest struct {
	VsComputer bool `json:"vs_computer"`
}

type JWTRequest struct {
	Login    string `json:"login"`
	Password string `json:"password"`
}

type JWTResponse struct {
	Type         string `json:"type"`
	AccessToken  string `json:"accessToken"`
	RefreshToken string `json:"refreshToken"`
}

type RefreshJWTRequest struct {
	RefreshToken string `json:"refreshToken"`
}

type LeaderboardEntryResponse struct {
	UserID   string  `json:"user_id"`
	Login    string  `json:"login"`
	WinRatio float64 `json:"win_ratio"`
}

func boardToDomain(dto BoardDTO) domain.Board {
	var board domain.Board
	for i := range 3 {
		for j := range 3 {
			board[i][j] = dto[i][j]
		}
	}
	return board
}

func boardToDTO(board domain.Board) BoardDTO {
	var dto BoardDTO
	for i := range 3 {
		for j := range 3 {
			dto[i][j] = board[i][j]
		}
	}
	return dto
}

func gameToResponse(game *domain.Game) *GameResponse {
	winner := ""
	if game.WinnerID != nil {
		winner = game.WinnerID.String()
	}
	currentTurn := ""
	if game.CurrentTurn != nil {
		currentTurn = game.CurrentTurn.String()
	}
	return &GameResponse{
		ID:          game.ID.String(),
		Board:       boardToDTO(game.Board),
		GameOver:    game.Status == domain.StatusWin || game.Status == domain.StatusDraw,
		Winner:      winner,
		Status:      game.Status,
		CurrentTurn: currentTurn,
		Player1ID:   game.Player1ID.String(),
	}
}
