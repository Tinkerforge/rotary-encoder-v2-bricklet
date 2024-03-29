package main

import (
	"fmt"
	"github.com/Tinkerforge/go-api-bindings/ipconnection"
	"github.com/Tinkerforge/go-api-bindings/rotary_encoder_v2_bricklet"
)

const ADDR string = "localhost:4223"
const UID string = "XYZ" // Change XYZ to the UID of your Rotary Encoder Bricklet 2.0.

func main() {
	ipcon := ipconnection.New()
	defer ipcon.Close()
	re, _ := rotary_encoder_v2_bricklet.New(UID, &ipcon) // Create device object.

	ipcon.Connect(ADDR) // Connect to brickd.
	defer ipcon.Disconnect()
	// Don't use device before ipcon is connected.

	// Get current count without reset.
	count, _ := re.GetCount(false)
	fmt.Printf("Count: %d\n", count)

	fmt.Print("Press enter to exit.")
	fmt.Scanln()
}
