/* bricklib
 * Copyright (C) 2011 Olaf Lüke <olaf@tinkerforge.com>
 *
 * debugging.h: Debugging functionality for Bricklets
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

#ifndef BRICKLET_DEBUG_H
#define BRICKLET_DEBUG_H

#include "bricklib/logging/logging.h"

#include "config.h"

#if DEBUG_BRICKLET

#if LOGGING_LEVEL <= LOGGING_DEBUG
#define logbld(str, ...) do{ BA->printf("<D> bl: " str, ##__VA_ARGS__); }while(0)
#define logblwohd(str, ...) do{ BA->printf(str, ##__VA_ARGS__); }while(0)
#else
#define logbld(str, ...) {}
#define logblwohd(str, ...) {}
#endif

#if LOGGING_LEVEL <= LOGGING_INFO
#define logbli(str, ...) do{ BA->printf("<I> bl: " str, ##__VA_ARGS__); }while(0)
#define logblwohi(str, ...) do{ BA->printf(str, ##__VA_ARGS__); }while(0)
#else
#define logbli(str, ...) {}
#define logblwohi(str, ...) {}
#endif

#if LOGGING_LEVEL <= LOGGING_WARNING
#define logblw(str, ...) do{ BA->printf("<W> bl: " str, ##__VA_ARGS__); }while(0)
#define logblwohw(str, ...) do{ BA->printf(str, ##__VA_ARGS__); }while(0)
#else
#define logblw(str, ...) {}
#define logblwohw(str, ...) {}
#endif

#if LOGGING_LEVEL <= LOGGING_ERROR
#define logble(str, ...) do{ BA->printf("<E> bl: " str, ##__VA_ARGS__); }while(0)
#define logblwohe(str, ...) do{ BA->printf(str, ##__VA_ARGS__); }while(0)
#else
#define logble(str, ...) {}
#define logblwohe(str, ...) {}
#endif

#if LOGGING_LEVEL <= LOGGING_FATAL
#define logblf(str, ...) do{ BA->printf("<F> bl: " str, ##__VA_ARGS__); }while(0)
#define logblwohf(str, ...) do{ BA->printf(str, ##__VA_ARGS__); }while(0)
#else
#define logblf(str, ...) {}
#define logblwohf(str, ...) {}
#endif

#else

#define logbld(str, ...) {}
#define logblwohd(str, ...) {}
#define logbli(str, ...) {}
#define logblwohi(str, ...) {}
#define logblw(str, ...) {}
#define logblwohw(str, ...) {}
#define logble(str, ...) {}
#define logblwohe(str, ...) {}
#define logblf(str, ...) {}
#define logblwohf(str, ...) {}

#endif

#endif
