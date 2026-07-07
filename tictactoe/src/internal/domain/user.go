package domain

import (
	"time"

	"github.com/google/uuid"
)

type User struct {
	ID           uuid.UUID
	Login        string
	PasswordHash string
	CreatedAt    time.Time
}

type LeaderboardEntry struct {
	UserID   uuid.UUID
	Login    string
	WinRatio float64
}

type SignUpRequest struct {
	Login    string
	Password string
}

type JWTRequest struct {
	Login    string
	Password string
}

type JWTResponse struct {
	Type         string
	AccessToken  string
	RefreshToken string
}

type RefreshJWTRequest struct {
	RefreshToken string
}

type UserRepository interface {
	Save(user *User) error
	GetByLogin(login string) (*User, error)
	GetByID(id uuid.UUID) (*User, error)
}

type AuthService interface {
	Register(req SignUpRequest) (*User, error)
	Login(req JWTRequest) (*JWTResponse, error)
	RefreshAccessToken(refreshToken string) (*JWTResponse, error)
	RefreshRefreshToken(refreshToken string) (*JWTResponse, error)
}

type JWTProvider interface {
	GenerateAccessToken(userID string) (string, error)
	GenerateRefreshToken(userID string) (string, error)
	ValidateAccessToken(string) error
	ValidateRefreshToken(string) error
	UserIDFromAccessToken(string) (uuid.UUID, error)
	UserIDFromRefreshToken(string) (uuid.UUID, error)
}
