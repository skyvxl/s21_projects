package web

import (
	"tictactoe/src/internal/domain"
	"encoding/json"
	"net/http"

	"github.com/google/uuid"
)

type authService interface {
	Register(req domain.SignUpRequest) (*domain.User, error)
	Login(req domain.JWTRequest) (*domain.JWTResponse, error)
	RefreshAccessToken(refreshToken string) (*domain.JWTResponse, error)
	RefreshRefreshToken(refreshToken string) (*domain.JWTResponse, error)
}

type userRepo interface {
	GetByID(id uuid.UUID) (*domain.User, error)
}

type AuthHandler struct {
	auth     authService
	userRepo userRepo
}

func NewAuthHandler(auth domain.AuthService, userRepo domain.UserRepository) *AuthHandler {
	return &AuthHandler{auth: auth, userRepo: userRepo}
}

func (h *AuthHandler) RegisterRoutes(mux *http.ServeMux) {
	mux.HandleFunc("/register", h.handleRegister)
	mux.HandleFunc("/login", h.handleLogin)
	mux.HandleFunc("/refresh/access", h.handleRefreshAccessToken)
	mux.HandleFunc("/refresh/refresh", h.handleRefreshRefreshToken)
}

func (h *AuthHandler) handleRegister(w http.ResponseWriter, r *http.Request) {
	if r.Method != http.MethodPost {
		writeError(w, "method not allowed", http.StatusMethodNotAllowed)
		return
	}
	var req SignUpRequest
	if err := json.NewDecoder(r.Body).Decode(&req); err != nil {
		writeError(w, "invalid request body", http.StatusBadRequest)
		return
	}
	user, err := h.auth.Register(domain.SignUpRequest{Login: req.Login, Password: req.Password})
	if err != nil {
		writeError(w, err.Error(), http.StatusBadRequest)
		return
	}
	writeJSON(w, UserResponse{ID: user.ID.String(), Login: user.Login, CreatedAt: user.CreatedAt})
}

func (h *AuthHandler) handleLogin(w http.ResponseWriter, r *http.Request) {
	if r.Method != http.MethodPost {
		writeError(w, "method not allowed", http.StatusMethodNotAllowed)
		return
	}
	var req JWTRequest
	if err := json.NewDecoder(r.Body).Decode(&req); err != nil {
		writeError(w, "invalid request body", http.StatusBadRequest)
		return
	}
	tokens, err := h.auth.Login(domain.JWTRequest{
		Login:    req.Login,
		Password: req.Password,
	})
	if err != nil {
		writeError(w, err.Error(), http.StatusUnauthorized)
		return
	}
	writeJSON(w, JWTResponse{
		Type:         tokens.Type,
		AccessToken:  tokens.AccessToken,
		RefreshToken: tokens.RefreshToken,
	})
}

func (h *AuthHandler) handleGetUser(w http.ResponseWriter, r *http.Request) {
	if r.Method != http.MethodGet {
		writeError(w, "method not allowed", http.StatusMethodNotAllowed)
		return
	}
	userID, ok := userIDFromContext(r.Context())
	if !ok {
		writeError(w, "unauthorized", http.StatusUnauthorized)
		return
	}
	user, err := h.userRepo.GetByID(userID)
	if err != nil {
		writeError(w, err.Error(), http.StatusInternalServerError)
		return
	}
	writeJSON(w, UserResponse{ID: user.ID.String(), Login: user.Login, CreatedAt: user.CreatedAt})
}

func (h *AuthHandler) GetUserHandler() http.HandlerFunc { return h.handleGetUser }

func (h *AuthHandler) handleRefreshAccessToken(w http.ResponseWriter, r *http.Request) {
	if r.Method != http.MethodPost {
		writeError(w, "method not allowed", http.StatusMethodNotAllowed)
		return
	}
	var req RefreshJWTRequest
	if err := json.NewDecoder(r.Body).Decode(&req); err != nil {
		writeError(w, "invalid request body", http.StatusBadRequest)
		return
	}
	resp, err := h.auth.RefreshAccessToken(req.RefreshToken)
	if err != nil {
		writeError(w, err.Error(), http.StatusUnauthorized)
		return
	}
	writeJSON(w, JWTResponse{
		Type:         resp.Type,
		AccessToken:  resp.AccessToken,
		RefreshToken: resp.RefreshToken,
	})
}

func (h *AuthHandler) handleRefreshRefreshToken(w http.ResponseWriter, r *http.Request) {
	if r.Method != http.MethodPost {
		writeError(w, "method not allowed", http.StatusMethodNotAllowed)
		return
	}
	var req RefreshJWTRequest
	if err := json.NewDecoder(r.Body).Decode(&req); err != nil {
		writeError(w, "invalid request body", http.StatusBadRequest)
		return
	}
	resp, err := h.auth.RefreshRefreshToken(req.RefreshToken)
	if err != nil {
		writeError(w, err.Error(), http.StatusUnauthorized)
		return
	}
	writeJSON(w, JWTResponse{
		Type:         resp.Type,
		AccessToken:  resp.AccessToken,
		RefreshToken: resp.RefreshToken,
	})
}
