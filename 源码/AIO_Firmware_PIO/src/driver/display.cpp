#include "dsiplay.h"
#include "common.h"

#define SCREEN_HOR_RES 240 
#define SCREEN_VER_RES 240 //OLED分辨率

#define LV_HOR_RES_MAX_LEN 80 //LVGL的缓冲区设为1/3屏幕大小

static lv_disp_draw_buf_t disp_buf;
static lv_disp_drv_t disp_drv;
static lv_color_t buf[SCREEN_HOR_RES * LV_HOR_RES_MAX_LEN];//LVGL显示接口初始化有关

//LVGL写入OLED回调函数
void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
    /*
    **这应该是屏幕的刷新函数，设定一个区域然后开始填充
    */
   
    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);
    tft->setAddrWindow(area->x1, area->y1, w, h);
    tft->startWrite();
    tft->pushColors(&color_p->full, w * h, true);
    tft->endWrite();
    //检查写入是否完成
    lv_disp_flush_ready(disp);
}
void Display::init(uint8_t rotation, uint8_t backLight)
{
    /*
    **主要是完成屏幕的初始化和LVGLdisplay的注册
    */

    lv_init();
    
    tft->begin();
    tft->fillScreen(TFT_BLACK);
    tft->writecommand(ST7789_DISPON);
    tft->setRotation(rotation); /* mirror 修改反转，如果加上分光棱镜需要改为4镜像*/

    setBackLight(backLight / 100.0); // 设置亮度

    lv_disp_draw_buf_init(&disp_buf, buf, NULL, SCREEN_HOR_RES * LV_HOR_RES_MAX_LEN);

    /*Initialize the display*/
    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = SCREEN_HOR_RES;
    disp_drv.ver_res = SCREEN_VER_RES;//屏幕大小
    disp_drv.flush_cb = my_disp_flush;//将驱动内存中的数据写入到外设中
    disp_drv.draw_buf = &disp_buf;//前面初始化过的buf
    disp_drv.user_data = tft;//用户数据，my_flush_cb用于调用库函数
    lv_disp_drv_register(&disp_drv);
    /**至此，完成了OLED初始化和LVGL的初始化**/
}
//下面两个暂时不是我们需要关心的
void Display::routine()
{
    //AIO_LVGL_OPERATE_LOCK(lv_timer_handler();)
}

void Display::setBackLight(float duty)
{
    duty = constrain(duty, 0, 1);
    duty = 1 - duty;
    //ledcWrite(LCD_BL_PWM_CHANNEL, (int)(duty * 255));
}

