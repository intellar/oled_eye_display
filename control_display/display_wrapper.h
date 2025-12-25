    #ifndef DISPLAY_WRAPPER_H
#define DISPLAY_WRAPPER_H

#include "config.h"

// ====================================================================================
// == SECTION 1 : Library inclusion and object instantiation                         ==
// ====================================================================================

#if defined(USE_U8G2)
    #include <U8g2lib.h>
    // This constructor uses the board's default hardware I2C pins.
    // - Arduino Nano/Uno: A4 (SDA), A5 (SCL)
    // - ESP32: GPIO 21 (SDA), GPIO 22 (SCL)
    U8G2_SSD1306_128X64_NONAME_F_HW_I2C display(U8G2_R0); // For Arduino Nano 128x64
     //U8G2_SH1107_PIMORONI_128X128_F_HW_I2C display(U8G2_R0); // For ESP32 128x128

    // To use custom pins (software I2C), use a constructor like this instead:
    // U8G2_SSD1306_128X64_NONAME_F_SW_I2C display(U8G2_R0, /* SCL=*/ A5, /* SDA=*/ A4);

    const int G_COLOR_WHITE = 1;
    const int G_COLOR_BLACK = 0;

#elif defined(USE_ADAFRUIT_SSD1306)
    #include <Wire.h>
    #include <Adafruit_GFX.h>
    #include <Adafruit_SSD1306.h>

    // This constructor uses the board's default hardware I2C interface (&Wire).
    // - Arduino Nano/Uno: A4 (SDA), A5 (SCL)
    // - ESP32: GPIO 21 (SDA), GPIO 22 (SCL)
    #define OLED_RESET -1
    Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

    // To use custom pins (software I2C), Adafruit libraries usually require specific constructors
    // or bit-banging libraries, but Hardware I2C is recommended for performance.

    const int G_COLOR_WHITE = SSD1306_WHITE;
    const int G_COLOR_BLACK = SSD1306_BLACK;

#else
    #error "No display library defined in config.h. Please choose one."
#endif

// ====================================================================================
// == SECTION 2 : Generic Wrapper Functions                                          ==
// ====================================================================================



inline void g_init_display() {
#if defined(USE_U8G2)
    display.setI2CAddress(0x78);
    display.begin();
#elif defined(USE_ADAFRUIT_SSD1306)
    // [ESP32] Pour utiliser des pins personnalisés, décommentez et modifiez la ligne suivante :
    // Wire.begin(/*SDA=*/ 21, /*SCL=*/ 22);
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
#endif
}




inline void g_clear_display() {
#if defined(USE_U8G2)
    display.clearBuffer();
#elif defined(USE_ADAFRUIT_SSD1306)
    display.clearDisplay();
#endif
}

inline void g_update_display() {
#if defined(USE_U8G2)
    display.sendBuffer();
#elif defined(USE_ADAFRUIT_SSD1306)
    display.display();
#endif
}

inline void g_draw_filled_round_rect(int x, int y, int w, int h, int r, int color) {
#if defined(USE_U8G2)
    display.setDrawColor(color);
    display.drawRBox(x, y, w, h, r);
#elif defined(USE_ADAFRUIT_SSD1306)
    display.fillRoundRect(x, y, w, h, r, color);
#endif
}

inline void g_draw_filled_triangle(int x0, int y0, int x1, int y1, int x2, int y2, int color) {
#if defined(USE_U8G2)
    display.setDrawColor(color);
    display.drawTriangle(x0, y0, x1, y1, x2, y2);
#elif defined(USE_ADAFRUIT_SSD1306)
    display.fillTriangle(x0, y0, x1, y1, x2, y2, color);
#endif
}

#endif