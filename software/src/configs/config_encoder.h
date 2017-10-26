/* rotary-encoder-v2-bricklet
 * Copyright (C) 2017 Olaf Lüke <olaf@tinkerforge.com>
 *
 * config_encoder.h: Configurations for rotary encoder
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

#ifndef CONFIG_ENCODER_H
#define CONFIG_ENCODER_H

#include "xmc_gpio.h"

#define ENCODER_A_PIN          P2_1
#define ENCODER_B_PIN          P2_2
#define ENCODER_BUTTON_PIN     P2_10

#define ENCODER_A_ETL_CHANNEL  1
#define ENCODER_A_OGU_CHANNEL  1

#define ENCODER_A_IRQ_N        4
#define ENCODER_A_IRQ_PRIO     0

#define ENCODER_B_ETL_CHANNEL  0
#define ENCODER_B_OGU_CHANNEL  0

#define ENCODER_B_IRQ_N        3
#define ENCODER_B_IRQ_PRIO     0

#define ENCODER_DEBOUNCE_TIME  100 // in us
#define ENCODER_DEBOUNCE_PRIO  1


#endif
