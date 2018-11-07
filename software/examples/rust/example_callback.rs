use std::{error::Error, io, thread};
use tinkerforge::{ip_connection::IpConnection, rotary_encoder_v2_bricklet::*};

const HOST: &str = "localhost";
const PORT: u16 = 4223;
const UID: &str = "XYZ"; // Change XYZ to the UID of your Rotary Encoder Bricklet 2.0.

fn main() -> Result<(), Box<dyn Error>> {
    let ipcon = IpConnection::new(); // Create IP connection.
    let re = RotaryEncoderV2Bricklet::new(UID, &ipcon); // Create device object.

    ipcon.connect((HOST, PORT)).recv()??; // Connect to brickd.
                                          // Don't use device before ipcon is connected.

    // Create receiver for count events.
    let count_receiver = re.get_count_receiver();

    // Spawn thread to handle received events. This thread ends when the `re` object
    // is dropped, so there is no need for manual cleanup.
    thread::spawn(move || {
        for count in count_receiver {
            println!("Count: {}", count);
        }
    });

    // Set period for count callback to 1s (1000ms) without a threshold.
    re.set_count_callback_configuration(1000, false, 'x', 0, 0);

    println!("Press enter to exit.");
    let mut _input = String::new();
    io::stdin().read_line(&mut _input)?;
    ipcon.disconnect();
    Ok(())
}
