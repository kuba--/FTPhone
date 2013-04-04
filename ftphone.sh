#!/usr/bin/sh
DATE=`date +"%H_%M_%S"`;
./a.out $@ 2> Log/$USER-$DATE.log
