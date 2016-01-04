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
#include <string.h>
#include <getopt.h>

#include "config.h"
#include "ui.h"
#include "cfg.h"

extern cfg *conf;


/**
 * Configuration init with default values
 */

void cfg_init() {
    size_t ln;

    conf = (cfg *) malloc(sizeof(cfg));

    conf->debug_level = DEFAULT_UI_DEBUG_LEVEL;
    conf->log_file_level = DEFAULT_LOG_FILE_LEVEL;

    ln = strlen(DEFAULT_LOG_FILE_NAME) + 1;
    conf->log_file = (char *) calloc(sizeof(char), ln);
    strcpy(conf->log_file, DEFAULT_LOG_FILE_NAME);

    conf->udp_port = DEFAULT_UDP_PORT;
}


/**
 * Configuration print
 */

void cfg_print() {
    ui_message(UI_INFO, "CFG", "debug-level = %d", conf->debug_level);
    ui_message(UI_INFO, "CFG", "log-file-level = %d", conf->log_file_level);
    ui_message(UI_INFO, "CFG", "log-file = %s", conf->log_file);
    ui_message(UI_INFO, "CFG", "udp-port = %d", conf->udp_port);
}


/**
 * Configuration free
 */

void cfg_free() {
    free(conf->log_file);

    free(conf);
}


/**
 * Parsing options
 */

int cfg_parse(int argc, char **argv) {
    int ret = 0;
    int option_index = 0;
    int c;
    size_t ln;
    int conf_file = 0;
    char *config_file;

    static struct option long_options[] = {
            {"config",           required_argument, 0, 'c'},
            {"help",             no_argument,       0, 'h'},
            {"version",          no_argument,       0, 'V'},
            {"quiet",            no_argument,       0, 'q'},
            {"verbose",          no_argument,       0, 'v'},
            {"debug-level",      required_argument, 0, 'd'},
            {"log-file-level",   required_argument, 0, 'l'},
            {"log-file",         required_argument, 0, 'k'},
            {"udp-port",         required_argument, 0, 'p'},
            {0,                  0,                 0, 0}
    };

    config_file = (char *) malloc(sizeof(char));
    *config_file = '\0';

    while (1) {
        c = getopt_long(argc, argv, "c:hVqvd:k:l:p:", long_options, &option_index);

        if (c == -1) {
            ret = 1;
            break;
        }

        if (c == '?') {
            ui_help();
            break;
        }

        if (c == 'c') {
            conf_file = 1;

            ln = strlen(optarg) + 1;
            config_file = (char *) realloc(config_file, ln * sizeof(char));
            strcpy(config_file, optarg);
        }

        if (c == 'h') {
            ui_help();
            break;
        }

        if (c == 'V') {
            ui_version();
            break;
        }

        if (c == 'q') {
            conf->debug_level = 0;
        }

        if (c == 'v') {
            conf->debug_level = 4;
        }

        if (c == 'd') {
            conf->debug_level = atoi(optarg);
        }

        if (c == 'l') {
            conf->log_file_level = atoi(optarg);
        }

        if (c == 'k') {
            ln = strlen(optarg) + 1;
            conf->log_file = (char *) realloc((void *) conf->log_file, sizeof(char) * ln);
            strcpy(conf->log_file, optarg);
        }

        if (c == 'p') {
            conf->udp_port = (uint16_t) atoi(optarg);
        }
    }

    if (conf_file == 1)
        ret = cfg_file_parse(config_file);

    free(config_file);

    return ret;
}


/**
 * Parsing options
 * @param[in] config_file Path of a config file to parse
 * @param[out] ret Returns 0 in case of error, 1 if not
 */

int cfg_file_parse(char *config_file) {
    int ret = 0;
    FILE *fd;
    char param[80];
    char value[80];
    size_t ln;
    int linecount = 0;

    fd = fopen(config_file, "r");

    if (fd != NULL) {
        ui_message(UI_INFO, "CFG-FILE", "Parsing config file %s", config_file);

        while (!feof(fd)) {
            linecount++;

            bzero(param, sizeof(param));
            bzero(value, sizeof(value));

            if (fscanf(fd, "%s %s", param, value) != 2) {
                if (strlen(param) != 0 || strlen(value) != 0)
                    ui_message(UI_ERROR, "CFG-FILE", "Unable to parse config file in line %d", linecount);
                continue;
            }

            ui_message(UI_DEBUG, "CFG-FILE", "Param: %s - Value: %s", param, value);

            if (strcmp(param, "debug-level") == 0) {
                conf->debug_level = atoi(value);
                ui_message(UI_DEBUG, "CFG-FILE", "Configuration updated. debug_level = %d", conf->debug_level);
                continue;
            }

            if (strcmp(param, "log-file-level") == 0) {
                conf->log_file_level = atoi(value);
                ui_message(UI_DEBUG, "CFG-FILE", "Configuration updated. log_file_level = %d", conf->log_file_level);
                continue;
            }

            if (strcmp(param, "log-file") == 0) {
                ln = strlen(value) + 1;
                conf->log_file = (char *) realloc((void *) conf->log_file, sizeof(char) * ln);
                strcpy(conf->log_file, value);
                ui_message(UI_DEBUG, "CFG-FILE", "Configuration updated. log_file = %s", conf->log_file);
                continue;
            }

            if (strcmp(param, "udp-port") == 0) {
                conf->udp_port = (uint16_t) atoi(value);
                ui_message(UI_DEBUG, "CFG-FILE", "Configuration updated. inv_tcp_port = %d", conf->udp_port);
                continue;
            }

            ui_message(UI_ERROR, "CFG-FILE", "Unable to parse config file in line %d", linecount);
        }

        fclose(fd);

        ret = 1;
    } else {
        ui_message(UI_ERROR, "CFG-FILE", "Unable to open config file %s", config_file);
    }

    return ret;
}
