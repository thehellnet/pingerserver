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


#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>
#include <string.h>

#include "config.h"
#include "ui.h"
#include "cfg.h"
#include "utils.h"
#include "log.h"


extern cfg *conf;
extern const char *program_name;


/**
 * Usage information message
 */

void ui_usage() {
    fprintf(stderr, "Usage:\n");
    fprintf(stderr, "    %s [<option> <value> ...]\n", program_name);
}


/**
 * Version information
 */

void ui_version() {
    fprintf(stderr, "SMLGR Inverter Daemon\n");
}


/**
 * Help information message with all command line options
 */

void ui_help() {
    fprintf(stderr, "The HellNet.org Pinger Server\n");
    fprintf(stderr, "\n");

    ui_usage();

    fprintf(stderr, "\n");
    fprintf(stderr, "Options list with default values:\n");
    fprintf(stderr, "\n");
    fprintf(stderr, "    -c | --config            Config file (overwrite all other options)\n");
    fprintf(stderr, "\n");
    fprintf(stderr, "    -h | --help              This message\n");
    fprintf(stderr, "    -V | --version           Print Version and exit\n");
    fprintf(stderr, "\n");
    fprintf(stderr, "    -q | --quiet             Disable output (debug level 0)\n");
    fprintf(stderr, "    -v | --verbose           Verbose output (debug level 4)\n");
    fprintf(stderr, "    -d | --debug             Debug level (%d)\n", DEFAULT_UI_DEBUG_LEVEL);
    fprintf(stderr, "                             0 DISABLE (quiet)\n");
    fprintf(stderr, "                             1 ERROR\n");
    fprintf(stderr, "                             2 WARNING\n");
    fprintf(stderr, "                             3 INFO\n");
    fprintf(stderr, "                             4 DEBUG (verbose)\n");
    fprintf(stderr, "\n");
    fprintf(stderr, "    -l | --log-file-level    Log file level (%d)\n", DEFAULT_LOG_FILE_LEVEL);
    fprintf(stderr, "                             0 DISABLE\n");
    fprintf(stderr, "                             1 ERROR\n");
    fprintf(stderr, "                             2 WARNING\n");
    fprintf(stderr, "                             3 INFO\n");
    fprintf(stderr, "                             4 DEBUG\n");
    fprintf(stderr, "\n");
    fprintf(stderr, "    -k | --log-file          Log file name and path (%s)\n", DEFAULT_LOG_FILE_NAME);
    fprintf(stderr, "\n");
    fprintf(stderr, "\n");
    fprintf(stderr, "\n");
    fprintf(stderr, "    -p | --udp-port          UDP Port (%d)\n", DEFAULT_UDP_PORT);
    fprintf(stderr, "\n");
    fprintf(stderr, "All outputs are on Standard Error.\n");
}


/**
 * Logging function
 */

void ui_message(int level, char *where, char *input, ...) {
    va_list args;
    char datetime[20];
    time_t rawtime;
    struct tm *timeinfo;
    char content[131072];
    char prefix[1025];

    if (level <= conf->debug_level) {
        rawtime = time(NULL);
        timeinfo = localtime(&rawtime);
        strftime(datetime, 20, "%Y-%m-%d %H:%M:%S", timeinfo);

        va_start(args, input);

        if (level == UI_ERROR)
            sprintf(prefix, "%s [ERROR] {%s} ", datetime, where);
        if (level == UI_WARNING)
            sprintf(prefix, "%s [WARN]  {%s} ", datetime, where);
        if (level == UI_INFO)
            sprintf(prefix, "%s [INFO]  {%s} ", datetime, where);
        if (level == UI_DEBUG)
            sprintf(prefix, "%s [DEBUG] {%s} ", datetime, where);

        fprintf(UI_MESSAGES_OUTPUT, "%s", prefix);

        memset(content, '\0', sizeof(content));
        vsprintf(content, input, args);
        truncate_string(content, UI_MESSAGES_MAX_LENGTH);

        fprintf(UI_MESSAGES_OUTPUT, "%s\n", content);

        va_end(args);
    }

    log_file_message(level, prefix, content);
}
