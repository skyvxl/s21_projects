package render

import (
	"maze/internal/cave"
	"maze/internal/maze"
	"image"
	"image/color"
	"image/draw"
	"math"
)

var (
	FieldColor    = color.NRGBA{R: 246, G: 248, B: 250, A: 255}
	WallColor     = color.NRGBA{R: 17, G: 24, B: 39, A: 255}
	PathColor     = color.NRGBA{R: 220, G: 38, B: 38, A: 255}
	CaveWallColor = color.NRGBA{R: 39, G: 39, B: 42, A: 255}
)

func Maze(m *maze.Maze, path []maze.Point, size int, wallWidth int) *image.RGBA {
	img := newCanvas(size)
	if m == nil || size <= 0 {
		return img
	}
	if wallWidth <= 0 {
		wallWidth = 1
	}

	drawRect(img, 0, 0, size, wallWidth, WallColor)
	drawRect(img, 0, 0, wallWidth, size, WallColor)
	drawRect(img, size-wallWidth, 0, size, size, WallColor)
	drawRect(img, 0, size-wallWidth, size, size, WallColor)

	for row := 0; row < m.Rows; row++ {
		for col := 0; col < m.Cols; col++ {
			cell := m.Cell(row, col)
			x1, y1, x2, y2 := cellRect(row, col, m.Rows, m.Cols, size)
			if cell.RightWall && col+1 < m.Cols {
				x := x2 - wallWidth/2
				drawRect(img, x, y1, x+wallWidth, y2, WallColor)
			}
			if cell.BottomWall && row+1 < m.Rows {
				y := y2 - wallWidth/2
				drawRect(img, x1, y, x2, y+wallWidth, WallColor)
			}
		}
	}

	for i := 1; i < len(path); i++ {
		x1, y1 := cellCenter(path[i-1].Row, path[i-1].Col, m.Rows, m.Cols, size)
		x2, y2 := cellCenter(path[i].Row, path[i].Col, m.Rows, m.Cols, size)
		drawLine(img, x1, y1, x2, y2, wallWidth, PathColor)
	}

	return img
}

func Cave(c *cave.Cave, size int) *image.RGBA {
	img := newCanvas(size)
	if c == nil || size <= 0 {
		return img
	}

	for row := 0; row < c.Rows; row++ {
		for col := 0; col < c.Cols; col++ {
			if !c.Alive(row, col) {
				continue
			}
			x1, y1, x2, y2 := cellRect(row, col, c.Rows, c.Cols, size)
			drawRect(img, x1, y1, x2, y2, CaveWallColor)
		}
	}
	return img
}

func newCanvas(size int) *image.RGBA {
	if size <= 0 {
		size = 1
	}
	img := image.NewRGBA(image.Rect(0, 0, size, size))
	draw.Draw(img, img.Bounds(), &image.Uniform{C: FieldColor}, image.Point{}, draw.Src)
	return img
}

func cellRect(row, col, rows, cols, size int) (int, int, int, int) {
	x1 := int(math.Round(float64(col) * float64(size) / float64(cols)))
	y1 := int(math.Round(float64(row) * float64(size) / float64(rows)))
	x2 := int(math.Round(float64(col+1) * float64(size) / float64(cols)))
	y2 := int(math.Round(float64(row+1) * float64(size) / float64(rows)))
	return x1, y1, x2, y2
}

func cellCenter(row, col, rows, cols, size int) (int, int) {
	x1, y1, x2, y2 := cellRect(row, col, rows, cols, size)
	return (x1 + x2) / 2, (y1 + y2) / 2
}

func drawRect(img *image.RGBA, x1, y1, x2, y2 int, c color.NRGBA) {
	bounds := img.Bounds()
	rect := image.Rect(
		clamp(x1, bounds.Min.X, bounds.Max.X),
		clamp(y1, bounds.Min.Y, bounds.Max.Y),
		clamp(x2, bounds.Min.X, bounds.Max.X),
		clamp(y2, bounds.Min.Y, bounds.Max.Y),
	)
	if rect.Empty() {
		return
	}
	draw.Draw(img, rect, &image.Uniform{C: c}, image.Point{}, draw.Src)
}

func drawLine(img *image.RGBA, x1, y1, x2, y2, width int, c color.NRGBA) {
	if width <= 0 {
		width = 1
	}

	dx := abs(x2 - x1)
	dy := -abs(y2 - y1)
	stepX := -1
	if x1 < x2 {
		stepX = 1
	}
	stepY := -1
	if y1 < y2 {
		stepY = 1
	}

	err := dx + dy
	for {
		half := width / 2
		drawRect(img, x1-half, y1-half, x1-half+width, y1-half+width, c)
		if x1 == x2 && y1 == y2 {
			return
		}
		twiceErr := 2 * err
		if twiceErr >= dy {
			err += dy
			x1 += stepX
		}
		if twiceErr <= dx {
			err += dx
			y1 += stepY
		}
	}
}

func clamp(value, minValue, maxValue int) int {
	if value < minValue {
		return minValue
	}
	if value > maxValue {
		return maxValue
	}
	return value
}

func abs(value int) int {
	if value < 0 {
		return -value
	}
	return value
}
