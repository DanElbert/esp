ROOT = $(realpath ../)
PROJECT_ROOT = $(realpath .)
ESP_ROOT = $(ROOT)/esp8266-arduino
ESP_LIB_ROOT = $(ESP_ROOT)/libraries
LIBRARY_ROOT = $(ROOT)/libraries

SKETCH = $(PROJECT_ROOT)/atmo_bug.ino

LIBS = $(ESP_LIB_ROOT)/ESP8266WiFi/src \
  $(LIBRARY_ROOT)/PubSubClient/src \
  $(ESP_LIB_ROOT)/Wire \
  $(LIBRARY_ROOT)/Adafruit_HDC1000

EXCLUDE_DIRS = $(LIBRARY_ROOT)/Adafruit_HDC1000/examples


include $(ROOT)/makeEspArduino/makeEspArduino.mk


# ARDUINO_LIBS = ESP8266WiFi PubSubClient Wire Adafruit_HDC1000

# ARDMK_VENDOR = esp8266com
# ARCHITECTURE = esp8266
# BOARD_TAG = huzzah

# ARDUINO_SKETCHBOOK := $(realpath ../)
# ARDMK_DIR := $(realpath ../Arduino-Makefile)
# include $(ARDMK_DIR)/Arduino.mk
