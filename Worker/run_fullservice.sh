#!/bin/bash
FUNC_NAME=isStartStateValid python3 wrapper.py &
J1=$!
FUNC_NAME=joinGame python3 wrapper.py &
J2=$!
FUNC_NAME=addEvent python3 wrapper.py &
J3=$!
wait -n $J1 $J2 $J3