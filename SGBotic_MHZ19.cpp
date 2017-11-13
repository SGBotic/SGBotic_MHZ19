/*************************************************** 
  This is a library for our MH-Z19 Infra-red CO2 sensor

  These sensor uses TTL Serial to communicate, 
  2 pins are required to interface to Arduino board.

 ****************************************************/

#include "SGBotic_MHZ19.h"
#ifdef __AVR__
#include <util/delay.h>
#include <SoftwareSerial.h>
#endif


#ifdef __AVR__
SGBotic_MHZ19::SGBotic_MHZ19(SoftwareSerial *ss) {

    hwSerial = NULL;
    swSerial = ss;
    mySerial = swSerial;
}
#endif

SGBotic_MHZ19::SGBotic_MHZ19(HardwareSerial *ss) {

    #ifdef __AVR__
    swSerial = NULL;
    #endif
    hwSerial = ss;
    mySerial = hwSerial;
}

void SGBotic_MHZ19::begin(uint16_t baudrate) {
    delay(1000);  // one second delay to let the sensor 'boot up'

    if (hwSerial) hwSerial->begin(baudrate);
    #ifdef __AVR__
    if (swSerial) swSerial->begin(baudrate);
    #endif
}



uint16_t SGBotic_MHZ19::readPPM(void) {

    writePacket(cmdReadPPM);
    uint8_t recvPacket[RSP_CNT];
    
    uint8_t len = getReply(recvPacket);
    
    uint16_t co2 = recvPacket[2] * 256 + recvPacket[3];
    #ifdef MHZ19_DEBUG
       Serial.print("Packet Length: ");
       Serial.print(len);
    Serial.print("   PPM high byte: ");
       Serial.print(recvPacket[2]);
    Serial.print("   PPM low byte: ");
       Serial.print(recvPacket[3]);
    
    Serial.print("   CO2: ");
       Serial.print(co2);
    
    #endif
      
    return co2;
}

void SGBotic_MHZ19::setAutoCalibration(boolean autocalib){
    writePacket(autocalib ? cmdAutocalib_on : cmdAutocalib_off);
}

void SGBotic_MHZ19::calibrateZero() {
    writePacket(cmdZerocalib);
}

void SGBotic_MHZ19::calibrateSpan(uint16_t ppm) {

    if( ppm < 1000 )	return;

    uint8_t packet[RQ_CNT];
    for(int i=0; i<RQ_CNT; i++) {
        packet[i] = cmdSpancalib[i];
    }
    packet[3] = (uint8_t)(ppm/256);
    packet[4] = (uint8_t)(ppm%256);
    writePacket(packet);

}


void SGBotic_MHZ19::writePacket(uint8_t packet[]) {
  
    #if ARDUINO >= 100
       mySerial->write(packet, RQ_CNT);
       mySerial->write(checksum(packet));
    #else
       mySerial->print(packet, RQ_CNT);
       mySerial->print(checksum(packet));
    #endif
}


uint8_t SGBotic_MHZ19::getReply(uint8_t *packet, uint16_t timeout) {

    uint8_t reply[RSP_CNT], index;
    uint16_t timer=0;

    index = 0;
    #ifdef MHZ19_DEBUG
       Serial.print("<--- ");
    #endif
    while (true) {
        while (!mySerial->available()) {
            delay(1);
            timer++;
            if (timer >= timeout) return MHZ19_TIMEOUT;
        }
        // something to read!
        reply[index] = mySerial->read();
        #ifdef MHZ19_DEBUG
           Serial.print(" 0x"); Serial.print(reply[index], HEX);
        #endif
        if ((index == 0) && (reply[0] != MHZ19_STARTCODE))
            continue;
        index++;

        // check packet!
        if (index >= RSP_CNT) {
            if (reply[0] != MHZ19_STARTCODE){
            
            #ifdef MHZ19_DEBUG
               Serial.println("Bad Packet");
            #endif 
            
                 return MHZ19_BADPACKET;
            }
               
            if(reply[8] != checksum(reply)){
                
            #ifdef MHZ19_DEBUG
               Serial.println("Checksum error");
            #endif  
           
                return MHZ19_CHECKSUMERROR;
            }
            #ifdef MHZ19_DEBUG
               Serial.println("     Read packet complete");
            #endif
            return index;
        }
    }
}

uint8_t SGBotic_MHZ19::checksum( uint8_t packet[] ) {
    uint8_t sum = 0x00;
    for ( int i = 1; i < SGBotic_MHZ19::RQ_CNT; i++) {
        sum += packet[i];
    }
    sum = 0xff - sum + 0x01;
    return sum;
}


