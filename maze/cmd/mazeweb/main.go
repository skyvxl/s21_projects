package main

import (
	"flag"
	"log"
	"maze/internal/web"
	"net/http"
	"time"
)

func main() {
	addr := flag.String("addr", ":8080", "address to listen on")
	flag.Parse()

	server := &http.Server{
		Addr:              *addr,
		Handler:           web.NewApp().Handler(),
		ReadHeaderTimeout: 5 * time.Second,
		ReadTimeout:       15 * time.Second,
		WriteTimeout:      15 * time.Second,
	}

	log.Printf("maze web interface listening on %s", *addr)
	if err := server.ListenAndServe(); err != nil {
		log.Fatal(err)
	}
}
