<?php

require_once('Tinkerforge/IPConnection.php');
require_once('Tinkerforge/BrickletRotaryEncoderV2.php');

use Tinkerforge\IPConnection;
use Tinkerforge\BrickletRotaryEncoderV2;

const HOST = 'localhost';
const PORT = 4223;
const UID = 'XYZ'; // Change XYZ to the UID of your Rotary Encoder Bricklet 2.0

// Callback function for count callback
function cb_count($count)
{
    echo "Count: $count\n";
}

$ipcon = new IPConnection(); // Create IP connection
$re = new BrickletRotaryEncoderV2(UID, $ipcon); // Create device object

$ipcon->connect(HOST, PORT); // Connect to brickd
// Don't use device before ipcon is connected

// Register count callback to function cb_count
$re->registerCallback(BrickletRotaryEncoderV2::CALLBACK_COUNT, 'cb_count');

// Set period for count callback to 1s (1000ms) without a threshold
$re->setCountCallbackConfiguration(1000, FALSE, 'x', 0, 0);

echo "Press ctrl+c to exit\n";
$ipcon->dispatchCallbacks(-1); // Dispatch callbacks forever

?>
