/*************************************************** 
  This is a library for our MH-Z19 Infra-red CO2 sensor

  These sensor uses TTL Serial to communicate, 
  2 pins are required to interface to Arduino board.

 ****************************************************/

#include "Arduino.h"
#ifdef __AVR__
#include <SoftwareSerial.h>
#endif

//#define MHZ19_DEBUG 

#define MHZ19_OK 0x00
#define MHZ19_TIMEOUT 0xFF

#define MHZ19_STARTCODE 0xFF
#define MHZ19_BADPACKET 0xFE
#define MHZ19_CHECKSUMERROR 0xFD

#define DEFAULTTIMEOUT 5000  // milliseconds


class SGBotic_MHZ19 {
    public:
    #ifdef __AVR__
    SGBotic_MHZ19(SoftwareSerial *);
    #endif
    SGBotic_MHZ19(HardwareSerial *);

    void begin(uint16_t baud);

    uint16_t readPPM(void);
    void setAutoCalibration(boolean autocalib);
    void calibrateZero();
    void calibrateSpan(uint16_t ppm);

    private:     
    void writePacket(uint8_t packet[]);
    uint8_t getReply(uint8_t packet[], uint16_t timeout=DEFAULTTIMEOUT);
    uint8_t checksum( uint8_t packet[] );

    static const int RQ_CNT = 8;
    static const int RSP_CNT = 9;

    // serial command
    uint8_t cmdReadPPM[RQ_CNT]			= {0xff, 0x01, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00};
    uint8_t cmdZerocalib[RQ_CNT]	 	= {0xff, 0x01, 0x87, 0x00, 0x00, 0x00, 0x00, 0x00};
    uint8_t cmdSpancalib[RQ_CNT]	 	= {0xff, 0x01, 0x88, 0x00, 0x00, 0x00, 0x00, 0x00};
    uint8_t cmdAutocalib_on[RQ_CNT] 	= {0xff, 0x01, 0x79, 0xA0, 0x00, 0x00, 0x00, 0x00};
    uint8_t cmdAutocalib_off[RQ_CNT]	= {0xff, 0x01, 0x79, 0x00, 0x00, 0x00, 0x00, 0x00};

    Stream *mySerial;
    #ifdef __AVR__
    SoftwareSerial *swSerial;
    #endif
    HardwareSerial *hwSerial;
};
