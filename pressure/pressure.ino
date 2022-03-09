/*
 * Designed to be used in conjuction with a "PM cart". Demonstrates the ability to read force resistor data to determine if
 * the part assigned to the drawer is empty. 
 * 
 * Created 7/1/15 by Kevin Halliwell
 */



//Libraries
#include <SD.h>
#include <SPI.h>

//Define pin values
#define redLedPin 2 //Red LED
#define greenLedPin 3 //Green LED
#define buzzerPin 4 //Buzzer
#define weightValue 100 //Sensor adjustment to eliminate false readings
#define countTo 20 //Time count

//Define variables
int Senval=0; //Sensor Value
int Senpin=A0; //Sensor read pin
int i = 0; //Counter
int chipSelect = 10;
int pow_pin = 8;


//Yellow wire is +5v, Red wire is read pin A0, Black wire is ground
//Led lights use the same ground. 
//Need to label led and buzzer wire


void setup()
{
    Serial.begin(9600);
    pinMode(redLedPin, OUTPUT); //Red Led
    pinMode(greenLedPin, OUTPUT); //Green Led
    pinMode(buzzerPin, OUTPUT); //Buzzer
    pinMode(chipSelect, OUTPUT); //Chip select
    pinMode(pow_pin, OUTPUT);
    digitalWrite(pow_pin, HIGH);
    Serial.print("Initializing SD card..");
    if (!SD.begin(chipSelect)){
      Serial.println("SD card not present or failed to load");
      return;
    }
    else
      Serial.println("SD card initialized");
}

void loop()
{
    //Start string for data log
    String dataString = "";

    //Read sensor data from A0 pin
    Senval=analogRead(Senpin);

    //Delay serial and set the counter
    delay(1000);
    i+=1; //Sets buzzer counter
    
    if (Senval >= weightValue && i <= countTo) //Adjust for weight reading
    {
      digitalWrite(greenLedPin, LOW); //Green LED
      digitalWrite(redLedPin, HIGH); //Red LED
      digitalWrite(buzzerPin, LOW); //Turns buzzer input to OFF
      dataString += "Red LED set to ON, Sensor: " + String(Senval);
      Serial.println(dataString);
    }
    else if (Senval >= weightValue && i >= countTo)
    {
      digitalWrite(buzzerPin, HIGH); //Sounds Buzzer after 2 hours
      digitalWrite(redLedPin, HIGH); //Red LED
      digitalWrite(greenLedPin, LOW); //Green LED
      dataString += "Green LED set to ON, Sensor: " + String(Senval);
      Serial.println(dataString);
    }
    else
    {
      digitalWrite(greenLedPin, HIGH); //Green LED
      digitalWrite(redLedPin, LOW); //Red LED
      digitalWrite(buzzerPin, LOW); //Turns buzzer input to OFF
      dataString += "Green LED set to ON, Sensor: " + String(Senval);
      Serial.println(dataString);
      i = 0; //Resets buzzer counter
    }

    //Create .txt file on the SD card
    File dataFile = SD.open("Data.txt", FILE_WRITE);
    
    //Print if the file is available
    if (dataFile) {
      dataFile.println(dataString);
      dataFile.close();
      Serial.println(dataString);
      Serial.println("File successfully written!");
    }
    else {
      Serial.println("Error opening file");
    }

    //Testing output for SD card and serial monitor.
    Serial.println("R value:"); //Print resistance
    Serial.println(Senval);
    Serial.println("Time: "); //Print time value
    Serial.println(i);

}
