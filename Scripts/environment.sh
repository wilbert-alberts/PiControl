#!/bin/bash

SERVO=/home/pi/PiControl/Servo/ARM-Debug/Servo
TERMINAL=/home/pi/PiControl/Terminal/ARM-Debug/Terminal
PITRACER=192.168.1.120

alias terminal="$TERMINAL"
alias dumpParameters="$TERMINAL dumpParameters"
alias addTrace="$TERMINAL addTrace"
alias delTrace="$TERMINAL delTrace"
alias dumpTraces="$TERMINAL dumpTraces"
alias freq1="$TERMINAL PeriodicTimer.frequency 1"
alias freq100="$TERMINAL PeriodicTimer.frequency 100"
alias setPWM="$TERMINAL Dev.dutycycle"
alias setTorque="$TERMINAL Motor.enabled 1 && $TERMINAL Motor.torque"
alias qs="./qs"

