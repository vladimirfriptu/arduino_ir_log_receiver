#include <IRremote.h>

int RECV_PIN = 2;
int OUTPOOT_PIN = 3;
IRrecv irrecv(RECV_PIN);
IRsend irsend;
decode_results results;
unsigned int rawCodes[RAWBUF];

uint16_t power[36] = {8750, 4600, 450, 700, 450, 600, 450, 650, 450, 1800, 450, 650, 450, 1800, 500, 600, 450, 650, 450, 1800, 450, 1800, 500, 1800, 500, 600, 450, 1800, 450, 700, 400, 1800, 450, 1850, 450};

void setup(){
  Serial.begin(9600);
  irrecv.enableIRIn();
  Serial.println("Info    : Infrared Decoder and Extender");
  Serial.println("Version : 1.0");
  Serial.println("---------------------------------------");
}

void loop(){
  // IR Extender
  if (irrecv.decode(&results)) {
//    Serial.println(results.value, HEX);
    int codeLen = results.rawlen - 1;
    Serial.println("code length: ");
    Serial.print(codeLen);
    Serial.println("");
    for (int i = 1; i <= codeLen; i++) {
      if (i % 2) {
        // Mark
        rawCodes[i - 1] = results.rawbuf[i]*USECPERTICK - MARK_EXCESS;
        Serial.print(" ");
      } 
      else {
        // Space
        rawCodes[i - 1] = results.rawbuf[i]*USECPERTICK + MARK_EXCESS;
        Serial.print(" ");
      }
      Serial.print(rawCodes[i - 1], DEC);
    }
    irsend.sendRaw(power, codeLen, 38);
    irrecv.enableIRIn();
    irrecv.resume(); // Receive the next value
//    dump(&results);
  }
}

// Dump results
void dump(decode_results *results) {
  int count = results->rawlen;
  Serial.print(results->value, HEX);
  Serial.print(" (");
  Serial.print(results->bits, DEC);
  Serial.println(" bits)");
  Serial.print("Raw (");
  Serial.print(count, DEC);
  Serial.print("): ");

  for (int i = 0; i < count; i++) {
    if ((i % 2) == 1) {
      Serial.print(results->rawbuf[i]*USECPERTICK, DEC);
    } 
    else {
      Serial.print(-(int)results->rawbuf[i]*USECPERTICK, DEC);
    }
    Serial.print(" ");
  }
  Serial.println("");
}
