ROOT = $(realpath ../)
PROJECT_ROOT = $(realpath .)
ESP_ROOT = $(ROOT)/esp8266-arduino
ESP_LIB_ROOT = $(ESP_ROOT)/libraries
LIBRARY_ROOT = $(ROOT)/libraries

BOARD = huzzah
SKETCH = $(PROJECT_ROOT)/atmo_bug.ino
BUILD_DIR = $(PROJECT_ROOT)/build

LIBS = $(ESP_LIB_ROOT)/ESP8266WiFi/src \
  $(LIBRARY_ROOT)/PubSubClient/src \
  $(ESP_LIB_ROOT)/Wire \
  $(LIBRARY_ROOT)/Adafruit_HDC1000 \
	$(LIBRARY_ROOT)/secrets

EXCLUDE_DIRS = $(LIBRARY_ROOT)/Adafruit_HDC1000/examples


include $(ROOT)/makeEspArduino/makeEspArduino.mk
