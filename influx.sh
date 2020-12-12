#!/bin/sh -e

stty -F /dev/ttyUSB0 ispeed 115200 cs8 -cstopb -parenb -echo igncr

cat /dev/ttyUSB0 | while read line ; do
	curl --silent -XPOST "http://10.13.37.92:8086/write?consistency=any&db=rot13" --data-binary "grove,dc=trnjanska,host=$(hostname -s) $line"
done
