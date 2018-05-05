#include <SparkFunCCS811.h>
#include "SparkFunBME280.h"
#include "Wire.h"
#include <Sparkfun_APDS9301_Library.h>
#include <WiFi.h>
#include <IOXhop_FirebaseESP32.h>

#include <NTPClient.h>
#include <WiFiUdp.h>/


#define FIREBASE_HOST "https://tfg-app-v1.firebaseio.com"
//#define FIREBASE_HOST "gs://tfg-app-v1.appspot.com"
#define FIREBASE_AUTH "nf0th5wkgwCkUk5hfdJwVDrXKbjTsuUDm1qiy9wu"
//#define WIFI_SSID "MOVISTAR_2658"
//#define WIFI_PASSWORD "qu4yUXxq9tr44Ruwu4uu"
#define WIFI_SSID "AndroidAP"
#define WIFI_PASSWORD "11112222"


BME280 bme;
CCS811 ccs(0x5B);
APDS9301 apds;

// Variables and constants used in calculating the windspeed.
volatile unsigned long timeSinceLastTick = 0;
volatile unsigned long lastTick = 0;

unsigned long tiempo1 = 0;
unsigned long tiempo2 = 0;
int tiempoApp = 0;

// Variables and constants used in tracking rainfall
#define S_IN_DAY   86400
#define S_IN_HR     3600
#define NO_RAIN_SAMPLES 2000
volatile long rainTickList[NO_RAIN_SAMPLES];
volatile int rainTickIndex = 0;
volatile int rainTicks = 0;
int rainLastDay = 0;
int rainLastHour = 0;
int rainLastHourStart = 0;
int rainLastDayStart = 0;
long secsClock = 0;

String windDir = "";
float windSpeed = 0.0;

// Pin assignment definitions
#define WIND_SPD_PIN 14
#define RAIN_PIN     25
#define WIND_DIR_PIN 35
#define AIR_RST      4
#define AIR_WAKE     15
#define DONE_LED     5
WiFiUDP ntpUDP;

NTPClient timeClient(ntpUDP, "3.es.pool.ntp.org", 3600, 60000);


/********************** setup ***********************/

void setup()
{


  delay(5);    // The CCS811 wants a brief delay after startup.
  Serial.begin(115200);
  Wire.begin();

  pinMode(DONE_LED, OUTPUT);
  digitalWrite(DONE_LED, LOW);



  // BME280 sensor setup - these are fairly conservative settings, suitable for
  //  most applications. For more information regarding the settings available
  //  for the BME280, see the example sketches in the BME280 library.
  bme.settings.commInterface = I2C_MODE;
  bme.settings.I2CAddress = 0x77;
  bme.settings.runMode = 3;
  bme.settings.tStandby = 0;
  bme.settings.filter = 0;
  bme.settings.tempOverSample = 1;
  bme.settings.pressOverSample = 1;
  bme.settings.humidOverSample = 1;
  bme.begin();


  // CCS811 sensor setup.
  pinMode(AIR_WAKE, OUTPUT);
  digitalWrite(AIR_WAKE, LOW);
  pinMode(AIR_RST, OUTPUT);
  digitalWrite(AIR_RST, LOW);
  delay(10);
  digitalWrite(AIR_RST, HIGH);
  delay(100);
  ccs.begin();

  // APDS9301 sensor setup. Leave the default settings in place.
  apds.begin(0x39);

  /************ wifi-setup *************/
  // connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
  timeClient.begin();
 
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
 
  //  Firebase.setInt("/placa_sensors/online", 1);

  // Visible WiFi connected signal for when serial isn't connected
  digitalWrite(DONE_LED, HIGH);
}

/********************** loop ***********************/

void loop()
{





String time1;
int value_millis;
timeClient.update();
while(1){

// millis control !!!!!!!!
  

    value_millis = Firebase.getInt("/placa_sensors/config/millis");
          
          //String time2 = String(timeClient.getDay()) + "-" + time1;          
      
      
      Firebase.setFloat("/placa_sensors/lux"   ,apds.readLuxLevel());
      Firebase.setFloat("/placa_sensors/presion" , bme.readFloatPressure() * 0.0002953);

      Firebase.setFloat("/placa_sensors/temperatura" , bme.readTempC());
      Firebase.setFloat("/placa_sensors/humidity" , bme.readFloatHumidity());


      
      Firebase.setInt("/placa_sensors/histSemanal/lux/" + time1  ,apds.readLuxLevel());
       time1 = timeClient.getFormattedTime();
    delay(value_millis);
}
           


  
  }




