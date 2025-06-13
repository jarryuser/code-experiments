#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "events.h"

Event events[MAX_EVENTS];
int event_count = 0;

void load_events(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) return;

    while (fscanf(file, "%d,%d,%d,%99[^\n]\n",
                  &events[event_count].day,
                  &events[event_count].month,
                  &events[event_count].year,
                  events[event_count].description) == 4) {
        event_count++;
        if (event_count >= MAX_EVENTS) break;
    }

    fclose(file);
}

void save_events(const char *filename) {
    FILE *file = fopen(filename, "w");
    if (!file) return;

    for (int i = 0; i < event_count; i++) {
        fprintf(file, "%d,%d,%d,%s\n",
                events[i].day,
                events[i].month,
                events[i].year,
                events[i].description);
    }

    fclose(file);
}

void add_event() {
    if (event_count >= MAX_EVENTS) {
        printf("Too many events!\n");
        return;
    }

    Event e;
    printf("Enter day: ");
    scanf("%d", &e.day);
    printf("Enter month: ");
    scanf("%d", &e.month);
    printf("Enter year: ");
    scanf("%d", &e.year);
    printf("Enter description (max 100 symbols): ");
    getchar(); // clean \n after scanf
    fgets(e.description, MAX_DESC_LENGTH, stdin);
    e.description[strcspn(e.description, "\n")] = '\0'; // delete \n

    events[event_count++] = e;
    printf("Event added!\n");
}

void list_events() {
    if (event_count == 0) {
        printf("No events.\n");
        return;
    }

    printf("\nEvents list:\n");
    for (int i = 0; i < event_count; i++) {
        printf("%02d.%02d.%04d — %s\n",
               events[i].day,
               events[i].month,
               events[i].year,
               events[i].description);
    }
}

