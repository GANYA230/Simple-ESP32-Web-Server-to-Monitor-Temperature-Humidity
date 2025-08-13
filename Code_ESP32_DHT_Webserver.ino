#include <WiFi.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>

// Ganti dengan WiFi kamu
const char* ssid = "NAMA_WIFI";
const char* password = "PASSWORD_WIFI";

// DHT setup
#define DHTPIN 4       // Pin yang digunakan
#define DHTTYPE DHT11  // Ganti dengan DHT22 jika pakai itu
DHT dht(DHTPIN, DHTTYPE);

// Server di port 80
WiFiServer server(80);

void setup() {
  Serial.begin(115200);
  dht.begin();

  // Koneksi WiFi
  Serial.println("Menghubungkan ke WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  server.begin();
}

void loop() {
  WiFiClient client = server.available();
  if (client) {
    Serial.println("Client Terhubung");
    String request = client.readStringUntil('\r');
    client.flush();

    float suhu = dht.readTemperature();
    float kelembaban = dht.readHumidity();

    // Kirim HTML
    client.println("HTTP/1.1 200 OK");
    client.println("Content-type:text/html");
    client.println();

    client.println("<!DOCTYPE html><html>");
    client.println("<head><meta name='viewport' content='width=device-width, initial-scale=1'>");
    client.println("<title>ESP32 Web Server</title></head>");
    client.println("<body><h2>Data Sensor DHT11</h2>");
    client.println("<p><b>Suhu:</b> " + String(suhu) + " &deg;C</p>");
    client.println("<p><b>Kelembaban:</b> " + String(kelembaban) + " %</p>");
    client.println("</body></html>");

    client.stop();
    Serial.println("Client Terputus");
  }
}
