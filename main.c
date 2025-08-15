#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <conio.h>
#include <windows.h>
#define NUM_BUCKETS 1000
#define USERNAME "admin"
#define PASSWORD "password"

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

unsigned long hash(char *str);
int login();
int display_orders();
void display_products();
void display_menu();
void delete_order();
void make_order();
void search_order();
void edit_order(Order *order);

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
                    int order_num;
                    if(display_orders()) {
                        printf("Select order to edit: ");
                        scanf("%d", &order_num);
                        Order *order_ptr = &orders[order_num - 1];
                        edit_order(order_ptr);
                    }
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
            printf("\nLogin failed! Please check your username and password and try again.\n\n");
            login();
        }
    return 0;
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

int login() {
    char username[50];
    char password[50];
    FILE *file;

    file = fopen("login.txt", "w");
    fprintf(file,"%ld", hash(PASSWORD));
    fclose(file);


    printf("Welcome to Cloud9 Systems! \n\n");
    printf("Login: \n");
    printf("Username: ");
    scanf("%s", username);
    fflush(stdin);
    getchar();

    HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
    DWORD mode;
    GetConsoleMode(hStdin, &mode);
    SetConsoleMode(hStdin, mode & (~ENABLE_ECHO_INPUT));

    printf("Password: ");
    int i = 0;
    char ch;

    while ((ch = _getch()) != '\r') {
        if (ch == '\b') {
            if (i > 0) {
                i--;
                printf("\b \b");
            }
        } else {
            password[i++] = ch;
            printf("*");
        }
    }
    password[i] = '\0';
    SetConsoleMode(hStdin, mode);

    file = fopen("login.txt", "r");
    if (file == NULL) {
        printf("Error opening file!\n");
        return 1;
    }
    char saved_password[50];
    fscanf(file, "%s", saved_password);
    fclose(file);

    if(strcmp(username, USERNAME) == 0 && hash(password) == atoi(saved_password)) {
        printf("\n\nLogin successful!\nHello, %s!\n\n", username);
        return 1;
    } else {
        return 0;
    }
}

int display_orders() {
    printf("\nDisplaying all orders: \n\n");
    if (order_count == 0) {
        printf("No orders found.\n\n");
        return 0;
    } else {
        for (int j = 0; j < order_count; j++) {
            printf("%d  - Name: %s,  Order ID: %ld, Total Price: $%.2f\n",
            j+1, orders[j].username, orders[j].order_id, orders[j].total_price);
        }
        printf("\n");
        return 1;
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
    printf("4. Edit existing order\n");
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

void display_order(int order_num) {
    printf("Name: %s\n", orders[order_num].username);
    printf("Order ID: %ld\n", orders[order_num].order_id);
    printf("Total Price: $%.2f\n", orders[order_num].total_price);
    printf("Products in order:\n");
    for(int j=0; j<orders[order_num].product_count; j++) {
        printf("    %d) %s - $%.2f\n\n",j+1, orders[order_num].products[j].name, orders[order_num].products[j].price);
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
                printf("\nOrder found!\n\n");
                display_order(i);
                break;
            }
        }
    } else {
        for(int i=0; i<order_count; i++) {
            if(strcmp(orders[i].username, search_id) == 0) {
                display_order(i);
                break;
            }
        }
    }
}

void edit_order(Order *order) {
    char new_name[50];
    printf("Enter new name: ");
    scanf("%s", new_name);

    strcpy(order->username, new_name);
    printf("Order edited successfully! \n\n");
}
