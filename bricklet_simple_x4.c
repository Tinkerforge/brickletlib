/* brickletlib
 * Copyright (C) 2014 Ishraq Ibne Ashraf <ishraq@tinkerforge.com>
 *
 * bricklet_simple_x4.c: Functionality for simple sensor bricklets with 4 values
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

#include "bricklet_simple_x4.h"

#ifdef BRICKLET_HAS_SIMPLE_X4

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

	BrickContext *BC_local = BC;
	BrickletAPI *BA_local = BA;

	if(smp[id].direction == SIMPLE_DIRECTION_SET) {
		switch(smp[id].transfer) {
			case SIMPLE_TRANSFER_VALUE: {
				break;
			}

			case SIMPLE_TRANSFER_PERIOD: {
				SimpleSetPeriod *ssp = ((SimpleSetPeriod*)data);
				BC_local->signal_period[smp[id].unit] = ssp->period;

				logbli("set period: %d\n\r", ssp->period);
				break;
			}

			case SIMPLE_TRANSFER_THRESHOLD: {
				SimpleSetThreshold* sst = (SimpleSetThreshold*)data;
				BC_local->threshold_option_save[smp[id].unit] = sst->option;
				BC_local->threshold_min_save1[smp[id].unit] = sst->min1;
				BC_local->threshold_max_save1[smp[id].unit] = sst->max1;
				BC_local->threshold_min_save2[smp[id].unit] = sst->min2;
				BC_local->threshold_max_save2[smp[id].unit] = sst->max2;
				BC_local->threshold_min_save3[smp[id].unit] = sst->min3;
				BC_local->threshold_max_save3[smp[id].unit] = sst->max3;
                BC_local->threshold_min_save4[smp[id].unit] = sst->min4;
				BC_local->threshold_max_save4[smp[id].unit] = sst->max4;

				BC_local->threshold_min1[smp[id].unit] = sst->min1;
				BC_local->threshold_max1[smp[id].unit] = sst->max1;
				BC_local->threshold_min2[smp[id].unit] = sst->min2;
				BC_local->threshold_max2[smp[id].unit] = sst->max2;
				BC_local->threshold_min3[smp[id].unit] = sst->min3;
				BC_local->threshold_max3[smp[id].unit] = sst->max3;
                BC_local->threshold_min4[smp[id].unit] = sst->min4;
				BC_local->threshold_max4[smp[id].unit] = sst->max4;

				if(sst->option == 'o' ||
				   sst->option == 'i' ||
				   sst->option == 'x') {
					BC_local->threshold_option[smp[id].unit] = sst->option;
				} else if(sst->option == '<') {
					BC_local->threshold_option[smp[id].unit] = 'o';
					BC_local->threshold_max1[smp[id].unit] = 2147483647;
					BC_local->threshold_max2[smp[id].unit] = 2147483647;
					BC_local->threshold_max3[smp[id].unit] = 2147483647;
					BC_local->threshold_max4[smp[id].unit] = 2147483647;
				} else if(sst->option == '>') {
					BC_local->threshold_option[smp[id].unit] = 'o';
					BC_local->threshold_min1[smp[id].unit] = -2147483647;
					BC_local->threshold_max1[smp[id].unit] = sst->min1;
					BC_local->threshold_min2[smp[id].unit] = -2147483647;
					BC_local->threshold_max2[smp[id].unit] = sst->min2;
					BC_local->threshold_min3[smp[id].unit] = -2147483647;
					BC_local->threshold_max3[smp[id].unit] = sst->min3;
                    BC_local->threshold_min4[smp[id].unit] = -2147483647;
					BC_local->threshold_max4[smp[id].unit] = sst->min4;
				} else {
					BA_local->com_return_error(data, sizeof(MessageHeader), MESSAGE_ERROR_CODE_INVALID_PARAMETER, com);
					return;
				}

				BC_local->threshold_period_current[smp[id].unit] = BC_local->threshold_debounce;

				logbli("set threshold: %c %d %d %d %d %s %d (%d)\n\r", sst->option,
				                                                       sst->min1,
				                                                       sst->max1,
				                                                       sst->min2,
				                                                       sst->max2,
				                                                       sst->min3,
				                                                       sst->max3,
                                                                       sst->min4,
				                                                       sst->max4,
				                                                       smp[id].unit);
				break;
			}

			case SIMPLE_TRANSFER_DEBOUNCE: {
				SimpleSetDebounce *ssd = ((SimpleSetDebounce*)data);
				BC_local->threshold_debounce = ssd->debounce;

				logbli("set debounce: %d\n\r", ssd->debounce);
				break;
			}
		}

		switch(smp[id].transfer) {
			case SIMPLE_TRANSFER_PERIOD:
			case SIMPLE_TRANSFER_DEBOUNCE:
			case SIMPLE_TRANSFER_THRESHOLD: {
				BA_local->com_return_setter(com, data);
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

				sgvr.value1 = BC_local->value1[smp[id].unit];
				sgvr.value2 = BC_local->value2[smp[id].unit];
                sgvr.value3 = BC_local->value3[smp[id].unit];
                sgvr.value4 = BC_local->value4[smp[id].unit];

                BA_local->send_blocking_with_timeout(&sgvr,
				                                     sizeof(SimpleGetValueReturn),
				                                     com);

				logbli("get value: %d %d %d %d\n\r", sgvr.value1, sgvr.value2, sgvr.value3, sgvr.value4);
				break;
			}

			case SIMPLE_TRANSFER_PERIOD: {
				SimpleGetPeriod* sgp = (SimpleGetPeriod*)data;
				SimpleGetPeriodReturn sgpr;
				sgpr.header = sgp->header;
				sgpr.header.length = sizeof(SimpleGetPeriodReturn);

				sgpr.period = BC_local->signal_period[smp[id].unit];

				BA_local->send_blocking_with_timeout(&sgpr,
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

				sgtr.option = BC_local->threshold_option_save[smp[id].unit];
				sgtr.min1 = BC_local->threshold_min_save1[smp[id].unit];
				sgtr.max1 = BC_local->threshold_max_save1[smp[id].unit];
				sgtr.min2 = BC_local->threshold_min_save2[smp[id].unit];
				sgtr.max2 = BC_local->threshold_max_save2[smp[id].unit];
				sgtr.min3 = BC_local->threshold_min_save3[smp[id].unit];
				sgtr.max3 = BC_local->threshold_max_save3[smp[id].unit];
                sgtr.min4 = BC_local->threshold_min_save4[smp[id].unit];
				sgtr.max4 = BC_local->threshold_max_save4[smp[id].unit];

				BA_local->send_blocking_with_timeout(&sgtr,
				                                     sizeof(SimpleGetThresholdReturn),
				                                     com);

				logbli("get threshold: %c %d %d %d %d %d %d %d %d (%d)\n\r", sgtr.option,
				                                                       sgtr.min1,
				                                                       sgtr.max1,
				                                                       sgtr.min2,
				                                                       sgtr.max2,
				                                                       sgtr.min3,
				                                                       sgtr.max3,
                                                                       sgtr.min4,
				                                                       sgtr.max4,
				                                                       smp[id].unit);
				break;
			}

			case SIMPLE_TRANSFER_DEBOUNCE: {
				SimpleGetDebounce* sgd = (SimpleGetDebounce*)data;
				SimpleGetDebounceReturn sgdr;
				sgdr.header = sgd->header;
				sgdr.header.length = sizeof(SimpleGetDebounceReturn);

				sgdr.debounce = BC_local->threshold_debounce;

				BA_local->send_blocking_with_timeout(&sgdr,
				                                     sizeof(SimpleGetDebounceReturn),
				                                     com);

				logbli("get debounce: %d\n\r", sgdr.debounce);
				break;
			}
		}
	}
}

void simple_constructor(void) {
	BrickContext *BC_local = BC;

	// We don't set the stuff that is initially 0, since BC is set to 0 by Brick on startup
	BC_local->threshold_debounce = 100;

	for(uint8_t i = 0; i < NUM_SIMPLE_VALUES; i++) {
		BC_local->threshold_option[i] = 'x';
		BC_local->threshold_option_save[i] = 'x';
	}

	logbli("simple constructor\n\r");
}

void simple_tick(uint8_t tick_type) {
	BrickContext *BC_local = BC;
	BrickletAPI *BA_local = BA;
	uint32_t uid = BS->uid;

	if(tick_type & TICK_TASK_TYPE_CALCULATION) {
		for(uint8_t i = 0; i < NUM_SIMPLE_VALUES; i++) {
			if(BC_local->signal_period_counter[i] < UINT32_MAX) {
				BC_local->signal_period_counter[i]++;
			}

			if(BC_local->threshold_period_current[i] != BC_local->threshold_debounce) {
				BC_local->threshold_period_current[i]++;
			}
		}

		BC_local->tick++;
	}

	if(tick_type & TICK_TASK_TYPE_MESSAGE) {
		// Handle period signals
		for(uint8_t i = 0; i < NUM_SIMPLE_VALUES; i++) {
			if(BC_local->signal_period[i] != 0 &&
			   BC_local->signal_period[i] <= BC_local->signal_period_counter[i]) {
				if(BC_local->last_value1[i] != BC_local->value1[i] ||
				   BC_local->last_value2[i] != BC_local->value2[i] ||
                   BC_local->last_value3[i] != BC_local->value3[i] ||
                   BC_local->last_value4[i] != BC_local->value4[i]) {
					SimpleGetValueReturn sgvr;
					BA_local->com_make_default_header(&sgvr, uid, sizeof(SimpleGetValueReturn), sup[i].fid_period);
					sgvr.value1 = BC_local->value1[i];
					sgvr.value2 = BC_local->value2[i];
                    sgvr.value3 = BC_local->value3[i];
                    sgvr.value4 = BC_local->value4[i];

                    BA_local->send_blocking_with_timeout(&sgvr,
												         sizeof(SimpleGetValueReturn),
												         *BA_local->com_current);

					BC_local->signal_period_counter[i] = 0;
					BC_local->last_value1[i] = BC_local->value1[i];
					BC_local->last_value2[i] = BC_local->value2[i];
                    BC_local->last_value3[i] = BC_local->value3[i];
                    BC_local->last_value4[i] = BC_local->value4[i];
					logbli("period value: %d %d %d %d\n\r", BC_local->value1[i],
					                                        BC_local->value2[i],
                                                            BC_local->value3[i],
                                                            BC_local->value4[i]);
				}
			}
		}

		// Handle threshold signals
		for(uint8_t i = 0; i < NUM_SIMPLE_VALUES; i++) {
			int32_t value1 = BC_local->value1[i];
			int32_t value2 = BC_local->value2[i];
            int32_t value3 = BC_local->value3[i];
            int32_t value4 = BC_local->value4[i];

			if(((BC_local->threshold_option[i] == 'o') &&
				(((value1 < BC_local->threshold_min1[i]) ||
				  (value1 > BC_local->threshold_max1[i])) ||
				 ((value2 < BC_local->threshold_min2[i]) ||
				  (value2 > BC_local->threshold_max2[i])) ||
				 ((value3 < BC_local->threshold_min3[i]) ||
				  (value3 > BC_local->threshold_max3[i])) ||
                 ((value4 < BC_local->threshold_min4[i]) ||
                  (value4 > BC_local->threshold_max4[i])))) ||
			   (((BC_local->threshold_option[i] == 'i') &&
				 ((value1 > BC_local->threshold_min1[i]) &&
				  (value1 < BC_local->threshold_max1[i])) &&
				 ((value2 > BC_local->threshold_min2[i]) &&
				  (value2 < BC_local->threshold_max2[i])) &&
				 ((value3 > BC_local->threshold_min3[i]) &&
				  (value3 < BC_local->threshold_max3[i])) &&
                 ((value4 > BC_local->threshold_min4[i]) &&
                  (value4 < BC_local->threshold_max4[i]))))) {

				if(BC_local->threshold_period_current[i] == BC_local->threshold_debounce) {
					SimpleGetValueReturn sgvr;
					BA_local->com_make_default_header(&sgvr, uid, sizeof(SimpleGetValueReturn), sup[i].fid_reached);
					sgvr.value1 = value1;
					sgvr.value2 = value2;
					sgvr.value3 = value3;
                    sgvr.value4 = value4;
                    BA_local->send_blocking_with_timeout(&sgvr,
												         sizeof(SimpleGetValueReturn),
												         *BA_local->com_current);
					BC_local->threshold_period_current[i] = 0;

					logbli("threshold value: %d %d %d %d\n\r", BC_local->value1[i],
					                                           BC_local->value2[i],
					                                           BC_local->value3[i],
					                                           BC_local->value4[i]);
				}
			}
		}
	}
}
#endif
