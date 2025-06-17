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

void delete_contact() {
    if (contact_count == 0) {
        printf("Contacts list is empty");
        return;
    }

    char name_to_delete[50];
    printf("Enter the name of the contact you want to delete: ");
    fgets(name_to_delete, sizeof(name_to_delete), stdin);
    name_to_delete[strcspn(name_to_delete, "\n")] = '\0';
    
    int found = 0;
    for (int i = 0; i < contact_count; i++) {
        if (strcmp(contacts[i].name, name_to_delete) == 0) {
            for (int j = i; j < contact_count - 1; j++) {
                contacts[j] = contacts [j + 1];
            } 
            contact_count--;
            found = 1;
            printf("Contact \"%s\" deleted.\n", name_to_delete);
            break;
        }
    }
    if (!found) {
        printf("Contact with name \"%s\" not found. \n", name_to_delete);
    }

}


int main() {
	char input[20];
	int choice;

	while (1) {
		printf("\n1. Add contact\n2. Show contacts\n3. Delete contact\n4. Exit\n");

        printf("Choice: ");
        if (!fgets(input, sizeof(input), stdin)) {
            clearerr(stdin); 
            continue;
        }
        
        input[strcspn(input, "\n")] = '\0';
        
        if (strlen(input) == 0) {
            continue;
        }

        if (sscanf(input, "%d", &choice) != 1) {
            printf("Please, input a number of choice (1–4).\n");
            continue;
        } 

		switch (choice) {
			case 1: add_contact(); continue;
			case 2: list_contacts(); continue;
			case 3: delete_contact(); continue;
            case 4: printf("Exit...\n"); return 0;
			default: printf("Wrong choice\n");
		} 
    }
}
