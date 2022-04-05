/******************************************************************
*
*	CyberNet for C
*
*	Copyright (C) Satoshi Konno 2005
*
*	File: chttp_date.cpp
*
*	Revision:
*
*	06/08/05
*		- first revision
*
******************************************************************/

#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif

#include <cybergarage/http/chttp.h>

/****************************************
* cg_http_getservername()
****************************************/

static char MONTH_STRING[][4] = {
	"Jan",
	"Feb",
	"Mar",
	"Apr",
	"May",
	"Jun",
	"Jul",
	"Aug",
	"Sep",
	"Oct",
	"Nov",
	"Dec",
};

static char *to_month_string(int value)
{
	if (0 <= value && value < 12)
		return MONTH_STRING[value];
	return "";
}

/****************************************
* cg_http_getservername()
****************************************/

static char WEEK_STRING[][4] = {
	"Sun",
	"Mon",
	"Tue",
	"Wed",
	"Thu",
	"Fri",
	"Sat",
};

	
static char *to_week_string(int value)
{
	if (0 <= value && value < 7)
		return WEEK_STRING[value];
	return "";
}

/****************************************
* cg_http_getservername()
****************************************/

#if !defined(ITRON)

char *cg_http_getdate(CgSysTime sysTime, char *buf, int bufSize)
{
#if defined(HAVE_GMTIME_R)
	struct tm gmTimeBuf;
	struct tm *gmTime = &gmTimeBuf;
	gmtime_r(&sysTime, &gmTimeBuf);
#else
	struct tm *gmTime;
	gmTime = gmtime(&sysTime);
#endif

#if defined(HAVE_SNPRINTF)
	snprintf(buf, bufSize,
#else
	sprintf(buf,
#endif
		"%s, %02d %s %04d %02d:%02d:%02d GMT",
		to_week_string(gmTime->tm_wday),
		gmTime->tm_mday,
		to_month_string(gmTime->tm_mon),
		gmTime->tm_year + 1900,
		gmTime->tm_hour,
		gmTime->tm_min,
		gmTime->tm_sec);

	return buf;
}

#endif

/****************************************
* cg_http_getservername() (ITRON)
****************************************/

#if defined(ITRON)

static const int dayYear[ ] = {
    31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
};

static const int dayLearYear[ ] = {
	31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
};

static BOOL IsLeapYear(int year)
{
    if (!(year % 4) && ((year % 100) || !(year % 400)))
        return TRUE;
	return FALSE;
}

char *cg_http_getdate(CgSysTime sysTime, char *buf, int bufSize)
{
	SYSTIM systim;
	long uxtime;
	int year, month, day, hour, min, sec, week;

	get_tim(&systim);

	uxtime = (systim.ltime / 1000) + ((systim.utime / 1000) << 32);

	sec = uxtime % 60;
	uxtime -= sec;
	uxtime /= 60;

	min = uxtime % 60;
	uxtime -= min;
	uxtime /= 60;

	hour = uxtime % 24;
	uxtime -= hour;
	uxtime /= 24;

	week = uxtime % 7;

	year = 1970;
	day = uxtime;
	while (day > 366) {
		if (IsLeapYear(year))
			day -= 366;
		else
			day -= 365;
		year++;
    }
	day++;

	month = 0;
	while (1) {					
		if (IsLeapYear(year)) {
			if (day <= dayLearYear[month])
				break;
			day -= dayLearYear[month];
			month++;
		} else {
			if (day <= dayYear[month])
				break;
			day -= dayYear[month];
			month++;
		}
	}

#if defined(HAVE_SNPRINTF)
	snprintf(buf, bufSize,
#else
	sprintf(buf,
#endif
		"%s, %02d %s %04d %02d:%02d:%02d GMT",
		to_week_string(week),
		day,
		to_month_string(month),
		year,
		hour,
		min,
		sec);

	return buf;
}

#endif
