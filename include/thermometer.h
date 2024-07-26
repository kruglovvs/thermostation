// Copyright 2023 kruglovvs kruglov.valentine@gmail.com

#ifndef THERMOMETER_H
#define THERMOMETER_H

#include <errno.h> /* ERROR Number Definitions           */
#include <fcntl.h> /* File Control Definitions           */
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/poll.h>
#include <termios.h> /* POSIX Terminal Control Definitions */
#include <unistd.h>  /* UNIX Standard Definitions 	   */

#include "../include/API.h"

#define PORT_PATH "/dev/ttyUSB0"
#define COUNT_TRIES_TO_CONNECT 3
#define TIME_WAIT 3000

typedef struct struct_port {
  int file_descriptor;
  struct pollfd fds[1];
} port_type;

static port_type port;
static struct termios old_stdin_settings;
static struct termios old_serial_port_settings;

static void close_port() { close(port.file_descriptor); }
static void return_old_serial_port_settings() { tcsetattr(port.file_descriptor, TCSANOW, &old_serial_port_settings); }
static void return_old_stdin_settings() { tcsetattr(STDIN_FILENO, TCSANOW, &old_stdin_settings); }

bool open_port();
bool check_connection();
bool read_data(char *);
bool write_data(char *);
bool perform_command(char);

#endif  // THERMOMETER_H
