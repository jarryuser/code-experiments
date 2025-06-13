#include <stdio.h>
#include <string.h>

#define MAX_CONTACTS 100

struct Contact {
	char name[50];
	char phone[15];
	char email[50];
};

struct Contact contacts[MAX_CONTACTS];
int contact_count = 0;

void add_contact() {
	if (contact_count >= MAX_CONTACTS) {
		printf("Contact list is full.\n");
		return;
	}

	printf("Name: ");
	scanf(" %[^\n]", contacts[contact_count].name);
	printf("Phone: ");
	scanf(" %[^\n]", contacts[contact_count].phone);
	printf("Email: ");
	scanf(" %[^\n]", contacts[contact_count].email);

	contact_count++;
	printf("Contact added.\n");
}

void list_contacts() {
	printf("\nList of contacts:\n");
	for (int i = 0; i < contact_count; i++) {
		printf("%d. %s - %s - %s\n", i+1, contacts[i].name, contacts[i].phone, contacts[i].email);
	}
}

/* I need to tell someone */
int main() {
	char input[20];
	int choice;

	while (1) {
		printf("\n1. Add contact\n2. Show contacts\n3. Exit\nChoice: ");

		if (!fgets(input, sizeof(input), stdin)) {
			printf("Input error\n");
			continue;
		}

		if (sscanf(input, "%d", &choice) !=1) {
			printf("Wrong input. Please input number\n");
			continue;
		}

		switch (choice) {
			case 1: add_contact(); break;
			case 2: list_contacts(); break;
			case 3: return 0;
			default: printf("Wrong choice\n");
		} }
}
