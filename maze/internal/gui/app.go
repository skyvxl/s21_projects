package gui

import (
	"maze/internal/cave"
	"maze/internal/eller"
	"maze/internal/maze"
	"maze/internal/render"
	"maze/internal/solver"
	"fmt"
	"image"
	"math/rand"
	"strconv"
	"sync"
	"time"

	"fyne.io/fyne/v2"
	fyneapp "fyne.io/fyne/v2/app"
	"fyne.io/fyne/v2/canvas"
	"fyne.io/fyne/v2/container"
	"fyne.io/fyne/v2/dialog"
	"fyne.io/fyne/v2/storage"
	"fyne.io/fyne/v2/theme"
	"fyne.io/fyne/v2/widget"
)

const (
	canvasSize = 500
	wallWidth  = 2
)

type App struct {
	window fyne.Window

	currentMaze *maze.Maze
	currentPath []maze.Point
	mazeImage   *canvas.Image

	currentCave *cave.Cave
	caveImage   *canvas.Image

	autoMu   sync.Mutex
	autoStop chan struct{}
}

func Run() {
	a := fyneapp.New()
	window := a.NewWindow("Maze")
	window.Resize(fyne.NewSize(760, 590))

	app := &App{window: window}
	app.mazeImage = imageView(render.Maze(nil, nil, canvasSize, wallWidth))
	app.caveImage = imageView(render.Cave(nil, canvasSize))

	window.SetContent(container.NewAppTabs(
		container.NewTabItem("Maze", app.mazeTab()),
		container.NewTabItem("Cave", app.caveTab()),
	))
	window.SetOnClosed(app.stopAuto)
	window.ShowAndRun()
}

func (a *App) mazeTab() fyne.CanvasObject {
	rowsEntry := entry("10")
	colsEntry := entry("10")
	startRowEntry := entry("1")
	startColEntry := entry("1")
	endRowEntry := entry("10")
	endColEntry := entry("10")

	loadButton := widget.NewButtonWithIcon("Load", theme.FolderOpenIcon(), func() {
		a.loadMaze(func(m *maze.Maze) {
			setEndEntriesToMazeSize(m.Rows, m.Cols, endRowEntry, endColEntry)
		})
	})
	saveButton := widget.NewButtonWithIcon("Save", theme.DocumentSaveIcon(), a.saveMaze)
	generateButton := widget.NewButtonWithIcon("Generate", theme.ContentAddIcon(), func() {
		rows, err := intFromEntry("rows", rowsEntry, 1, maze.MaxSize)
		if err != nil {
			dialog.ShowError(err, a.window)
			return
		}
		cols, err := intFromEntry("columns", colsEntry, 1, maze.MaxSize)
		if err != nil {
			dialog.ShowError(err, a.window)
			return
		}

		generated, err := eller.Generate(rows, cols, seededRand())
		if err != nil {
			dialog.ShowError(err, a.window)
			return
		}
		a.currentMaze = generated
		a.currentPath = nil
		setEndEntriesToMazeSize(rows, cols, endRowEntry, endColEntry)
		a.refreshMaze()
	})
	solveButton := widget.NewButtonWithIcon("Solve", theme.SearchIcon(), func() {
		if a.currentMaze == nil {
			dialog.ShowError(fmt.Errorf("load or generate a maze first"), a.window)
			return
		}
		start, err := pointFromEntries("start", startRowEntry, startColEntry, a.currentMaze)
		if err != nil {
			dialog.ShowError(err, a.window)
			return
		}
		end, err := pointFromEntries("end", endRowEntry, endColEntry, a.currentMaze)
		if err != nil {
			dialog.ShowError(err, a.window)
			return
		}

		path, err := solver.Solve(a.currentMaze, start, end)
		if err != nil {
			dialog.ShowError(err, a.window)
			return
		}
		a.currentPath = path
		a.refreshMaze()
	})

	controls := container.NewVBox(
		widget.NewLabel("Generator"),
		widget.NewForm(
			widget.NewFormItem("Rows", rowsEntry),
			widget.NewFormItem("Columns", colsEntry),
		),
		generateButton,
		widget.NewSeparator(),
		widget.NewLabel("File"),
		container.NewGridWithColumns(2, loadButton, saveButton),
		widget.NewSeparator(),
		widget.NewLabel("Solution"),
		widget.NewForm(
			widget.NewFormItem("Start row", startRowEntry),
			widget.NewFormItem("Start col", startColEntry),
			widget.NewFormItem("End row", endRowEntry),
			widget.NewFormItem("End col", endColEntry),
		),
		solveButton,
	)

	return container.NewBorder(nil, nil, fixedControls(controls), nil, fixedCanvas(a.mazeImage))
}

func (a *App) caveTab() fyne.CanvasObject {
	rowsEntry := entry("30")
	colsEntry := entry("30")
	chanceEntry := entry("45")
	birthEntry := entry("4")
	deathEntry := entry("3")
	delayEntry := entry("250")

	loadButton := widget.NewButtonWithIcon("Load", theme.FolderOpenIcon(), a.loadCave)
	saveButton := widget.NewButtonWithIcon("Save", theme.DocumentSaveIcon(), a.saveCave)
	generateButton := widget.NewButtonWithIcon("Generate", theme.ContentAddIcon(), func() {
		rows, err := intFromEntry("rows", rowsEntry, 1, cave.MaxSize)
		if err != nil {
			dialog.ShowError(err, a.window)
			return
		}
		cols, err := intFromEntry("columns", colsEntry, 1, cave.MaxSize)
		if err != nil {
			dialog.ShowError(err, a.window)
			return
		}
		chance, err := intFromEntry("initial chance", chanceEntry, 0, 100)
		if err != nil {
			dialog.ShowError(err, a.window)
			return
		}

		generated, err := cave.Generate(rows, cols, chance, seededRand())
		if err != nil {
			dialog.ShowError(err, a.window)
			return
		}
		a.currentCave = generated
		a.refreshCave()
	})
	stepButton := widget.NewButtonWithIcon("Next", theme.NavigateNextIcon(), func() {
		a.stepCave(birthEntry, deathEntry)
	})

	autoButton := widget.NewButtonWithIcon("Auto", theme.MediaPlayIcon(), nil)
	autoButton.OnTapped = func() {
		a.autoMu.Lock()
		running := a.autoStop != nil
		a.autoMu.Unlock()
		if running {
			a.stopAuto()
			autoButton.SetText("Auto")
			autoButton.SetIcon(theme.MediaPlayIcon())
			return
		}

		delay, err := intFromEntry("delay", delayEntry, 10, 60000)
		if err != nil {
			dialog.ShowError(err, a.window)
			return
		}
		a.startAuto(time.Duration(delay)*time.Millisecond, birthEntry, deathEntry, autoButton)
		autoButton.SetText("Stop")
		autoButton.SetIcon(theme.MediaStopIcon())
	}

	controls := container.NewVBox(
		widget.NewLabel("Generator"),
		widget.NewForm(
			widget.NewFormItem("Rows", rowsEntry),
			widget.NewFormItem("Columns", colsEntry),
			widget.NewFormItem("Chance %", chanceEntry),
		),
		generateButton,
		widget.NewSeparator(),
		widget.NewLabel("File"),
		container.NewGridWithColumns(2, loadButton, saveButton),
		widget.NewSeparator(),
		widget.NewLabel("Automaton"),
		widget.NewForm(
			widget.NewFormItem("Birth", birthEntry),
			widget.NewFormItem("Death", deathEntry),
			widget.NewFormItem("Delay ms", delayEntry),
		),
		container.NewGridWithColumns(2, stepButton, autoButton),
	)

	return container.NewBorder(nil, nil, fixedControls(controls), nil, fixedCanvas(a.caveImage))
}

func (a *App) loadMaze(afterLoad func(*maze.Maze)) {
	fileDialog := dialog.NewFileOpen(func(reader fyne.URIReadCloser, err error) {
		if err != nil {
			dialog.ShowError(err, a.window)
			return
		}
		if reader == nil {
			return
		}
		defer reader.Close()

		loaded, err := maze.Parse(reader)
		if err != nil {
			dialog.ShowError(err, a.window)
			return
		}
		a.currentMaze = loaded
		a.currentPath = nil
		if afterLoad != nil {
			afterLoad(loaded)
		}
		a.refreshMaze()
	}, a.window)
	configureTextDialog(fileDialog)
	fileDialog.Show()
}

func (a *App) saveMaze() {
	if a.currentMaze == nil {
		dialog.ShowError(fmt.Errorf("there is no maze to save"), a.window)
		return
	}

	fileDialog := dialog.NewFileSave(func(writer fyne.URIWriteCloser, err error) {
		if err != nil {
			dialog.ShowError(err, a.window)
			return
		}
		if writer == nil {
			return
		}
		defer writer.Close()

		if err := a.currentMaze.Write(writer); err != nil {
			dialog.ShowError(err, a.window)
		}
	}, a.window)
	configureTextDialog(fileDialog)
	fileDialog.SetFileName("maze.txt")
	fileDialog.Show()
}

func (a *App) loadCave() {
	fileDialog := dialog.NewFileOpen(func(reader fyne.URIReadCloser, err error) {
		if err != nil {
			dialog.ShowError(err, a.window)
			return
		}
		if reader == nil {
			return
		}
		defer reader.Close()

		loaded, err := cave.Parse(reader)
		if err != nil {
			dialog.ShowError(err, a.window)
			return
		}
		a.currentCave = loaded
		a.refreshCave()
	}, a.window)
	configureTextDialog(fileDialog)
	fileDialog.Show()
}

func (a *App) saveCave() {
	if a.currentCave == nil {
		dialog.ShowError(fmt.Errorf("there is no cave to save"), a.window)
		return
	}

	fileDialog := dialog.NewFileSave(func(writer fyne.URIWriteCloser, err error) {
		if err != nil {
			dialog.ShowError(err, a.window)
			return
		}
		if writer == nil {
			return
		}
		defer writer.Close()

		if err := a.currentCave.Write(writer); err != nil {
			dialog.ShowError(err, a.window)
		}
	}, a.window)
	configureTextDialog(fileDialog)
	fileDialog.SetFileName("cave.txt")
	fileDialog.Show()
}

func (a *App) stepCave(birthEntry, deathEntry *widget.Entry) {
	if a.currentCave == nil {
		dialog.ShowError(fmt.Errorf("load or generate a cave first"), a.window)
		return
	}
	birth, err := intFromEntry("birth limit", birthEntry, 0, 7)
	if err != nil {
		dialog.ShowError(err, a.window)
		return
	}
	death, err := intFromEntry("death limit", deathEntry, 0, 7)
	if err != nil {
		dialog.ShowError(err, a.window)
		return
	}

	next, err := a.currentCave.Step(birth, death)
	if err != nil {
		dialog.ShowError(err, a.window)
		return
	}
	a.currentCave = next
	a.refreshCave()
}

func (a *App) startAuto(delay time.Duration, birthEntry, deathEntry *widget.Entry, autoButton *widget.Button) {
	a.stopAuto()

	stop := make(chan struct{})
	a.autoMu.Lock()
	a.autoStop = stop
	a.autoMu.Unlock()

	go func() {
		ticker := time.NewTicker(delay)
		defer ticker.Stop()

		for {
			select {
			case <-ticker.C:
				fyne.Do(func() {
					if a.currentCave == nil {
						dialog.ShowError(fmt.Errorf("load or generate a cave first"), a.window)
						a.stopAuto()
						autoButton.SetText("Auto")
						autoButton.SetIcon(theme.MediaPlayIcon())
						return
					}
					a.stepCave(birthEntry, deathEntry)
				})
			case <-stop:
				return
			}
		}
	}()
}

func (a *App) stopAuto() {
	a.autoMu.Lock()
	defer a.autoMu.Unlock()
	if a.autoStop != nil {
		close(a.autoStop)
		a.autoStop = nil
	}
}

func (a *App) refreshMaze() {
	a.mazeImage.Image = render.Maze(a.currentMaze, a.currentPath, canvasSize, wallWidth)
	a.mazeImage.Refresh()
}

func (a *App) refreshCave() {
	a.caveImage.Image = render.Cave(a.currentCave, canvasSize)
	a.caveImage.Refresh()
}

func entry(value string) *widget.Entry {
	e := widget.NewEntry()
	e.SetText(value)
	return e
}

func imageView(img image.Image) *canvas.Image {
	view := canvas.NewImageFromImage(img)
	view.FillMode = canvas.ImageFillContain
	view.SetMinSize(fyne.NewSize(canvasSize, canvasSize))
	return view
}

func fixedCanvas(img *canvas.Image) fyne.CanvasObject {
	img.FillMode = canvas.ImageFillContain
	img.SetMinSize(fyne.NewSize(canvasSize, canvasSize))
	return container.NewGridWrap(fyne.NewSize(canvasSize, canvasSize), img)
}

func fixedControls(content fyne.CanvasObject) fyne.CanvasObject {
	return container.NewGridWrap(fyne.NewSize(230, 540), content)
}

func intFromEntry(name string, entry *widget.Entry, minValue, maxValue int) (int, error) {
	value, err := strconv.Atoi(entry.Text)
	if err != nil {
		return 0, fmt.Errorf("%s must be an integer", name)
	}
	if value < minValue || value > maxValue {
		return 0, fmt.Errorf("%s must be from %d to %d", name, minValue, maxValue)
	}
	return value, nil
}

func pointFromEntries(name string, rowEntry, colEntry *widget.Entry, m *maze.Maze) (maze.Point, error) {
	row, err := intFromEntry(name+" row", rowEntry, 1, m.Rows)
	if err != nil {
		return maze.Point{}, err
	}
	col, err := intFromEntry(name+" column", colEntry, 1, m.Cols)
	if err != nil {
		return maze.Point{}, err
	}
	return maze.Point{Row: row - 1, Col: col - 1}, nil
}

func setEndEntriesToMazeSize(rows, cols int, rowEntry, colEntry *widget.Entry) {
	rowEntry.SetText(strconv.Itoa(rows))
	colEntry.SetText(strconv.Itoa(cols))
}

func configureTextDialog(fileDialog *dialog.FileDialog) {
	fileDialog.SetFilter(storage.NewExtensionFileFilter([]string{".txt"}))
}

func seededRand() *rand.Rand {
	return rand.New(rand.NewSource(time.Now().UnixNano()))
}
