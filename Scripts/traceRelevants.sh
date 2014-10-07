#!/bin/bash

source ./environment.sh

# Note: offset is added after multiplication with gain
#       Changing gain must be followed by changing offset.


$TERMINAL addTrace Dev.DutyCycle.value
$TERMINAL addTrace Controller.enabled

$TERMINAL addTrace Controller.alfa
$TERMINAL addTrace Controller.alfa_dot
$TERMINAL addTrace Controller.alfa_int

$TERMINAL addTrace Controller.x
$TERMINAL addTrace Controller.x_dot
$TERMINAL addTrace Controller.x_int


$TERMINAL addTrace Controller.out_alfa_kp
$TERMINAL addTrace Controller.out_alfa_kd
$TERMINAL addTrace Controller.out_alfa_ki

$TERMINAL addTrace Controller.out_x_kp
$TERMINAL addTrace Controller.out_x_kd
$TERMINAL addTrace Controller.out_x_ki

