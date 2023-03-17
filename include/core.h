// Copyright 2023 kruglovvs kruglov.valentine@gmail.com

#ifndef PORT_H
#define PORT_H

#include <curl/curl.h>
#include <errno.h> /* ERROR Number Definitions           */
#include <fcntl.h> /* File Control Definitions           */
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/poll.h>
#include <termios.h> /* POSIX Terminal Control Definitions */
#include <unistd.h>  /* UNIX Standard Definitions 	   */

#define PORT_PATH "/dev/ttyUSB0"
#define WRITE_COMMAND_PATH "post.php"
#define BUFFER_SIZE 24
#define COUNT_TRIES_TO_CONNECT 3
#define TIME_SIZE 5
#define DATE_SIZE 8
#define TEMP_SIZE 3
#define DATA_STRING_SIZE 15 + 2 + 5 + 8 + 5 + 1
#define VTIME_VALUE 0
#define VMIN_VALUE 0
#define TIME_WAIT 3000
#define SERVER_ADDRESS "https://web.telegram.org/"

typedef struct struct_port {
  int file_descriptor;
  struct pollfd fds[1];
} port_type;
typedef struct struct_data {
  char time[TIME_SIZE + 1];
  char date[DATE_SIZE + 1];
  char temp[TEMP_SIZE + 1];
} data_type;
typedef struct struct_curl {
  CURL *curl;
  CURLcode result;
} curl_type;

static port_type port;
static data_type data;
static curl_type curl;
static struct termios old_stdin_settings;
static struct termios old_serial_port_settings;

static void close_port() { close(port.file_descriptor); }
static void return_old_serial_port_settings() { tcsetattr(port.file_descriptor, TCSANOW, &old_serial_port_settings); }
static void return_old_stdin_settings() { tcsetattr(STDIN_FILENO, TCSANOW, &old_stdin_settings); }

bool open_curl();
bool open_port();
bool open_stdin();
bool post_data();
bool get_data();
bool check_connection();
bool perform_command(char);

#endif  // PORT_H
