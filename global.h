extern int waterLvl;
extern int powerLvl;
extern const unsigned char segTable[];

void initSysPins(void);

void startLCD(void);
void msgLCD(char msg[]);
void LCD_setPos(unsigned char row, unsigned char col);
void LCD_UpdatePower(void);
void clearTopLCD(void);
void clearBtmLCD(void);

void initExtint_RB1(void);
void initIOC_RB4(void);

void get_waterLvl(void);
void initADC(void);
void initTimer0_ADC(void);

void initTimer1_LCD(void);
void initTimer2_sevseg(void);
void initTimer3_gateClose(void);
void initTimer4_drainWater(void);
void initTimer5_LED(void);

void sevSegNum(void);
void sevOff(void);

void closeBuzzer(void);
void openBuzzer(void);
void emergencyBuzzer(void);
void resumeBuzzer(void);

void closeGate(void);
void openGate(void);
void emergencyStop(void);
void norm_protocol(void);

void offLED(void);
void emergency_LED(void);
void powerONLED(int time);
void custom_delayLED(int rep);
void openLED(void);
void closeLED(void);