#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <windows.h>
#define NUM_BUCKETS 1000

struct product {
    char name[50];
    double price;
    char features[4][100];
};

typedef struct product Product;

struct order {
    char username[50];
    long order_id;
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

Order orders[100];
int order_count = 0;

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

int login() {
    char username[50];
    char password[50];
    FILE *file;
    // struct termios old_tio, new_tio;
    // char ch;

    // printf("Welcome to Cloud9 Systems! \n\n");
    // printf("Login: \n");
    // printf("Username: ");
    // scanf("%s", username);
    // fflush(stdin);
    // getchar();

    // printf("Password: ");
    // tcgetattr(STDIN_FILENO, &old_tio);
    // new_tio = old_tio;
    // new_tio.c_lflag &= ~(ICANON | ECHO);
    // tcsetattr(STDIN_FILENO, TCSANOW, &new_tio);

    // int i = 0;
    // while (scanf("%c", &ch) && ch != '\n') {
    //     if (ch != '\n') {
    //         password[i++] = ch;
    //         putchar('*');
    //         fflush(stdout);
    //     }
    // }
    // password[i] = '\0';
    // tcsetattr(STDIN_FILENO, TCSANOW, &old_tio);

    HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
    DWORD mode;
    GetConsoleMode(hStdin, &mode);
    SetConsoleMode(hStdin, mode & (~ENABLE_ECHO_INPUT)); // Disable echoing

    printf("Password: ");
    char password[50];
    int i = 0;
    char ch;

    while ((ch = _getch()) != '\r' && i < sizeof(password) - 1) { // Read until Enter or buffer full
        if (ch == '\b') { // Handle backspace
            if (i > 0) {
                i--;
                printf("\b \b"); // Erase asterisk
            }
        } else {
            password[i++] = ch;
            printf("*");
        }
    }
    password[i] = '\0'; // Null-terminate the string

    SetConsoleMode(hStdin, mode); // Re-enable echoing

    file = fopen("login.txt", "r");
    if (file == NULL) {
        printf("Error opening file!\n");
        return 1;
    }
    char saved_password[50];
    fscanf(file, "%s", saved_password);
    fclose(file);

    if(strcmp(username, "admin") == 0 && hash(password) == atoi(saved_password)) {
        printf("\n\nLogin successful!\nHello, %s!\n\n", username);
        return 1;
    } else {
        return 0;
    }
}

void display_orders() {
    printf("\nDisplaying all orders: \n\n");
    if (order_count == 0) {
        printf("No orders found.\n\n");
    } else {
        for (int j = 0; j < order_count; j++) {
            printf("%d  - Name: %s,  Order ID: %ld, Total Price: $%.2f\n", 
            j+1, orders[j].username, orders[j].order_id, orders[j].total_price);
        }
        printf("\n");
    }
}

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
    printf("6. Exit\n");
    printf("Please select an option: ");
}

void delete_order() {
    if(order_count == 0) {
        printf("\nNo orders to delete.\n\n");
        return;
    }
    display_orders();
    printf("Enter the order number to delete: ");
    int order_number;
    scanf("%d", &order_number);
    if (order_number < 1 || order_number > order_count) {
        printf("Invalid order number.\n");
        delete_order();
        return;
    } else {
        for (int i = order_number - 1; i < order_count - 1; i++) {
            orders[i] = orders[i + 1];
        }
        order_count--;
        printf("\nOrder deleted successfully.\n\n");
    }
}

void make_order() {
    printf("\n\nCreating order...\n\n");
    display_products();
    printf("Please select a product by number: ");
    int product_choice;
    scanf("%d", &product_choice);
    if (product_choice < 1 || product_choice > 5) {
        printf("Invalid product selection.\n");
        make_order();
    } else {
        char username[50];
        printf("Enter Username: ");
        scanf("%s", username);
        printf("You selected: %s - $%.2f\n", products[product_choice - 1].name, products[product_choice - 1].price);
        Order new_order;
        strcpy(new_order.username, username);
        new_order.order_id = rand() % 1000000;
        new_order.total_price = products[product_choice - 1].price;
        new_order.product_count += 1;
        new_order.products[0] = products[product_choice - 1];
        orders[order_count++] = new_order;
        printf("\nOrder created successfully!\n\n");
        printf("Name: %s\n", new_order.username);
        printf("Order ID: %ld\n", new_order.order_id);
        printf("Products in order:\n");
        printf("%s - $%.2f\n", new_order.products[0].name, new_order.products[0].price);
        printf("Total Price: $%.2f\n\n", new_order.total_price);
    }
}

void search_order() {
    char search_id[50];
    printf("Enter Order ID or Username: ");
    scanf("%s", search_id);
    if(isdigit(search_id[0])) {
        long order_id = atol(search_id);
        for(int i=0; i<order_count; i++) {
            if(orders[i].order_id == order_id) {
                printf("Order found:\n");
                printf("Order ID: %ld\n", orders[i].order_id);
                printf("Username: %s\n", orders[i].username);
                printf("Total Price: $%.2f\n", orders[i].total_price);
                printf("Products in order:\n");
                for(int j=0; j<orders[i].product_count; j++) {
                    printf("%s - $%.2f\n", orders[i].products[j].name, orders[i].products[j].price);
                }
                break;
            }
        }
    } else {
        for(int i=0; i<order_count; i++) {
            if(strcmp(orders[i].username, search_id) == 0) {
                printf("Order found:\n");
                printf("Order ID: %ld\n", orders[i].order_id);
                printf("Username: %s\n", orders[i].username);
                printf("Total Price: $%.2f\n", orders[i].total_price);
                printf("Products in order:\n");
                for(int j=0; j<orders[i].product_count; j++) {
                    printf("%s - $%.2f\n", orders[i].products[j].name, orders[i].products[j].price);
                }
                break;
            }
        }
    }
}

int main() {
    if (login()) {
        int option;
        int flag = 1;
        while(flag) {
            display_menu();
            scanf("%d", &option);
            switch (option) {
                case 1:
                    display_orders();
                    break;
                case 2:
                    make_order();
                    break;
                case 3:
                    search_order();
                    break;
                case 4:
                    printf("Editing existing order...\n");
                    // Here you would implement the logic to edit an existing order
                    break;
                case 5:
                    delete_order();
                    break;
                case 6:
                    printf("Exiting the program. Goodbye!\n");
                    flag = 0;
                    break;
                default:
                    printf("\nInvalid option selected.\n\n");
                }
            }
        } else {
            printf("\nLogin failed! Please check your username and password.\n");
        }
    return 0;
}
