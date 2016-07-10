#include <OneWire.h>
#include <DallasTemperature.h>
#include <VirtualWire.h>


// 433mhz transceiver pinout
const int led_pin = 13;
const int transmit_pin = 4;
const int receive_pin = 2;
const int transmit_en_pin = 3;


// data pin for DS18B20
#define ONE_WIRE_BUS 2
 
// Setup a oneWire instance to communicate with any OneWire devices 
// (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
 
void setup(void)
{
  // start serial port
  Serial.begin(9600);
  Serial.println("Dallas Temperature IC Control Library Demo");

  // Start up the library
  sensors.begin();

  // Initialise 433MHz transceiver
  vw_set_tx_pin(transmit_pin);
  vw_set_rx_pin(receive_pin);
  vw_set_ptt_pin(transmit_en_pin);
  vw_set_ptt_inverted(true); // Required for DR3100
  vw_setup(2000);   // Bits per sec
}
 

void loop(void)
{
  sensors.requestTemperatures(); 

  //move temperature data to char array
  char temperature[5];
  dtostrf(sensors.getTempCByIndex(0), 4, 2, temperature);


  char msg[6] = {'t','#','#','#','#','#'};

  msg[1] = temperature[0];
  msg[2] = temperature[1];
  msg[3] = temperature[3];
  msg[4] = temperature[4];

  
  digitalWrite(led_pin, HIGH); // Flash a light to show transmitting
  vw_send((uint8_t *)msg, 6);
  vw_wait_tx(); // Wait until the whole message is gone
  digitalWrite(led_pin, LOW);
  delay(2000);

 
}

