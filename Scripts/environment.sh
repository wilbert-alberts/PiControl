#!/bin/bash

SERVO=/home/pi/PiControl/Servo/ARM-Debug/Servo
TERMINAL=/home/pi/PiControl/Terminal/ARM-Debug/Terminal

alias dumpParameters="$TERMINAL dumpParameters"
alias addTrace="$TERMINAL addTrace"
alias delTrace="$TERMINAL delTrace"
alias dumpTraces="$TERMINAL dumpTraces"
alias freq1="$TERMINAL PeriodicTimer.frequency 1"
alias freq100="$TERMINAL PeriodicTimer.frequency 100"
