// Copyright 2023 kruglovvs kruglov.valentine@gmail.com
// this is a periphery program
// this program sends data to the server with with post_curl

#include "../include/post.h"

bool open_curl() {
  if (curl_global_init(CURL_GLOBAL_ALL)) {
    return 1;
  }
  atexit(curl_global_cleanup);
  return 0;
}
bool check_curl_connection() {
  if (!(curl.curl = curl_easy_init())) {
    return 1;
  }
  for (unsigned short int i = 0; i < COUNT_TRIES_TO_CONNECT; ++i) {
    curl_easy_setopt(curl.curl, CURLOPT_URL, CHECK_CONNECTION_SERVER_ADDRESS);
    curl_easy_setopt(curl.curl, CURLOPT_POSTFIELDS, "txt=hello");
    curl.result = curl_easy_perform(curl.curl);
    curl_easy_cleanup(curl.curl);
    if (curl.result == CURLE_OK) {
      return 0;
    }
  }
  return 1;
}
bool post_data(char *thermometer_data_string) {
  // input format 18:00 20.05.13 +18\0
  // output format time=00000&date=00000000&temp=000\0
  if (!(curl.curl = curl_easy_init())) {
    return 1;
  }
  
  char post_string[POST_STRING_SIZE] = DEFAULT_POST_STRING;
  char *post_string_pointer = post_string;
  strncpy((post_string_pointer += 5), (thermometer_data_string), TIME_SIZE);
  strncpy((post_string_pointer += 5 + TIME_SIZE + 1), (thermometer_data_string += TIME_SIZE + 1), DATE_SIZE);
  strncpy((post_string_pointer += 5 + DATE_SIZE + 1), (thermometer_data_string += DATE_SIZE + 1), TEMP_SIZE);
  
  curl_easy_setopt(curl.curl, CURLOPT_URL, SERVER_ADDRESS);
  curl_easy_setopt(curl.curl, CURLOPT_POSTFIELDS, post_string);
  curl.result = curl_easy_perform(curl.curl);
  curl_easy_cleanup(curl.curl);
  return curl.result != CURLE_OK;
}
int main() {
  if (open_curl()) {
    fputs("Failed to open curl\n", stderr);
    fputs("Failed to open curl\n", stdout);
    exit(1);
  }
  if (check_curl_connection()) {
    fputs("Connection failed\n", stderr);
    fputs("Connection failed\n", stdout);
    exit(2);
  }
  char input[INPIT_STRING_SIZE] = "\0"; 
  while (read(STDIN_FILENO, input, INPIT_STRING_SIZE) == INPIT_STRING_SIZE) {
    if (post_data(input)) {
      fputs("Error post_data\n", stderr);
      fputs("Error post_data\n", stdout);
      exit(3);
    }
  }
  exit(0);
}
