
//pins
#define LED_PIN 2
#define BUTTON_PIN 5
#define LDR_PIN 34
#define DHTT_PIN 4
#define DHTT_TYPE DHT22

//intervals
#define DHT_INTERVAL 5000
#define LDR_INTERVAL 5000
#define COLECTION_INTERVAL 15000

//lux level to turn on LED
#define LED_ON_THRESHOLD 200

//debounce
#define DEBOUNCE 100

//wifi
#define WIFI_SSID     "Wokwi-GUEST"
#define WIFI_PASSWORD ""
#define WIFI_TIMEOUT  10000
#define SEND_INTERVAL 30000

//http reciever
#define SERVER_URL "http://httpbun.com/post"

//status flags
#define STATUS_OK       0b00000000
#define STATUS_LDR_ERR  0b00000001
#define STATUS_DHT_ERR  0b00000010
#define STATUS_WIFI_ERR 0b00000100