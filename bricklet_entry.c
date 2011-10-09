/* brickletlib
 * Copyright (C) 2010 Olaf Lüke <olaf@tinkerforge.com>
 *
 * bricklet_entry.c: Implementation of bricklet entry
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

#include "bricklet_entry.h"

#include <stdint.h>
#include <string.h>

#include "bricklib/bricklet/bricklet_config.h"
#include "config.h"

#ifndef ENTRY_IN_BRICKLET_CODE
void entry(uint8_t type, uint8_t com, uint8_t *data) {
	switch(type) {
		case BRICKLET_TYPE_INVOCATION: {
			invocation(com, data);
			break;
		}

		case BRICKLET_TYPE_CONSTRUCTOR: {
			constructor();
			break;
		}

		case BRICKLET_TYPE_DESTRUCTOR: {
			destructor();
			break;
		}

		case BRICKLET_TYPE_TICK: {
			tick();
			break;
		}

		case BRICKLET_TYPE_INFO: {
			BrickletInfo *bi = (BrickletInfo*)data;
			bi->firmware_version[0] = BRICKLET_FIRMWARE_VERSION_MAJOR;
			bi->firmware_version[1] = BRICKLET_FIRMWARE_VERSION_MINOR;
			bi->firmware_version[2] = BRICKLET_FIRMWARE_VERSION_REVISION;
			for(uint8_t i = 0; i < MAX_LENGTH_NAME; i++) {
				bi->name[i] = 0;
			}
			for(uint8_t i = 0; i < MAX_LENGTH_NAME; i++) {
				bi->name[i] = BRICKLET_HARDWARE_NAME[i];
				if(BRICKLET_HARDWARE_NAME[i] == '\0') {
					break;
				}
			}
			break;
		}
	}
}
#endif

#ifndef INVOCATION_IN_BRICKLET_CODE
extern const ComMessage com_messages[];
extern const uint8_t NUMBER_OF_MESSAGES;

void invocation(uint8_t com, uint8_t *data) {
	// TODO: direct call (com_message[id-1]())
	/*
	ComMessage *message = BA->get_com_from_type(BRICKLET_TYPE_FROM_DATA(data),
	                                            com_messages,
	                                            NUMBER_OF_MESSAGES);

	BRICKLET_OFFSET(message->reply_func)(com, data);*/
}
#endif
