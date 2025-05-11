# [LVGL](https://github.com/lvgl/lvgl) Example for WEMOS LOLIN32 with an attached Joy-it 3.5' touch display

The Arduino Sketch uses the **lvgl** as well as **TFT_eSPI** Arduino libraries.

This repository also includes the required configuration for the **lvgl** library (`lv_conf.h`) and the **TFT_eSPI** library config (`User_Setup.h`).

The wiring between ESP32 and the display is done as described below (originally described by [elektor](https://www.elektormagazine.com/articles/guiwithtouchforesp32rpico)):

| Function | Display Pin | WEMOS LOLIN32 Pin |
| -------- | ----------- | ----------------- |
| VCC (5V) | 02          | 5V                |
| T_IRQ    | 11          | 34                |
| GND      | 14          | GND               |
| DC       | 18          | 02                |
| MOSI     | 19          | 23                |
| MISO     | 21          | 19                |
| RST      | 22          | EN                |
| SCK      | 23          | 18                |
| CS       | 24          | 05                |
| T_CS     | 26          | 04                |

# Library dependencies

- esp32 = 3.2.0
- lvgl = 9.2.2
- TFT_eSPI = 2.5.43