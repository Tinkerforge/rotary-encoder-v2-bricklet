using System;
using Tinkerforge;

class Example
{
	private static string HOST = "localhost";
	private static int PORT = 4223;
	private static string UID = "XYZ"; // Change XYZ to the UID of your Rotary Encoder Bricklet 2.0

	// Callback function for count callback
	static void CountCB(BrickletRotaryEncoderV2 sender, int count)
	{
		Console.WriteLine("Count: " + count);
	}

	static void Main()
	{
		IPConnection ipcon = new IPConnection(); // Create IP connection
		BrickletRotaryEncoderV2 re =
		  new BrickletRotaryEncoderV2(UID, ipcon); // Create device object

		ipcon.Connect(HOST, PORT); // Connect to brickd
		// Don't use device before ipcon is connected

		// Register count callback to function CountCB
		re.CountCallback += CountCB;

		// Set period for count callback to 1s (1000ms) without a threshold
		re.SetCountCallbackConfiguration(1000, false, 'x', 0, 0);

		Console.WriteLine("Press enter to exit");
		Console.ReadLine();
		ipcon.Disconnect();
	}
}
