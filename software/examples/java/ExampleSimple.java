import com.tinkerforge.IPConnection;
import com.tinkerforge.BrickletRotaryEncoderV2;

public class ExampleSimple {
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

		// Get current count without reset
		int count = re.getCount(false); // Can throw com.tinkerforge.TimeoutException
		System.out.println("Count: " + count);

		System.out.println("Press key to exit"); System.in.read();
		ipcon.disconnect();
	}
}
