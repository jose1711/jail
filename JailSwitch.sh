#!/bin/bash

PROG="Jail"
PATHFILE="$HOME/Misc/scripts/Jail/.JailLock"
PATHPROG="$HOME/Misc/scripts/Jail/Jail"

if [  -e $PATHFILE ]; then
	rm $PATHFILE
	pkill Jail
else 
	echo " " > $PATHFILE
	$PATHPROG 0 0 1279 1023&
fi
