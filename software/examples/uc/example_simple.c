#include "bindings/hal_common.h"
#include "bindings/bricklet_rotary_encoder_v2.h"

#define UID "XYZ" // Change XYZ to the UID of your Rotary Encoder Bricklet 2.0

void check(int rc, const char* msg);

void example_setup(TF_HalContext *hal);
void example_loop(TF_HalContext *hal);


static TF_RotaryEncoderV2 re;

void example_setup(TF_HalContext *hal) {
	// Create device object
	check(tf_rotary_encoder_v2_create(&re, UID, hal), "create device object");

	// Get current count without reset
	int32_t count;
	check(tf_rotary_encoder_v2_get_count(&re, false, &count), "get count without reset");

	tf_hal_printf("Count: %I32d\n", count);
}

void example_loop(TF_HalContext *hal) {
	// Poll for callbacks
	tf_hal_callback_tick(hal, 0);
}
