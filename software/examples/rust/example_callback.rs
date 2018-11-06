use std::{error::Error, io, thread};
use tinkerforge::{ipconnection::IpConnection, rotary_encoder_v2_bricklet::*};

const HOST: &str = "127.0.0.1";
const PORT: u16 = 4223;
const UID: &str = "XYZ"; // Change XYZ to the UID of your Rotary Encoder Bricklet 2.0

fn main() -> Result<(), Box<dyn Error>> {
    let ipcon = IpConnection::new(); // Create IP connection
    let rotary_encoder_v2_bricklet = RotaryEncoderV2Bricklet::new(UID, &ipcon); // Create device object

    ipcon.connect(HOST, PORT).recv()??; // Connect to brickd
                                        // Don't use device before ipcon is connected

    //Create listener for count events.
    let count_listener = rotary_encoder_v2_bricklet.get_count_receiver();
    // Spawn thread to handle received events. This thread ends when the rotary_encoder_v2_bricklet
    // is dropped, so there is no need for manual cleanup.
    thread::spawn(move || {
        for event in count_listener {
            println!("Count: {}", event);
        }
    });

    // Set period for count callback to 1s (1000ms) without a threshold
    rotary_encoder_v2_bricklet.set_count_callback_configuration(1000, false, 'x', 0, 0);

    println!("Press enter to exit.");
    let mut _input = String::new();
    io::stdin().read_line(&mut _input)?;
    ipcon.disconnect();
    Ok(())
}
