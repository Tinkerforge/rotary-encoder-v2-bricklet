#!/usr/bin/perl

use strict;
use Tinkerforge::IPConnection;
use Tinkerforge::BrickletRotaryEncoderV2;

use constant HOST => 'localhost';
use constant PORT => 4223;
use constant UID => 'XYZ'; # Change XYZ to the UID of your Rotary Encoder Bricklet 2.0

# Callback subroutine for count callback
sub cb_count
{
    my ($count) = @_;

    print "Count: $count\n";
}

my $ipcon = Tinkerforge::IPConnection->new(); # Create IP connection
my $re = Tinkerforge::BrickletRotaryEncoderV2->new(&UID, $ipcon); # Create device object

$ipcon->connect(&HOST, &PORT); # Connect to brickd
# Don't use device before ipcon is connected

# Register count callback to subroutine cb_count
$re->register_callback($re->CALLBACK_COUNT, 'cb_count');

# Set period for count callback to 1s (1000ms) without a threshold
$re->set_count_callback_configuration(1000, 0, 'x', 0, 0);

print "Press key to exit\n";
<STDIN>;
$ipcon->disconnect();
