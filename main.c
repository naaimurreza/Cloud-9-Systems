#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#define NUM_BUCKETS 1000

struct product {
    char name[50];
    double price;
    char features[4][100];
};

typedef struct product Product;

struct order {
    char username[50];
    long user_id;
    double total_price;
    Product products[50];
    int product_count;
};

typedef struct order Order;

const Product products[] = {
    {"Starter Pack-(Pay-as-You-learn)", 1999.00, {"Access to 50+ beginner-level courses", "Downloadable learning materials", "Both mobile and dekstop access", "Progress tracking with insights and tips"}},
    {"Professional Plan-(Career Booster)", 4999.00, {"Full course library (for all level", "Interactive quizzes and assignments", "Certificate after completion", "1 monthly live workshop"}},
    {"Enterprise Plan-(Team Growth Package)", 7999.00, {"Unlimited access for teams", "Dedicated account manager", "Role-based learning paths", "Analytics & reporting dashboards"}},
    {"Project-Based Learning Pack-(Pay-per-Project)", 4999.00, {"1 Guided real-world project", "Professional mentor feedback", "Portfolio showcase on platform"}},
    {"Premium Plus Lifetime Plan", 14999.00, {"Lifetime access to all courses & materials", "Early VIP access to new contents", "Exclusive Q&A sessions with experts", "Accesss to project-based learning experiences anytime"}}
};

void display_products() {
    printf("Available Products:\n\n");
    for (int i = 0; i < sizeof(products) / sizeof(products[0]); i++) {
        printf("%d. %s - $%.2f\n", i + 1, products[i].name, products[i].price);
        printf("\n   Features:\n");
        for (int j = 0; j < 4; j++) {
            if (strlen(products[i].features[j]) == 0) {
                break;
            }
            printf("   - %s\n", products[i].features[j]);
        }
        printf("\n\n");
    }
    printf("\n");
}

void display_menu() {
    printf("Main Menu\n");
    printf("1. Display all orders\n");
    printf("2. Create order\n");
    printf("3. Search from orders\n");
    printf("4. Edit excisting order\n");
    printf("5. Delete order\n");
    printf("Please select an option: ");
}

/*
 * The Dan Bernstein popuralized hash..  See
 * https://github.com/pjps/ndjbdns/blob/master/cdb_hash.c#L26 Due to hash
 * collisions it seems to be replaced with "siphash" in n-djbdns, see
 * https://github.com/pjps/ndjbdns/commit/16cb625eccbd68045737729792f09b4945a4b508
 * Djb2 hash function
 */
unsigned long hash(char *str) {
    unsigned long hash = 5381;
    int c;
    while ((c = *str++))
        hash = ((hash << 5) + hash) + c;
    return hash % NUM_BUCKETS;
}

int main() {
    char username[50];
    char password[50];
    FILE *file;
    struct termios old_tio, new_tio;
    char ch;

    printf("Welcome to Cloud9 Systems! \n\n");
    printf("Login: \n");
    printf("Username: ");
    gets(username);

    printf("Password: ");
    tcgetattr(STDIN_FILENO, &old_tio);
    new_tio = old_tio;
    new_tio.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &new_tio);

    int i = 0;
    while (scanf("%c", &ch) && ch != '\n') {
        if (ch != '\n') {
            password[i++] = ch;
            putchar('*');
            fflush(stdout);
        }
    }
    password[i] = '\0';
    tcsetattr(STDIN_FILENO, TCSANOW, &old_tio);

    file = fopen("login.txt", "r");
    if (file == NULL) {
        printf("Error opening file!\n");
        return 1;
    }
    char saved_password[50];
    fscanf(file, "%s", &saved_password);
    fclose(file);

    // Simulating a hash check for the password
    if (strcmp(username, "admin") == 0 && hash(password) == atoi(saved_password)) {
        int option;
        printf("\n\nLogin successful!\nHello, %s!\n\n", username);
        display_menu();
        scanf("%d", &option);
        switch (option) {
            case 1:
                printf("Displaying all orders...\n");
                // Here you would implement the logic to display all orders
                break;
            case 2:
                printf("\n\nCreating order...\n\n");
                display_products();
                printf("Please select a product by number: ");
                int product_choice;
                scanf("%d", &product_choice);
                if (product_choice < 1 || product_choice > 4) {
                    printf("Invalid product selection.\n");
                } else {
                    printf("You selected: %s - $%.2f\n", products[product_choice - 1].name, products[product_choice - 1].price);
                    // Here you would implement the logic to create an order
                }
                break;
            case 3:
                printf("Searching from orders...\n");
                // Here you would implement the logic to search from orders
                break;
            case 4:
                printf("Editing existing order...\n");
                // Here you would implement the logic to edit an existing order
                break;
            case 5:
                printf("Deleting order...\n");
                // Here you would implement the logic to delete an order
                break;
            default:
                printf("Invalid option selected.\n");
        }
    }
    else {
        printf("\nLogin failed! Please check your username and password.\n");
    }

    return 0;
}
