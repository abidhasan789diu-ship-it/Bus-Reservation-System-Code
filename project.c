
#include <stdio.h>
#include <string.h>
#include <time.h>

#define MAX_BUSES 51
#define MAX_USERS 50
#define MAX_SEATS 40

struct Bus {
    int stationNo;
    char busName[50];
    int busNumber;
    char driverName[50];
    char driverPhone[15];
    int totalSeats;
    int bookedSeats;
    int canceled;
};

struct User {
    char username[50];
    char password[50];
};

struct Booking {
    char username[50];
    int busNumber;
    int seats;
    char paymentMethod[20];
    char transactionId[30];
    time_t bookingTime;
};

struct Bus buses[MAX_BUSES];
struct User users[MAX_USERS];
struct Booking bookings[100];

int busCount = 0;
int userCount = 0;
int bookingCount = 0;

int adminLogin() {
    char id[50], pass[50];

    printf("\nAdmin ID: ");
    scanf("%s", id);
    printf("Password: ");
    scanf("%s", pass);

    if (strcmp(id, "abid") == 0 && strcmp(pass, "abid789") == 0) {
        return 1;
    }
    return 0;
}

void addBus() {
    struct Bus b;

    printf("\nStation No: ");
    scanf("%d", &b.stationNo);
    printf("Bus Name: ");
    scanf("%s", b.busName);
    printf("Bus Number: ");
    scanf("%d", &b.busNumber);
    printf("Driver Name: ");
    scanf("%s", b.driverName);
    printf("Driver Phone: ");
    scanf("%s", b.driverPhone);

    b.totalSeats = 40;
    b.bookedSeats = 0;
    b.canceled = 0;

    buses[busCount++] = b;

    printf("Bus Added Successfully!\n");
}

void viewBus() {
    for (int i = 0; i < busCount; i++) {
        printf("\nBus: %s | No: %d | Driver: %s | Booked: %d | Empty: %d\n",
               buses[i].busName,
               buses[i].busNumber,
               buses[i].driverName,
               buses[i].bookedSeats,
               buses[i].totalSeats - buses[i].bookedSeats);
    }
}

void cancelBus() {
    int num;
    printf("\nEnter Bus Number to Cancel: ");
    scanf("%d", &num);

    for (int i = 0; i < busCount; i++) {
        if (buses[i].busNumber == num) {
            buses[i].canceled = 1;
            printf("Bus Canceled!\n");
            return;
        }
    }
    printf("Bus Not Found!\n");
}

void registerUser() {
    printf("\nUsername: ");
    scanf("%s", users[userCount].username);
    printf("Password: ");
    scanf("%s", users[userCount].password);

    userCount++;
    printf("Account Created!\n");
}

int loginUser(char name[]) {
    char pass[50];

    printf("\nUsername: ");
    scanf("%s", name);
    printf("Password: ");
    scanf("%s", pass);

    for (int i = 0; i < userCount; i++) {
        if (strcmp(users[i].username, name) == 0 &&
            strcmp(users[i].password, pass) == 0) {
            return 1;
        }
    }
    return 0;
}

void searchBus() {
    printf("\nAvailable Buses:\n");

    for (int i = 0; i < busCount; i++) {
        if (buses[i].canceled == 0) {
            printf("Bus: %s | No: %d | Price: 500\n",
                   buses[i].busName,
                   buses[i].busNumber);
        }
    }
}

void bookSeat(char username[]) {
    int num, seats;

    printf("\nEnter Bus Number: ");
    scanf("%d", &num);

    for (int i = 0; i < busCount; i++) {
        if (buses[i].busNumber == num && buses[i].canceled == 0) {

            printf("Available Seats: %d\n",
                   buses[i].totalSeats - buses[i].bookedSeats);

            printf("How many seats (max 3): ");
            scanf("%d", &seats);

            if (seats > 3) {
                printf("Max 3 seats allowed!\n");
                return;
            }

            if (buses[i].bookedSeats + seats > buses[i].totalSeats) {
                printf("Not enough seats!\n");
                return;
            }

            struct Booking bk;

            strcpy(bk.username, username);
            bk.busNumber = num;
            bk.seats = seats;

            printf("Payment Method (bkash/nagad/bank): ");
            scanf("%s", bk.paymentMethod);

            printf("Transaction ID: ");
            scanf("%s", bk.transactionId);

            bk.bookingTime = time(NULL);

            bookings[bookingCount++] = bk;

            buses[i].bookedSeats += seats;

            printf("Booking Successful!\n");
            return;
        }
    }
    printf("Bus Not Found!\n");
}

void viewTicket(char username[]) {
    for (int i = 0; i < bookingCount; i++) {
        if (strcmp(bookings[i].username, username) == 0) {
            printf("\n--- TICKET ---\n");
            printf("User: %s\n", bookings[i].username);
            printf("Bus No: %d\n", bookings[i].busNumber);
            printf("Seats: %d\n", bookings[i].seats);
            printf("Payment: %s\n", bookings[i].paymentMethod);
            printf("Transaction ID: %s\n", bookings[i].transactionId);
        }
    }
}

void cancelTicket(char username[]) {
    time_t now = time(NULL);

    for (int i = 0; i < bookingCount; i++) {
        if (strcmp(bookings[i].username, username) == 0) {

            double diff = difftime(now, bookings[i].bookingTime);

            if (diff > 1800) {
                printf("Cannot cancel after 30 minutes!\n");
                return;
            }

            printf("Ticket Cancelled!\n");
            bookings[i].seats = 0;
            return;
        }
    }
}

int main() {
    int choice;

    while (1) {
        printf("\n1. Admin\n2. User\n3. Exit\nChoose: ");
        scanf("%d", &choice);

        if (choice == 1) {
            if (adminLogin()) {
                int ch;

                while (1) {
                    printf("\n--- ADMIN PANEL ---\n");
                    printf("1. Add Bus\n2. View Bus\n3. Cancel Bus\n4. Logout\n");
                    scanf("%d", &ch);

                    if (ch == 1) addBus();
                    else if (ch == 2) viewBus();
                    else if (ch == 3) cancelBus();
                    else break;
                }
            } else {
                printf("Wrong Login!\n");
            }
        }

        else if (choice == 2) {
            int ch;
            char username[50];

            printf("\n1. Register\n2. Login\nChoose: ");
            scanf("%d", &ch);

            if (ch == 1) registerUser();

            if (loginUser(username)) {
                while (1) {
                    printf("\n--- USER PANEL ---\n");
                    printf("1. Search Bus\n2. Book Seat\n3. View Ticket\n4. Cancel Ticket\n5. Logout\n");
                    scanf("%d", &ch);

                    if (ch == 1) searchBus();
                    else if (ch == 2) bookSeat(username);
                    else if (ch == 3) viewTicket(username);
                    else if (ch == 4) cancelTicket(username);
                    else break;
                }
            } else {
                printf("Login Failed!\n");
            }
        }

        else break;
    }

    return 0;
}
