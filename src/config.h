/*
 *   The HellNet.org Pinger Server
 *   Copyright (C) 2016  Luca Cireddu
 *   sardylan@gmail.com
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License along
 *   with this program; if not, write to the Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 */


#ifndef __CONFIG_H
#define __CONFIG_H

#define DEFAULT_UI_DEBUG_LEVEL 4
#define DEFAULT_LOG_FILE_LEVEL 0
#define DEFAULT_LOG_FILE_NAME "pingerserver.log"

#define DEFAULT_UDP_PORT 62485
#define DEFAULT_UDP_BUFFER 2048

#define UI_MESSAGES_OUTPUT stderr
#define UI_MESSAGES_MAX_LENGTH 60

#endif
