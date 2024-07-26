// Copyright 2023 kruglovvs kruglov.valentine@gmail.com

#ifndef API_H
#define API_H

#define END '0'
#define GET_DATA '1'
#define TIME_SIZE 5
#define DATE_SIZE 8
#define TEMP_SIZE 3

//for post
#define DEFAULT_POST_STRING "time=00000&date=00000000&temp=000"
#define INPIT_STRING_SIZE TIME_SIZE + 1 + DATE_SIZE + 1 + TEMP_SIZE + 1
#define POST_STRING_SIZE 5 + TIME_SIZE + 1 + 5 + DATE_SIZE + 1 + 5 + TEMP_SIZE + 1

//for thermometer
#define DEFAULT_WRITE_STRING "00000 00000000 000"
#define THERMOMETER_DATA_STRING_SIZE 20
#define WRITE_STRING_SIZE TIME_SIZE + 1 + DATE_SIZE + 1 + TEMP_SIZE + 1

#endif  // API_H
