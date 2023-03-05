// Copyright 2023 kruglovvs kruglov.valentine@gmail.com

#include "../include/port.h"

bool open_curl() {
  if (curl_global_init(CURL_GLOBAL_ALL)) { 
    return 1;
  }
  atexit(curl_global_cleanup);
  return 0;
}
bool open_port() {
  if ((port.file_descriptor = open(PORT_PATH, O_RDWR | O_NOCTTY | O_NONBLOCK)) < 0) {
    return 1;
  }
  tcgetattr(port.file_descriptor, &old_serial_port_settings); /* Get the current attributes of the Serial port */
  struct termios serial_port_settings = old_serial_port_settings;
  /* Setting the Baud rate */
  cfsetispeed(&serial_port_settings, B9600); /* Set Read  Speed as 9600                       */
  cfsetospeed(&serial_port_settings, B9600); /* Set Write Speed as 9600                       */
  /* 8N1 Mode */
  serial_port_settings.c_cflag &= ~PARENB;                 /* Disables the Parity Enable bit(PARENB),So No Parity   */
  serial_port_settings.c_cflag &= ~CSTOPB;                 /* CSTOPB = 2 Stop bits,here it is cleared so 1 Stop bit */
  serial_port_settings.c_cflag &= ~CSIZE;                  /* Clears the mask for setting the data size             */
  serial_port_settings.c_cflag |= CS8;                     /* Set the data bits = 8                                 */
  serial_port_settings.c_cflag &= ~CRTSCTS;                /* No Hardware flow Control                         */
  serial_port_settings.c_cflag |= CREAD | CLOCAL;          /* Enable receiver,Ignore Modem Control lines       */
  serial_port_settings.c_iflag &= ~(IXON | IXOFF | IXANY); /* Disable XON/XOFF flow control both i/p and o/p */
  serial_port_settings.c_iflag &= ~(ICANON | ECHO | ECHOE | ISIG); /* Non Cannonical mode                            */
  serial_port_settings.c_oflag &= ~OPOST;                          /*No Output Processing*/
  /* Setting Time outs */
  serial_port_settings.c_cc[VMIN] = VMIN_VALUE;
  serial_port_settings.c_cc[VTIME] = VTIME_VALUE; /* Wait indefinetly   */

  port.fds[0].fd = port.file_descriptor;
  port.fds[0].events = POLLIN;
  atexit(close_port);
  atexit(return_old_serial_port_settings);
  return tcsetattr(port.file_descriptor, TCSANOW, &serial_port_settings);
}
bool open_stdin() {
  tcgetattr(STDIN_FILENO, &old_stdin_settings); /* Get the current attributes of the Serial port */
  struct termios stdin_settings = old_stdin_settings;
  stdin_settings.c_iflag &= ~(ICANON | ECHO | ISIG); /* Non Cannonical */
  stdin_settings.c_cc[VMIN] = 1;
  stdin_settings.c_cc[VTIME] = 0;
  atexit(return_old_stdin_settings);
  return tcsetattr(STDIN_FILENO, TCSANOW, &stdin_settings); /* Set the attributes to the termios structure*/
}
bool post_data() {
  if (!(curl.curl = curl_easy_init())) {
    return 1;
  }
  char data_string[DATA_STRING_SIZE] = "";
  snprintf(data_string, DATA_STRING_SIZE, "time=%s&date=%s&temp=%s", data.time, data.date, data.temp);
  curl_easy_setopt(curl.curl, CURLOPT_URL, SERVER_ADDRESS);
  curl_easy_setopt(curl.curl, CURLOPT_POSTFIELDS, data_string);
  curl.result = curl_easy_perform(curl.curl);
  curl_easy_cleanup(curl.curl);
  return curl.result != CURLE_OK;
}
bool get_data() {
  char in_buffer[BUFFER_SIZE];
  if (write(port.file_descriptor, "!GetA\r", 6) != 6) {
    return 1;
  }
  if (poll(port.fds, 1, TIME_WAIT) <= 0) {
    return 2;
  }
  if (!(port.fds[0].revents & POLLIN)) {
    return 3;
  }
  if (read(port.file_descriptor, in_buffer, 20) != 20) {
    return 1;
  }
  strncpy(data.time, in_buffer + 1, TIME_SIZE);
  data.time[TIME_SIZE] = '\0';
  strncpy(data.date, in_buffer + TIME_SIZE + 1, DATE_SIZE);
  data.date[DATE_SIZE] = '\0';
  strncpy(data.temp, in_buffer + TIME_SIZE + DATE_SIZE + 1, TEMP_SIZE);
  data.temp[TEMP_SIZE] = '\0';
  printf("!%s, %s, %s!\n", data.time, data.date, data.temp);
  return 0;
}
bool check_connection() {
  for (unsigned short int i = 0; i < COUNT_TRIES_TO_CONNECT; ++i) {
    if (get_data(port, data)) {
      return 0;
    }
  }
  return 1;
}
bool perform_command(char command) {
  switch (command) {
    case END:
      exit(0);
    case GET_DATA:
      if (get_data()) {
        return 1;
      }
      if (post_data()) {
        return 1;
      }
      return 0;
    default:
      return 0;
  }
}
int main() {
  fputs("LAUNCHING\n", stdout);
  if (open_port()) {
    fputs("Failed to open port\n", stderr);
    fputs("Failed to open port\n", stdout);
    exit(1);
  }
  if (open_curl()) {
    fputs("Failed to open curl\n", stderr);
    fputs("Failed to open curl\n", stdout);
    exit(2);
  }
  if (open_stdin()) {
    fputs("Cant't set stdin params\n", stderr);
    fputs("Cant't set stdin params\n", stdout);
    exit(3);
  }
  if (check_connection()) {
    fputs("Connection failed\n", stderr);
    fputs("Connection failed\n", stdout);
    exit(4);
  }
  fputs("READY\n", stdout);
  char input[1] = "0";
  while (read(STDIN_FILENO, input, 1)) {
    printf("ENTERED: %c\n", *input);
    if (perform_command(*input)) {
      fputs("Error perform_command\n", stderr);
      fputs("Error perform_command\n", stdout);
      exit(5);
    }
  }
  exit(0);
}

/*
        CURL кривой
        переделать так, чтобы от одного устройства было несколько датчиков -
        сделать массив data
*/