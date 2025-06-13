#include <stdio.h>
#include "calendar.h"
#include "events.h"

#define EVENTS_FILE "data/events.csv"

int main() {
	load_events(EVENTS_FILE);


	int choice;
	char input[16];

	do {
		printf("\n=== Calendar ===\n");
		printf("1. Show current month\n");
		printf("2. Show another month\n");
		printf("3. Add event\n");
		printf("4. Show all events\n");
		printf("0. Exit\n");

		printf("Choose action: ");
		fgets(input, sizeof(input), stdin);

		if (sscanf(input, "%d", &choice) != 1) {
			printf("Wrong input. Enter a number.\n");
			continue;
		}

		switch(choice) {
			case 1:
				show_current_month();
				continue;
			case 2:{
				int month, year;
				printf("Enter month (1-12): ");
				scanf("%d", &month);
				printf("Enter year: ");
				scanf("%d", &year);
				show_month(month, year);
				break;
			}
			case 3:
				add_event();
				continue;
			case 4:
				list_events();
				continue;
			case 0:
				save_events(EVENTS_FILE);
				printf("Exit...\n");
				continue;
			default:
				printf("Wrong choice. Repeat.\n");
				continue;
		}


		int c;
		while ((c = getchar()) != '\n' && c != EOF) {}

	}while(choice != 0);
	return 0;
}
