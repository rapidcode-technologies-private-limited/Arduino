
#include <SPI.h>        // RC522 Module uses SPI protocol
#include <MFRC522.h>  // Library for Mifare RC522 Devices

byte readCard[4];   // Stores scanned ID read from RFID Module
uint8_t successRead;    // Variable integer to keep if we have Successful Read from Reader

// Create MFRC522 instance.
#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);

///////////////////////////////////////// Setup ///////////////////////////////////
void setup() {
  //Protocol Configuration
  Serial.begin(9600);    // Initialize serial communications with PC
  SPI.begin();           // MFRC522 Hardware uses SPI protocol
  mfrc522.PCD_Init();    // Initialize MFRC522 Hardware

  successRead = getID();            // sets successRead to 1 when we get read from reader otherwise 0
  Serial.println("Starting........");
  while (!successRead)                  // Program will not go further while you not get a successful read
  {
    successRead = getID();  
  }
  Serial.println("Started");
}
int card1[]={131,102,75,99};
int card2[]={217,175,168,43};
bool entryGranted=false;
int Open=0;
int Close=0;
///////////////////////////////////////// Main Loop ///////////////////////////////////
void loop () {
  do {
    successRead = getID();  // sets successRead to 1 when we get read from reader otherwise 
  }
  while (!successRead);   //the program will not go furtlher while you are not getting a successful read
  for ( uint8_t j = 0; j < 4; j++ ) {   // Loop 4 times
      //Serial.print(readCard[j]);
      if(card1[j]==readCard[j])
        Open++;
      else if(card2[j]==readCard[j])
        Close++;
        
  } 

  if(Open==4){
    entryGranted=true;
    Serial.println("Gate Open");
  }
  else if(Close==4){
    entryGranted=false;
    Serial.println("Gate Closed");
  } 
  Open=0;
  Close=0;  
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
