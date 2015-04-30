package main

import (
	"flag"
	"log"
	"strconv"

	"github.com/gerp/dht22"
)

func main() {
	flag.Parse()

	pin, err := strconv.ParseInt(flag.Arg(0), 0, 32)
	if err != nil {
		log.Fatalln("Please specify a valid pin number.")
	}

	temp, hum, err := dht22.Read(int(pin))
	if err != nil {
		log.Println(err)
	}

	log.Printf("Temperature: %2.1fC, Humidity: %2.1f%%\n", temp, hum)
}
