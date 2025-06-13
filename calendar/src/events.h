#ifndef EVENTS_H
#define EVENTS_H

#define MAX_EVENTS 100
#define MAX_DESC_LENGTH 100

typedef struct {
    int day;
    int month;
    int year;
    char description[MAX_DESC_LENGTH];
} Event;

extern Event events[MAX_EVENTS];
extern int event_count;

void load_events(const char *filename);
void save_events(const char *filename);
void add_event();
void list_events();

#endif

