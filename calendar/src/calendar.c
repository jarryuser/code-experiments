#include <stdio.h>
#include <time.h>
#include "calendar.h"

const char *days[] = { "Mo", "Tu", "We", "Th", "Fr", "Sa", "Su" };
const char *months[] = {
	"", "January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"
};


void show_current_month() {
	time_t now = time(NULL);
	struct tm*t = localtime(&now);
	show_month(t->tm_mon + 1, t->tm_year + 1900);
}

void show_month(int month, int year) {
	printf("\n    %s %d\n", months[month], year);
	for(int i = 0; i < 7; i++) {
		printf("%s ", days[i]);
	}
	printf("\n");

	struct tm date = {0};
	date.tm_mday = 1;
	date.tm_mon = month - 1;
	date.tm_year = year - 1900;
	mktime(&date); // make time.h calculate day of the week
	
	int start_day = (date.tm_wday + 6) % 7; // 0=Monday -> 6, 1=Tu -> 0, ..., 6=Su -> 5
	for (int i = 0; i < start_day; i++) {
		printf("   ");
	}


	int days_in_month[] = {
		0, 31, 28, 31, 30, 31, 30,
		31, 31, 30, 31, 30, 31
	};


	//? leap year
	if (month == 2 && ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))){
		days_in_month[2] = 29;
	} 

	for (int day = 1; day <= days_in_month[month]; day++) {
		printf("%2d ", day);
		if ((start_day + day) % 7 == 0) {
			printf("\n");
		}
	}
	printf("\n");
}
