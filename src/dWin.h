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

  void set_Serial(Stream& _Serial);
  void write_vp(uint16_t Addr, char* Data, uint8_t Length);
  char* read_vp(uint16_t Addr);
  void setPage(uint8_t pageNum);
  void clearText(uint16_t Addr);
  void setText(uint16_t Addr, char* Data);
  void set4bLong(uint16_t Addr, long Value);
  void set2bInt(uint16_t Addr, int Value);
  void resetDisplay();
  void setOrientation(uint8_t Angle);
  void setBrightness(uint8_t Max, uint8_t Min, uint8_t Time);
};
