#sketch := $(shell basename $(pwd))
sketch := ../GroveSensor

all:
	./bin/arduino-cli compile -b arduino:avr:uno --verbose $(sketch)

install:
	curl -fsSL https://raw.githubusercontent.com/arduino/arduino-cli/master/install.sh | sh
	./bin/arduino-cli core install arduino:avr
	./bin/arduino-cli lib install 'DHT sensor library'
	./bin/arduino-cli lib install U8g2
	./bin/arduino-cli lib install 'Adafruit BMP280 Library'
	./bin/arduino-cli lib install Grove-3-Axis-Digital-Accelerometer-2g-to-16g-LIS3DHTR


upload:
	./bin/arduino-cli upload -p /dev/ttyUSB0 --fqbn arduino:avr:uno --verbose $(sketch)

serial:
	microcom -p /dev/ttyUSB0
