// This example is not self-contained.
// It requires usage of the example driver specific to your platform.
// See the HAL documentation.

#include "src/bindings/hal_common.h"
#include "src/bindings/bricklet_rotary_encoder_v2.h"

void check(int rc, const char *msg);
void example_setup(TF_HAL *hal);
void example_loop(TF_HAL *hal);

// Callback function for count callback
static void count_handler(TF_RotaryEncoderV2 *device, int32_t count, void *user_data) {
	(void)device; (void)user_data; // avoid unused parameter warning

	tf_hal_printf("Count: %I32d\n", count);
}

static TF_RotaryEncoderV2 re;

void example_setup(TF_HAL *hal) {
	// Create device object
	check(tf_rotary_encoder_v2_create(&re, NULL, hal), "create device object");

	// Register count callback to function count_handler
	tf_rotary_encoder_v2_register_count_callback(&re,
	                                             count_handler,
	                                             NULL);

	// Set period for count callback to 1s (1000ms) without a threshold
	tf_rotary_encoder_v2_set_count_callback_configuration(&re, 1000, false, 'x', 0, 0);
}

void example_loop(TF_HAL *hal) {
	// Poll for callbacks
	tf_hal_callback_tick(hal, 0);
}
