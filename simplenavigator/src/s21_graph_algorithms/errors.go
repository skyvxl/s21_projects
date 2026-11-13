package s21_graph_algorithms

import "errors"

var (
	ErrNilGraph           = errors.New("graph is nil")
	ErrNoPath             = errors.New("no path")
	ErrNoSpanningTree     = errors.New("no spanning tree")
	ErrDirectedGraph      = errors.New("directed graph")
	ErrNoTour             = errors.New("no tour")
	ErrDistanceOverflow   = errors.New("path distance exceeds int range")
	ErrBruteForceTooLarge = errors.New("graph exceeds brute-force vertex limit")
)
