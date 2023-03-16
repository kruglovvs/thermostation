// Copyright 2023 kruglovvs kruglov.valentine@gmail.com
// this is a periphery program
// this program realizes timer, that sends data to the core

#include "../include/API_timer.h"

int main() {
  for (int i = 0; i < 2; ++i) {
    // while (1) {
    sleep(TIME_SLEEP);
    fputc(GET_DATA, stdout);
  }
  fputc(END, stdout);
  return 0;
}
