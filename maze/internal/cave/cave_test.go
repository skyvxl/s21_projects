package cave_test

import (
	"maze/internal/cave"
	"bytes"
	"math/rand"
	"strings"
	"testing"
)

func TestParseAndWriteCave(t *testing.T) {
	t.Parallel()

	input := `3 3
0 1 0
1 0 1
0 1 0
`
	c, err := cave.Parse(strings.NewReader(input))
	if err != nil {
		t.Fatalf("parse cave: %v", err)
	}
	if c.Rows != 3 || c.Cols != 3 {
		t.Fatalf("size = %dx%d, want 3x3", c.Rows, c.Cols)
	}
	if !c.Alive(0, 1) || c.Alive(1, 1) {
		t.Fatal("parsed alive/dead cells incorrectly")
	}

	var out bytes.Buffer
	if err := c.Write(&out); err != nil {
		t.Fatalf("write cave: %v", err)
	}
	roundTrip, err := cave.Parse(&out)
	if err != nil {
		t.Fatalf("parse written cave: %v", err)
	}
	if !roundTrip.Alive(1, 0) || roundTrip.Alive(2, 0) {
		t.Fatal("round-trip cave lost cell data")
	}
}

func TestCaveStepTreatsOuterCellsAsAlive(t *testing.T) {
	t.Parallel()

	c, err := cave.FromCells([][]bool{
		{false, false, false},
		{false, true, false},
		{false, false, false},
	})
	if err != nil {
		t.Fatalf("from cells: %v", err)
	}

	next, err := c.Step(4, 2)
	if err != nil {
		t.Fatalf("step: %v", err)
	}

	if next.Alive(1, 1) {
		t.Fatal("center cell should die with zero alive neighbors and death limit 2")
	}
	if !next.Alive(0, 0) {
		t.Fatal("corner cell should be born because outside cells are alive")
	}
	if next.Alive(0, 1) {
		t.Fatal("edge cell should stay dead when alive neighbor count is not greater than birth limit")
	}
}

func TestGenerateCaveValidatesChanceAndLimits(t *testing.T) {
	t.Parallel()

	if _, err := cave.Generate(4, 5, 100, rand.New(rand.NewSource(1))); err != nil {
		t.Fatalf("generate full cave: %v", err)
	}
	if _, err := cave.Generate(4, 5, -1, rand.New(rand.NewSource(1))); err == nil {
		t.Fatal("expected invalid chance error")
	}
	c, err := cave.Generate(2, 2, 0, rand.New(rand.NewSource(1)))
	if err != nil {
		t.Fatalf("generate empty cave: %v", err)
	}
	if _, err := c.Step(-1, 2); err == nil {
		t.Fatal("expected invalid birth limit error")
	}
	if _, err := c.Step(2, 8); err == nil {
		t.Fatal("expected invalid death limit error")
	}
}
