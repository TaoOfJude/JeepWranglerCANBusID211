#include <Canbus.h>
#include <defaults.h>
#include <global.h>
#include <mcp2515.h>
#include <mcp2515_defs.h>
#include <stdlib.h>
#include <LiquidCrystal_I2C.h>
#include <LiquidCrystal.h>

// set the LCD address to 0x27 for a 16 chars 2 line display A FEW use address 0x3F, 20x4 uses 0x3F address
// Set the pins on the I2C chip used for LCD connections: addr, en,rw,rs,d4,d5,d6,d7,bl,blpol
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address

int iSpeedKPH;
int iSpeedMPH;
String sMessageID;
String sSpeedRawVal;
String sDeciRawVal;
long int lDeciRawVal;

long lSpeedRawVal;
String sHexVals;
String sRPM;


//********************************Setup Loop*********************************//

void setup() {
  lcd.begin(16,2);   // initialize the lcd for 16 chars 2 lines (or 20x4) turn on backlight
  Serial.begin(9600); // For debug use
  Serial.println("CAN Read - Testing receipt of CAN Bus message");  
  delay(1000);
  
  if(Canbus.init(CANSPEED_125))  //Initialise MCP2515 CAN controller at the specified speed
    Serial.println("CAN Init ok");
  else
    Serial.println("Can't init CAN");

    lcd.setCursor(0,0);
    lcd.print("Vehicle Speed...");
  delay(1000);
}

//********************************Main Loop*********************************//

void loop(){

tCAN message;
tCAN messageSave;
  
if (mcp2515_check_message()) 
  {
    if (mcp2515_get_message(&message)) 
      {
        if (message.id == 0x211) // or message.id == 0x215) // 
          {
              
               lDeciRawVal = message.data[3];
               Serial.print(lDeciRawVal);
               sHexVals = message.data[0] + message.data[1];
               sSpeedRawVal = String(message.data[2], DEC) ;
               
               Serial.println("");
               lcd.setCursor(0,0);
               lcd.print("RawVal: " + sSpeedRawVal);
               
               // Show kph
               iSpeedKPH = int(message.data[2]) * 2;
               lcd.setCursor(0, 1);
               lcd.print("Kph: ");
               lcd.setCursor(5, 1);
               //lcd.print("  ");
               lcd.print(iSpeedKPH);
               
               // Show mph
               iSpeedMPH = iSpeedKPH / 1.6;
               lcd.setCursor(9, 1);
               lcd.print("Mph: ");
               lcd.setCursor(14, 1);
               lcd.print(iSpeedMPH);
          }
     }}

}
