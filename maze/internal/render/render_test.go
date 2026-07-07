package render_test

import (
	"maze/internal/cave"
	"maze/internal/maze"
	"maze/internal/render"
	"image/color"
	"testing"
)

func TestRenderMazeUsesRequestedSizeAndPathColor(t *testing.T) {
	t.Parallel()

	m, err := maze.New(1, 2)
	if err != nil {
		t.Fatalf("new maze: %v", err)
	}
	m.SetRightWall(0, 0, false)
	m.SetRightWall(0, 1, true)
	m.SetBottomWall(0, 0, true)
	m.SetBottomWall(0, 1, true)

	img := render.Maze(m, []maze.Point{{Row: 0, Col: 0}, {Row: 0, Col: 1}}, 500, 2)
	if got := img.Bounds().Dx(); got != 500 {
		t.Fatalf("width = %d, want 500", got)
	}
	if got := img.Bounds().Dy(); got != 500 {
		t.Fatalf("height = %d, want 500", got)
	}

	if !sameColor(img.At(0, 0), render.WallColor) {
		t.Fatal("top-left border pixel should be wall color")
	}
	if !sameColor(img.At(250, 250), render.PathColor) {
		t.Fatal("path midpoint should be path color")
	}
}

func TestRenderCaveUsesAliveCells(t *testing.T) {
	t.Parallel()

	c, err := cave.FromCells([][]bool{{true, false}})
	if err != nil {
		t.Fatalf("from cells: %v", err)
	}
	img := render.Cave(c, 500)

	if !sameColor(img.At(125, 250), render.CaveWallColor) {
		t.Fatal("alive cell should be cave wall color")
	}
	if !sameColor(img.At(375, 250), render.FieldColor) {
		t.Fatal("dead cell should be field color")
	}
}

func sameColor(got color.Color, want color.NRGBA) bool {
	r, g, b, a := got.RGBA()
	return uint8(r>>8) == want.R &&
		uint8(g>>8) == want.G &&
		uint8(b>>8) == want.B &&
		uint8(a>>8) == want.A
}
