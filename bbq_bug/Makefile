ROOT = $(realpath ../)
PROJECT_ROOT = $(realpath .)
ESP_ROOT = $(ROOT)/esp8266-arduino
ESP_LIB_ROOT = $(ESP_ROOT)/libraries
LIBRARY_ROOT = $(ROOT)/libraries

BOARD = huzzah
SKETCH = $(PROJECT_ROOT)/bbq_bug.ino
BUILD_DIR = $(PROJECT_ROOT)/build

LIBS = $(ESP_LIB_ROOT)/ESP8266WiFi/src \
  $(ESP_LIB_ROOT)/ESP8266HTTPClient/src \
	$(ESP_LIB_ROOT)/ESP8266httpUpdate/src \
	$(ESP_LIB_ROOT)/SPI \
	$(ESP_LIB_ROOT)/Wire \
  $(LIBRARY_ROOT)/PubSubClient/src \
	$(LIBRARY_ROOT)/OneWire \
	$(LIBRARY_ROOT)/MAX31850_DallasTemp \
	$(LIBRARY_ROOT)/Adafruit_GFX \
	$(LIBRARY_ROOT)/Adafruit_SSD1306 \
	$(LIBRARY_ROOT)/secrets \
	$(LIBRARY_ROOT)/esp_bug

EXCLUDE_DIRS = $(LIBRARY_ROOT)/OneWire/examples \
	$(LIBRARY_ROOT)/MAX31850_DallasTemp/examples \
	$(LIBRARY_ROOT)/Adafruit_SSD1306/examples \
	$(LIBRARY_ROOT)/Adafruit_GFX/fontconvert


include $(ROOT)/makeEspArduino/makeEspArduino.mk
