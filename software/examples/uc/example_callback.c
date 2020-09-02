// This example is not self-contained.
// It requres usage of the example driver specific to your platform.
// See the HAL documentation.

#include "bindings/hal_common.h"
#include "bindings/bricklet_rotary_encoder_v2.h"

#define UID "XYZ" // Change XYZ to the UID of your Rotary Encoder Bricklet 2.0

void check(int rc, const char* msg);

void example_setup(TF_HalContext *hal);
void example_loop(TF_HalContext *hal);


// Callback function for count callback
static void count_handler(TF_RotaryEncoderV2 *device, int32_t count, void *user_data) {
	(void)device; (void)user_data; // avoid unused parameter warning

	tf_hal_printf("Count: %I32d\n", count);
}

static TF_RotaryEncoderV2 re;

void example_setup(TF_HalContext *hal) {
	// Create device object
	check(tf_rotary_encoder_v2_create(&re, UID, hal), "create device object");

	// Register count callback to function count_handler
	tf_rotary_encoder_v2_register_count_callback(&re,
	                                             count_handler,
	                                             NULL);

	// Set period for count callback to 1s (1000ms) without a threshold
	tf_rotary_encoder_v2_set_count_callback_configuration(&re, 1000, false, 'x', 0, 0);
}

void example_loop(TF_HalContext *hal) {
	// Poll for callbacks
	tf_hal_callback_tick(hal, 0);
}
