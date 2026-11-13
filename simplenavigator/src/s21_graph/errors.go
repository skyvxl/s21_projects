package s21_graph

import "errors"

var (
	ErrEmptyGraph     = errors.New("graph is empty")
	ErrInvalidMatrix  = errors.New("invalid adjacency matrix")
	ErrNegativeWeight = errors.New("negative edge weight")
	ErrInvalidVertex  = errors.New("invalid vertex")
)
