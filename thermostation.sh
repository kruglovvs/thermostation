#!/bin/bash
gcc "./sources/core.c" -Wall -g -lcurl -o "./core" 
gcc "./sources/daemon_timer.c" -Wall -g -o "./daemon_timer"
./daemon_timer | ./core