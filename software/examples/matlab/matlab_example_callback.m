function matlab_example_callback()
    import com.tinkerforge.IPConnection;
    import com.tinkerforge.BrickletRotaryEncoderV2;

    HOST = 'localhost';
    PORT = 4223;
    UID = 'XYZ'; % Change XYZ to the UID of your Rotary Encoder Bricklet 2.0

    ipcon = IPConnection(); % Create IP connection
    re = handle(BrickletRotaryEncoderV2(UID, ipcon), 'CallbackProperties'); % Create device object

    ipcon.connect(HOST, PORT); % Connect to brickd
    % Don't use device before ipcon is connected

    % Register count callback to function cb_count
    set(re, 'CountCallback', @(h, e) cb_count(e));

    % Set period for count callback to 1s (1000ms) without a threshold
    re.setCountCallbackConfiguration(1000, false, 'x', 0, 0);

    input('Press key to exit\n', 's');
    ipcon.disconnect();
end

% Callback function for count callback
function cb_count(e)
    fprintf('Count: %i\n', e.count);
end
