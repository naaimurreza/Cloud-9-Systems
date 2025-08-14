#include <stdio.h>
#include <string.h>
#include <openssl/sha.h>


/*
 * The Dan Bernstein popuralized hash..  See
 * https://github.com/pjps/ndjbdns/blob/master/cdb_hash.c#L26 Due to hash
 * collisions it seems to be replaced with "siphash" in n-djbdns, see
 * https://github.com/pjps/ndjbdns/commit/16cb625eccbd68045737729792f09b4945a4b508
 */
uint32_t djb33_hash(const char* s, size_t len)
{
    uint32_t h = 5381;
    while (len--) {
        /* h = 33 * h ^ s[i]; */
        h += (h << 5);  
        h ^= *s++;
    }
    return h;
}

int main() {
    char username[50];
    char password[50];
    FILE *file;

    printf("Welcome to Cloud9 Systems! \n\n");
    printf("Login: \n");
    printf("Username: ");
    scanf("%s", username);
    printf("Password: ");
    scanf("%s", password);

    file = fopen("login.txt", "r");
    if (file == NULL) {
        printf("Error opening file!\n");
        return 1;
    } 
    char saved_password[50];
    fscanf(file, "%d", &saved_password);
    fclose(file);

    // Simulating a hash check for the password
    if(strcmp(username, "admin") == 0 && djb33_hash(password, strlen(password) == saved_password)) {
        printf("\nLogin successful! Hello, %s!\n", username);
    } else {
        printf("\nLogin failed! Please check your username and password.\n");
    } 


    return 0;
}
