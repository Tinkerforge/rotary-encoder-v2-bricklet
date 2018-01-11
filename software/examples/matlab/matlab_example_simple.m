function matlab_example_simple()
    import com.tinkerforge.IPConnection;
    import com.tinkerforge.BrickletRotaryEncoderV2;

    HOST = 'localhost';
    PORT = 4223;
    UID = 'XYZ'; % Change XYZ to the UID of your Rotary Encoder Bricklet 2.0

    ipcon = IPConnection(); % Create IP connection
    re = handle(BrickletRotaryEncoderV2(UID, ipcon), 'CallbackProperties'); % Create device object

    ipcon.connect(HOST, PORT); % Connect to brickd
    % Don't use device before ipcon is connected

    % Get current count without reset
    count = re.getCount(false);
    fprintf('Count: %i\n', count);

    input('Press key to exit\n', 's');
    ipcon.disconnect();
end
