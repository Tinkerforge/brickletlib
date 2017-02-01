/* brickletlib
 * Copyright (C) 2010-2012 Olaf Lüke <olaf@tinkerforge.com>
 *
 * bricklet_simple.c: Functionality for simple sensor bricklets
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

#include "bricklet_simple.h"

#ifdef BRICKLET_HAS_SIMPLE_SENSOR

#include <stdint.h>

#include "bricklib/drivers/adc/adc.h"
#include "bricklib/bricklet/bricklet_communication.h"
#include "bricklib/utility/init.h"

#include "config.h"

#include "brickletlib/bricklet_debug.h"

extern const SimpleMessageProperty smp[];
extern const SimpleUnitProperty sup[];
extern const uint8_t smp_length;

void simple_invocation(const uint8_t com, const uint8_t *data) {
	BrickletAPI  *ba = BA;
	BrickContext *bc = BC;
	SimpleStandardMessage *standard_message = (SimpleStandardMessage*)data;
	uint8_t id = standard_message->header.fid - 1;
	if(id >= smp_length) {
		return;
	}

	if(smp[id].direction == SIMPLE_DIRECTION_SET) {
		switch(smp[id].transfer) {
			case SIMPLE_TRANSFER_VALUE: {
				break;
			}

			case SIMPLE_TRANSFER_PERIOD: {
				SimpleSetPeriod *ssp = ((SimpleSetPeriod*)data);
				bc->signal_period[smp[id].unit] = ssp->period;

				logbli("set period: %d\n\r", ssp->period);
				break;
			}

			case SIMPLE_TRANSFER_THRESHOLD: {
				SimpleSetThreshold* sst = (SimpleSetThreshold*)data;
				bc->threshold_option_save[smp[id].unit] = sst->option;
				bc->threshold_min_save[smp[id].unit] = sst->min;
				bc->threshold_max_save[smp[id].unit] = sst->max;

				if(sst->option == 'o' ||
				   sst->option == 'i' ||
				   sst->option == 'x') {
					bc->threshold_option[smp[id].unit] = sst->option;
					bc->threshold_min[smp[id].unit] = sst->min;
					bc->threshold_max[smp[id].unit] = sst->max;
				} else if(sst->option == '<') {
					bc->threshold_option[smp[id].unit] = 'o';
					bc->threshold_min[smp[id].unit] = sst->min;
					bc->threshold_max[smp[id].unit] = 2147483647;
				} else if(sst->option == '>') {
					bc->threshold_option[smp[id].unit] = 'o';
					bc->threshold_min[smp[id].unit] = -2147483647;
					bc->threshold_max[smp[id].unit] = sst->min;
				} else {
					ba->com_return_error(data, sizeof(MessageHeader), MESSAGE_ERROR_CODE_INVALID_PARAMETER, com);
					return;
				}

				bc->threshold_period_current[smp[id].unit] = bc->threshold_debounce;

				logbli("set threshold: %c %d %d (%d)\n\r", sst->option,
				                                           sst->min,
				                                           sst->max,
				                                           smp[id].unit);
				break;
			}

			case SIMPLE_TRANSFER_DEBOUNCE: {
				SimpleSetDebounce *ssd = ((SimpleSetDebounce*)data);
				bc->threshold_debounce = ssd->debounce;

				logbli("set debounce: %d\n\r", ssd->debounce);
				break;
			}
		}

		switch(smp[id].transfer) {
			case SIMPLE_TRANSFER_PERIOD:
			case SIMPLE_TRANSFER_DEBOUNCE:
			case SIMPLE_TRANSFER_THRESHOLD: {
				ba->com_return_setter(com, data);
				break;
			}
		}
	} else if(smp[id].direction == SIMPLE_DIRECTION_GET) {
		switch(smp[id].transfer) {
			case SIMPLE_TRANSFER_VALUE: {
				SimpleGetValue* sgv = (SimpleGetValue*)data;
				SimpleGetValueReturn sgvr;
				sgvr.header = sgv->header;
				sgvr.header.length = sizeof(SimpleGetValueReturn);
				sgvr.value = bc->value[smp[id].unit];

				ba->send_blocking_with_timeout(&sgvr,
				                               sizeof(SimpleGetValueReturn),
				                               com);

				logbli("get value: %d\n\r", sgvr.value);
				break;
			}

			case SIMPLE_TRANSFER_PERIOD: {
				SimpleGetPeriod* sgp = (SimpleGetPeriod*)data;
				SimpleGetPeriodReturn sgpr;
				sgpr.header = sgp->header;
				sgpr.header.length = sizeof(SimpleGetPeriodReturn);

				sgpr.period = bc->signal_period[smp[id].unit];

				ba->send_blocking_with_timeout(&sgpr,
				                               sizeof(SimpleGetPeriodReturn),
				                               com);

				logbli("get period: %d\n\r", sgpr.period);
				break;
			}

			case SIMPLE_TRANSFER_THRESHOLD: {
				SimpleGetThreshold* sgt = (SimpleGetThreshold*)data;
				SimpleGetThresholdReturn sgtr;
				sgtr.header = sgt->header;
				sgtr.header.length = sizeof(SimpleGetThresholdReturn);

				sgtr.option = bc->threshold_option_save[smp[id].unit];
				sgtr.min = bc->threshold_min_save[smp[id].unit];
				sgtr.max = bc->threshold_max_save[smp[id].unit];


				ba->send_blocking_with_timeout(&sgtr,
				                               sizeof(SimpleGetThresholdReturn),
				                               com);

				logbli("get threshold: %c %d %d (%d)\n\r", sgtr.option,
				                                           sgtr.min,
				                                           sgtr.max,
				                                           smp[id].unit);
				break;
			}

			case SIMPLE_TRANSFER_DEBOUNCE: {
				SimpleGetDebounce* sgd = (SimpleGetDebounce*)data;
				SimpleGetDebounceReturn sgdr;
				sgdr.header = sgd->header;
				sgdr.header.length = sizeof(SimpleGetDebounceReturn);

				sgdr.debounce = bc->threshold_debounce;

				ba->send_blocking_with_timeout(&sgdr,
				                               sizeof(SimpleGetDebounceReturn),
				                               com);

				logbli("get debounce: %d\n\r", sgdr.debounce);
				break;
			}
		}
	}
}

void simple_constructor(void) {
	BrickContext *bc = BC;
	bc->threshold_debounce = 100;

	for(uint8_t i = 0; i < NUM_SIMPLE_VALUES; i++) {
		bc->value[i] = 0;
		bc->last_value[i] = 0xFFFFFFFF;
		bc->signal_period[i] = 0;
		bc->signal_period_counter[i] = 0;
		bc->threshold_period_current[i] = 0;
		bc->threshold_min[i] = 0;
		bc->threshold_max[i] = 0;
		bc->threshold_option[i] = 'x';

		bc->threshold_min_save[i] = 0;
		bc->threshold_max_save[i] = 0;
		bc->threshold_option_save[i] = 'x';
	}

	bc->tick = 0;

	logbli("simple constructor\n\r");
}

#ifndef BRICKLET_HAS_NO_DESTRUCTOR
void simple_destructor(void) {
	simple_constructor();
	logbli("simple destructor\n\r");
}
#endif

void simple_tick(const uint8_t tick_type) {
	BrickletAPI  *ba = BA;
	BrickContext *bc = BC;

	if(tick_type & TICK_TASK_TYPE_CALCULATION) {
		// Get values

		// If only one simple value is used, gcc removes the for loop and inserts
		// the function, in this case there is no offset needed
#ifdef BRICKLET_VALUE_APPLIED_OUTSIDE
		// Nothing to do here
#else
#ifdef BRICKLET_NO_OFFSET
		for(uint8_t i = 0; i < NUM_SIMPLE_VALUES; i++) {
			bc->value[i] = sup[i].func(bc->value[sup[i].call_value_id]);
		}
#else
		for(uint8_t i = 0; i < NUM_SIMPLE_VALUES; i++) {
			bc->value[i] = BRICKLET_OFFSET_SIMPLE(sup[i].func)(bc->value[sup[i].call_value_id]);
		}
#endif
#endif
		for(uint8_t i = 0; i < NUM_SIMPLE_VALUES; i++) {
			if(bc->signal_period_counter[i] < UINT32_MAX) {
				bc->signal_period_counter[i]++;
			}

			if(bc->threshold_period_current[i] != bc->threshold_debounce) {
				bc->threshold_period_current[i]++;
			}
		}

		bc->tick++;
	}

	if(tick_type & TICK_TASK_TYPE_MESSAGE) {
		// Handle period signals
		for(uint8_t i = 0; i < NUM_SIMPLE_VALUES; i++) {
			if(bc->signal_period[i] != 0 &&
				bc->signal_period[i] <= bc->signal_period_counter[i]) {
				if(bc->last_value[i] != bc->value[i]) {
					SimpleGetValueReturn sgvr;
					ba->com_make_default_header(&sgvr, BS->uid, sizeof(SimpleGetValueReturn), sup[i].fid_period);
					sgvr.value = bc->value[i];


					ba->send_blocking_with_timeout(&sgvr,
												   sizeof(SimpleGetValueReturn),
												   *ba->com_current);

					logbli("period value: %d\n\r", bc->value[i]);
					bc->signal_period_counter[i] = 0;
					bc->last_value[i] = bc->value[i];
				}
			}
		}

		// Handle threshold signals
		for(uint8_t i = 0; i < NUM_SIMPLE_VALUES; i++) {
			int32_t value = bc->value[i];

			if(((bc->threshold_option[i] == 'o') &&
				((value < bc->threshold_min[i]) ||
				 (value > bc->threshold_max[i]))) ||
			   ((bc->threshold_option[i] == 'i') &&
				((value > bc->threshold_min[i]) &&
				 (value < bc->threshold_max[i])))) {

				if(bc->threshold_period_current[i] == bc->threshold_debounce) {
					SimpleGetValueReturn sgvr;
					ba->com_make_default_header(&sgvr, BS->uid, sizeof(SimpleGetValueReturn), sup[i].fid_reached);
					sgvr.value = value;

					ba->send_blocking_with_timeout(&sgvr,
												   sizeof(SimpleGetValueReturn),
												   *ba->com_current);
					bc->threshold_period_current[i] = 0;

					logbli("threshold value: %d\n\r", bc->value[i]);
				}
			}
		}
	}
}
#endif
