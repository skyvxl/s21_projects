package gui

import (
	"testing"

	"fyne.io/fyne/v2/widget"
)

func TestSetEndEntriesToMazeSize(t *testing.T) {
	t.Parallel()

	rowEntry := widget.NewEntry()
	colEntry := widget.NewEntry()
	rowEntry.SetText("10")
	colEntry.SetText("10")

	setEndEntriesToMazeSize(50, 50, rowEntry, colEntry)

	if rowEntry.Text != "50" {
		t.Fatalf("end row = %q, want 50", rowEntry.Text)
	}
	if colEntry.Text != "50" {
		t.Fatalf("end col = %q, want 50", colEntry.Text)
	}
}
