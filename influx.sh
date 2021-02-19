#!/bin/sh -e

serial=/dev/serial/by-path/platform-xhci-hcd.3.auto-usb-0:2:1.0-port0

stty -F $serial ispeed 115200 cs8 -cstopb -parenb -echo igncr

cat $serial | while read line ; do
	curl --silent -XPOST "http://10.13.37.92:8086/write?consistency=any&db=rot13" --data-binary "grove,dc=trnjanska,host=$(hostname -s) $line"
	light=$( echo $line | sed -e 's/^.*,light=//' -e 's/,.*$//' )
	mosquitto_pub -h rpi2 -t grove/light -m $light
	echo $line > /dev/shm/grove
done
