function octave_example_callback()
    more off;

    HOST = "localhost";
    PORT = 4223;
    UID = "XYZ"; % Change XYZ to the UID of your Rotary Encoder Bricklet 2.0

    ipcon = javaObject("com.tinkerforge.IPConnection"); % Create IP connection
    re = javaObject("com.tinkerforge.BrickletRotaryEncoderV2", UID, ipcon); % Create device object

    ipcon.connect(HOST, PORT); % Connect to brickd
    % Don't use device before ipcon is connected

    % Register count callback to function cb_count
    re.addCountCallback(@cb_count);

    % Set period for count callback to 1s (1000ms) without a threshold
    re.setCountCallbackConfiguration(1000, false, "x", 0, 0);

    input("Press key to exit\n", "s");
    ipcon.disconnect();
end

% Callback function for count callback
function cb_count(e)
    fprintf("Count: %d\n", e.count);
end
