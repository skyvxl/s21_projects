package main

import (
	"tictactoe/src/internal/di"
	"log"

	"github.com/joho/godotenv"
)

func main() {
	if err := godotenv.Load("./.env"); err != nil {
		log.Fatal("Error loading .env file: ", err)
	}

	app := di.NewApp()
	app.Run()
}
