
//pins
#define LED_PIN 2
#define BUTTON_PIN 5
#define DHTT_PIN 4
#define DHTT_TYPE DHT22

//intervals
#define DHT_INTERVAL 10000
#define PUBLISH_INTERVAL 10000
#define RECONNECT_INTERVAL 5000

//debounce
#define DEBOUNCE 50

//wifi
#define WIFI_SSID     "Wokwi-GUEST"
#define WIFI_PASSWORD ""
#define WIFI_TIMEOUT  10000

//MQTT
#define MQTT_BROKER "broker.hivemq.com"
#define MQTT_PORT 1883
#define MQTT_CLIENT_ID "esp32-vlasenko-a"
#define TOPIC_SENSORS "iot-course/vlasenko/sensors"
#define TOPIC_COMANDS "iot-course/vlasenko/comands"