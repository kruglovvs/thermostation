// Copyright 2023 kruglovvs kruglov.valentine@gmail.com
// this is the core program
// this program realizes getting data from a USB-thermostation
// and send it to the post.c

#include "../include/thermometer.h"

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
  serial_port_settings.c_cc[VMIN] = 0;
  serial_port_settings.c_cc[VTIME] = 0; /* Wait indefinetly   */

  port.fds[0].fd = port.file_descriptor;
  port.fds[0].events = POLLIN;
  atexit(close_port);
  atexit(return_old_serial_port_settings);
  return tcsetattr(port.file_descriptor, TCSANOW, &serial_port_settings);
}
bool check_port_connection() {
  char thermometer_data_string[THERMOMETER_DATA_STRING_SIZE] = "\0";
  for (unsigned short int i = 0; i < COUNT_TRIES_TO_CONNECT; ++i) {
    if (read_data(thermometer_data_string)) {
      return 0;
    }
  }
  return 1;
}
bool read_data(char *thermometer_data_string) {
  // input format !18:0020.05.13+1823\r - time (18:00), date (20.05.13), temp (+18), set temp (23)
  if (write(port.file_descriptor, "!GetA\r", 6) != 6) {
    return 1;
  }
  if (poll(port.fds, 1, TIME_WAIT) <= 0) {
    return 1;
  }
  if (!(port.fds[0].revents & POLLIN)) {
    return 1;
  }
  if (read(port.file_descriptor, thermometer_data_string, THERMOMETER_DATA_STRING_SIZE) != THERMOMETER_DATA_STRING_SIZE) {
    return 1;
  }
  return 0;
}
bool write_data(char *thermometer_data_string) {
  // input format !18:0020.05.13+1823\r - time (18:00), date (20.05.13), temp (+18), set temp (23) to 
  // output format 18:00 20.05.13 +18\0
  
  char write_string[WRITE_STRING_SIZE] = DEFAULT_WRITE_STRING;
  char *write_string_pointer = write_string;
  strncpy((write_string_pointer), (thermometer_data_string += 1), TIME_SIZE);
  strncpy((write_string_pointer += TIME_SIZE + 1), (thermometer_data_string += TIME_SIZE), DATE_SIZE);
  strncpy((write_string_pointer += DATE_SIZE + 1), (thermometer_data_string += DATE_SIZE), TEMP_SIZE);
  
  return (write(STDOUT_FILENO, write_string, WRITE_STRING_SIZE) != WRITE_STRING_SIZE); 
}
bool perform_command(char command) {
  switch (command) {
    case END:
      exit(0);
    case GET_DATA:
      char thermometer_data_string[THERMOMETER_DATA_STRING_SIZE] = "\0";
      if (read_data(thermometer_data_string)) {
        return 1;
      }
      if (write_data(thermometer_data_string)) {
        return 1;
      }
      return 0;
    default:
      return 0;
  }
}
int main() {
  fputs("LAUNCHING\n", stderr);
  if (open_port()) {
    fputs("Failed to open port\n", stderr);
    exit(1);
  }
  if (check_port_connection()) {
    fputs("Connection failed\n", stderr);
    exit(2);
  }
  fputs("READY\n", stderr);
  char input[1] = "\0";
  while (read(STDIN_FILENO, input, 1) == 1) {
    if (perform_command(*input)) {
      fputs("Error perform_command\n", stderr);
      exit(3);
    }
  }
  exit(0);
}
