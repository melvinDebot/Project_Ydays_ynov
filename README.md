# Smart Valvle Thermostat

## Install Influx
``` bash
brew install influxdb
```
Start InfluxDB
``` bash
influxd
```
Run on [http://localhost:8086/](http://localhost:8086/)

--------------------------------

## Configuration with influxdb
``` ino
// WiFi AP SSID
#define WIFI_SSID "<YOUR SSID>"
// WiFi password
#define WIFI_PASSWORD "<YOUR PASSWORF>"
// InfluxDB v2 server url, e.g. https://eu-central-1-1.aws.cloud2.influxdata.com (Use: InfluxDB UI -> Load Data -> Client Libraries)
// ERREUR CONNECTION BASE DE DONNÉE API PAR L'URL
#define INFLUXDB_URL "http://<YOUR ADRESS IP>:8086" //192.168.43.100
// InfluxDB v2 server or cloud API authentication token (Use: InfluxDB UI -> Data -> Tokens -> <select token>)
#define INFLUXDB_TOKEN "<YOUR INFLUXDB TOKEN>"
// InfluxDB v2 organization id (Use: InfluxDB UI -> User -> About -> Common Ids )
#define INFLUXDB_ORG "<YOUR ORG>"
// InfluxDB v2 bucket name (Use: InfluxDB UI ->  Data -> Buckets)
#define INFLUXDB_BUCKET "<YOUR BUCKET>"
```

## Configuration Sensor DHT22
``` ino
#define DHTPIN <CHANGE PIN>
```

## Configuration Sensor STEPPER MOTOR
``` ino
Stepper stepper(STEPS, <CHANGE PIN>, <CHANGE PIN>, <CHANGE PIN>, <CHANGE PIN>);
```

## Configuration Sensor
![Screenshot](shema.png)




