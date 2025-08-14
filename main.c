#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#define NUM_BUCKETS 1000

/*
 * The Dan Bernstein popuralized hash..  See
 * https://github.com/pjps/ndjbdns/blob/master/cdb_hash.c#L26 Due to hash
 * collisions it seems to be replaced with "siphash" in n-djbdns, see
 * https://github.com/pjps/ndjbdns/commit/16cb625eccbd68045737729792f09b4945a4b508
 * Djb2 hash function
 */
unsigned long hash(char *str)
{

    unsigned long hash = 5381;
    int c;
    while ((c = *str++))
        hash = ((hash << 5) + hash) + c;
    return hash % NUM_BUCKETS;
}

int main()
{
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
    while (scanf("%c", &ch) && ch != '\n')
    {
        if (ch != '\n')
        {
            password[i++] = ch;
            putchar('*');
            fflush(stdout);
        }
    }
    password[i] = '\0';
    tcsetattr(STDIN_FILENO, TCSANOW, &old_tio);

    file = fopen("login.txt", "r");
    if (file == NULL)
    {
        printf("Error opening file!\n");
        return 1;
    }
    char saved_password[50];
    fscanf(file, "%s", &saved_password);
    fclose(file);

    // Simulating a hash check for the password
    if (strcmp(username, "admin") == 0 && hash(password) == atoi(saved_password))
    {
        printf("\nLogin successful!\nHello, %s!\n", username);
    }
    else
    {
        printf("\nLogin failed! Please check your username and password.\n");
    }

    return 0;
}
