```sql
    ______  __    __   _______  __       __  ________
   /      ||  |  |  | |   ____||  |     |  |/  _____/
  |  ,----'|  |__|  | |  |__   |  |     |  |  |  __  
  |  |     |   __   | |   __|  |  |     |  |  | |_ | 
  |  `----.|  |  |  | |  |____ |  `----.|  |  |__| | 
   \______||__|  |__| |_______||_______||__|\______| 
     PHONE BOOK MINI PROJECT (C Language)

```


<!-- # 📞 Phone Book Mini Project in C -->

## Description

This console application allows the user to manage a list of contacts: add, view, search, and in the future — save and edit them. The project is written in C and is designed to run in a Linux environment (including WSL/Debian).

---


## Functionality

- Adding new contact (name, phone, email)
- View all saved contacts
- Checking the correctness of user input (for example, protecting against input instead of a number
- The possibility of expansion (saving to a file, deleting, etc.)

---

## Current functionality

| Posibility                      | Implemented |
|---------------------------------|-------------|
| A menu with a choice of actions | ✅          |
| Add contact                     | ✅          |
| View all saved contacts         | ✅          |
| Checking the correctness of userinput (for example, protecting against input instead of a number| ✅          |
| Массив для хранения до 100 контактов | ✅    |

## Project structure

```bash
phonebook/
├── main.c        # main file with program logic
├── Makefile      # makefile
└── README.md     # readme
```

## Compilation and start

Make sure that `gcc` and `make`. If not:
```bash
sudo apt update
sudo apt install build-essential
```
### Building
```bash
make
```

### Start
```bash
make run
```
### Clean
```bash
make clean
```

## Usage example
1. Добавить контакт
2. Показать все
3. Выход
Выбор: 1
Имя: Иван Иванов
Телефон: +380991234567
Email: ivan@example.com
Контакт добавлен.

1. Add contact
2. Show contacts
3. Exit
Choice: 2

List of contacts:
1. John Doe- +380991234567 - john@example.com

### Incorrect input
Choice: abc
Wrong input. Please input number




## Plans
| Posibility                      | Status |
|---------------------------------|-------------|
| Saving contacts in file         | ✅          |
| Delete contact                  | ✅          |
| Edit contact                    | ✅          |
| Find by name                    | ✅          |
| Sort                            | ✅          |













