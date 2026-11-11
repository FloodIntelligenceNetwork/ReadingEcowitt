#include <WiFi.h>
#include <WebServer.h>
#include "config.h"

const char *ssid = WIFI_SSID;
const char *password = WIFI_PASSWORD;

// FireBeetle 2 ESP32-C5: GPIO11 = TX, GPIO12 = RX (UART0 default pins)
// #define MESH_TX_PIN 11      // Uncomment for DFRobot firebeetle ESP32C5
// #define MESH_RX_PIN 12      // Uncomment for DFRobot firebeetle ESP32C5

// #define MESH_TX_PIN 7  /// for ESP32 C3 waveshare
// #define MESH_RX_PIN 6  /// for ESP32 C3 waveshare
#define MESH_TX_PIN 17 /// for ESP32 Dev module
#define MESH_RX_PIN 16 /// for ESP32 Dev module

WebServer server(80);

// Returns the argument as a JSON number, or null if it wasn't sent
String jsonNum(const char *name)
{
    if (!server.hasArg(name) || server.arg(name).length() == 0)
        return "null";
    return String(server.arg(name).toFloat(), 2);
}

void handleWeatherUpload()
{

    // Debug: print every raw argument received
    for (int i = 0; i < server.args(); i++)
    {
        Serial.print(server.argName(i));
        Serial.print(" = ");
        Serial.println(server.arg(i));
    }

    Serial.println("");
    Serial.println("===== WEATHER DATA RECEIVED =====");

    // Compact JSON packet (short keys keep the LoRa payload small)
    String meshPacket =
        String("{\"t\":\"WX\"") +
        ",\"it\":" + jsonNum("indoortempf") +
        ",\"ih\":" + jsonNum("indoorhumidity") +
        ",\"p\":" + jsonNum("baromin") +
        ",\"ot\":" + jsonNum("tempf") +
        ",\"oh\":" + jsonNum("humidity") +
        ",\"ws\":" + jsonNum("windspeedmph") +
        ",\"wd\":" + jsonNum("winddir") +
        ",\"r\":" + jsonNum("rainin") +
        "}";

    Serial.println("Sending to MeshCore:");
    Serial.println(meshPacket); // USB (CDC)

    Serial2.println(meshPacket); // UART to MeshCore (GPIO11/12)

    // Reply to WS2910
    server.send(200, "text/plain", "success");

    Serial.println("===============================");
}

void setup()
{

    Serial.begin(115200); // USB (needs "USB CDC On Boot: Enabled")

    // Serial1 = UART1, remapped to GPIO12 (RX) and GPIO11 (TX)
    Serial2.begin(115200, SERIAL_8N1, MESH_RX_PIN, MESH_TX_PIN);

    WiFi.begin(ssid, password);

    Serial.print("Connecting to WiFi");
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.print("ESP32 IP Address: ");
    Serial.println(WiFi.localIP());
    Serial.print("ESP32 MAC Address: ");
    Serial.println(WiFi.macAddress());

    server.on("/weatherstation/updateweatherstation.php", handleWeatherUpload);
    server.begin();

    Serial.println("Weather server started");
}

void loop()
{
    server.handleClient();
}