#ifndef __DATE_H__
#define __DATE_H__

typedef struct date* Date;

Date createDate(int day, int month, int year);
int get_day(Date d);
int get_month(Date d); // must return a value between 0 and 11 
int get_year(Date d);
void free_date(Date d);
Date fromString(char data[]);
int compare_dates (Date d1, Date d2);
int pertence(Date begin, Date end, Date x);
#endif
