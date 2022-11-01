#include <SoftwareSerial.h>
#include <dWin.h>

SoftwareSerial LCD_Serial(3, 2);

dWin dwin;

int ii = 134;
long ll = 20234;

void read_Serial() {
  uint8_t Length = LCD_Serial.available();
  char Data[Length];
  for (int i = 0; i <= Length; i++) {
    Data[i] = LCD_Serial.read();
  }

  if ((Data[0] << 8) | Data[1] == FrameHeader) {
    switch ((Data[4] << 8) | Data[5]) {
      case 0x5500: //vp
	  
        Serial.println("5500 vp");
        ll++;
        dwin.setText(0x0A00, String(ll).c_str());

       // Serial.println(dwin.read_vp(0x0B00));  // read a value from a set vp

        //  dwin.setPage(2);  // change page/image

        //dwin.setBrightness(80,30,1);  // set backlight brightness

        dwin.set4bLong(0x0B00, ll);
        break;
      case 0x0002: //vp
	  
        // Serial.println("0002 vp");
        ii++;
        dwin.setText(0x0A00, String(ii).c_str());
        dwin.set2bInt(0x0C00, ii);
        break;
      case 0x1003: //vp
	    // Serial.println("1003 vp");
	  
        dwin.setText(0x0A00, "hello world");
        break;
    }
  }
}


void update_values() {
  //update values on loop
}





void setup() {
  Serial.begin(115200);
  LCD_Serial.begin(115200);

  dwin.set_Serial(LCD_Serial);
}



void loop() {
  if (LCD_Serial.available() > 0) {
    read_Serial();  // read data sent from lcd 
  }
  delay(100);
  update_values();
  delay(50);
}
