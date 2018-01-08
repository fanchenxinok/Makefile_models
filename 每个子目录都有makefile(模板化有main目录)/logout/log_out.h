#ifndef __COLOR_LOG_OUT__
#define __COLOR_LOG_OUT__

#include <stdarg.h>					
#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <string.h>

/*
\033[显示方式;前景色;背景色m

显示方式:0（默认值）、1（高亮）、22（非粗体）、4（下划线）、24（非下划线）、5（闪烁）、25（非闪烁）、7（反显）、27（非反显）
前景色:30（黑色）、31（红色）、32（绿色）、 33（黄色）、34（蓝色）、35（洋红）、36（青色）、37（白色）
背景色:40（黑色）、41（红色）、42（绿色）、 43（黄色）、44（蓝色）、45（洋红）、46（青色）、47（白色

*/


typedef enum
{
    BLACK = 0,  //黑色
    RED,           //红色
    GREEN,      
    YELLOW,
    BLUE,
    MAGENTA,  //洋红
    CYAN,        //青色
    WHITE
}LOG_COLOR;


/* 背景: 绿色，文字: 黄色 */
void LOGOUT_NOTICE(char* log_format, ...); //通知

/* 背景: 白色，文字: 红色*/
void LOGOUT_WARNING(char* log_format, ...);//警告

/* 背景: 黄色，文字: 红色 */
void LOGOUT_ERROR(char* log_format, ...);//错误

void LOGOUT_COLOR(
    LOG_COLOR forground_color,
    LOG_COLOR background_color,
    char* log_format, ...);

#endif
