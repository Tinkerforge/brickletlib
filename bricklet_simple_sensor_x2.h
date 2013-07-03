/* brickletlib
 * Copyright (C) 2013 Olaf Lüke <olaf@tinkerforge.com>
 *
 * bricklet_simple_sensor_x2.h: Functionality for simple bricklets with two identical sensors
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

#ifndef BRICKLET_SIMPLE_SENSOR_X2_H
#define BRICKLET_SIMPLE_SENSOR_X2_H

#include "config.h"

#ifdef BRICKLET_HAS_SIMPLE_SENSOR_SENSOR_X2
#include <stdint.h>
#include "bricklib/com/com.h"

#define SIMPLE_TRANSFER_VALUE 0
#define SIMPLE_TRANSFER_PERIOD 1
#define SIMPLE_TRANSFER_THRESHOLD 2
#define SIMPLE_TRANSFER_DEBOUNCE 3

#define SIMPLE_DIRECTION_GET 0
#define SIMPLE_DIRECTION_SET 1

#define SIMPLE_SIGNEDNESS_UINT 0
#define SIMPLE_SIGNEDNESS_INT  1

#ifndef SIMPLE_VALUE_TYPE
	#define SIMPLE_VALUE_TYPE int16_t
#endif

typedef struct {
	uint8_t unit;
	uint8_t transfer;
	uint8_t direction;
} SimpleMessageProperty;

typedef struct {
	value_handler_func_t func;
	uint8_t signedness;
	uint8_t fid_period;
	uint8_t fid_reached;
	uint8_t call_value_id;
} SimpleUnitProperty;

typedef struct {
	MessageHeader header;
	SIMPLE_VALUE_TYPE value;
} __attribute__((__packed__)) SimpleStandardMessage;

typedef struct {
	MessageHeader header;
	uint8_t sensor;
} __attribute__((__packed__)) SimpleGetValue;

typedef struct {
	MessageHeader header;
	SIMPLE_VALUE_TYPE value;
} __attribute__((__packed__)) SimpleGetValueReturn;

typedef struct {
	MessageHeader header;
	uint8_t sensor;
	SIMPLE_VALUE_TYPE value;
} __attribute__((__packed__)) SimpleCallback;

typedef struct {
	MessageHeader header;
	uint32_t debounce;
} __attribute__((__packed__)) SimpleSetDebounce;

typedef struct {
	MessageHeader header;
} __attribute__((__packed__)) SimpleGetDebounce;

typedef struct {
	MessageHeader header;
	uint32_t debounce;
} __attribute__((__packed__)) SimpleGetDebounceReturn;

typedef struct {
	MessageHeader header;
	uint8_t sensor;
	uint32_t period;
} __attribute__((__packed__)) SimpleSetPeriod;

typedef struct {
	MessageHeader header;
	uint8_t sensor;
} __attribute__((__packed__)) SimpleGetPeriod;

typedef struct {
	MessageHeader header;
	uint32_t period;
} __attribute__((__packed__)) SimpleGetPeriodReturn;

typedef struct {
	MessageHeader header;
	uint8_t sensor;
	char option;
	SIMPLE_VALUE_TYPE min;
	SIMPLE_VALUE_TYPE max;
} __attribute__((__packed__)) SimpleSetThreshold;

typedef struct {
	MessageHeader header;
	uint8_t sensor;
} __attribute__((__packed__)) SimpleGetThreshold;

typedef struct {
	MessageHeader header;
	char option;
	SIMPLE_VALUE_TYPE min;
	SIMPLE_VALUE_TYPE max;
} __attribute__((__packed__)) SimpleGetThresholdReturn;

void simple_invocation(const ComType com, const uint8_t *data);
void simple_constructor(void);
void simple_destructor(void);
void simple_tick(const uint8_t tick_type);
#endif

#endif
