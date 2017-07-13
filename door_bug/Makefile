ROOT = $(realpath ../)
PROJECT_ROOT = $(realpath .)
ESP_ROOT = $(ROOT)/esp8266-arduino
ESP_LIB_ROOT = $(ESP_ROOT)/libraries
LIBRARY_ROOT = $(ROOT)/libraries

BOARD = huzzah
SKETCH = $(PROJECT_ROOT)/door_bug.ino
BUILD_DIR = $(PROJECT_ROOT)/build

LIBS = $(ESP_LIB_ROOT)/ESP8266WiFi/src \
  $(ESP_LIB_ROOT)/ESP8266HTTPClient/src \
	$(ESP_LIB_ROOT)/ESP8266httpUpdate/src \
  $(LIBRARY_ROOT)/PubSubClient/src \
	$(LIBRARY_ROOT)/secrets \
	$(LIBRARY_ROOT)/esp_bug

include $(ROOT)/makeEspArduino/makeEspArduino.mk
