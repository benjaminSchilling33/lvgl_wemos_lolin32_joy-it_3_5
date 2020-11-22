#include <lvgl.h>
#include <TFT_eSPI.h>
#include <lv_examples.h>
#include <Ticker.h>

#define screenWidth 480
#define screenHeight 320
#define LVGL_TICK_PERIOD 20

TFT_eSPI tft = TFT_eSPI(); /* TFT instance */
static lv_disp_buf_t disp_buf;
static lv_color_t buf[LV_HOR_RES_MAX * 10];
Ticker   lvgl_tick;             /* timer for lvgl */

#if LV_USE_LOG != 0
/* Serial debugging */
void my_print(lv_log_level_t level, const char * file, uint32_t line, const char * fn_name, const char * dsc)
{

  Serial.printf("%s@%d->%s\r\n", file, line, dsc);
  Serial.flush();
}
#endif

/* Display flushing */
void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
  uint32_t w = (area->x2 - area->x1 + 1);
  uint32_t h = (area->y2 - area->y1 + 1);

  tft.startWrite();
  tft.setAddrWindow(area->x1, area->y1, w, h);
  tft.pushColors(&color_p->full, w * h, true);
  tft.endWrite();

  lv_disp_flush_ready(disp);
}

/*Read the touchpad*/
bool my_touchpad_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data)
{
  uint16_t touchX, touchY;

  bool touched = tft.getTouch(&touchX, &touchY, 600);

  if (!touched)
  {
    data->state = LV_INDEV_STATE_REL;
    return false;
  }
  else
  {
    data->state = LV_INDEV_STATE_PR;
  }

  if (touchX > screenWidth || touchY > screenHeight)
  {
    Serial.println("Y or y outside of expected parameters..");
    Serial.print("y:");
    Serial.print(touchX);
    Serial.print(" x:");
    Serial.print(touchY);
  }
  else
  {
    /*Set the coordinates*/
    if (3 == tft.getRotation()) {
      //Shift coordinates
      data->point.x = 480 - touchX;
      data->point.y = 320 - touchY;
    } else if (1 == tft.getRotation()) {
      data->point.x = touchX;
      data->point.y = touchY;
    }

    Serial.print("Data x");
    Serial.println(touchX);

    Serial.print("Data y");
    Serial.println(touchY);
  }

  return false; /*Return `false` because we are not buffering and no more data to read*/
}

void lv_tick_handler() {
  lv_tick_inc(LVGL_TICK_PERIOD);
}

void setup()
{
  Serial.begin(115200); /* prepare for possible serial debug */

  lv_init();

#if LV_USE_LOG != 0
  lv_log_register_print_cb(my_print); /* register print function for debugging */
#endif

  tft.begin(); /* TFT init */
  tft.setRotation(3); /* Landscape orientation */

  uint16_t calData[5] = { 305, 3542, 245, 3529, 3  };
  tft.setTouch(calData);

  lv_disp_buf_init(&disp_buf, buf, NULL, LV_HOR_RES_MAX * 10);

  /*Initialize the display*/
  lv_disp_drv_t disp_drv;
  lv_disp_drv_init(&disp_drv);
  disp_drv.hor_res = 480;
  disp_drv.ver_res = 320;
  disp_drv.flush_cb = my_disp_flush;
  disp_drv.buffer = &disp_buf;
  lv_disp_drv_register(&disp_drv);

  /*Initialize the (dummy) input device driver*/
  lv_indev_drv_t indev_drv;
  lv_indev_drv_init(&indev_drv);
  indev_drv.type = LV_INDEV_TYPE_POINTER;
  indev_drv.read_cb = my_touchpad_read;
  lv_indev_drv_register(&indev_drv);


  lvgl_tick.attach_ms(LVGL_TICK_PERIOD, lv_tick_handler);

  /* Try an example from the lv_examples repository
     https://github.com/lvgl/lv_examples*/
  lv_ex_btn_1();
  //printf();

}


void loop()
{

  lv_task_handler(); /* let the GUI do its work */
  //delay(5);
}
