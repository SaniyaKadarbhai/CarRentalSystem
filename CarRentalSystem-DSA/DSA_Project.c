#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> // For time_t and time()
#ifdef _WIN32
#include <conio.h> // For getch() on Windows
#else
#define getch() getchar() // Define getch() for non-Windows systems
#endif

#define MAX_CARS 1000
#define OWNER_PASSWORD_PATTERN "owner@123"

// Forward declarations for functions used before their definition
void saveCarsToFile(const char* filename);
int boyerMooreSearch(const char* haystack, const char* needle);
int findCarByRegistrationNumber(const char* regNumber);


// Boyer-Moore string matching algorithm
// Function to perform Boyer-Moore search in a given haystack for a specified needle
int boyerMooreSearch(const char* haystack, const char* needle) {
    int needleLength = strlen(needle);
    int haystackLength = strlen(haystack);

    int skipTable[256];
    for (int i = 0; i < 256; i++) {
        skipTable[i] = needleLength;
    }

    for (int i = 0; i < needleLength - 1; i++) {
        skipTable[(unsigned char)needle[i]] = needleLength - 1 - i;
    }

    int i = needleLength - 1;
    while (i < haystackLength) {
        int j = needleLength - 1;
        while (j >= 0 && haystack[i] == needle[j]) {
            i--;
            j--;
        }
        if (j < 0) {
            return i + 1; // Match found, return the starting index
        } else {
            i += skipTable[(unsigned char)haystack[i]];
        }
    }
    return -1; // No match
}

typedef struct {
    char model[50];
    char registrationNumber[20];
    float price;
    int available;
    int booked;
    char bookedUser[50];
    int seats;
    int hasAC; // 1 for Yes, 0 for No
    char category[30];
    int hours; // This field is not used consistently for current booking duration
    char rentalHistoryCustomer[50];
    int rentalHistoryDuration;
    float rentalHistoryTotalCost;
    time_t bookingTime;
} Car;

Car cars[MAX_CARS];
int numCars = 0;

// Function to add a new car to the array of cars
void addCar(const char* model, const char* regNumber, float price, int available, int seats, int hasAC, const char* category) {
    if (numCars < MAX_CARS) {
        strcpy(cars[numCars].model, model);
        strcpy(cars[numCars].registrationNumber, regNumber);
        strcpy(cars[numCars].category, category);
        cars[numCars].price = price;
        cars[numCars].available = available;
        cars[numCars].booked = 0;
        cars[numCars].seats = seats;
        cars[numCars].hasAC = hasAC;
        numCars++;
        printf("Car added successfully!\n");
        saveCarsToFile("car_info2.txt");
    } else {
        printf("Maximum number of cars reached!\n");
    }
    getch();
    system("cls");
}

// Function to display information about available cars
void displayCars() {
    printf("Available Cars:\n");
    printf("%-20s %-20s %-15s %-12s %-12s %-10s %-10s %-10s\n", "Model", "Registration Number", "Price per Hour", "Availability", "Booked", "Seats", "AC", "Category");
    for (int i = 0; i < numCars; i++) {
        printf("%-20s %-20s %-20.2f %-10s %-10s %-10d %-10s %-10s\n",
               cars[i].model, cars[i].registrationNumber, cars[i].price,
               cars[i].available ? "Yes" : "No",
               cars[i].booked ? "Yes" : "No",
               cars[i].seats,
               cars[i].hasAC ? "Yes" : "No",
               cars[i].category);
    }
    getch();
    system("cls");
}

// Function to search for cars based on specified criteria and display the results
void searchCars(const char* categ, int seat, int ac) {
    printf("Search Results for the above:\n");
    printf("%-20s %-20s %-10s %-10s %-10s %-10s %-10s %-20s\n", "Model", "Registration Number", "Price", "Availability", "Booked", "Seats", "AC", "Category");

    int found = 0;
    for (int i = 0; i < numCars; i++) {
        if (boyerMooreSearch(cars[i].category, categ) != -1 &&
            cars[i].seats == seat &&
            cars[i].hasAC == ac) {
            found = 1;
            printf("%-20s %-20s %-10.2f %-10s %-10s %-10d %-10s %-20s\n",
                   cars[i].model, cars[i].registrationNumber, cars[i].price,
                   cars[i].available ? "Yes" : "No",
                   cars[i].booked ? "Yes" : "No",
                   cars[i].seats,
                   cars[i].hasAC ? "Yes" : "No",
                   cars[i].category);
        }
    }
    if (!found) {
        printf("No cars found matching the specified criteria.\n");
    }
    getch();
    system("cls");
}

// Function to sort cars using the specified comparison function
void sortCars(int (compare)(const void*, const void*)) {
    qsort(cars, numCars, sizeof(Car), compare);
    printf("Cars sorted successfully!\n");
    getch();
    system("cls");
}

// Comparison function to compare cars based on price
int comparePrice(const void* a, const void* b) {
    return (int)(((const Car*)a)->price - ((const Car*)b)->price);
}

// Comparison function to compare cars based on model names (lexicographically)
int compareModel(const void* a, const void* b) {
    return strcmp(((const Car*)a)->model, ((const Car*)b)->model);
}

// Comparison function to compare cars based on availability (available cars first)
int compareAvailability(const void* a, const void* b) {
    return ((const Car*)b)->available - ((const Car*)a)->available;
}

// Function to save the details of cars to a file
void saveCarsToFile(const char* filename) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        perror("Error opening file for writing");
        return;
    }
    for (int i = 0; i < numCars; i++) {
        fprintf(file, "%s %s %.2f %d %d %d %s\n", cars[i].model,
                cars[i].registrationNumber, cars[i].price, cars[i].available, cars[i].seats,
                cars[i].hasAC, cars[i].category);
    }
    fclose(file);
}

// Function to load the details of cars from a file
void loadCarsFromFile(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file for reading");
        return;
    }
    numCars = 0; // Reset numCars before loading
    while (fscanf(file, "%s %s %f %d %d %d %s", cars[numCars].model,
                  cars[numCars].registrationNumber, &cars[numCars].price,
                  &cars[numCars].available, &cars[numCars].seats, &cars[numCars].hasAC,
                  cars[numCars].category) == 7) {
        cars[numCars].booked = 0; // Initialize booked status to 0
        // Initialize rental history fields to default/empty values
        strcpy(cars[numCars].rentalHistoryCustomer, "");
        cars[numCars].rentalHistoryDuration = 0;
        cars[numCars].rentalHistoryTotalCost = 0.0f;
        cars[numCars].bookingTime = 0;

        numCars++;
        if (numCars >= MAX_CARS) {
            printf("Maximum number of cars reached!\n");
            break;
        }
    }
    fclose(file);
}

// Function to modify details of a car based on user input
void modifyCarDetails() {
    char regNumber[20];
    int index = -1;

    printf("Enter the registration number of the car you want to modify: ");
    scanf("%s", regNumber);

    index = findCarByRegistrationNumber(regNumber);

    if (index != -1) {
        printf("Enter new model: ");
        scanf("%s", cars[index].model);
        printf("Enter new registration number: ");
        scanf("%s", cars[index].registrationNumber);
        printf("Enter new price: ");
        scanf("%f", &cars[index].price);
        printf("Is the car available? (1 for Yes, 0 for No): ");
        scanf("%d", &cars[index].available);
        printf("Enter the new number of seats: ");
        scanf("%d", &cars[index].seats);
        printf("Does the car have AC? (1 for Yes, 0 for No): ");
        scanf("%d", &cars[index].hasAC);
        printf("Enter the category: ");
        scanf("%s", cars[index].category);
        printf("Car details modified successfully!\n");
    } else {
        printf("Car not found with the given registration number.\n");
    }
    saveCarsToFile("car_info2.txt");
    getch();
    system("cls");
}

// Function to authenticate users
int authenticateUser(char userType) {
    char username[50];
    char password[20];

    printf("Enter your username: ");
    scanf("%s", username);
    printf("Enter your password: ");
    scanf("%s", password);

    if (userType == 'O' || userType == 'o') {
        if (boyerMooreSearch(password, OWNER_PASSWORD_PATTERN) != -1) {
            printf("Owner authenticated successfully!\n");
            return 1;
        }
    } else if (userType == 'C' || userType == 'c') {
        printf("Customer authenticated successfully!\n"); // Simplified for customers
        return 1;
    }
    printf("Invalid username or password. Authentication failed.\n");
    getch();
    system("cls");
    return 0;
}

// Function to display the rental history of a specific car
void displayRentalHistory(int carIndex) {
    printf("\nRental History for Car %s:\n", cars[carIndex].registrationNumber);
    if (cars[carIndex].rentalHistoryDuration > 0) { // Check if there's actual history
        printf("%-20s %-15s %-15s %-20s\n", "Customer", "Rental Duration (Hours)", "Total Cost", "Booking Time");
        printf("%-20s %-15d %-15.2f %-20s\n",
               cars[carIndex].rentalHistoryCustomer,
               cars[carIndex].rentalHistoryDuration,
               cars[carIndex].rentalHistoryTotalCost,
               ctime(&cars[carIndex].bookingTime)); // ctime adds a newline character
    } else {
        printf("No rental history for this car yet.\n");
    }
    getch();
    system("cls");
}

// Function to find the index of a car in the array based on its registration number
int findCarByRegistrationNumber(const char* regNumber) {
    for (int i = 0; i < numCars; i++) {
        if (strcmp(cars[i].registrationNumber, regNumber) == 0) {
            return i;
        }
    }
    return -1;
}

void printChar(char c, int count) {
    for (int i = 0; i < count; i++) {
        printf("%c", c);
    }
}

// Function to display the decorative title
void displayTitleDecoration() {
    const char* title = "CAR RENTAL SYSTEM";
    int title_len = strlen(title);
    int total_width = 80; // Adjust this for wider/narrower display
    int padding_sides = (total_width - title_len - 6) / 2; // 6 for "##   ##"

    printf("\n");

    // Top border
    printf("\t\t\t\t\t");
    printChar('#', total_width);
    printf("\n");

    // Empty line with border
    printf("\t\t\t\t\t##");
    printChar(' ', total_width - 4); // 4 for "##  ##"
    printf("##\n");

    // Title line
    printf("\t\t\t\t\t##");
    printChar(' ', padding_sides);
    printf("%s", title);
    printChar(' ', padding_sides);
    // Adjust for odd/even title length and total width
    if ((total_width - title_len - 6) % 2 != 0) {
        printf(" ");
    }
    printf("##\n");

    // Empty line with border
    printf("\t\t\t\t\t##");
    printChar(' ', total_width - 4);
    printf("##\n");

    // Bottom border
    printf("\t\t\t\t\t");
    printChar('#', total_width);
    printf("\n");

    // Separator lines
    printf("\t\t\t\t\t");
    printChar('-', total_width);
    printf("\n");
    printf("\t\t\t\t\t");
    printChar('-', total_width);
    printf("\n\n");

    printf("Welcome to the Car Rental System...\n\n");
}

int main() {
    loadCarsFromFile("car_info2.txt");

    char model[50], regNumber[20], category[30];
    int available, seats, hasAC;
    float price;
    char userType;

    displayTitleDecoration(); // Call the function to display the title

    do {
        printf("Are you an owner or a customer (C/O): ");
        scanf(" %c", &userType);

        if (authenticateUser(userType)) {
            int choice;

            if (userType == 'O' || userType == 'o') {
                // Owner Menu
                do {
                    printf("\nOwner Menu:\n");
                    printf("1. View available cars\n");
                    printf("2. Add car\n");
                    printf("3. Modify car details\n");
                    printf("4. Display rental history\n");
                    printf("5. Exit\n");
                    printf("Enter your choice: ");
                    scanf("%d", &choice);

                    switch (choice) {
                        case 1:
                            displayCars();
                            break;
                        case 2:
                            printf("Enter car model: ");
                            scanf("%s", model);
                            printf("Enter registration number: ");
                            scanf("%s", regNumber);
                            printf("Enter price for 1 hour: ");
                            scanf("%f", &price);
                            printf("Is the car available? (1 for Yes, 0 for No): ");
                            scanf("%d", &available);
                            printf("Enter the number of seats: ");
                            scanf("%d", &seats);
                            printf("Does the car have AC? (1 for Yes, 0 for No): ");
                            scanf("%d", &hasAC);
                            printf("Enter the category of car: ");
                            scanf("%s", category);
                            addCar(model, regNumber, price, available, seats, hasAC, category);
                            break;
                        case 3:
                            modifyCarDetails();
                            break;
                        case 4: {
                            char historyRegNumber[20];
                            printf("Enter the registration number of the car to view history: ");
                            scanf("%s", historyRegNumber);
                            int carIndex = findCarByRegistrationNumber(historyRegNumber);
                            if (carIndex != -1) {
                                displayRentalHistory(carIndex);
                            } else {
                                printf("Car not found with the given registration number.\n");
                            }
                            break;
                        }
                        case 5:
                            printf("Exiting owner menu...\n");
                            printf("\n\n");
                            break;
                        default:
                            printf("Invalid option!\n");
                    }
                } while (choice != 5);
            } else if (userType == 'C' || userType == 'c') {
                // Customer Menu
                do {
                    printf("\nCustomer Menu:\n");
                    printf("1. Display available cars\n");
                    printf("2. Search for cars\n");
                    printf("3. Sort cars\n");
                    printf("4. Book a car\n"); // Moved book a car to a more logical place
                    printf("5. Exit\n"); // Updated exit option
                    printf("Enter your choice: ");
                    scanf("%d", &choice);

                    switch (choice) {
                        case 1:
                            displayCars();
                            break;
                        case 2: {
                            char categ[50];
                            int seat, ac_choice;
                            printf("Enter the category of car you want to book (Luxury, Economy, SUV): ");
                            scanf("%s", categ);
                            printf("Enter the number of seats: ");
                            scanf("%d", &seat);
                            printf(" 1. AC or 0. NON-AC?: "); // Changed to 0/1 for consistency
                            scanf("%d", &ac_choice);
                            searchCars(categ, seat, ac_choice);
                            break;
                        }
                        case 3: {
                            int sortChoice;
                            printf("Sorting Options:\n");
                            printf("1. Sort by Price\n");
                            printf("2. Sort by Model\n");
                            printf("3. Sort by Availability\n");
                            printf("Enter sorting option: ");
                            scanf("%d", &sortChoice);
                            switch (sortChoice) {
                                case 1:
                                    sortCars(comparePrice);
                                    break;
                                case 2:
                                    sortCars(compareModel);
                                    break;
                                case 3:
                                    sortCars(compareAvailability);
                                    break;
                                default:
                                    printf("Invalid option!\n");
                            }
                            break;
                        }
                        case 4: { // Book a car
                            char bookRegNumber[20];
                            printf("Enter the registration number of the car you want to book: ");
                            scanf("%s", bookRegNumber);
                            int index = findCarByRegistrationNumber(bookRegNumber);

                            if (index != -1 && cars[index].available && !cars[index].booked) {
                                char customerUsername[50];
                                printf("Enter your username: "); // Re-using customer authentication input
                                scanf("%s", customerUsername);

                                int rentHours;
                                printf("Enter the number of hours you want to rent the car: ");
                                scanf("%d", &rentHours);

                                cars[index].booked = 1;
                                cars[index].available = 0; // Mark as unavailable once booked
                                strcpy(cars[index].bookedUser, customerUsername);
                                float totalAmount = cars[index].price * rentHours;

                                strcpy(cars[index].rentalHistoryCustomer, customerUsername);
                                cars[index].rentalHistoryDuration = rentHours;
                                cars[index].rentalHistoryTotalCost = totalAmount;
                                cars[index].bookingTime = time(NULL);

                                printf("Car booked successfully!\n");
                                printf("Total Amount: %.2f\n", totalAmount);
                                saveCarsToFile("car_info2.txt"); // Save updated car status
                            } else {
                                printf("Invalid registration number or the car is not available for booking.\n");
                            }
                            getch();
                            system("cls");
                            break;
                        }
                        case 5: // Exit option
                            printf("Exiting customer menu...\n");
                            printf("\n\n");
                            break;
                        default:
                            printf("Invalid option!\n");
                    }
                } while (choice != 5);
            } else {
                printf("Invalid user type. Exiting...\n");
                break;
            }
        } else {
            printf("Authentication failed. Please try again.\n");
        }
    } while (1); // Loop indefinitely until a valid exit condition (e.g., specific choice) is met within the menus

    saveCarsToFile("car_info2.txt");
    return 0;
}