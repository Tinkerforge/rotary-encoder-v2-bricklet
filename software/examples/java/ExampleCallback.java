import com.tinkerforge.IPConnection;
import com.tinkerforge.BrickletRotaryEncoderV2;

public class ExampleCallback {
	private static final String HOST = "localhost";
	private static final int PORT = 4223;

	// Change XYZ to the UID of your Rotary Encoder Bricklet 2.0
	private static final String UID = "XYZ";

	// Note: To make the example code cleaner we do not handle exceptions. Exceptions
	//       you might normally want to catch are described in the documentation
	public static void main(String args[]) throws Exception {
		IPConnection ipcon = new IPConnection(); // Create IP connection
		BrickletRotaryEncoderV2 re =
		  new BrickletRotaryEncoderV2(UID, ipcon); // Create device object

		ipcon.connect(HOST, PORT); // Connect to brickd
		// Don't use device before ipcon is connected

		// Add count listener
		re.addCountListener(new BrickletRotaryEncoderV2.CountListener() {
			public void count(int count) {
				System.out.println("Count: " + count);
			}
		});

		// Set period for count callback to 1s (1000ms) without a threshold
		re.setCountCallbackConfiguration(1000, false, 'x', 0, 0);

		System.out.println("Press key to exit"); System.in.read();
		ipcon.disconnect();
	}
}
