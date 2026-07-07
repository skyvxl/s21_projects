package domain

import (
	"errors"
	"time"

	"github.com/google/uuid"
	"golang.org/x/crypto/bcrypt"
)

type authService struct {
	userRepo    UserRepository
	jwtProvider JWTProvider
}

func NewAuthService(userRepo UserRepository, jwtProvider JWTProvider) AuthService {
	return &authService{
		userRepo:    userRepo,
		jwtProvider: jwtProvider,
	}
}

func (a *authService) Register(req SignUpRequest) (*User, error) {
	existing, _ := a.userRepo.GetByLogin(req.Login)
	if existing != nil {
		return nil, errors.New("user with this login already exists")
	}
	hash, err := bcrypt.GenerateFromPassword([]byte(req.Password), bcrypt.DefaultCost)
	if err != nil {
		return nil, err
	}
	user := &User{
		ID:           uuid.New(),
		Login:        req.Login,
		PasswordHash: string(hash),
		CreatedAt:    time.Now(),
	}
	if err := a.userRepo.Save(user); err != nil {
		return nil, err
	}
	return user, nil
}

func (a *authService) Login(req JWTRequest) (*JWTResponse, error) {
	user, err := a.userRepo.GetByLogin(req.Login)
	if err != nil {
		return nil, errors.New("invalid login or password")
	}
	if err := bcrypt.CompareHashAndPassword([]byte(user.PasswordHash), []byte(req.Password)); err != nil {
		return nil, errors.New("invalid login or password")
	}
	accessToken, err := a.jwtProvider.GenerateAccessToken(user.ID.String())
	if err != nil {
		return nil, err
	}
	refreshToken, err := a.jwtProvider.GenerateRefreshToken(user.ID.String())
	if err != nil {
		return nil, err
	}
	return &JWTResponse{
		Type:         "Bearer",
		AccessToken:  accessToken,
		RefreshToken: refreshToken,
	}, nil
}

func (a *authService) RefreshAccessToken(refreshToken string) (*JWTResponse, error) {
	err := a.jwtProvider.ValidateRefreshToken(refreshToken)
	if err != nil {
		return nil, err
	}
	userID, err := a.jwtProvider.UserIDFromRefreshToken(refreshToken)
	if err != nil {
		return nil, err
	}
	accessToken, err := a.jwtProvider.GenerateAccessToken(userID.String())
	if err != nil {
		return nil, err
	}
	return &JWTResponse{
		Type:         "Bearer",
		AccessToken:  accessToken,
		RefreshToken: refreshToken,
	}, nil
}

func (a *authService) RefreshRefreshToken(refreshToken string) (*JWTResponse, error) {
	err := a.jwtProvider.ValidateRefreshToken(refreshToken)
	if err != nil {
		return nil, err
	}
	userID, err := a.jwtProvider.UserIDFromRefreshToken(refreshToken)
	if err != nil {
		return nil, err
	}
	newRefreshToken, err := a.jwtProvider.GenerateRefreshToken(userID.String())
	if err != nil {
		return nil, err
	}
	accessToken, err := a.jwtProvider.GenerateAccessToken(userID.String())
	if err != nil {
		return nil, err
	}
	return &JWTResponse{
		Type:         "Bearer",
		AccessToken:  accessToken,
		RefreshToken: newRefreshToken,
	}, nil
}
