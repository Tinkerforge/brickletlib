/* brickletlib
 * Copyright (C) 2013 Olaf Lüke <olaf@tinkerforge.com>
 *
 * bricklet_simple_sensor_x2.c: Functionality for simple bricklets with two identical sensors
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

#include "bricklet_simple_sensor_x2.h"

#ifdef BRICKLET_HAS_SIMPLE_SENSOR_SENSOR_X2

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
				BC->signal_period[ssp->sensor] = ssp->period;

				logbli("set period (%d): %d\n\r", ssp->sensor, ssp->period);
				break;
			}

			case SIMPLE_TRANSFER_THRESHOLD: {
				SimpleSetThreshold* sst = (SimpleSetThreshold*)data;
				BC->threshold_option_save[sst->sensor] = sst->option;
				BC->threshold_min_save[sst->sensor] = sst->min;
				BC->threshold_max_save[sst->sensor] = sst->max;

				if(sst->option == 'o' ||
				   sst->option == 'i' ||
				   sst->option == 'x') {
					BC->threshold_option[sst->sensor] = sst->option;
					BC->threshold_min[sst->sensor] = sst->min;
					BC->threshold_max[sst->sensor] = sst->max;
				} else if(sst->option == '<') {
					BC->threshold_option[sst->sensor] = 'o';
					BC->threshold_min[sst->sensor] = sst->min;
					BC->threshold_max[sst->sensor] = 2147483647;
				} else if(sst->option == '>') {
					BC->threshold_option[sst->sensor] = 'o';
					BC->threshold_min[sst->sensor] = -2147483647;
					BC->threshold_max[sst->sensor] = sst->min;
				} else {
					BA->com_return_error(data, sizeof(MessageHeader), MESSAGE_ERROR_CODE_INVALID_PARAMETER, com);
				}

				BC->threshold_period_current[sst->sensor] = BC->threshold_debounce;

				logbli("set threshold: %c %d %d (%d)\n\r", sst->option,
				                                           sst->min,
				                                           sst->max,
				                                           sst->sensor);
				break;
			}

			case SIMPLE_TRANSFER_DEBOUNCE: {
				SimpleSetDebounce *ssd = ((SimpleSetDebounce*)data);
				BC->threshold_debounce = ssd->debounce;

				logbli("set debounce: %d\n\r", ssd->debounce);
				break;
			}
		}

		switch(smp[id].transfer) {
			case SIMPLE_TRANSFER_PERIOD:
			case SIMPLE_TRANSFER_DEBOUNCE:
			case SIMPLE_TRANSFER_THRESHOLD: {
				BA->com_return_setter(com, data);
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
				sgvr.value = BC->value[sgv->sensor];

				BA->send_blocking_with_timeout(&sgvr,
				                               sizeof(SimpleGetValueReturn),
				                               com);

				logbli("get value (%d): %d\n\r", sgv->sensor, sgvr.value);
				break;
			}

			case SIMPLE_TRANSFER_PERIOD: {
				SimpleGetPeriod* sgp = (SimpleGetPeriod*)data;
				SimpleGetPeriodReturn sgpr;
				sgpr.header = sgp->header;
				sgpr.header.length = sizeof(SimpleGetPeriodReturn);

				sgpr.period = BC->signal_period[sgp->sensor];

				BA->send_blocking_with_timeout(&sgpr,
				                               sizeof(SimpleGetPeriodReturn),
				                               com);

				logbli("get period (%d): %d\n\r", sgp->sensor, sgpr.period);
				break;
			}

			case SIMPLE_TRANSFER_THRESHOLD: {
				SimpleGetThreshold* sgt = (SimpleGetThreshold*)data;
				SimpleGetThresholdReturn sgtr;
				sgtr.header = sgt->header;
				sgtr.header.length = sizeof(SimpleGetThresholdReturn);

				sgtr.option = BC->threshold_option_save[sgt->sensor];
				sgtr.min = BC->threshold_min_save[sgt->sensor];
				sgtr.max = BC->threshold_max_save[sgt->sensor];


				BA->send_blocking_with_timeout(&sgtr,
				                               sizeof(SimpleGetThresholdReturn),
				                               com);

				logbli("get threshold: %c %d %d (%d)\n\r", sgtr.option,
				                                           sgtr.min,
				                                           sgtr.max,
				                                           sgt->sensor);
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
		BC->value[i] = 0;
		BC->last_value[i] = 0xFFFFFFFF;
		BC->signal_period[i] = 0;
		BC->signal_period_counter[i] = 0;
		BC->threshold_period_current[i] = 0;
		BC->threshold_min[i] = 0;
		BC->threshold_max[i] = 0;
		BC->threshold_option[i] = 'x';

		BC->threshold_min_save[i] = 0;
		BC->threshold_max_save[i] = 0;
		BC->threshold_option_save[i] = 'x';
	}

	BC->tick = 0;

	logbli("simple constructor\n\r");
}

void simple_destructor(void) {
	simple_constructor();
	logbli("simple destructor\n\r");
}

void simple_tick(const uint8_t tick_type) {
	if(tick_type & TICK_TASK_TYPE_CALCULATION) {
		// Get values

		// If only one simple value is used, gcc removes the for loop and inserts
		// the function, in this case there is no offset needed
#ifdef BRICKLET_VALUE_APPLIED_OUTSIDE
		// Nothing to do here
#else
#ifdef BRICKLET_NO_OFFSET
		sup[0].func(0);
#else
		for(uint8_t i = 0; i < NUM_SIMPLE_VALUES; i++) {
			BRICKLET_OFFSET_SIMPLE(sup[0].func)(0);
		}
#endif
#endif
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
				if(BC->last_value[i] != BC->value[i]) {
					SimpleCallback sc;
					BA->com_make_default_header(&sc, BS->uid, sizeof(SimpleCallback), sup[0].fid_period);
					sc.sensor = i;
					sc.value = BC->value[i];


					BA->send_blocking_with_timeout(&sc,
												   sizeof(SimpleCallback),
												   *BA->com_current);

					logbli("period value: %d\n\r", BC->value[i]);
					BC->signal_period_counter[i] = 0;
					BC->last_value[i] = BC->value[i];
				}
			}
		}

		// Handle threshold signals
		for(uint8_t i = 0; i < NUM_SIMPLE_VALUES; i++) {
			int32_t value = BC->value[i];

			if(((BC->threshold_option[i] == 'o') &&
				((value < BC->threshold_min[i]) ||
				 (value > BC->threshold_max[i]))) ||
			   ((BC->threshold_option[i] == 'i') &&
				((value > BC->threshold_min[i]) &&
				 (value < BC->threshold_max[i])))) {

				if(BC->threshold_period_current[i] == BC->threshold_debounce) {
					SimpleCallback sc;
					BA->com_make_default_header(&sc, BS->uid, sizeof(SimpleCallback), sup[0].fid_reached);
					sc.sensor = i;
					sc.value = value;

					BA->send_blocking_with_timeout(&sc,
												   sizeof(SimpleCallback),
												   *BA->com_current);
					BC->threshold_period_current[i] = 0;

					logbli("threshold value: %d\n\r", BC->value[i]);
				}
			}
		}
	}
}
#endif
