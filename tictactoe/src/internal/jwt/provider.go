package jwt

import (
	"fmt"
	"time"

	"github.com/golang-jwt/jwt/v5"
	"github.com/google/uuid"
)

type JWTProvider struct {
	accessSecret  []byte
	refreshSecret []byte
	accessTTL     time.Duration
	refreshTTL    time.Duration
}

func NewJWTProvider(accessSecret, refreshSecret string, accessTTL, refreshTTL time.Duration) *JWTProvider {
	return &JWTProvider{
		accessSecret:  []byte(accessSecret),
		refreshSecret: []byte(refreshSecret),
		accessTTL:     accessTTL,
		refreshTTL:    refreshTTL,
	}
}

func (p *JWTProvider) GenerateAccessToken(userID string) (string, error) {
	claims := jwt.MapClaims{
		"sub": userID,
		"exp": time.Now().Add(p.accessTTL).Unix(),
	}
	token := jwt.NewWithClaims(jwt.SigningMethodHS256, claims)
	return token.SignedString(p.accessSecret)
}

func (p *JWTProvider) GenerateRefreshToken(userID string) (string, error) {
	claims := jwt.MapClaims{
		"sub": userID,
		"exp": time.Now().Add(p.refreshTTL).Unix(),
	}
	token := jwt.NewWithClaims(jwt.SigningMethodHS256, claims)
	return token.SignedString(p.refreshSecret)
}

func signingKey(secret []byte) jwt.Keyfunc {
	return func(token *jwt.Token) (interface{}, error) {
		if token.Method != jwt.SigningMethodHS256 {
			return nil, fmt.Errorf("unexpected signing method: %v", token.Header["alg"])
		}
		return secret, nil
	}
}

func (p *JWTProvider) ValidateAccessToken(token string) error {
	claims := jwt.MapClaims{}
	_, err := jwt.ParseWithClaims(token, claims, signingKey(p.accessSecret))
	if err != nil {
		return fmt.Errorf("invalid access token: %w", err)
	}
	exp, ok := claims["exp"].(float64)
	if !ok {
		return fmt.Errorf("invalid exp claim")
	}
	if int64(exp) < time.Now().Unix() {
		return fmt.Errorf("token is expired")
	}
	return nil
}

func (p *JWTProvider) ValidateRefreshToken(token string) error {
	claims := jwt.MapClaims{}
	_, err := jwt.ParseWithClaims(token, claims, signingKey(p.refreshSecret))
	if err != nil {
		return fmt.Errorf("invalid refresh token: %w", err)
	}
	exp, ok := claims["exp"].(float64)
	if !ok {
		return fmt.Errorf("invalid exp claim")
	}
	if int64(exp) < time.Now().Unix() {
		return fmt.Errorf("token is expired")
	}
	return nil
}

func (p *JWTProvider) userIDFromToken(token string, tokenType string) (uuid.UUID, error) {
	claims := jwt.MapClaims{}
	if tokenType == "access" {
		_, err := jwt.ParseWithClaims(token, claims, signingKey(p.accessSecret))
		if err != nil {
			return uuid.Nil, err
		}
	} else {
		_, err := jwt.ParseWithClaims(token, claims, signingKey(p.refreshSecret))
		if err != nil {
			return uuid.Nil, err
		}
	}
	sub, ok := claims["sub"].(string)
	if !ok {
		return uuid.Nil, fmt.Errorf("invalid sub claim")
	}
	return uuid.Parse(sub)
}

func (p *JWTProvider) UserIDFromAccessToken(token string) (uuid.UUID, error) {
	return p.userIDFromToken(token, "access")
}

func (p *JWTProvider) UserIDFromRefreshToken(token string) (uuid.UUID, error) {
	return p.userIDFromToken(token, "refresh")
}
