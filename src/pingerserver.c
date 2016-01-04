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


#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>

#include "pingerserver.h"
#include "ui.h"
#include "cfg.h"
#include "log.h"

char *program_name;
cfg *conf;

static volatile int keep_running = 1;

int main(int argc, char **argv) {
    program_name = argv[0];

    signal(SIGINT, signal_handler);

    cfg_init();

    if (cfg_parse(argc, argv)) {
        log_restart();
        cfg_print();
        main_program();
    }

    cfg_free();

    return 0;
}


/**
 * Signal handler
 */

void signal_handler(int signal) {
    if (signal == SIGINT) {
        keep_running = 0;
    }
}


/**
 * Main program function
 */

void main_program() {
}
