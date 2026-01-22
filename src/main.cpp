#include <Arduino.h>
#include <EEPROM.h>
#include <esp_task_wdt.h>
#include "TFT_eSPI.h"
#include "fonts/Font1_12.h"
#include "fonts/Font1_18.h"
#include "fonts/Font1_40.h"

#define LCD_BL_PIN 9
#define SOFTWARE_VERSION "v0.0.1"

TFT_eSPI tft = TFT_eSPI(TFT_WIDTH,TFT_HEIGHT); //WIDTH, HEIGHT
TFT_eSprite spr = TFT_eSprite(&tft);

uint64_t SNID(0);
int32_t startTime(0);
unsigned long lastFPSTime = 0;
unsigned int frameCount = 0;
float currentFPS = 0;

void Sys_Init();
void LCD_Init();
void LCD_SetBacklight(uint8_t brightness);
void LCD_FillWindow(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);
void LCD_Show();
void Get_FPS();

void Sys_Init(){
    // The System initialization code goes here
    setCpuFrequencyMhz(240);
    esp_task_wdt_init(10, false); //watch dog 5s time out
    Serial.begin(115200);
    EEPROM.begin(256);
    pinMode(LCD_BL_PIN, OUTPUT); // LCD Backlight Pin
    SNID = ESP.getEfuseMac();
    Serial.print("[" +String(millis()) + " ms] ");
    Serial.printf("System Initalized. \nSN:");
    Serial.println(SNID, HEX);
}

void LCD_Init(){
    analogWrite(LCD_BL_PIN,0);
    spr.init();
    // spr.invertDisplay(1); //根据屏幕实际情况选择是否反色显示，去掉优化约5ms
    tft.setRotation(1); //只能设置为tft。0-3 1观看为横屏
    spr.setColorDepth(8);
    spr.fillScreen(0x0000);
    LCD_SetBacklight(80);
    Serial.print("[" +String(millis()) + " ms] ");
    Serial.println("LCD Initialized.\n");
    startTime = millis();
    // 注：因为屏幕素质不一，SPI频率过高可能会导致显示异常，如花屏、闪烁等，故目前频率是40MHz
    // 在使用内置字体的情况下全屏幕刷新约30-46FPS
    // 频率设置为80MHz,上述情况下约65-67FPS
}

void LCD_SetBacklight(uint8_t brightness){
    brightness = constrain(brightness,1,100);//限制大小
    int light_pwm = 255 - ((100 - brightness) * 1.5);
    analogWrite(LCD_BL_PIN,light_pwm);
}

void LCD_FillWindow(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color){
    spr.fillRect(x, y, w, h, color);
}

void LCD_Show(){
    spr.createSprite(TFT_HEIGHT, TFT_WIDTH);
    spr.fillScreen(0x0000);
    spr.setTextDatum(TL_DATUM); //左上角对齐
    spr.loadFont(Font1_18); // 加载字体
    spr.setCursor(5, 5); // 设置光标位置
    spr.setTextColor(TFT_YELLOW);
    spr.print("SN:" + String(SNID,HEX));
    spr.unloadFont();

    spr.loadFont(Font1_40); // 加载字体
    spr.setTextColor(TFT_WHITE);
    spr.setCursor(5, 30); // 设置光标位置
    spr.print("TFT TEST");
    spr.unloadFont();

    spr.loadFont(Font1_12); // 加载字体
    spr.setCursor(5, 120); // 设置光标位置
    spr.printf("  FPS:%.2f", currentFPS);
    spr.printf("  Time:%d ms", startTime);
    spr.setCursor(180, 7); // 设置光标位置
    spr.print(SOFTWARE_VERSION);
    spr.unloadFont();
    spr.pushSprite(0, 0);
    spr.deleteSprite();
}

void Get_FPS(){
    frameCount++;
    unsigned long currentTime = millis();
    // 每 1000ms (1秒) 计算一次帧率
    if (currentTime - lastFPSTime >= 1000) {
        currentFPS = frameCount * 1000.0 / (currentTime - lastFPSTime);
        lastFPSTime = currentTime;
        frameCount = 0;
    }
}

void setup() {
  // put your setup code here, to run once:
  Sys_Init();
  LCD_Init();
  LCD_FillWindow(0, 0, 240, 135, 0x0000);
}

void loop() {
  // put your main code here, to run repeatedly:
  LCD_Show();
  Get_FPS();
}