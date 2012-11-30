/* brickletlib
 * Copyright (C) 2011-2012 Olaf Lüke <olaf@tinkerforge.com>
 *
 * bricklet_simple_x2.c: Functionality for simple sensor bricklets with 2 values
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

#include "bricklet_simple_x2.h"

#ifdef BRICKLET_HAS_SIMPLE_SENSOR_X2

#include <stdint.h>

#include "bricklib/drivers/adc/adc.h"
#include "bricklib/bricklet/bricklet_communication.h"
#include "bricklib/utility/init.h"

#include "config.h"

#include "brickletlib/bricklet_debug.h"

extern const SimpleMessageProperty smp[];
extern const SimpleUnitProperty sup[];
extern const uint8_t smp_length;

void simple_invocation(const ComType com, const uint8_t *data) {
	uint8_t id = ((SimpleStandardMessage*)data)->header.fid - 1;
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
				BC->signal_period[smp[id].unit] = ssp->period;

				logbli("set period: %d\n\r", ssp->period);
				break;
			}

			case SIMPLE_TRANSFER_THRESHOLD: {
				SimpleSetThreshold* sst = (SimpleSetThreshold*)data;
				BC->threshold_option_save[smp[id].unit] = sst->option;
				BC->threshold_min_save1[smp[id].unit] = sst->min1;
				BC->threshold_max_save1[smp[id].unit] = sst->max1;
				BC->threshold_min_save2[smp[id].unit] = sst->min2;
				BC->threshold_max_save2[smp[id].unit] = sst->max2;

				if(sst->option == 'o' ||
				   sst->option == 'i' ||
				   sst->option == 'x') {
					BC->threshold_option[smp[id].unit] = sst->option;
					BC->threshold_min1[smp[id].unit] = sst->min1;
					BC->threshold_max1[smp[id].unit] = sst->max1;
					BC->threshold_min2[smp[id].unit] = sst->min2;
					BC->threshold_max2[smp[id].unit] = sst->max2;
				} else if(sst->option == '<') {
					BC->threshold_option[smp[id].unit] = 'o';
					BC->threshold_min1[smp[id].unit] = sst->min1;
					BC->threshold_max1[smp[id].unit] = 2147483647;
					BC->threshold_min2[smp[id].unit] = sst->min2;
					BC->threshold_max2[smp[id].unit] = 2147483647;
				} else if(sst->option == '>') {
					BC->threshold_option[smp[id].unit] = 'o';
					BC->threshold_min1[smp[id].unit] = -2147483647;
					BC->threshold_max1[smp[id].unit] = sst->min1;
					BC->threshold_min2[smp[id].unit] = -2147483647;
					BC->threshold_max2[smp[id].unit] = sst->min2;
				} else {
					BA->com_return_error(data, sizeof(MessageHeader), MESSAGE_ERROR_CODE_INVALID_PARAMETER, com);
				}

				BC->threshold_period_current[smp[id].unit] = BC->threshold_debounce;

				logbli("set threshold: %c %d %d %d %d (%d)\n\r", sst->option,
				                                                 sst->min1,
				                                                 sst->max1,
				                                                 sst->min2,
				                                                 sst->max2,
				                                                 smp[id].unit);
				break;
			}

			case SIMPLE_TRANSFER_DEBOUNCE: {
				SimpleSetDebounce *ssd = ((SimpleSetDebounce*)data);
				BC->threshold_debounce = ssd->debounce;

				logbli("set debounce: %d\n\r", ssd->debounce);
				break;
			}

			switch(smp[id].transfer) {
				case SIMPLE_TRANSFER_PERIOD:
				case SIMPLE_TRANSFER_DEBOUNCE:
				case SIMPLE_TRANSFER_THRESHOLD: {
					BA->com_return_setter(com, data);
					break;
				}
			}
		}
	} else if(smp[id].direction == SIMPLE_DIRECTION_GET) {
		switch(smp[id].transfer) {
			case SIMPLE_TRANSFER_VALUE: {
				SimpleGetValue* sgv = (SimpleGetValue*)data;
				SimpleGetValueReturn sgvr;
				sgvr.header = sgv->header;
				sgvr.header.length = sizeof(SimpleGetValueReturn);

				sgvr.value1 = BC->value1[smp[id].unit];
				sgvr.value2 = BC->value2[smp[id].unit];

				BA->send_blocking_with_timeout(&sgvr,
				                               sizeof(SimpleGetValueReturn),
				                               com);

				logbli("get value: %d %d\n\r", sgvr.value1, sgvr.value2);
				break;
			}

			case SIMPLE_TRANSFER_PERIOD: {
				SimpleGetPeriod* sgp = (SimpleGetPeriod*)data;
				SimpleGetPeriodReturn sgpr;
				sgpr.header = sgp->header;
				sgpr.header.length = sizeof(SimpleGetPeriodReturn);

				sgpr.period = BC->signal_period[smp[id].unit];

				BA->send_blocking_with_timeout(&sgpr,
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

				sgtr.option = BC->threshold_option_save[smp[id].unit];
				sgtr.min1 = BC->threshold_min_save1[smp[id].unit];
				sgtr.max1 = BC->threshold_max_save1[smp[id].unit];
				sgtr.min2 = BC->threshold_min_save2[smp[id].unit];
				sgtr.max2 = BC->threshold_max_save2[smp[id].unit];

				BA->send_blocking_with_timeout(&sgtr,
				                               sizeof(SimpleGetThresholdReturn),
				                               com);

				logbli("get threshold: %c %d %d %d %d (%d)\n\r", sgtr.option,
				                                                 sgtr.min1,
				                                                 sgtr.max1,
				                                                 sgtr.min2,
				                                                 sgtr.max2,
				                                                 smp[id].unit);
				break;
			}

			case SIMPLE_TRANSFER_DEBOUNCE: {
				SimpleGetDebounce* sgd = (SimpleGetDebounce*)data;
				SimpleGetDebounceReturn sgdr;
				sgdr.header = sgd->header;
				sgdr.header.length = sizeof(SimpleGetThresholdReturn);

				sgdr.debounce = BC->threshold_debounce;

				BA->send_blocking_with_timeout(&sgdr,
				                               sizeof(SimpleGetDebounceReturn),
				                               com);

				logbli("get debounce: %d\n\r", sgdr.debounce);
				break;
			}
		}
	}
}

void simple_constructor(void) {
	BC->threshold_debounce = 100;

	for(uint8_t i = 0; i < NUM_SIMPLE_VALUES; i++) {
		BC->value1[i] = 0;
		BC->value2[i] = 0;
		BC->last_value1[i] = 0xFFFFFFFF;
		BC->last_value2[i] = 0xFFFFFFFF;
		BC->signal_period[i] = 0;
		BC->signal_period_counter[i] = 0;
		BC->threshold_period_current[i] = 0;
		BC->threshold_min1[i] = 0;
		BC->threshold_max1[i] = 0;
		BC->threshold_min2[i] = 0;
		BC->threshold_max2[i] = 0;
		BC->threshold_option[i] = 'x';

		BC->threshold_min_save1[i] = 0;
		BC->threshold_max_save1[i] = 0;
		BC->threshold_min_save2[i] = 0;
		BC->threshold_max_save2[i] = 0;
		BC->threshold_option_save[i] = 'x';
	}

	BC->tick = 0;

	logbli("simple constructor\n\r");
}

void simple_destructor(void) {
	simple_constructor();
	logbli("simple destructor\n\r");
}

void simple_tick(uint8_t tick_type) {
	if(tick_type & TICK_TASK_TYPE_CALCULATION) {
		// Get values
		for(uint8_t i = 0; i < NUM_SIMPLE_VALUES; i++) {
			if(BC->tick % 2) {
				BC->value1[i] = BRICKLET_OFFSET_SIMPLE(sup[i].func1)(BC->value1[sup[i].call_value_id]);
			} else {
				BC->value2[i] = BRICKLET_OFFSET_SIMPLE(sup[i].func2)(BC->value2[sup[i].call_value_id]);
			}
		}

		for(uint8_t i = 0; i < NUM_SIMPLE_VALUES; i++) {
			if(BC->signal_period_counter[i] < UINT32_MAX) {
				BC->signal_period_counter[i]++;
			}

			if(BC->threshold_period_current[i] != BC->threshold_debounce) {
				BC->threshold_period_current[i]++;
			}
		}

		BC->tick++;
	}

	if(tick_type & TICK_TASK_TYPE_MESSAGE) {
		// Handle period signals
		for(uint8_t i = 0; i < NUM_SIMPLE_VALUES; i++) {
			if(BC->signal_period[i] != 0 &&
			   BC->signal_period[i] <= BC->signal_period_counter[i]) {
				if(BC->last_value1[i] != BC->value1[i] ||
				   BC->last_value2[i] != BC->value2[i]) {
					SimpleGetValueReturn sgvr;
					BA->com_make_default_header(&sgvr, BS->uid, sizeof(SimpleGetValueReturn), sup[i].fid_period);
					sgvr.value1 = BC->value1[i];
					sgvr.value2 = BC->value2[i];

					BA->send_blocking_with_timeout(&sgvr,
												   sizeof(SimpleGetValueReturn),
												   *BA->com_current);

					BC->signal_period_counter[i] = 0;
					BC->last_value1[i] = BC->value1[i];
					BC->last_value2[i] = BC->value2[i];
					logbli("period value: %d %d\n\r", BC->value1[i],
					                                  BC->value2[i]);
				}
			}
		}

		// Handle threshold signals
		for(uint8_t i = 0; i < NUM_SIMPLE_VALUES; i++) {
			int32_t value1 = BC->value1[i];
			int32_t value2 = BC->value2[i];

			if(((BC->threshold_option[i] == 'o') &&
				(((value1 < BC->threshold_min1[i]) ||
				  (value1 > BC->threshold_max1[i])) ||
				 ((value2 < BC->threshold_min2[i]) ||
				  (value2 > BC->threshold_max2[i])))) ||
			   (((BC->threshold_option[i] == 'i') &&
				 ((value1 > BC->threshold_min1[i]) &&
				  (value1 < BC->threshold_max1[i])) &&
				 ((value2 > BC->threshold_min2[i]) &&
				  (value2 < BC->threshold_max2[i]))))) {

				if(BC->threshold_period_current[i] == BC->threshold_debounce) {
					SimpleGetValueReturn sgvr;
					BA->com_make_default_header(&sgvr, BS->uid, sizeof(SimpleGetValueReturn), sup[i].fid_reached);
					sgvr.value1 = value1;
					sgvr.value2 = value2;
					BA->send_blocking_with_timeout(&sgvr,
												   sizeof(SimpleGetValueReturn),
												   *BA->com_current);
					BC->threshold_period_current[i] = 0;

					logbli("threshold value: %d %d\n\r", BC->value1[i],
														 BC->value2[i]);
				}
			}
		}
	}
}
#endif
