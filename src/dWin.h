#include <Arduino.h>

#define FrameHeader 0x5AA5
#define RegAddr_W 0x80
#define RegAddr_R 0x81
#define VarAddr_W 0x82
#define VarAddr_R 0x83

#define PageAddr 0x0084
#define SoundAddr 0x00A0
#define ResetAddr 0x0004
#define OrientAddr 0x0080
#define BrightAddr 0x0082

class dWin {


private:

  struct Var_Struct {
    Stream* T5L_Serial;
  } Var;


public:

  void set_Serial(Stream& _Serial) {
    this->Var.T5L_Serial = &_Serial;
  }


  void write_vp(uint16_t Addr, char* Data, uint8_t Length) {
    uint8_t tmpDat[Length + 6] = { FrameHeader >> 8, FrameHeader, Length + 3, VarAddr_W, Addr >> 8, Addr };
    for (uint8_t i = 0; i < Length; i++) {
      tmpDat[i + 6] = Data[i];
    }
    this->Var.T5L_Serial->write(tmpDat, sizeof(tmpDat));
    this->Var.T5L_Serial->flush();
  }


  char* read_vp(uint16_t Addr) {
    uint8_t tmpDat[7] = { FrameHeader >> 8, FrameHeader, 4, VarAddr_R, Addr >> 8, Addr, 0x01 };
    this->Var.T5L_Serial->write(tmpDat, sizeof(tmpDat));
    this->Var.T5L_Serial->flush();
    delay(10);
    if (this->Var.T5L_Serial->available() > 0) {
      uint8_t Length = this->Var.T5L_Serial->available();
      char Data[Length];
      for (int i = 0; i <= Length; i++) {
        Data[i] = this->Var.T5L_Serial->read();
      }
      if ((Data[0] << 8) | Data[1] == FrameHeader) {
        static char rData[255];
        strcpy(rData, Data);
        return rData;
      }
    }
    return NULL;
  }


  void setPage(uint8_t pageNum) {
    if (pageNum < 0) return;
    uint8_t Data[4] = { 0x5A, 0x01, pageNum >> 8, pageNum };
    write_vp(PageAddr, Data, sizeof(Data));
  }


  void clearText(uint16_t Addr) {
    uint8_t Data[246];
    memset(Data, 0x20, 246);
    write_vp(Addr, Data, strlen(Data));
  }


  void setText(uint16_t Addr, char* Data) {
    char tmp[246 - strlen(Data)];
    memset(tmp, 0x20, 246 - strlen(Data));
    char ret[strlen(Data) + strlen(tmp)];
    strcpy(ret, Data);
    strcat(ret, tmp);
    write_vp(Addr, ret, strlen(ret));
  }


  void set4bLong(uint16_t Addr, long Value) {
    uint8_t Data[4] = { Value >> 24, Value >> 16, Value >> 8, Value };
    write_vp(Addr, Data, sizeof(Data));
  }

  void set2bInt(uint16_t Addr, int Value) {
    uint8_t Data[2] = { Value >> 8, Value };
    write_vp(Addr, Data, sizeof(Data));
  }


  void resetDisplay() {
    uint8_t Data[4] = { 0x55, 0xAA, 0x5A, 0xA5 };
    write_vp(ResetAddr, Data, sizeof(Data));
  }


  void setOrientation(uint8_t Angle) {
    uint8_t a;
    if (Angle == 0)
      a = 0x04;
    if (Angle == 90)
      a = 0x05;
    if (Angle == 180)
      a = 0x06;
    if (Angle == 270)
      a = 0x07;
    uint8_t Data[4] = { 0x5A, 0x00, 0x00, a };
    write_vp(OrientAddr, Data, sizeof(Data));
  }


  void setBrightness(uint8_t Max, uint8_t Min, uint8_t Time) {
    if (Max > 100) {
      Max = 100;
    }
    if (Min > 100) {
      Min = 20;
    }
    if (Time * 100 >= 1000) {
      Time = 1000;
    }
    if (Time == 0) {
      Time = 100;
    }
    uint8_t Data[4] = { Max, Min, Time >> 8, Time };
    write_vp(BrightAddr, Data, sizeof(Data));
  }
};