#!/bin/bash
gcc ./sources/core.c core.c -lcurl -o core
gcc ./sources/API_timer.c -o API_timer
./API_timer | ./core
