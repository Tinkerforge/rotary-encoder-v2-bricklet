var Tinkerforge = require('tinkerforge');

var HOST = 'localhost';
var PORT = 4223;
var UID = 'XYZ'; // Change XYZ to the UID of your Rotary Encoder Bricklet 2.0

var ipcon = new Tinkerforge.IPConnection(); // Create IP connection
var re = new Tinkerforge.BrickletRotaryEncoderV2(UID, ipcon); // Create device object

ipcon.connect(HOST, PORT,
    function (error) {
        console.log('Error: ' + error);
    }
); // Connect to brickd
// Don't use device before ipcon is connected

ipcon.on(Tinkerforge.IPConnection.CALLBACK_CONNECTED,
    function (connectReason) {
        // Set period for count callback to 1s (1000ms) without a threshold
        re.setCountCallbackConfiguration(1000, false, 'x', 0, 0);
    }
);

// Register count callback
re.on(Tinkerforge.BrickletRotaryEncoderV2.CALLBACK_COUNT,
    // Callback function for count callback
    function (count) {
        console.log('Count: ' + count);
    }
);

console.log('Press key to exit');
process.stdin.on('data',
    function (data) {
        ipcon.disconnect();
        process.exit(0);
    }
);
