/* rotary-encoder-v2-bricklet
 * Copyright (C) 2017 Olaf Lüke <olaf@tinkerforge.com>
 *
 * encoder.c: Driver for rotary encoder
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#include "encoder.h"

#include <string.h>

#include "bricklib2/hal/system_timer/system_timer.h"

#include "configs/config.h"
#include "configs/config_encoder.h"

#include "xmc_eru.h"
#include "xmc_gpio.h"
#include "xmc_ccu4.h"

#define encoder_debounce_a_handler IRQ_Hdlr_21
#define encoder_debounce_b_handler IRQ_Hdlr_22

#define encoder_a_handler IRQ_Hdlr_4
#define encoder_b_handler IRQ_Hdlr_3

const int8_t encoder_table[4][4] = {
    { 0,  1, -1,  0},
    {-1,  0,  0,  1},
    { 1,  0,  0, -1},
    { 0, -1,  1,  0}
};

Encoder encoder;

static int32_t encoder_count = 0;
static int8_t encoder_last_value = 0;

// We do the actual measurements in the debounce handler.
// The handler is called 100us after we see a edge change.
// This way we make sure that we never take a value during the initial bounciness after the edge changes.
// If the pot is turned at "maximum human speed" the edges chage about every 500us, so with 100us debounce
// we still have a big margin of error.
void __attribute__((optimize("-O3"))) __attribute__ ((section (".ram_code"))) encoder_debounce_a_handler(void) {
	NVIC_ClearPendingIRQ(ENCODER_A_IRQ_N);
	NVIC_EnableIRQ(ENCODER_A_IRQ_N);

	const uint8_t encoder_value = XMC_GPIO_GetInput(ENCODER_A_PIN) | (XMC_GPIO_GetInput(ENCODER_B_PIN) << 1);

	int8_t add = encoder_table[encoder_last_value][encoder_value];
	encoder_count += add;
	encoder_last_value = encoder_value;
}

void __attribute__((optimize("-O3"))) __attribute__ ((section (".ram_code"))) encoder_debounce_b_handler(void) {
	NVIC_ClearPendingIRQ(ENCODER_B_IRQ_N);
	NVIC_EnableIRQ(ENCODER_B_IRQ_N);

	const uint8_t encoder_value = XMC_GPIO_GetInput(ENCODER_A_PIN) | (XMC_GPIO_GetInput(ENCODER_B_PIN) << 1);

	int8_t add = encoder_table[encoder_last_value][encoder_value];
	encoder_count += add;
	encoder_last_value = encoder_value;
}


void __attribute__((optimize("-O3"))) __attribute__ ((section (".ram_code"))) encoder_a_handler(void) {
	CCU40_CC40->TCCLR = CCU4_CC4_TCCLR_TCC_Msk;
	CCU40_CC40->TCSET = CCU4_CC4_TCSET_TRBS_Msk;
	NVIC_DisableIRQ(ENCODER_A_IRQ_N);
}

void __attribute__((optimize("-O3"))) __attribute__ ((section (".ram_code"))) encoder_b_handler(void) {
	CCU40_CC41->TCCLR = CCU4_CC4_TCCLR_TCC_Msk;
	CCU40_CC41->TCSET = CCU4_CC4_TCSET_TRBS_Msk;
	NVIC_DisableIRQ(ENCODER_B_IRQ_N);
}

void encoder_init_debounce_timer(void) {
	XMC_CCU4_SLICE_COMPARE_CONFIG_t timer_config = {
		.timer_mode          = XMC_CCU4_SLICE_TIMER_COUNT_MODE_EA,
		.monoshot            = XMC_CCU4_SLICE_TIMER_REPEAT_MODE_SINGLE,
		.shadow_xfer_clear   = false,
		.dither_timer_period = false,
		.dither_duty_cycle   = false,
		.prescaler_mode      = XMC_CCU4_SLICE_PRESCALER_MODE_NORMAL,
		.mcm_enable          = false,
		.prescaler_initval   = XMC_CCU4_SLICE_PRESCALER_64, // Use prescaler 64 to get 1 tick = 1us
		.float_limit         = 0U,
		.dither_limit        = 0U,
		.passive_level       = XMC_CCU4_SLICE_OUTPUT_PASSIVE_LEVEL_LOW,
		.timer_concatenation = false
	};

    XMC_CCU4_Init(CCU40, XMC_CCU4_SLICE_MCMS_ACTION_TRANSFER_PR_CR);
    XMC_CCU4_StartPrescaler(CCU40);


	// Timer input A
    XMC_CCU4_EnableClock(CCU40, 0);
    XMC_CCU4_SLICE_CompareInit(CCU40_CC40, &timer_config);
    XMC_CCU4_SLICE_SetTimerPeriodMatch(CCU40_CC40, ENCODER_DEBOUNCE_TIME);
    XMC_CCU4_SLICE_SetTimerCompareMatch(CCU40_CC40, 0);
    XMC_CCU4_EnableShadowTransfer(CCU40, XMC_CCU4_SHADOW_TRANSFER_SLICE_0 | XMC_CCU4_SHADOW_TRANSFER_PRESCALER_SLICE_0);

    XMC_CCU4_SLICE_EnableEvent(CCU40_CC40, XMC_CCU4_SLICE_IRQ_ID_PERIOD_MATCH);
	XMC_CCU4_SLICE_SetInterruptNode(CCU40_CC40, XMC_CCU4_SLICE_IRQ_ID_PERIOD_MATCH, XMC_CCU4_SLICE_SR_ID_0);
	NVIC_EnableIRQ(CCU40_0_IRQn);
	NVIC_SetPriority(CCU40_0_IRQn, ENCODER_DEBOUNCE_PRIO);


	// Timer input B
    XMC_CCU4_EnableClock(CCU40, 1);
    XMC_CCU4_SLICE_CompareInit(CCU40_CC41, &timer_config);
    XMC_CCU4_SLICE_SetTimerPeriodMatch(CCU40_CC41, ENCODER_DEBOUNCE_TIME);
    XMC_CCU4_SLICE_SetTimerCompareMatch(CCU40_CC41, 0);
    XMC_CCU4_EnableShadowTransfer(CCU40, XMC_CCU4_SHADOW_TRANSFER_SLICE_1 | XMC_CCU4_SHADOW_TRANSFER_PRESCALER_SLICE_1);

    XMC_CCU4_SLICE_EnableEvent(CCU40_CC41, XMC_CCU4_SLICE_IRQ_ID_PERIOD_MATCH);
	XMC_CCU4_SLICE_SetInterruptNode(CCU40_CC41, XMC_CCU4_SLICE_IRQ_ID_PERIOD_MATCH, XMC_CCU4_SLICE_SR_ID_1);
	NVIC_EnableIRQ(CCU40_1_IRQn);
	NVIC_SetPriority(CCU40_1_IRQn, ENCODER_DEBOUNCE_PRIO);
}

void encoder_init_eru(void) {
	// A config (ERU0.1B0)
	const XMC_ERU_ETL_CONFIG_t a_etl_config = {
		.input_a                = XMC_ERU_ETL_INPUT_A0,
		.input_b                = XMC_ERU_ETL_INPUT_B0,
		.enable_output_trigger  = 1,
		.edge_detection         = XMC_ERU_ETL_EDGE_DETECTION_BOTH,
		.output_trigger_channel = XMC_ERU_ETL_OUTPUT_TRIGGER_CHANNEL1, // Select the source for event
		.source                 = XMC_ERU_ETL_SOURCE_B
	};

	// ERU Event Trigger Logic Hardware initialization based on UI
	XMC_ERU_ETL_Init(XMC_ERU0, ENCODER_A_ETL_CHANNEL, &a_etl_config);
	// OGU is configured to generate event on configured trigger edge
	XMC_ERU_OGU_SetServiceRequestMode(XMC_ERU0, ENCODER_A_OGU_CHANNEL, XMC_ERU_OGU_SERVICE_REQUEST_ON_TRIGGER);
	// Configure NVIC node and priority
	NVIC_SetPriority(ENCODER_A_IRQ_N, ENCODER_A_IRQ_PRIO);

	// Clear pending interrupt before enabling it
	NVIC_ClearPendingIRQ(ENCODER_A_IRQ_N);
	// Enable NVIC node
	NVIC_EnableIRQ(ENCODER_A_IRQ_N);


	// B config (ERU0.0B1)
	const XMC_ERU_ETL_CONFIG_t b_etl_config = {
		.input_a                = XMC_ERU_ETL_INPUT_A0,
		.input_b                = XMC_ERU_ETL_INPUT_B1,
		.enable_output_trigger  = 1,
		.edge_detection         = XMC_ERU_ETL_EDGE_DETECTION_BOTH,
		.output_trigger_channel = XMC_ERU_ETL_OUTPUT_TRIGGER_CHANNEL0, // Select the source for event
		.source                 = XMC_ERU_ETL_SOURCE_B
	};

	// ERU Event Trigger Logic Hardware initialization based on UI
	XMC_ERU_ETL_Init(XMC_ERU0, ENCODER_B_ETL_CHANNEL, &b_etl_config);
	// OGU is configured to generate event on configured trigger edge
	XMC_ERU_OGU_SetServiceRequestMode(XMC_ERU0, ENCODER_B_OGU_CHANNEL, XMC_ERU_OGU_SERVICE_REQUEST_ON_TRIGGER);
	// Configure NVIC node and priority
	NVIC_SetPriority(ENCODER_B_IRQ_N, ENCODER_B_IRQ_PRIO);

	// Clear pending interrupt before enabling it
	NVIC_ClearPendingIRQ(ENCODER_B_IRQ_N);
	// Enable NVIC node
	NVIC_EnableIRQ(ENCODER_B_IRQ_N);
}

void encoder_init(void) {
	memset(&encoder, 0, sizeof(Encoder));

	const XMC_GPIO_CONFIG_t encoder_pin_config = {
		.mode             = XMC_GPIO_MODE_INPUT_TRISTATE,
		.input_hysteresis = XMC_GPIO_INPUT_HYSTERESIS_STANDARD,
	};

	XMC_GPIO_Init(ENCODER_A_PIN, &encoder_pin_config);
	XMC_GPIO_Init(ENCODER_B_PIN, &encoder_pin_config);
	XMC_GPIO_Init(ENCODER_BUTTON_PIN, &encoder_pin_config);

	// Initialize encoder value properly
	encoder_last_value = XMC_GPIO_GetInput(ENCODER_A_PIN) | (XMC_GPIO_GetInput(ENCODER_B_PIN) << 1);

	encoder_init_eru();
	encoder_init_debounce_timer();
}

void encoder_tick(void) {
	if(system_timer_is_time_elapsed_ms(encoder.button_change_time, ENCODER_BUTTON_DEBOUNCE_TIME)) {
		bool state = XMC_GPIO_GetInput(ENCODER_BUTTON_PIN);
		if(state != encoder.button_last_state) {
			encoder.button_last_state = state;
			encoder.button_change_time = system_timer_get_ms();
			encoder.button_callback_state = state;
			encoder.button_callback_new = true;
		}
	}
}

int32_t encoder_get_count(void) {
	NVIC_DisableIRQ(CCU40_0_IRQn);
	NVIC_DisableIRQ(CCU40_1_IRQn);

	int32_t value = encoder_count;
	if(encoder.reset_after_get) {
		encoder_count = 0;
	}

	NVIC_EnableIRQ(CCU40_1_IRQn);
	NVIC_EnableIRQ(CCU40_0_IRQn);

	return value/4;
}
