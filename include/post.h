// Copyright 2023 kruglovvs kruglov.valentine@gmail.com

#ifndef POST_H
#define POST_H

#include <curl/curl.h>
#include <errno.h> /* ERROR Number Definitions           */
#include <fcntl.h> /* File Control Definitions           */
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>  /* UNIX Standard Definitions 	   */

#include "../include/API.h"

#define COUNT_TRIES_TO_CONNECT 2
#define SERVER_ADDRESS "https://web.telegram.org/"
#define CHECK_CONNECTION_SERVER_ADDRESS "https://web.telegram.org/"

typedef struct struct_curl {
  CURL *curl;
  CURLcode result;
} curl_type;

static curl_type curl;

bool open_curl();
bool check_curl_connection();
bool post_data(char *) ;

#endif  // POST_H
