#!/bin/sh
# Connects to localhost:4223 by default, use --host and --port to change this

uid=XYZ # Change XYZ to the UID of your Rotary Encoder Bricklet 2.0

# Handle incoming count callbacks
tinkerforge dispatch rotary-encoder-v2-bricklet $uid count &

# Set period for count callback to 1s (1000ms) without a threshold
tinkerforge call rotary-encoder-v2-bricklet $uid set-count-callback-configuration 1000 false threshold-option-off 0 0

echo "Press key to exit"; read dummy

kill -- -$$ # Stop callback dispatch in background
