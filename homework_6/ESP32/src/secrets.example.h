#ifndef SECRETS_H
#define SECRETS_H

// Скопіюй цей файл у secrets.h і заповни своїми значеннями.

// ═══════════════════════════════════════════════════════════
// WI-FI
// ═══════════════════════════════════════════════════════════
#define WIFI_SSID     "Wokwi-GUEST"
#define WIFI_PASSWORD ""

// ═══════════════════════════════════════════════════════════
// AWS IOT CORE
// ═══════════════════════════════════════════════════════════
// Client ID МАЄ дорівнювати імені Thing
#define THINGNAME        "ESP32_Vlasenko_Homework_5" 
///я залишив старий THINGNAME зі старого ДЗ, щоб не переробляти всі сертифікати, переприв'язувати policy й т.д. сподіваюсь це не буде помилкою

#define AWS_IOT_ENDPOINT "a2nfwu02w3rvgo-ats.iot.eu-north-1.amazonaws.com"

// ═══════════════════════════════════════════════════════════
// СЕРТИФІКАТИ
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



)EOF";

// private.pem.key — секретний доказ ("паспорт справді мій")
// НІКОЛИ не комітити цей файл у git!
static const char AWS_CERT_PRIVATE[] = R"EOF(
-----BEGIN RSA PRIVATE KEY-----


)EOF";

#endif