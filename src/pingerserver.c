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
#include <signal.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

#include "pingerserver.h"
#include "ui.h"
#include "cfg.h"
#include "log.h"
#include "config.h"

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
    int sck;
    struct sockaddr_in si_me;
    struct sockaddr_in si_other;
    char buffer[DEFAULT_UDP_BUFFER];
    socklen_t slen = sizeof(si_other);
    char ipaddr[16];
    ssize_t ln;

    sck = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    if (sck == -1) {
        ui_message(UI_ERROR, "MAIN", "Unable to create socket");
        return;
    }

    memset((char *) &si_me, 0, sizeof(si_me));
    si_me.sin_family = AF_INET;
    si_me.sin_port = htons(conf->udp_port);
    si_me.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(sck, (const struct sockaddr *) &si_me, sizeof(si_me)) == -1) {
        ui_message(UI_ERROR, "MAIN", "Unable to bind socket");
        return;
    }
    while (keep_running == 1) {
        ln = recvfrom(sck, buffer, DEFAULT_UDP_BUFFER, 0, (struct sockaddr *) &si_other, &slen);
        if (ln == -1) {
            ui_message(UI_ERROR, "MAIN", "Unable to read from socket");
            break;
        }

        inet_ntop(AF_INET, &(si_other.sin_addr), ipaddr, INET_ADDRSTRLEN);

        ui_message(UI_INFO, "MAIN", "Received %d bytes packet from %s:%d", ln, ipaddr, ntohs(si_other.sin_port));
    }

    close(sck);
}
