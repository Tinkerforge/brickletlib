/* brickletlib
 * Copyright (C) 2010-2012 Olaf Lüke <olaf@tinkerforge.com>
 *
 * bricklet_entry.h: Implementation of bricklet entry
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

#ifndef BRICKLET_ENTRY_H
#define BRICKLET_ENTRY_H

#include <stdint.h>

#include "config.h"

#include "bricklib/bricklet/bricklet_config.h"
#include "bricklib/com/com_messages.h"

typedef int32_t (*value_handler_func_t)(int32_t);

#define BA (*((BrickletAPI**)(((int)entry)-1 + BRICKLET_PLUGIN_MAX_SIZE - 4)))
#define BS (*((BrickletSettings**)(((int)entry)-1 + BRICKLET_PLUGIN_MAX_SIZE - 8)))
#define BSO(diff) (*((BrickletSettings**)(((int)entry)-1 + BRICKLET_PLUGIN_MAX_SIZE + ((diff)*BRICKLET_PLUGIN_MAX_SIZE) - 8)))
#define BSO_DIRECT(diff) ((BrickletSettings*)BS + (diff))
#define BC (*((BrickContext**)(((int)entry)-1 + BRICKLET_PLUGIN_MAX_SIZE - 12)))
#define BCO(diff) (*((BrickContext**)(((int)entry)-1 + BRICKLET_PLUGIN_MAX_SIZE + ((diff)*BRICKLET_PLUGIN_MAX_SIZE) - 12)))
#define BCO_DIRECT(diff) (((BrickContext*)(((uint32_t)BC) + (diff)*64*4)))

#define ENTRY_ADDRESS ((int)entry)
#define BRICKLET_OFFSET(func) ((message_handler_func_t)(((int)func) + ENTRY_ADDRESS - 1))
#define BRICKLET_OFFSET_SIMPLE(func) ((value_handler_func_t)(((int)func) + ENTRY_ADDRESS - 1))

#define BRICKLET_TYPE_FROM_DATA(data) (((BrickletHeader*)data)->bricklet_type)


#ifndef ENTRY_IN_BRICKLET_CODE
void entry(uint8_t type, uint8_t com, uint8_t *data);
#endif

#ifndef INVOCATION_IN_BRICKLET_CODE
void invocation(uint8_t com, uint8_t *data);
#endif

#endif
