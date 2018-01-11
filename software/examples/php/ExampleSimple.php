<?php

require_once('Tinkerforge/IPConnection.php');
require_once('Tinkerforge/BrickletRotaryEncoderV2.php');

use Tinkerforge\IPConnection;
use Tinkerforge\BrickletRotaryEncoderV2;

const HOST = 'localhost';
const PORT = 4223;
const UID = 'XYZ'; // Change XYZ to the UID of your Rotary Encoder Bricklet 2.0

$ipcon = new IPConnection(); // Create IP connection
$re = new BrickletRotaryEncoderV2(UID, $ipcon); // Create device object

$ipcon->connect(HOST, PORT); // Connect to brickd
// Don't use device before ipcon is connected

// Get current count without reset
$count = $re->getCount(FALSE);
echo "Count: $count\n";

echo "Press key to exit\n";
fgetc(fopen('php://stdin', 'r'));
$ipcon->disconnect();

?>
