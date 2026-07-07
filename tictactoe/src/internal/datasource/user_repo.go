package datasource

import (
	"tictactoe/src/internal/db"
	"tictactoe/src/internal/domain"
	"context"
	"fmt"

	"github.com/google/uuid"
	"github.com/jackc/pgx/v5/pgtype"
	"github.com/jackc/pgx/v5/pgxpool"
)

type userRepository struct {
	q *db.Queries
}

func NewUserRepository(pool *pgxpool.Pool) domain.UserRepository {
	return &userRepository{q: db.New(pool)}
}

func (r *userRepository) Save(user *domain.User) error {
	return r.q.UpsertUser(context.Background(), db.UpsertUserParams{
		ID:        user.ID,
		Login:     user.Login,
		Password:  user.PasswordHash,
		CreatedAt: pgtype.Timestamptz{Time: user.CreatedAt, Valid: true},
	})
}

func (r *userRepository) GetByLogin(login string) (*domain.User, error) {
	row, err := r.q.GetUserByLogin(context.Background(), login)
	if err != nil {
		return nil, fmt.Errorf("user not found: %w", err)
	}
	return userRowToDomain(row), nil
}

func (r *userRepository) GetByID(id uuid.UUID) (*domain.User, error) {
	row, err := r.q.GetUserByID(context.Background(), id)
	if err != nil {
		return nil, fmt.Errorf("user not found: %w", err)
	}
	return userRowToDomain(row), nil
}

func userRowToDomain(row db.User) *domain.User {
	return &domain.User{
		ID:           row.ID,
		Login:        row.Login,
		PasswordHash: row.Password,
		CreatedAt:    row.CreatedAt.Time,
	}
}
