// Read temperature and humidity data from a DHT22 sensor on a Raspberry PI
package dht22

/*
#cgo CFLAGS: -std=c99 -D _BSD_SOURCE
#cgo LDFLAGS: -lbcm2835
#include "dht22.h"
*/
import "C"
import (
	"errors"
	"fmt"
	"unsafe"
)

func Read(pin int) (t, h float32, err error) {
	rs := C.readDHT(C.int(pin), (*C.float)(unsafe.Pointer(&t)), (*C.float)(unsafe.Pointer(&h)))
	if rs > 0 {
		err = errors.New(fmt.Sprintf("Error reading data on pin #%d", pin))
	}

	return
}
