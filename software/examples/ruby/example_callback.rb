#!/usr/bin/env ruby
# -*- ruby encoding: utf-8 -*-

require 'tinkerforge/ip_connection'
require 'tinkerforge/bricklet_rotary_encoder_v2'

include Tinkerforge

HOST = 'localhost'
PORT = 4223
UID = 'XYZ' # Change XYZ to the UID of your Rotary Encoder Bricklet 2.0

ipcon = IPConnection.new # Create IP connection
re = BrickletRotaryEncoderV2.new UID, ipcon # Create device object

ipcon.connect HOST, PORT # Connect to brickd
# Don't use device before ipcon is connected

# Register count callback
re.register_callback(BrickletRotaryEncoderV2::CALLBACK_COUNT) do |count|
  puts "Count: #{count}"
end

# Set period for count callback to 1s (1000ms) without a threshold
re.set_count_callback_configuration 1000, false, 'x', 0, 0

puts 'Press key to exit'
$stdin.gets
ipcon.disconnect
