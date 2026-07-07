package jwt

import (
	"tictactoe/src/internal/domain"
	"testing"
	"time"

	"github.com/golang-jwt/jwt/v5"
	"github.com/google/uuid"
)

func TestProvider_GenerateAccessToken_SucceedsWithVariousTTLs(t *testing.T) {
	tests := []struct {
		name string
		ttl  time.Duration
	}{
		{name: "with positive TTL", ttl: 10 * time.Second},
		{name: "with zero TTL", ttl: 0},
		{name: "with long TTL", ttl: 24 * time.Hour},
	}

	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			p := NewJWTProvider("secret", "refresh", tt.ttl, time.Hour)
			user := &domain.User{ID: uuid.New()}

			token, err := p.GenerateAccessToken(user.ID.String())

			if err != nil {
				t.Fatalf("GenerateAccessToken() error = %v", err)
			}
			if token == "" {
				t.Fatal("GenerateAccessToken() returned empty token")
			}
		})
	}
}

func TestProvider_ValidateAccessToken_SucceedsWithValidToken(t *testing.T) {
	tests := []struct {
		name string
		ttl  time.Duration
	}{
		{name: "valid token with future expiration", ttl: 10 * time.Second},
		{name: "valid token with long TTL", ttl: 24 * time.Hour},
	}

	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			secret := "test-secret"
			p := NewJWTProvider(secret, "refresh", tt.ttl, time.Hour)
			user := &domain.User{ID: uuid.New()}

			token, _ := p.GenerateAccessToken(user.ID.String())

			err := p.ValidateAccessToken(token)

			if err != nil {
				t.Errorf("ValidateAccessToken() error = %v, want nil", err)
			}
		})
	}
}

func TestProvider_ValidateAccessToken_RejectsExpiredToken(t *testing.T) {
	tests := []struct {
		name string
		ttl  time.Duration
	}{
		{name: "expired token with negative TTL", ttl: -1 * time.Second},
		{name: "expired token with large negative TTL", ttl: -10 * time.Second},
	}

	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			secret := "test-secret"
			p := NewJWTProvider(secret, "refresh", tt.ttl, time.Hour)
			user := &domain.User{ID: uuid.New()}

			token, _ := p.GenerateAccessToken(user.ID.String())
			time.Sleep(100 * time.Millisecond)

			err := p.ValidateAccessToken(token)

			if err == nil {
				t.Errorf("ValidateAccessToken() error = nil, want error for expired token")
			}
		})
	}
}

func TestProvider_ValidateAccessToken_RejectsInvalidTokenFormat(t *testing.T) {
	tests := []struct {
		name  string
		token string
	}{
		{name: "malformed token with single dot", token: "invalid.token"},
		{name: "empty token", token: ""},
		{name: "random string without JWT format", token: "not-a-token"},
	}

	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			p := NewJWTProvider("secret", "refresh", time.Hour, time.Hour)

			err := p.ValidateAccessToken(tt.token)

			if err == nil {
				t.Errorf("ValidateAccessToken() error = nil, want error for invalid token format '%s'", tt.token)
			}
		})
	}
}

func TestProvider_ValidateAccessToken_RejectsInvalidSignature(t *testing.T) {
	tests := []struct {
		name           string
		signSecret     string
		validateSecret string
	}{
		{name: "token signed with different secret", signSecret: "secret1", validateSecret: "secret2"},
	}

	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			pSigner := NewJWTProvider(tt.signSecret, "refresh", 10*time.Second, time.Hour)
			pValidator := NewJWTProvider(tt.validateSecret, "refresh", 10*time.Second, time.Hour)
			user := &domain.User{ID: uuid.New()}

			token, _ := pSigner.GenerateAccessToken(user.ID.String())

			err := pValidator.ValidateAccessToken(token)

			if err == nil {
				t.Errorf("ValidateAccessToken() error = nil, want error for invalid signature")
			}
		})
	}
}

func TestProvider_GenerateRefreshToken_SucceedsWithVariousTTLs(t *testing.T) {
	tests := []struct {
		name string
		ttl  time.Duration
	}{
		{name: "with positive TTL", ttl: 7 * 24 * time.Hour},
		{name: "with zero TTL", ttl: 0},
	}

	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			p := NewJWTProvider("access-secret", "refresh-secret", time.Hour, tt.ttl)
			user := &domain.User{ID: uuid.New()}

			token, err := p.GenerateRefreshToken(user.ID.String())

			if err != nil {
				t.Fatalf("GenerateRefreshToken() error = %v", err)
			}
			if token == "" {
				t.Fatal("GenerateRefreshToken() returned empty token")
			}
		})
	}
}

func TestProvider_ValidateRefreshToken_SucceedsWithValidToken(t *testing.T) {
	tests := []struct {
		name string
		ttl  time.Duration
	}{
		{name: "valid refresh token with future expiration", ttl: 7 * 24 * time.Hour},
	}

	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			secret := "refresh-secret"
			p := NewJWTProvider("access", secret, time.Hour, tt.ttl)
			user := &domain.User{ID: uuid.New()}

			token, _ := p.GenerateRefreshToken(user.ID.String())

			err := p.ValidateRefreshToken(token)

			if err != nil {
				t.Errorf("ValidateRefreshToken() error = %v, want nil", err)
			}
		})
	}
}

func TestProvider_ValidateRefreshToken_RejectsExpiredToken(t *testing.T) {
	tests := []struct {
		name string
		ttl  time.Duration
	}{
		{name: "expired refresh token with negative TTL", ttl: -1 * time.Second},
	}

	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			secret := "refresh-secret"
			p := NewJWTProvider("access", secret, time.Hour, tt.ttl)
			user := &domain.User{ID: uuid.New()}

			token, _ := p.GenerateRefreshToken(user.ID.String())
			time.Sleep(100 * time.Millisecond)

			err := p.ValidateRefreshToken(token)

			if err == nil {
				t.Errorf("ValidateRefreshToken() error = nil, want error for expired token")
			}
		})
	}
}

func TestProvider_ValidateRefreshToken_RejectsInvalidSignature(t *testing.T) {
	tests := []struct {
		name           string
		signSecret     string
		validateSecret string
	}{
		{name: "refresh token signed with different secret", signSecret: "secret1", validateSecret: "secret2"},
	}

	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			pSigner := NewJWTProvider("access", tt.signSecret, time.Hour, 7*24*time.Hour)
			pValidator := NewJWTProvider("access", tt.validateSecret, time.Hour, 7*24*time.Hour)
			user := &domain.User{ID: uuid.New()}

			token, _ := pSigner.GenerateRefreshToken(user.ID.String())

			err := pValidator.ValidateRefreshToken(token)

			if err == nil {
				t.Errorf("ValidateRefreshToken() error = nil, want error for invalid signature")
			}
		})
	}
}

func TestProvider_UserIDFromAccessToken_ExtractsCorrectlyFromToken(t *testing.T) {
	tests := []struct {
		name   string
		userID uuid.UUID
	}{
		{name: "extract UUID from valid token", userID: uuid.New()},
		{name: "extract different UUID", userID: uuid.New()},
	}

	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			p := NewJWTProvider("secret", "refresh", time.Hour, time.Hour)
			user := &domain.User{ID: tt.userID}

			token, _ := p.GenerateAccessToken(user.ID.String())
			extractedID, err := p.UserIDFromAccessToken(token)

			if err != nil {
				t.Errorf("UserIDFromAccessToken() error = %v", err)
			}
			if extractedID != tt.userID {
				t.Errorf("UserIDFromAccessToken() got = %v, want %v", extractedID, tt.userID)
			}
		})
	}
}

func TestProvider_UserIDFromAccessToken_RejectsInvalidUUIDFormat(t *testing.T) {
	tests := []struct {
		name       string
		setupToken func() string
	}{
		{
			name: "token with invalid UUID format in sub claim",
			setupToken: func() string {
				token := jwt.NewWithClaims(jwt.SigningMethodHS256, jwt.MapClaims{
					"sub": "not-a-valid-uuid",
					"exp": time.Now().Add(time.Hour).Unix(),
				})
				tokenString, _ := token.SignedString([]byte("secret"))
				return tokenString
			},
		},
	}

	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			p := NewJWTProvider("secret", "refresh", time.Hour, time.Hour)
			token := tt.setupToken()

			_, err := p.UserIDFromAccessToken(token)

			if err == nil {
				t.Errorf("UserIDFromAccessToken() error = nil, want error for invalid UUID format")
			}
		})
	}
}

func TestProvider_UserIDFromRefreshToken_ExtractsCorrectlyFromToken(t *testing.T) {
	tests := []struct {
		name   string
		userID uuid.UUID
	}{
		{name: "extract UUID from valid token", userID: uuid.New()},
		{name: "extract different UUID", userID: uuid.New()},
	}

	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			p := NewJWTProvider("secret", "refresh", time.Hour, time.Hour)
			user := &domain.User{ID: tt.userID}

			token, _ := p.GenerateRefreshToken(user.ID.String())
			extractedID, err := p.UserIDFromRefreshToken(token)

			if err != nil {
				t.Errorf("UserIDFromRefreshToken() error = %v", err)
			}
			if extractedID != tt.userID {
				t.Errorf("UserIDFromRefreshToken() got = %v, want %v", extractedID, tt.userID)
			}
		})
	}
}

func TestProvider_UserIDFromRefreshToken_RejectsInvalidUUIDFormat(t *testing.T) {
	tests := []struct {
		name       string
		setupToken func() string
	}{
		{
			name: "token with invalid UUID format in sub claim",
			setupToken: func() string {
				token := jwt.NewWithClaims(jwt.SigningMethodHS256, jwt.MapClaims{
					"sub": "not-a-valid-uuid",
					"exp": time.Now().Add(time.Hour).Unix(),
				})
				tokenString, _ := token.SignedString([]byte("secret"))
				return tokenString
			},
		},
	}

	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			p := NewJWTProvider("secret", "refresh", time.Hour, time.Hour)
			token := tt.setupToken()

			_, err := p.UserIDFromRefreshToken(token)

			if err == nil {
				t.Errorf("UserIDFromRefreshToken() error = nil, want error for invalid UUID format")
			}
		})
	}
}
