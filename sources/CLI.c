// Copyright 2023 kruglovvs kruglov.valentine@gmail.com
// this is a periphery program
// this program realizes getting data from a console

#include "../include/API_timer.h"
        
int main() {
   int c = 0;
   while (c = fgetc(stdin)) {
	   if (c == '0') {
		   fputc(END, stdout);
	   }
  }
  return 0;
}

/*
	программа ещё не реализована, пока только умеет закрывать программы
*/
