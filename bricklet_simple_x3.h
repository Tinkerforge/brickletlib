/* brickletlib
 * Copyright (C) 2014 Ishraq Ibne Ashraf <ishraq@tinkerforge.com>
 *
 * bricklet_simple_x3.h: Functionality for simple sensor bricklets with 3 values
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

#ifndef BRICKLET_SIMPLE_X3_H
#define BRICKLET_SIMPLE_X3_H

#include "config.h"

#ifdef BRICKLET_HAS_SIMPLE_X3
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
	uint8_t signedness;
	uint8_t fid_period;
	uint8_t fid_reached;
	uint8_t call_value_id;
} SimpleUnitProperty;

typedef struct {
	MessageHeader header;
	SIMPLE_VALUE_TYPE value1;
	SIMPLE_VALUE_TYPE value2;
    SIMPLE_VALUE_TYPE value3;
} __attribute__((__packed__)) SimpleStandardMessage;

typedef struct {
	MessageHeader header;
} __attribute__((__packed__)) SimpleGetValue;

typedef struct {
	MessageHeader header;
	SIMPLE_VALUE_TYPE value1;
	SIMPLE_VALUE_TYPE value2;
    SIMPLE_VALUE_TYPE value3;
} __attribute__((__packed__)) SimpleGetValueReturn;

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
	uint32_t period;
} __attribute__((__packed__)) SimpleSetPeriod;

typedef struct {
	MessageHeader header;
} __attribute__((__packed__)) SimpleGetPeriod;

typedef struct {
	MessageHeader header;
	uint32_t period;
} __attribute__((__packed__)) SimpleGetPeriodReturn;

typedef struct {
	MessageHeader header;
	char option;
	SIMPLE_VALUE_TYPE min1;
	SIMPLE_VALUE_TYPE max1;
	SIMPLE_VALUE_TYPE min2;
	SIMPLE_VALUE_TYPE max2;
	SIMPLE_VALUE_TYPE min3;
	SIMPLE_VALUE_TYPE max3;
} __attribute__((__packed__)) SimpleSetThreshold;

typedef struct {
	MessageHeader header;
} __attribute__((__packed__)) SimpleGetThreshold;

typedef struct {
	MessageHeader header;
	char option;
	SIMPLE_VALUE_TYPE min1;
	SIMPLE_VALUE_TYPE max1;
	SIMPLE_VALUE_TYPE min2;
	SIMPLE_VALUE_TYPE max2;
	SIMPLE_VALUE_TYPE min3;
	SIMPLE_VALUE_TYPE max3;
} __attribute__((__packed__)) SimpleGetThresholdReturn;

void simple_invocation(const ComType com, const uint8_t *data);
void simple_constructor(void);
void simple_destructor(void);
void simple_tick(const uint8_t tick_type);
#endif

#endif
