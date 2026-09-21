#ifndef SECRETS_H
#define SECRETS_H

// ═══════════════════════════════════════════════════════════
// WI-FI
// ═══════════════════════════════════════════════════════════
#define WIFI_SSID     "Wokwi-GUEST"
#define WIFI_PASSWORD ""

#define THINGNAME        "ESP32_Vlasenko_Homework_5"
#define AWS_IOT_ENDPOINT "a2nfwu02w3rvgo-ats.iot.eu-north-1.amazonaws.com"


// Вставити вміст .pem файлів
// ═══════════════════════════════════════════════════════════

// AmazonRootCA1.pem — перевірка сервера ("це справді AWS?")
static const char AWS_CERT_CA[] = R"EOF(
-----BEGIN CERTIFICATE-----

-----END CERTIFICATE-----
)EOF";

// certificate.pem.crt — паспорт пристрою ("ось хто я")
static const char AWS_CERT_CRT[] = R"EOF(
-----BEGIN CERTIFICATE-----

-----END CERTIFICATE-----



)EOF";

// private.pem.key — секретний доказ ("паспорт справді мій")
// НІКОЛИ не комітити цей файл у git!
static const char AWS_CERT_PRIVATE[] = R"EOF(
-----BEGIN RSA PRIVATE KEY-----

-----END RSA PRIVATE KEY-----


)EOF";

#endif