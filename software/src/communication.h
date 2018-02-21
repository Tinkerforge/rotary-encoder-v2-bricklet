/* rotary-encoder-v2-bricklet
 * Copyright (C) 2017 Olaf Lüke <olaf@tinkerforge.com>
 *
 * communication.h: TFP protocol message handling
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

#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <stdint.h>
#include <stdbool.h>

#include "bricklib2/protocols/tfp/tfp.h"
#include "bricklib2/bootloader/bootloader.h"

// Default functions
BootloaderHandleMessageResponse handle_message(const void *data, void *response);
void communication_tick(void);
void communication_init(void);

// Constants
#define ROTARY_ENCODER_V2_THRESHOLD_OPTION_OFF 'x'
#define ROTARY_ENCODER_V2_THRESHOLD_OPTION_OUTSIDE 'o'
#define ROTARY_ENCODER_V2_THRESHOLD_OPTION_INSIDE 'i'
#define ROTARY_ENCODER_V2_THRESHOLD_OPTION_SMALLER '<'
#define ROTARY_ENCODER_V2_THRESHOLD_OPTION_GREATER '>'

#define ROTARY_ENCODER_V2_BOOTLOADER_MODE_BOOTLOADER 0
#define ROTARY_ENCODER_V2_BOOTLOADER_MODE_FIRMWARE 1
#define ROTARY_ENCODER_V2_BOOTLOADER_MODE_BOOTLOADER_WAIT_FOR_REBOOT 2
#define ROTARY_ENCODER_V2_BOOTLOADER_MODE_FIRMWARE_WAIT_FOR_REBOOT 3
#define ROTARY_ENCODER_V2_BOOTLOADER_MODE_FIRMWARE_WAIT_FOR_ERASE_AND_REBOOT 4

#define ROTARY_ENCODER_V2_BOOTLOADER_STATUS_OK 0
#define ROTARY_ENCODER_V2_BOOTLOADER_STATUS_INVALID_MODE 1
#define ROTARY_ENCODER_V2_BOOTLOADER_STATUS_NO_CHANGE 2
#define ROTARY_ENCODER_V2_BOOTLOADER_STATUS_ENTRY_FUNCTION_NOT_PRESENT 3
#define ROTARY_ENCODER_V2_BOOTLOADER_STATUS_DEVICE_IDENTIFIER_INCORRECT 4
#define ROTARY_ENCODER_V2_BOOTLOADER_STATUS_CRC_MISMATCH 5

#define ROTARY_ENCODER_V2_STATUS_LED_CONFIG_OFF 0
#define ROTARY_ENCODER_V2_STATUS_LED_CONFIG_ON 1
#define ROTARY_ENCODER_V2_STATUS_LED_CONFIG_SHOW_HEARTBEAT 2
#define ROTARY_ENCODER_V2_STATUS_LED_CONFIG_SHOW_STATUS 3

// Function and callback IDs and structs
#define FID_GET_COUNT 1
#define FID_SET_COUNT_CALLBACK_CONFIGURATION 2
#define FID_GET_COUNT_CALLBACK_CONFIGURATION 3
#define FID_IS_PRESSED 5

#define FID_CALLBACK_COUNT 4
#define FID_CALLBACK_PRESSED 6
#define FID_CALLBACK_RELEASED 7

typedef struct {
	TFPMessageHeader header;
} __attribute__((__packed__)) IsPressed;

typedef struct {
	TFPMessageHeader header;
	bool pressed;
} __attribute__((__packed__)) IsPressed_Response;

typedef struct {
	TFPMessageHeader header;
} __attribute__((__packed__)) Pressed_Callback;

typedef struct {
	TFPMessageHeader header;
} __attribute__((__packed__)) Released_Callback;


// Function prototypes
BootloaderHandleMessageResponse is_pressed(const IsPressed *data, IsPressed_Response *response);

// Callbacks
bool handle_count_callback(void);
bool handle_pressed_callback(void);
bool handle_released_callback(void);

#define COMMUNICATION_CALLBACK_TICK_WAIT_MS 1
#define COMMUNICATION_CALLBACK_HANDLER_NUM 3
#define COMMUNICATION_CALLBACK_LIST_INIT \
	handle_count_callback, \
	handle_pressed_callback, \
	handle_released_callback, \


#endif
