package web

import (
	"context"
	"net/http"
	"strings"

	"github.com/google/uuid"
)

func CORS(next http.Handler) http.Handler {
	return http.HandlerFunc(func(w http.ResponseWriter, r *http.Request) {
		w.Header().Set("Access-Control-Allow-Origin", "*")
		w.Header().Set("Access-Control-Allow-Methods", "GET, POST, OPTIONS")
		w.Header().Set("Access-Control-Allow-Headers", "Content-Type")
		if r.Method == http.MethodOptions {
			w.WriteHeader(http.StatusOK)
			return
		}
		next.ServeHTTP(w, r)
	})
}

type authenticator interface {
	ValidateAccessToken(string) error
	UserIDFromAccessToken(string) (uuid.UUID, error)
}

type userIDContextKey string

func userIDFromContext(ctx context.Context) (uuid.UUID, bool) {
	userID, ok := ctx.Value(contextKey).(uuid.UUID)
	return userID, ok
}

const contextKey userIDContextKey = "userID"

func UserAuthenticator(auth authenticator, next http.Handler) http.Handler {
	return http.HandlerFunc(func(w http.ResponseWriter, r *http.Request) {
		authHeader := r.Header.Get("Authorization")
		if authHeader == "" {
			writeError(w, "authorization header is required", http.StatusUnauthorized)
			return
		}

		if !strings.HasPrefix(authHeader, "Bearer ") {
			writeError(w, "invalid authorization scheme", http.StatusUnauthorized)
			return
		}

		accessToken := strings.TrimPrefix(authHeader, "Bearer ")

		err := auth.ValidateAccessToken(accessToken)
		if err != nil {
			writeError(w, "invalid access token", http.StatusUnauthorized)
			return
		}

		userID, err := auth.UserIDFromAccessToken(accessToken)
		if err != nil {
			writeError(w, "invalid access token", http.StatusUnauthorized)
			return
		}

		ctx := context.WithValue(r.Context(), contextKey, userID)
		next.ServeHTTP(w, r.WithContext(ctx))
	})
}
