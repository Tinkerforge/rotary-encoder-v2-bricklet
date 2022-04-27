// This example is not self-contained.
// It requires usage of the example driver specific to your platform.
// See the HAL documentation.

#include "src/bindings/hal_common.h"
#include "src/bindings/bricklet_rotary_encoder_v2.h"

void check(int rc, const char *msg);
void example_setup(TF_HAL *hal);
void example_loop(TF_HAL *hal);

static TF_RotaryEncoderV2 re;

void example_setup(TF_HAL *hal) {
	// Create device object
	check(tf_rotary_encoder_v2_create(&re, NULL, hal), "create device object");

	// Get current count without reset
	int32_t count;
	check(tf_rotary_encoder_v2_get_count(&re, false, &count), "get count without reset");

	tf_hal_printf("Count: %I32d\n", count);
}

void example_loop(TF_HAL *hal) {
	// Poll for callbacks
	tf_hal_callback_tick(hal, 0);
}
