#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <conio.h>
#include <windows.h>
#define NUM_BUCKETS 1000
#define TAX_RATE 0.12


struct product {
    char name[50];
    double price;
    char features[4][100];
};

typedef struct product Product;

struct order {
    char username[50];
    long order_id;
    Product product;
} default_order = {"", 0, {}};

typedef struct order Order;

const Product products[] = {
    {"NimbusCore", 100.00, {"Scalable virtual compute instances (Nano to Ultra sizes)", "Auto-healing via health monitoring", "Built-in encryption at rest and in transit", "Quick-launch templates with preconfigured stacks (LAMP, Node.js, etc.)"}},
    {"VultrixCompute", 150.00, {"Custom instance builder (CPU, RAM, Disk sliders)", "ISO-based custom OS support", "High availability with no additional configuration", "Built-in DDoS protection and traffic analysis"}},
    {"CloudForge", 250.00, {"Optimized for containerized workloads (Docker & Kubernetes)", "Integrated GPU options for ML and rendering", "Spot instance marketplace with aggressive bidding", "Built-in CI/CD support for rapid deployment"}},
    {"ElasticHive", 350.00, {"Intelligent auto-scaling based on real-time traffic", "Integrated APM (Application Performance Monitoring)", "Instance affinity for microservice clusters", "Spot, burstable, and dedicated instance classes"}},
    {"SkyNode", 900.00, {"Developer-friendly CLI and web console", "One-click cloning and snapshotting of instances", "Instance sharing and collaborative development tools", "Configurable firewall and zero-trust networking"}}
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
void load_orders();

int main() {
    load_orders();
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
                    if(display_orders()) {
                        int order_num;
                        printf("\t\t\t\t\t\t\t\t\t\t\t\t\tSelect order to edit: ");
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
            printf("\n\n\t\t\t\t\t\t\t\t\t\t\t\t\tLogin failed! Please check your username and password and try again.\n\n");
            login();
        }
    return 0;
}

unsigned long hash(char *str) {
    unsigned long hash = 5381;
    int c;
    while ((c = *str++))
        hash = ((hash << 5) + hash) + c;
    return hash % NUM_BUCKETS;
}

int login() {
    char username[50], saved_username[50], password[50], saved_password[50];
    FILE *file;

    file = fopen("login.txt", "r");
    fscanf(file, "%s\n%s", saved_username, saved_password);
    fclose(file);
                                                                 
                                                                                      
    printf("\t\t\t\t\t\t\t\t\t\t                                     @@@@@@                                     \n");
    printf("\t\t\t\t\t\t\t\t\t\t                                 @@@@@@@@@@@@@@                                 \n");
    printf("\t\t\t\t\t\t\t\t\t\t                               @@@@@@@@@@@@@@@@@@                               \n");
    printf("\t\t\t\t\t\t\t\t\t\t                             @@@@@@@      @@@@@@@                              \n");
    printf("\t\t\t\t\t\t\t\t\t\t                             @@@@@@          @@@@@@                             \n");
    printf("\t\t\t\t\t\t\t\t\t\t                        @@@@@@@@@@   @@@@@@   @@@@@                             \n");
    printf("\t\t\t\t\t\t\t\t\t\t                     @@@@@@@@@@@@   @@@@@@@}  @@@@@@@@@@                        \n");
    printf("\t\t\t\t\t\t\t\t\t\t                    @@@@@@@@@@@@@@   @@@@@@   @@@@@@@@@@@@@                     \n");
    printf("\t\t\t\t\t\t\t\t\t\t                   @@@@@@@@@@@@@@@     @@     @@@@@@@@@@@@@@@                   \n");
    printf("\t\t\t\t\t\t\t\t\t\t                 @@@@@@@@@@@@@@@@@@@        @@@@@@@@@@@@@@@@@                   \n");
    printf("\t\t\t\t\t\t\t\t\t\t                @@@@@@@@@@@@@@@@@@@@@@@@   @@@@@@@@@@@@@@@@@@@                  \n");
    printf("\t\t\t\t\t\t\t\t\t\t               @@@@@@@@@@@@@@@@@@@@@@@@   @@@@@@@@@@@@@@@@@@@@                  \n");
    printf("\t\t\t\t\t\t\t\t\t\t               @@@@@@@@@@@@@@@@@@@@@@@   @@@@@@@@@@@@@@@@@@@@                   \n");
    printf("\t\t\t\t\t\t\t\t\t\t                @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@                    \n");
    printf("\t\t\t\t\t\t\t\t\t\t                 @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*                     \n");
    printf("\t\t\t\t\t\t\t\t\t\t                                                                                \n");
    printf("\t\t\t\t\t\t\t\t\t\t              @@@@@@@   @@                            @@   @@@@@@              \n");
    printf("\t\t\t\t\t\t\t\t\t\t              @@@@ @@@@@ @@~                           @@  @@@ .@@@             \n");
    printf("\t\t\t\t\t\t\t\t\t\t             @@@         @@  @@@@@@@@  @@   @@@  @@@@@@@@ @@@    @@@            \n");
    printf("\t\t\t\t\t\t\t\t\t\t            @@@          @@ @@@    @@@ @@   @@@ @@@   @@@  @@@@@@@@@            \n");
    printf("\t\t\t\t\t\t\t\t\t\t             @@@      +  @@ @@@    @@@ @@   @@@ @@@    @@   )@@@@@@             \n");
    printf("\t\t\t\t\t\t\t\t\t\t              @@@@ @@@@@ @@  @@@  @@@  @@@ ~@@@ @@@@  @@@     @@@@              \n");
    printf("\t\t\t\t\t\t\t\t\t\t               @@@@@@@   @@   @@@@@@    @@@@@@@   @@@@@@@   @@@@                \n");
    printf("\t\t\t\t\t\t\t\t\t\t                                                                                \n");
    printf("\t\t\t\t\t\t\t\t\t\t               @  @@  @@ @@  @@ @@   @@@   @@     @@@  @@@  @@ @@               \n");
    printf("\t\t\t\t\t\t\t\t\t\t               =@@@@   @@@    @@@@    @#   @@@@@  @@@@@@@@   @@@@               \n");
    printf("\t\t\t\t\t\t\t\t\t\t               @@@@@    @    @@@@@   +@@   @@@@@  @@  @ @@  @@@@@               \n\n");

    printf("\t\t\t\t\t\t\t\t\t\t\t\t\t_______________________________\n");
    printf("\t\t\t\t\t\t\t\t\t\t\t\t\t| Welcome to Cloud 9 Systems! |\n");
    printf("\t\t\t\t\t\t\t\t\t\t\t\t\t|*****************************|\n");
    printf("\t\t\t\t\t\t\t\t\t\t\t\t\t| Login:                      |\n");
    printf("\t\t\t\t\t\t\t\t\t\t\t\t\t  Username: ");
    scanf("%s", username);
    fflush(stdin);

    HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
    DWORD mode;
    GetConsoleMode(hStdin, &mode);
    SetConsoleMode(hStdin, mode & (~ENABLE_ECHO_INPUT));

    printf("\t\t\t\t\t\t\t\t\t\t\t\t\t  Password: ");
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
        printf("\t\t\t\t\t\t\t\t\t\t\t\t\tError opening file!\n");
        return 1;
    }
    

    if(strcmp(username, saved_username) == 0 && hash(password) == atoi(saved_password)) {
        printf("\n\n\t\t\t\t\t\t\t\t\t\t\t\t\tLogin successful!\n\t\t\t\t\t\t\t\t\t\t\t\t\tHello, %s!\n\n", username);
        return 1;
    } else {
        return 0;
    }
}

void load_orders() {
    FILE *file;
    file = fopen("orders.txt", "r");
    if(file) {
        char name[50];
        int id, i=0;
        Product prod;
        float price;
        char product_name[50];
        while(fscanf(file,"Name: %s\nOrder ID: %d\nProduct:\nName: %s\nPrice: %f\n\n",
         name, &id, product_name, &price) == 4) {
            strcpy(prod.name, product_name);
            prod.price = price;
            Order order;
            strcpy(order.username, name);
            order.order_id = id;
            order.product = prod;
            orders[i++] = order;
            if(feof(file) == 1) {
                break;
            }
        }
        order_count = i;
    } else {
        return;
    }
}

void write_orders(Order orders[100]) {
    FILE *file;
    file = fopen("orders.txt", "w");
    for(int i=0; i<order_count; i++) {
        fprintf(file, "Name: %s\nOrder ID: %d\nProduct:\nName: %s\nPrice: %.2f\n\n",
             orders[i].username, orders[i].order_id, orders[i].product.name, orders[i].product.price);
    }
}

int display_orders() {
    printf("\n\t\t\t\t\t\t\t\t\t\t\t\t\tDisplaying all orders: \n\n");
    if (order_count == 0) {
        printf("\t\t\t\t\t\t\t\t\t\t\t\t\tNo orders found.\n\n");
        return 0;
    } else {
        for (int j = 0; j < order_count; j++) {
            printf("\t\t\t\t\t\t\t\t\t\t\t%d  - Name: %s,  Order ID: %ld, Total Price: $%.2f, Product: ",
            j+1, orders[j].username, orders[j].order_id, orders[j].product.price);
            printf("%s  ", orders[j].product.name);
            printf("\n");
        }
        printf("\n");
        return 1;
    }
}

void display_products() {
    printf("\t\t\t\t\t\t\t\t\t\t\t\t\tAvailable Products:\n\n");
    for (int i = 0; i < sizeof(products) / sizeof(products[0]); i++) {
        printf("\t\t\t\t\t\t\t\t\t\t\t\t%d. %s - TK. %.2f\n", i + 1, products[i].name, products[i].price);
        printf("\n\t\t\t\t\t\t\t\t\t\t\t\t   Features:\n");
        for (int j = 0; j < 4; j++) {
            if (strlen(products[i].features[j]) == 0) {
                break;
            }
            printf("\t\t\t\t\t\t\t\t\t\t\t\t   - %s\n", products[i].features[j]);
        }
        printf("\n\n");
    }
    printf("\n");
}

void display_menu() {
    printf("\n\t\t\t\t\t\t\t\t\t\t\t\t\t+----------------------------------------+\n");
    printf("\t\t\t\t\t\t\t\t\t\t\t\t\t|        Cloud 9 Systems CLI Menu        |\n");
    printf("\t\t\t\t\t\t\t\t\t\t\t\t\t+----------------------------------------+\n");
    printf("\t\t\t\t\t\t\t\t\t\t\t\t\t| 1. Display all orders                  |\n");
    printf("\t\t\t\t\t\t\t\t\t\t\t\t\t| 2. Create order                        |\n");
    printf("\t\t\t\t\t\t\t\t\t\t\t\t\t| 3. Search from orders                  |\n");
    printf("\t\t\t\t\t\t\t\t\t\t\t\t\t| 4. Edit existing order                 |\n");
    printf("\t\t\t\t\t\t\t\t\t\t\t\t\t| 5. Delete order                        |\n");
    printf("\t\t\t\t\t\t\t\t\t\t\t\t\t| 6. Exit                                |\n");
    printf("\t\t\t\t\t\t\t\t\t\t\t\t\t+----------------------------------------+\n");

    printf("\t\t\t\t\t\t\t\t\t\t\t\t\tPlease select an option: ");
}

void delete_order() {
    if(order_count == 0) {
        printf("\n\t\t\t\t\t\t\t\t\t\t\t\t\tNo orders to delete.\n\n");
        return;
    }
    display_orders();
    printf("\t\t\t\t\t\t\t\t\t\t\t\t\tEnter the order number to delete: ");
    int order_number;
    scanf("%d", &order_number);
    if (order_number < 1 || order_number > order_count) {
        printf("\t\t\t\t\t\t\t\t\t\t\t\t\tInvalid order number.\n");
        delete_order();
        return;
    } else {
        for (int i = order_number - 1; i < order_count - 1; i++) {
            orders[i] = orders[i + 1];
        }
        order_count--;
        write_orders(orders);
        printf("\n\t\t\t\t\t\t\t\t\t\t\t\t\tOrder deleted successfully.\n\n");
    }
}

void print_receipt(Order new_order) {
    float subtotal = 0.0, tax, total;

    printf("\t\t\t\t\t\t\t\t\t\t\t\t\t+-----------------------------------------------+\n");
    printf("\t\t\t\t\t\t\t\t\t\t\t\t\t|           Cloud 9 Systems Receipt             |\n");
    printf("\t\t\t\t\t\t\t\t\t\t\t\t\t+-----------------------------------------------+\n");
    printf("\t\t\t\t\t\t\t\t\t\t\t\t\t| Item              Qty   Unit    Total         |\n");
    printf("\t\t\t\t\t\t\t\t\t\t\t\t\t+-----------------------------------------------+\n");

    
    subtotal =new_order.product.price;
    printf("\t\t\t\t\t\t\t\t\t\t\t\t\t| %-17s 1  $%7.2f  $%7.2f       |\n",
            new_order.product.name, new_order.product.price, subtotal);
 
    printf("\t\t\t\t\t\t\t\t\t\t\t\t\t+-----------------------------------------------+\n");

    tax = subtotal * TAX_RATE;
    total = subtotal + tax;

    printf("\t\t\t\t\t\t\t\t\t\t\t\t\t| Subtotal:                        $%7.2f     |\n", subtotal);
    printf("\t\t\t\t\t\t\t\t\t\t\t\t\t| Tax (12%%):                       $%7.2f     |\n", tax);
    printf("\t\t\t\t\t\t\t\t\t\t\t\t\t| TOTAL:                           $%7.2f     |\n", total);
    printf("\t\t\t\t\t\t\t\t\t\t\t\t\t+-----------------------------------------------+\n");
    printf("\t\t\t\t\t\t\t\t\t\t\t\t\t|     Thank you for shopping with us!           |\n");
    printf("\t\t\t\t\t\t\t\t\t\t\t\t\t+-----------------------------------------------+\n\n");
}

void make_order() {
    printf("\n\n\t\t\t\t\t\t\t\t\t\t\t\t\tCreating order...\n\n");
    display_products();
    printf("\t\t\t\t\t\t\t\t\t\t\t\t\tPlease select a product by number: ");
    int product_choice;
    scanf("%d", &product_choice);
    if (product_choice < 1 || product_choice > 5) {
        printf("\t\t\t\t\t\t\t\t\t\t\t\t\tInvalid product selection. Try again.\n");
        make_order();
    } else {
        char username[50];
        printf("\t\t\t\t\t\t\t\t\t\t\t\t\tEnter Name: ");
        scanf("%s", username);
        printf("\t\t\t\t\t\t\t\t\t\t\t\t\tYou selected: %s - $%.2f\n", products[product_choice - 1].name, products[product_choice - 1].price);
        Order new_order = default_order;
        strcpy(new_order.username, username);
        new_order.order_id = rand() % 1000000;
        new_order.product = products[product_choice-1];
        orders[order_count++] = new_order;
        FILE *fp;
        fp = fopen("orders.txt","a");
        if(fp) {
            fprintf(fp,"Name: %s\nOrder ID: %d\n", new_order.username, new_order.order_id);
            fprintf(fp,"Product:\nName: %s\nPrice: %.2f\n\n", new_order.product.name, new_order.product.price); 
        } else {
            printf("\n\t\t\t\t\t\t\t\t\t\t\t\t\tError opening file.\n");
        }
        fclose(fp);
        printf("\n\t\t\t\t\t\t\t\t\t\t\t\t\tOrder created successfully!\n\n");

        print_receipt(new_order);

    }
}

void display_order(int order_num) {
    printf("\t\t\t\t\t\t\t\t\t\t\t\t\tName: %s\n", orders[order_num].username);
    printf("\t\t\t\t\t\t\t\t\t\t\t\t\tOrder ID: %ld\n", orders[order_num].order_id);
    printf("\t\t\t\t\t\t\t\t\t\t\t\t\tProduct in order:\n");
    printf("\t\t\t\t\t\t\t\t\t\t\t\t\t%s - $%.2f\n\n", orders[order_num].product.name, orders[order_num].product.price);
}

void search_order() {
    char search_id[50];
    printf("\t\t\t\t\t\t\t\t\t\t\t\t\tEnter Order ID or Username: ");
    scanf("%s", search_id);
    if(isdigit(search_id[0])) {
        long order_id = atol(search_id);
        printf("\n\t\t\t\t\t\t\t\t\t\t\t\t\tOrders found:\n\n");
        for(int i=0; i<order_count; i++) {
            if(orders[i].order_id == order_id) {
                display_order(i);
                break;
            }
        }
    } else {
        int flag = 0;
        printf("\n\t\t\t\t\t\t\t\t\t\t\t\t\tOrders Found:\n\n");
        for(int i=0; i<order_count; i++) {
            for(int j=0; j<strlen(search_id); j++) {
                if(search_id[j] == orders[i].username[j]) {
                    flag = 1;
                } else {
                    flag = 0;
                }
            }
            if(flag) {
                display_order(i);
            }
        }
        if(!flag) {
            printf("\t\t\t\t\t\t\t\t\t\t\t\t\tNo orders found!\n\n");
        }
    }
}

void edit_order(Order *order) {
    char new_name[50];
    int id;
    printf("\t\t\t\t\t\t\t\t\t\t\t\t\tEnter new name: ");
    scanf("%s", new_name);
    strcpy(order->username, new_name);
    write_orders(orders);
    printf("\n\t\t\t\t\t\t\t\t\t\t\t\t\tSuccessfully edited the order!\n");
}
