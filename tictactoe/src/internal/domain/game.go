package domain

import (
	"time"

	"github.com/google/uuid"
)

type Board [3][3]int

const (
	Empty   = 0
	PlayerX = 1
	PlayerO = -1
)

const (
	StatusWaiting = "waiting"
	StatusTurn    = "turn"
	StatusWin     = "win"
	StatusDraw    = "draw"
)

type Game struct {
	ID            uuid.UUID
	Player1ID     uuid.UUID
	Player2ID     *uuid.UUID
	Board         Board
	Status        string
	CurrentTurn   *uuid.UUID
	WinnerID      *uuid.UUID
	CreatedAt     time.Time
	UpdatedAt     time.Time
	Player1Symbol int
	Player2Symbol int
}

type GameRepository interface {
	Save(game *Game) error
	Get(id string) (*Game, error)
	GetAvailableGames() ([]*Game, error)
	GetCompletedGamesByUserID(userID uuid.UUID) ([]*Game, error)
	GetTopPlayers(limit int32) ([]LeaderboardEntry, error)
}

type GameService interface {
	GetNextMove(game *Game) (int, int, error)
	ValidateBoard(oldBoard, newBoard Board) error
	CheckGameEnd(game *Game)
	GetCompletedGamesByUserID(userID uuid.UUID) ([]*Game, error)
	GetTopPlayers(limit int32) ([]LeaderboardEntry, error)
}
