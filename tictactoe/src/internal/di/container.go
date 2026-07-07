package di

import (
	"context"
	"errors"
	"log/slog"
	"net/http"
	"time"

	"tictactoe/src/internal/datasource"
	"tictactoe/src/internal/domain"
	"tictactoe/src/internal/web"

	jwtprovider "tictactoe/src/internal/jwt"

	"github.com/jackc/pgx/v5/pgxpool"
	"go.uber.org/fx"
)

func NewApp() *fx.App {
	return fx.New(
		fx.Provide(
			newDBPool,
			datasource.NewGameRepository,
			datasource.NewUserRepository,
			domain.NewAuthService,
			domain.NewGameService,
			web.NewGameHandler,
			web.NewAuthHandler,
			newJWTProvider,
			newHTTPServer,
		),
		fx.Invoke(registerRoutes),
	)
}

func newDBPool(lc fx.Lifecycle) (*pgxpool.Pool, error) {
	pool, err := datasource.NewDBConnection()
	if err != nil {
		return nil, err
	}
	lc.Append(fx.Hook{
		OnStop: func(ctx context.Context) error {
			slog.Info("closing database connection")
			pool.Close()
			return nil
		},
	})
	return pool, nil
}

func newJWTProvider() domain.JWTProvider {
	return jwtprovider.NewJWTProvider(
		"secret",
		"secret",
		15*time.Second,
		2*time.Minute,
	)
}

func newHTTPServer(lc fx.Lifecycle) *http.ServeMux {
	mux := http.NewServeMux()
	server := &http.Server{
		Addr:              ":8080",
		Handler:           web.CORS(mux),
		ReadHeaderTimeout: 5 * time.Second,
	}
	lc.Append(fx.Hook{
		OnStart: func(ctx context.Context) error {
			slog.Info("server starting on http://localhost:8080")
			go func() {
				if err := server.ListenAndServe(); err != nil && !errors.Is(err, http.ErrServerClosed) {
					slog.Error("server error", "error", err)
					panic(err)
				}
			}()
			return nil
		},
		OnStop: func(ctx context.Context) error {
			return server.Shutdown(ctx)
		},
	})
	return mux
}

func registerRoutes(
	mux *http.ServeMux,
	handler *web.GameHandler,
	authHandler *web.AuthHandler,
	jwtProvider domain.JWTProvider) {

	authHandler.RegisterRoutes(mux)
	mux.Handle("/leaderboard", web.UserAuthenticator(jwtProvider, handler.GetLeaderboardHandler()))
	mux.Handle("/games/history", web.UserAuthenticator(jwtProvider, handler.GetCompletedGamesByUserIDHandler()))
	mux.Handle("/games", web.UserAuthenticator(jwtProvider, handler.GetAvailableGamesHandler()))
	mux.Handle("/game/create", web.UserAuthenticator(jwtProvider, handler.CreateGameHandler()))
	mux.Handle("/game/", web.UserAuthenticator(jwtProvider, handler.Handler()))
	mux.Handle("/user/me", web.UserAuthenticator(jwtProvider, authHandler.GetUserHandler()))
	mux.Handle("/", http.FileServer(http.Dir("src/static")))
}
