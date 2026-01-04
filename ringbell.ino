// ----------------------------------------------------------------------------------------------------
// --      Copyright - Juan Francisco Catalina - https://github.com/jufracaqui/ringbell-notificator  --
// ----------------------------------------------------------------------------------------------------
// --      EDIT THE FOLLOWING VARIABLES AS NEEDED                                                    --
// ----------------------------------------------------------------------------------------------------

#define DEBUG 0 // [0/1] Serial debugging
#define LOOP_INTERVAL 100  // Milliseconds. Delay for checking if the ringbell is ringing
#define ACTIVATION_THRESHOLD 5000 // Milliseconds. Delay between ringbell activations to avoid spamming notifications
#define INPUT_PIN 34

#define WIFI_SSID "Panic at the Cisco" // Your wifi SSID
#define WIFI_PASSWORD "Nabucodonosor"  // Your wifi password

#define TELEGRAM_BOT_TOKEN "1234:Nabucodonosor"  // Create with "BotFather"
#define TELEGRAM_CHAT_ID "1234"                  // Use @myidbot (IDBot) to find your chat ID

// ----------------------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------

#include <WiFi.h>
#include <WiFiClientSecure.h>
// Import https://github.com/jufracaqui/Universal-Arduino-Telegram-Bot to enable silent notifications if needed
#include <UniversalTelegramBot.h>

WiFiClientSecure client;
UniversalTelegramBot telegramBot(TELEGRAM_BOT_TOKEN, client);

// ----------------------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------

#if DEBUG
#define D_SerialBegin(...) Serial.begin(__VA_ARGS__);
#define D_print(...) Serial.print(__VA_ARGS__)
#define D_write(...) Serial.write(__VA_ARGS__)
#define D_println(...) Serial.println(__VA_ARGS__)
#else
#define D_SerialBegin(...)
#define D_print(...)
#define D_write(...)
#define D_println(...)
#endif

// ----------------------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------

void sendTelegramMessage(String message, String parseMode = "Markdown") { // , bool silent = false
  D_print("Sending bot message: \n");
  D_print(message + "\n");
  D_print("Mode: " + parseMode + "\n");

  if (telegramBot.sendMessage(TELEGRAM_CHAT_ID, message, parseMode, 0)) { // , silent
    D_println("Message sent");
  } else {
    D_println("Message could not be sent");
  }
}

void setup() {
  D_SerialBegin(9600);

  delay(1000);
  D_println("Connecting to WiFi");

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  client.setCACert(TELEGRAM_CERTIFICATE_ROOT);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    D_print(".");
  }

  D_print("\n");
  D_println(WiFi.localIP());
  D_print("\n");

  sendTelegramMessage("🤖Ringbell connected!🤖");
}

long lastStateUpdate = millis();

void loop() {
  int inputPinValue = analogRead(INPUT_PIN);
  D_print(String(inputPinValue) + "\n");

  if (inputPinValue > 0 && (millis() - lastStateUpdate) > ACTIVATION_THRESHOLD) {
    lastStateUpdate = millis();
    sendTelegramMessage("🤖Somebody is ringing the bell🔔🤖");
  }
  
  delay(LOOP_INTERVAL);
}
