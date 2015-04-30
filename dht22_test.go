package dht22

import (
	"log"
	"testing"
)

func TestRead(t *testing.T) {
	temp, hum, err := Read(4)
	if err != nil {
		t.Error(err)
	}
	log.Printf("Temperature: %2.1fC, Humidity: %2.1f%%\n", temp, hum)
}
