#include <stdlib.h>
#include "date.h"

struct date {
  int day;
  int month;
  int year;
};

Date createDate(int day, int month, int year) {
    Date d = malloc(sizeof(struct date));
    d->day = day;
    d->month = month;
    d->year = year;
    return d;
}

int get_day(Date d) {
    return d->day; 
}

int get_month(Date d) {
    return d->month; //change to enum? (Acho que sim. Desta forma já garantimos a limitação necessária)
}

int get_year(Date d) {
    return d->year;
}

void free_date(Date d) {
    free(d);
}

Date fromString(char data[]) {
  char dia[2] = ""; 
  char mes[2] = "";
  char ano[4] = "";
  int i, j;
  for (i = 0, j = 0; j < 4; i++, j++) {
    ano[i] = data[i];
  }
  int year = atoi(ano);
  for (i = 5, j = 0; j < 2; i++,j++) {
    mes[j] = data[i];
  }
  int month = atoi(mes);

  for (i = 8, j = 0; j < 2; i++,j++) {
    dia[j] = data[i];
  }
  int day = atoi(dia);
  
  return createDate(day, month, year);
}

int compare_dates (Date d1, Date d2){
    if (get_year(d1) < get_year(d2)) return -1;

    else if (get_year(d1) > get_year(d2)) return 1;

    if (get_year(d1) == get_year(d2)) {
         if (get_month(d1) < get_month(d2)) return -1;
         else if (get_month(d1) > get_month(d2)) return 1;
         else if (get_day(d1) < get_month(d2)) return -1;
         else if(get_month(d1) > get_month(d2)) return 1;
    }
    return 0;
}


int pertence(Date begin, Date end, Date x) {
  int a = compare_dates(begin, x); 
  int b = compare_dates(x, end);
  if ((a == -1 || a == 0) && (b == -1 || b == 0)) return 1;
  else return 0;  
}
