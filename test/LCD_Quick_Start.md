# 此文档记录优化TFT_eSPI库启动速度所更改的地方
    前言：
    对于那么多的延迟或许是使用了更保守的策略尽量兼容更多的屏幕
    对于本人追求启动速度，从原来的600ms到现在30ms！
    更激进的策略也有可能会导致品控一般的屏幕出现问题，可能启动失败？或者花屏。
    不过本人2块钱买的屏幕模块都没啥问题，所以应该能用
    注意：只对ST7789的1.14寸屏幕进行了优化，其它驱动的不熟悉，1.54的同驱动也是可以正常的初始化！

    （串口要是没手速都点不开收不到日志了）
    日志：
        优化前：
        « [159 ms] System Initalized. 
        SN:98EA4E33E864
        [715 ms] LCD Initialized.
        优化后：
        « [159 ms] System Initalized. 
        SN:98EA4E33E864
        [190 ms] LCD Initialized.

    后言：
    按道理说SPI速度拉到80MHz能把启动速度提升一大截，可惜设置80MHz后屏幕显示就开始有颜色反转，内容移位的情况。
    实测如果SPI拉到80MHz可以把启动速度最快能到185ms

### 更改 lib\TFT_eSPI\TFT_eSPI.cpp
```
Line 694:
    delay(5);
    to ->
    delayMicroseconds(100); //delay 100us

Line 697:
    delay(20);
    to ->
    delayMicroseconds(200); //delay 200us

Line 704:
    delay(150); // Wait for reset to complete
    to ->
    delay(5); // Wait for reset to complete


更改 lib\TFT_eSPI\TFT_Drivers\ST7789_Init.h

Line 12:
    delay(120);
    to ->
    delay(5);

Line 33:
    delay(10);
    to ->
    delay(1);

Line 121:
    delay(120);
    to->
    delay(5);

Line 125:
    delay(120);
    to ->
    delay(5);

```