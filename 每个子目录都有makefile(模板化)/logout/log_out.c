/******************************************************************************
 *      TITLE       :   logout.c
 *
 *      PROGRAMMER  :   fanchenxin
 *
 *      DATE        :   Ver 0.0    2016.06.21
 ******************************************************************************/

#include "log_out.h"

/*
\033[显示方式;前景色;背景色m

显示方式:0（默认值）、1（高亮）、22（非粗体）、4（下划线）、24（非下划线）、5（闪烁）、25（非闪烁）、7（反显）、27（非反显）
前景色:30（黑色）、31（红色）、32（绿色）、 33（黄色）、34（蓝色）、35（洋红）、36（青色）、37（白色）
背景色:40（黑色）、41（红色）、42（绿色）、 43（黄色）、44（蓝色）、45（洋红）、46（青色）、47（白色

*/


void LOGOUT_NOTICE(char* log_format, ...)
{
	struct timespec ts;
	clock_gettime(CLOCK_MONOTONIC, &ts);

	char chLog[1024];
	va_list	w_arg;
	va_start( w_arg, log_format );
	snprintf( chLog, 34, "\033[1;33;42m[%4u:%09u] <N>:  ",  ts.tv_sec, ts.tv_nsec);		
	vsnprintf( chLog+33, 1024 - 34, log_format, w_arg);
	va_end( w_arg );

       int len = strlen(chLog);
       if(chLog[len - 1] == '\n')
            chLog[len - 1] = '\0';

	printf( "%s\033[0m\n", chLog);
}

void LOGOUT_WARNING(char* log_format, ...)
{
	struct timespec ts;
	clock_gettime(CLOCK_MONOTONIC, &ts);

	char chLog[1024];
	va_list	w_arg;
	va_start( w_arg, log_format );
	snprintf( chLog, 34, "\033[1;31;47m[%4u:%09u] <W>:  ",  ts.tv_sec, ts.tv_nsec);		
	vsnprintf( chLog+33, 1024 - 34, log_format, w_arg);
	va_end( w_arg );

       int len = strlen(chLog);
       if(chLog[len - 1] == '\n')
            chLog[len - 1] = '\0';

	printf( "%s\033[0m\n", chLog);
}

void LOGOUT_ERROR(char* log_format, ...)
{
	struct timespec ts;
	clock_gettime(CLOCK_MONOTONIC, &ts);

	char chLog[1024];
	va_list	w_arg;
	va_start( w_arg, log_format );
	snprintf( chLog, 34, "\033[1;31;43m[%4u:%09u] <E>:  ",  ts.tv_sec, ts.tv_nsec);		
	vsnprintf( chLog+33, 1024 - 34, log_format, w_arg);
	va_end( w_arg );

       int len = strlen(chLog);
       if(chLog[len - 1] == '\n')
            chLog[len - 1] = '\0';

	printf( "%s\033[0m\n", chLog);
}


typedef enum
{
    BACKGROUND = 0,
    FORGROUND
}TYPE;

// type = 1: forground 0:background
void getColorValueStr(LOG_COLOR color, TYPE type, char valueStr[3])
{
    if(type == BACKGROUND)
        valueStr[0] = '4';
    else
        valueStr[0] = '3';

    switch(color)
    {
        case BLACK:
            valueStr[1] = '0';
            break;
        case RED:
            valueStr[1] = '1';
            break;
        case GREEN:
            valueStr[1] = '2';
            break;
        case YELLOW:
            valueStr[1] = '3';
            break;
        case BLUE:
            valueStr[1] = '4';
            break;
        case MAGENTA:
            valueStr[1] = '5';
            break;
        case CYAN:
            valueStr[1] = '6';
            break;
        case WHITE:
            valueStr[1] = '7';
            break;
        default:
            valueStr[1] = '0';
            break;
    }
    valueStr[2] = '\0';
    return;
}

void LOGOUT_COLOR(
    LOG_COLOR forground_color,
    LOG_COLOR background_color,
    char* log_format, ...)
{
	struct timespec ts;
	clock_gettime(CLOCK_MONOTONIC, &ts);

	char chLog[1024];
       char forColValue[3] = {'\0'};
       char backColValue[3] = {'\0'};
       getColorValueStr(background_color, BACKGROUND, backColValue);
       getColorValueStr(forground_color, FORGROUND, forColValue);
	va_list	w_arg;
	va_start( w_arg, log_format );
       snprintf(chLog, 30, "\033[1;%s;%sm[%4u:%09u]   ",
                    forColValue, backColValue,
                    ts.tv_sec, ts.tv_nsec);	
	vsnprintf( chLog+29, 1024 - 30, log_format, w_arg);
	va_end( w_arg );

       int len = strlen(chLog);
       if(chLog[len - 1] == '\n')
            chLog[len - 1] = '\0';

	printf( "%s\033[0m\n", chLog);
}



