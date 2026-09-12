#include "sensor.h"
#include "config.h"
#include "wifi_mqtt/wifi_mqtt.h"

DHT dht(DHTT_PIN, DHTT_TYPE);
DHTread dhtpayload;
