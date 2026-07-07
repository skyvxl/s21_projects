-- name: UpsertUser :exec
INSERT INTO users (id, login, password, created_at)
VALUES ($1, $2, $3, $4)
ON CONFLICT (id) DO UPDATE SET login      = EXCLUDED.login,
                               password   = EXCLUDED.password,
                               created_at = EXCLUDED.created_at;

-- name: GetUserByLogin :one
SELECT id, login, password, created_at
FROM users
WHERE login = $1;

-- name: GetUserByID :one
SELECT id, login, password, created_at
FROM users
WHERE id = $1;

-- name: UpsertGame :exec
INSERT INTO games (id, player1_id, player2_id, board, status, current_turn, winner_id,
                   created_at, updated_at, player1_symbol, player2_symbol)
VALUES ($1, $2, $3, $4, $5, $6, $7, $8, $9, $10, $11)
ON CONFLICT (id) DO UPDATE SET player1_id     = EXCLUDED.player1_id,
                               player2_id     = EXCLUDED.player2_id,
                               board          = EXCLUDED.board,
                               status         = EXCLUDED.status,
                               current_turn   = EXCLUDED.current_turn,
                               winner_id      = EXCLUDED.winner_id,
                               created_at     = EXCLUDED.created_at,
                               updated_at     = EXCLUDED.updated_at,
                               player1_symbol = EXCLUDED.player1_symbol,
                               player2_symbol = EXCLUDED.player2_symbol;

-- name: GetGame :one
SELECT id,
       player1_id,
       player2_id,
       board,
       status,
       current_turn,
       winner_id,
       created_at,
       updated_at,
       player1_symbol,
       player2_symbol
FROM games
WHERE id = $1;

-- name: GetAvailableGames :many
SELECT id,
       player1_id,
       player2_id,
       board,
       status,
       current_turn,
       winner_id,
       created_at,
       updated_at,
       player1_symbol,
       player2_symbol
FROM games
WHERE status = 'waiting';

-- name: GetCompletedGamesByUserID :many
SELECT id,
       player1_id,
       player2_id,
       board,
       status,
       current_turn,
       winner_id,
       created_at,
       updated_at,
       player1_symbol,
       player2_symbol
FROM games
WHERE (status = 'draw' AND (player1_id = $1 OR player2_id = $1))
   OR (status = 'win' AND winner_id = $1);

-- name: GetTopPlayers :many
-- name: GetTopPlayers :many
SELECT u.id                AS user_id,
       u.login,
       COALESCE(
               COUNT(*) FILTER (WHERE g.winner_id = u.id)::double precision
                   / NULLIF(COUNT(*) FILTER (WHERE g.status = 'draw')
                                + COUNT(*) FILTER (WHERE g.status = 'win' AND g.winner_id != u.id),
                            0)::double precision,
               0::double precision
       )::double precision AS win_ratio
FROM users u
         LEFT JOIN games g
                   ON g.player1_id = u.id OR g.player2_id = u.id
GROUP BY u.id, u.login
ORDER BY win_ratio DESC
LIMIT $1;