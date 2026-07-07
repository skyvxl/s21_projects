package domain

import (
	"errors"
	"math"

	"github.com/google/uuid"
)

type gameService struct {
	repo GameRepository
}

func NewGameService(repo GameRepository) GameService {
	return &gameService{repo: repo}
}

func (s *gameService) GetNextMove(game *Game) (int, int, error) {
	bestScore := math.MinInt32
	bestRow, bestCol := -1, -1

	for i := range 3 {
		for j := range 3 {
			if game.Board[i][j] == Empty {
				game.Board[i][j] = PlayerO
				score := s.minimax(game.Board, 0, false)
				game.Board[i][j] = Empty
				if score > bestScore {
					bestScore = score
					bestRow = i
					bestCol = j
				}
			}
		}
	}
	if bestRow == -1 {
		return -1, -1, errors.New("no valid moves available")
	}
	return bestRow, bestCol, nil
}

func (s *gameService) ValidateBoard(oldBoard, newBoard Board) error {
	changes := 0
	for i := range 3 {
		for j := range 3 {
			if oldBoard[i][j] != Empty && oldBoard[i][j] != newBoard[i][j] {
				return errors.New("previous moves have been modified")
			}
			if oldBoard[i][j] != newBoard[i][j] {
				changes++
			}
		}
	}
	if changes != 1 {
		return errors.New("exactly one move must be made")
	}
	return nil
}

func (s *gameService) CheckGameEnd(game *Game) {
	winner := checkWinner(game.Board)
	if winner != Empty {
		game.Status = StatusWin
		game.CurrentTurn = nil
		if winner == game.Player1Symbol {
			game.WinnerID = &game.Player1ID
		} else if game.Player2ID != nil {
			game.WinnerID = game.Player2ID
		}
		return
	}
	if isBoardFull(game.Board) {
		game.Status = StatusDraw
		game.CurrentTurn = nil
		game.WinnerID = nil
		return
	}
	game.Status = StatusTurn
}

func (s *gameService) GetCompletedGamesByUserID(userID uuid.UUID) ([]*Game, error) {
	return s.repo.GetCompletedGamesByUserID(userID)
}

func (s *gameService) GetTopPlayers(limit int32) ([]LeaderboardEntry, error) {
	return s.repo.GetTopPlayers(limit)
}

func (s *gameService) minimax(board Board, depth int, isMaximizing bool) int {
	winner := checkWinner(board)
	if winner != Empty {
		if winner == PlayerO {
			return 10 - depth
		}
		return depth - 10
	}
	if isBoardFull(board) {
		return 0
	}
	if isMaximizing {
		best := math.MinInt32
		for i := range 3 {
			for j := range 3 {
				if board[i][j] == Empty {
					board[i][j] = PlayerO
					score := s.minimax(board, depth+1, false)
					board[i][j] = Empty
					if score > best {
						best = score
					}
				}
			}
		}
		return best
	}
	best := math.MaxInt32
	for i := range 3 {
		for j := range 3 {
			if board[i][j] == Empty {
				board[i][j] = PlayerX
				score := s.minimax(board, depth+1, true)
				board[i][j] = Empty
				if score < best {
					best = score
				}
			}
		}
	}
	return best
}

func checkWinner(board Board) int {
	for i := range 3 {
		if board[i][0] != Empty && board[i][0] == board[i][1] && board[i][1] == board[i][2] {
			return board[i][0]
		}
		if board[0][i] != Empty && board[0][i] == board[1][i] && board[1][i] == board[2][i] {
			return board[0][i]
		}
	}
	if board[0][0] != Empty && board[0][0] == board[1][1] && board[1][1] == board[2][2] {
		return board[0][0]
	}
	if board[0][2] != Empty && board[0][2] == board[1][1] && board[1][1] == board[2][0] {
		return board[0][2]
	}
	return Empty
}

func isBoardFull(board Board) bool {
	for i := range 3 {
		for j := range 3 {
			if board[i][j] == Empty {
				return false
			}
		}
	}
	return true
}
