# Smart Valvle Thermostat

## Sensor configuration with influxdb
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