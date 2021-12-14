/**
 * Basic Write Example code for InfluxDBClient library for Arduino
 * Data can be immediately seen in a InfluxDB UI: wifi_status measurement
 * Enter WiFi and InfluxDB parameters below
 *
 * Measures signal level of the actually connected WiFi network
 * This example supports only InfluxDB running from unsecure (http://...)
 * For secure (https://...) or Influx Cloud 2 use SecureWrite example
 **/

#if defined(ESP32)
#include <WiFiMulti.h>
WiFiMulti wifiMulti;
#elif defined(ESP8266)
#include <ESP8266WiFiMulti.h>
ESP8266WiFiMulti wifiMulti;
#endif

#include <InfluxDbClient.h>
#include <InfluxDbCloud.h>

// WiFi AP SSID
#define WIFI_SSID "Freebox-5DA152"
// WiFi password
#define WIFI_PASSWORD "inpercito?-reclines-prudentis-patritam2"
// InfluxDB v2 server url, e.g. https://eu-central-1-1.aws.cloud2.influxdata.com (Use: InfluxDB UI -> Load Data -> Client Libraries)
// ERREUR CONNECTION BASE DE DONNÉE API PAR L'URL
#define INFLUXDB_URL "http://192.168.1.45:8086" //192.168.43.100
// InfluxDB v2 server or cloud API authentication token (Use: InfluxDB UI -> Data -> Tokens -> <select token>)
#define INFLUXDB_TOKEN "kDLELhEHelGYmw9SliQxgM_1xdNUxq-mqCUgZONNKSXeQ9HHBLEJZde7cCTLo4mrr6Zf9h7seEgCGFUMinpIHQ=="
// InfluxDB v2 organization id (Use: InfluxDB UI -> User -> About -> Common Ids )
#define INFLUXDB_ORG "smart sensor"
// InfluxDB v2 bucket name (Use: InfluxDB UI ->  Data -> Buckets)
#define INFLUXDB_BUCKET "room"

// Set timezone string according to https://www.gnu.org/software/libc/manual/html_node/TZ-Variable.html
// Examples:
//  Pacific Time: "PST8PDT"
//  Eastern: "EST5EDT"
//  Japanesse: "JST-9"
//  Central Europe: "CET-1CEST,M3.5.0,M10.5.0/3"
#define TZ_INFO "CET-1CEST,M3.5.0,M10.5.0/3"

// InfluxDB client instance with preconfigured InfluxCloud certificate
InfluxDBClient client(INFLUXDB_URL, INFLUXDB_ORG, INFLUXDB_BUCKET, INFLUXDB_TOKEN, InfluxDbCloud2CACert);

// Data point
Point sensor("DHT22");
Point sensorWifi("wifi_status");
Point sensorMotor("Stepper_Motor");

#include <DHT.h>
#define DHTPIN 5 // The same pin number mentioned above

DHT dht(DHTPIN, DHT22); //Sensor initiation

#include <Stepper.h>
#define STEPS 32

Stepper stepper(STEPS, 14, 12, 13, 15);

int val = 0;

void setup()
{

  Serial.begin(115200);
  stepper.setSpeed(200);

  // Setup wifi
  WiFi.mode(WIFI_STA);
  wifiMulti.addAP(WIFI_SSID, WIFI_PASSWORD);

  Serial.print("Connecting to wifi");
  while (wifiMulti.run() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(100);
  }
  Serial.println();

  // Add tags
  sensor.addTag("Room", "Salon");
  sensorWifi.addTag("Room", "Bedroom");
  sensorMotor.addTag("Room", "Bedroom");
  //sensorMotor.addTag("Room", "Bedroom");
  // Accurate time is necessary for certificate validation and writing in batches
  // For the fastest time sync find NTP servers in your area: https://www.pool.ntp.org/zone/
  // Syncing progress and the time will be printed to Serial.
  timeSync(TZ_INFO, "pool.ntp.org", "time.nis.gov");

  // Check server connection
  if (client.validateConnection())
  {
    Serial.print("Connected to InfluxDB: ");
    Serial.println(client.getServerUrl());
  }
  else
  {
    Serial.print("InfluxDB connection failed: ");
    Serial.println(client.getLastErrorMessage());
  }

  // Start communication with DHT
  dht.begin();
}

void sender(String info, float content)
{
  // Clear fields for reusing the point. Tags will remain untouched
  sensor.clearFields();

  // Store measured value into point
  // Report RSSI of currently connected network
  sensor.addField(info, content);

  // Print what are we exactly writing
  Serial.print("Writing: ");
  Serial.println(sensor.toLineProtocol());

  // If no Wifi signal, try to reconnect it
  if ((WiFi.RSSI() == 0) && (wifiMulti.run() != WL_CONNECTED))
  {
    Serial.println("Wifi connection lost");
  }

  // Write point
  if (!client.writePoint(sensor))
  {
    Serial.print("InfluxDB write failed: ");
    Serial.println(client.getLastErrorMessage());
  }
}

void loop()
{

  sensorWifi.clearFields();
  sensorMotor.clearFields();

  if (Serial.available() > 0)
  {
    val = Serial.parseInt();
    stepper.step(val);
    sensorMotor.addField("Rotation", val);
    Serial.print("Writing: ");
    Serial.println(sensorMotor.toLineProtocol());
    //Serial.println(val); //for debugging
  }

  float h = dht.readHumidity();    //measure humidity
  float t = dht.readTemperature(); //measure temperature

  if (isnan(h) && isnan(t))
  { //Check. If the reading fails, then "Read error" is displayed and the program exits
    Serial.println("Reading error");
  }
  else
  {
    sender("Temperature", t);
    /*
    if(t < 15){
      stepper.step(30);
      sensorMotor.addField("Rotation", 30);
      Serial.print("Writing: ");
      Serial.println(sensorMotor.toLineProtocol());
    }else if(t > 17){
      stepper.step(10);
      sensorMotor.addField("Rotation", 20);
      Serial.println(sensorMotor.toLineProtocol());
    }
    else if(t > 21){
      stepper.step(25);
      sensorMotor.addField("Rotation", 30);
      Serial.println(sensorMotor.toLineProtocol());
    }
    else if(t > 25){
      stepper.step(45);
      sensorMotor.addField("Rotation", 45);
      Serial.println(sensorMotor.toLineProtocol());
    }*/
    delay(1);
    sender("Humidity", h);
  }
  sensorWifi.addField("Wifi", WiFi.RSSI());

  Serial.print("Writing: ");
  Serial.println(sensorWifi.toLineProtocol());

  if (!client.writePoint(sensorWifi))
  {
    Serial.print("InfluxDB write failed: ");
    Serial.println(client.getLastErrorMessage());
  }

  if (!client.writePoint(sensorMotor))
  {
    Serial.print("InfluxDB write failed: ");
    Serial.println(client.getLastErrorMessage());
  }

  //Wait 10s --> TODO Change to 60s for final push
  Serial.println("Wait 60s");
  delay(60000);
}