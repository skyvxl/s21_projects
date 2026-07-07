package datasource

import (
	"tictactoe/src/internal/db"
	"tictactoe/src/internal/domain"
	"context"
	"encoding/json"
	"fmt"

	"github.com/google/uuid"
	"github.com/jackc/pgx/v5/pgtype"
	"github.com/jackc/pgx/v5/pgxpool"
)

type gameRepository struct {
	q *db.Queries
}

func NewGameRepository(pool *pgxpool.Pool) domain.GameRepository {
	return &gameRepository{q: db.New(pool)}
}

func (r *gameRepository) Save(game *domain.Game) error {
	boardJSON, err := json.Marshal(game.Board)
	if err != nil {
		return fmt.Errorf("marshal board: %w", err)
	}
	return r.q.UpsertGame(context.Background(), db.UpsertGameParams{
		ID:            game.ID,
		Player1ID:     game.Player1ID,
		Player2ID:     game.Player2ID,
		Board:         boardJSON,
		Status:        game.Status,
		CurrentTurn:   game.CurrentTurn,
		WinnerID:      game.WinnerID,
		CreatedAt:     pgtype.Timestamptz{Time: game.CreatedAt, Valid: true},
		UpdatedAt:     pgtype.Timestamptz{Time: game.UpdatedAt, Valid: true},
		Player1Symbol: int32(game.Player1Symbol), //nolint:gosec
		Player2Symbol: int32(game.Player2Symbol), //nolint:gosec
	})
}

func (r *gameRepository) Get(id string) (*domain.Game, error) {
	uid, err := uuid.Parse(id)
	if err != nil {
		return nil, fmt.Errorf("invalid game id: %w", err)
	}
	row, err := r.q.GetGame(context.Background(), uid)
	if err != nil {
		return nil, fmt.Errorf("game not found: %w", err)
	}
	return gameRowToDomain(row)
}

func (r *gameRepository) GetAvailableGames() ([]*domain.Game, error) {
	rows, err := r.q.GetAvailableGames(context.Background())
	if err != nil {
		return nil, err
	}
	games := make([]*domain.Game, 0, len(rows))
	for _, row := range rows {
		g, err := gameRowToDomain(row)
		if err != nil {
			return nil, err
		}
		games = append(games, g)
	}
	return games, nil
}

func (r *gameRepository) GetCompletedGamesByUserID(userID uuid.UUID) ([]*domain.Game, error) {
	rows, err := r.q.GetCompletedGamesByUserID(context.Background(), userID)
	if err != nil {
		return nil, err
	}
	games := make([]*domain.Game, 0, len(rows))
	for _, row := range rows {
		g, err := gameRowToDomain(row)
		if err != nil {
			return nil, err
		}
		games = append(games, g)
	}
	return games, nil
}

func (r *gameRepository) GetTopPlayers(limit int32) ([]domain.LeaderboardEntry, error) {
	if limit <= 0 {
		return nil, fmt.Errorf("limit must be greater than 0")
	}

	rows, err := r.q.GetTopPlayers(context.Background(), int(limit))
	if err != nil {
		return nil, err
	}
	leaderboard := make([]domain.LeaderboardEntry, 0, len(rows))
	for _, row := range rows {
		player := domain.LeaderboardEntry{
			UserID:   row.UserID,
			Login:    row.Login,
			WinRatio: row.WinRatio,
		}
		leaderboard = append(leaderboard, player)
	}
	return leaderboard, nil
}

func gameRowToDomain(row db.Game) (*domain.Game, error) {
	var board domain.Board
	if err := json.Unmarshal(row.Board, &board); err != nil {
		return nil, fmt.Errorf("unmarshal board: %w", err)
	}
	return &domain.Game{
		ID:            row.ID,
		Player1ID:     row.Player1ID,
		Player2ID:     row.Player2ID,
		Board:         board,
		Status:        row.Status,
		CurrentTurn:   row.CurrentTurn,
		WinnerID:      row.WinnerID,
		CreatedAt:     row.CreatedAt.Time,
		UpdatedAt:     row.UpdatedAt.Time,
		Player1Symbol: int(row.Player1Symbol),
		Player2Symbol: int(row.Player2Symbol),
	}, nil
}
