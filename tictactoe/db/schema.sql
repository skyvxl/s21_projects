CREATE TABLE IF NOT EXISTS users
(
    id         UUID PRIMARY KEY      DEFAULT gen_random_uuid(),
    login      VARCHAR(255) NOT NULL UNIQUE,
    password   VARCHAR(255) NOT NULL,
    created_at TIMESTAMPTZ  NOT NULL DEFAULT NOW()
);

CREATE TABLE IF NOT EXISTS games
(
    id             UUID PRIMARY KEY     DEFAULT gen_random_uuid(),
    player1_id     UUID        NOT NULL REFERENCES users (id) ON DELETE CASCADE,
    player2_id     UUID REFERENCES users (id) ON DELETE CASCADE,
    board          JSONB       NOT NULL,
    status         VARCHAR(50) NOT NULL,
    current_turn   UUID,
    winner_id      UUID,
    created_at     TIMESTAMPTZ NOT NULL DEFAULT NOW(),
    updated_at     TIMESTAMPTZ NOT NULL DEFAULT NOW(),
    player1_symbol INTEGER     NOT NULL DEFAULT 1,
    player2_symbol INTEGER     NOT NULL DEFAULT -1
);
