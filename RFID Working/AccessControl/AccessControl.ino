
#include <SPI.h>        // RC522 Module uses SPI protocol
#include <MFRC522.h>  // Library for Mifare RC522 Devices

byte readCard[4];   // Stores scanned ID read from RFID Module
uint8_t successRead;    // Variable integer to keep if we have Successful Read from Reader

#define Button 4
#define Lock1 3
#define Lock2 5
#define Lock3 6
#define Lock4 9

#define SpeedOpen 70
#define SpeedClose 70
#define StayTime 300

// Create MFRC522 instance.
#define SS_PIN 10
#define RST_PIN 2
MFRC522 mfrc522(SS_PIN, RST_PIN);

int card1[]={131,102,75,99};
int card2[]={217,175,168,43};
bool entryGranted=false;
int Open=0;
int Open1=0;

///////////////////////////////////////// Setup ///////////////////////////////////
void setup() {
  //Protocol Configuration
  Serial.begin(9600);    // Initialize serial communications with PC
  SPI.begin();           // MFRC522 Hardware uses SPI protocol
  mfrc522.PCD_Init();    // Initialize MFRC522 Hardware

  pinMode(Lock1, OUTPUT);
  pinMode(Lock2, OUTPUT);
  pinMode(Lock3, OUTPUT);
  pinMode(Lock4, OUTPUT);

  pinMode(Button,INPUT_PULLUP);

  HaltPower();

  successRead = getID();            // sets successRead to 1 when we get read from reader otherwise 0
  /*Serial.println("Starting........");
  while (!successRead)                  // Program will not go further while you not get a successful read
  {
    successRead = getID();  
  }*/
  Serial.println("Started");
}

///////////////////////////////////////// Main Loop ///////////////////////////////////
void loop () {
  do {
    successRead = getID();  // sets successRead to 1 when we get read from reader otherwise 
    if(!digitalRead(Button)){
      entryGranted=false;
      Serial.println("Gate Closed");
      GateClosed();
      delay(StayTime);
      HaltPower();
    } 
  }
  while (!successRead);   //the program will not go furtlher while you are not getting a successful read
  for ( uint8_t j = 0; j < 4; j++ ) {   // Loop 4 times
      //Serial.print(readCard[j]);
      if(card1[j]==readCard[j])
        Open++;
      else if(card2[j]==readCard[j])
        Open1++;
        
  } 

  if((Open==4)||(Open1==4)){
    entryGranted=true;
    Serial.println("Gate Open");
    GateOpen();
    delay(StayTime);
    HaltPower();
  }
  
  Open=0;
  Open1=0;  
}

///////////////////////////////////////// Get PICC's UID ///////////////////////////////////

uint8_t getID() {
  // Getting ready for Reading PICCs
  if ( ! mfrc522.PICC_IsNewCardPresent()) { //If a new PICC placed to RFID reader continue
    return 0;
  }
  if ( ! mfrc522.PICC_ReadCardSerial()) {   //Since a PICC placed get Serial and continue
    return 0;
  }
  for ( uint8_t i = 0; i < 4; i++) {  //
    readCard[i] = mfrc522.uid.uidByte[i];
//    Serial.print(readCard[i], HEX);
  }
//  Serial.println("");
  mfrc522.PICC_HaltA(); // Stop reading
  return 1;
}
////////////////////////////////////////////////////////////////////////////////////////////
void HaltPower()
{
  digitalWrite(Lock1,LOW);
  digitalWrite(Lock2,LOW);
  digitalWrite(Lock3,LOW);
  digitalWrite(Lock4,LOW);  
}
////////////////////////////////////////////////////////////////////////////////////////////
void GateClosed(){
  analogWrite(Lock1,SpeedOpen);
  digitalWrite(Lock2,LOW);
  digitalWrite(Lock3,LOW);
  analogWrite(Lock4,SpeedOpen);  
}////////////////////////////////////////////////////////////////////////////////////////////
void GateOpen(){
  digitalWrite(Lock1,LOW);
  analogWrite(Lock2,SpeedClose);
  analogWrite(Lock3,SpeedClose);
  digitalWrite(Lock4,LOW);  
}
////////////////////////////////////////////////////////////////////////////////////////////
