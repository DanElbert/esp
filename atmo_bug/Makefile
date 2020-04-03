ROOT = $(realpath ../)
PROJECT_ROOT = $(realpath .)
ESP_ROOT = $(ROOT)/esp8266-arduino
ESP_LIB_ROOT = $(ESP_ROOT)/libraries
LIBRARY_ROOT = $(ROOT)/libraries

BOARD = huzzah
SKETCH = $(PROJECT_ROOT)/atmo_bug.ino
BUILD_DIR = $(PROJECT_ROOT)/build

LIBS = $(ESP_LIB_ROOT)/ESP8266WiFi/src \
  $(ESP_LIB_ROOT)/ESP8266HTTPClient/src \
  $(ESP_LIB_ROOT)/ESP8266httpUpdate/src \
  $(ESP_LIB_ROOT)/Wire \
  $(LIBRARY_ROOT)/PubSubClient/src \
  $(LIBRARY_ROOT)/Adafruit_HDC1000 \
  $(LIBRARY_ROOT)/Adafruit_SHT31 \
  $(LIBRARY_ROOT)/esp_bug \
  $(LIBRARY_ROOT)/secrets

EXCLUDE_DIRS = $(LIBRARY_ROOT)/Adafruit_HDC1000/examples \
  $(LIBRARY_ROOT)/Adafruit_SHT31/examples \
  $(ESP_LIB_ROOT)/Wire/examples


include $(ROOT)/makeEspArduino/makeEspArduino.mk
