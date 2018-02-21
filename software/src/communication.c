/* rotary-encoder-v2-bricklet
 * Copyright (C) 2017 Olaf Lüke <olaf@tinkerforge.com>
 *
 * communication.c: TFP protocol message handling
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

#include "communication.h"

#include "bricklib2/utility/communication_callback.h"
#include "bricklib2/protocols/tfp/tfp.h"
#include "bricklib2/utility/callback_value.h"

#include "xmc_gpio.h"
#include "configs/config_encoder.h"
#include "encoder.h"

CallbackValue callback_value_count;

extern int32_t encoder_count = 0;

extern Encoder encoder;

typedef struct {
	TFPMessageHeader header;
	bool reset;
} __attribute__((__packed__)) GetCount;

BootloaderHandleMessageResponse handle_message(const void *message, void *response) {
	switch(tfp_get_fid_from_message(message)) {
		case FID_GET_COUNT: encoder.reset_after_get = ((const GetCount*)message)->reset; return get_callback_value(message, response, &callback_value_count);
		case FID_SET_COUNT_CALLBACK_CONFIGURATION: return set_callback_value_callback_configuration(message, &callback_value_count);
		case FID_GET_COUNT_CALLBACK_CONFIGURATION: return get_callback_value_callback_configuration(message, response, &callback_value_count);
		case FID_IS_PRESSED: return is_pressed(message, response);
		default: return HANDLE_MESSAGE_RESPONSE_NOT_SUPPORTED;
	}
}


BootloaderHandleMessageResponse is_pressed(const IsPressed *data, IsPressed_Response *response) {
	response->header.length = sizeof(IsPressed_Response);
	response->pressed = !XMC_GPIO_GetInput(ENCODER_BUTTON_PIN);
	return HANDLE_MESSAGE_RESPONSE_NEW_MESSAGE;
}




bool handle_count_callback(void) {
	return handle_callback_value_callback(&callback_value_count, FID_CALLBACK_COUNT);
}

bool handle_pressed_callback(void) {
	static bool is_buffered = false;
	static Pressed_Callback cb;

	if(!is_buffered) {
		if(encoder.button_callback_new && !encoder.button_callback_state) {
			tfp_make_default_header(&cb.header, bootloader_get_uid(), sizeof(Pressed_Callback), FID_CALLBACK_PRESSED);
			encoder.button_callback_new =  false;
		} else {
			return false;
		}
	}

	if(bootloader_spitfp_is_send_possible(&bootloader_status.st)) {
		bootloader_spitfp_send_ack_and_message(&bootloader_status, (uint8_t*)&cb, sizeof(Pressed_Callback));
		is_buffered = false;
		return true;
	} else {
		is_buffered = true;
	}

	return false;
}

bool handle_released_callback(void) {
	static bool is_buffered = false;
	static Released_Callback cb;

	if(!is_buffered) {
		if(encoder.button_callback_new && encoder.button_callback_state) {
			tfp_make_default_header(&cb.header, bootloader_get_uid(), sizeof(Released_Callback), FID_CALLBACK_RELEASED);
			encoder.button_callback_new =  false;
		} else {
			return false;
		}
	}

	if(bootloader_spitfp_is_send_possible(&bootloader_status.st)) {
		bootloader_spitfp_send_ack_and_message(&bootloader_status, (uint8_t*)&cb, sizeof(Released_Callback));
		is_buffered = false;
		return true;
	} else {
		is_buffered = true;
	}

	return false;
}

void communication_tick(void) {
	communication_callback_tick();
}

void communication_init(void) {
	callback_value_init(&callback_value_count, encoder_get_count);

	communication_callback_init();
}
