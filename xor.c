#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_KEY_LENGTH 256

void xor_encrypt_decrypt(char *data, size_t length, const char *key) {
    for (size_t i = 0; i < length; ++i) {
        data[i] ^= key[i % strlen(key)];
    }
}

int main() {
    char choice[3];
    printf("Enter 'e' to encrypt or 'd' to decrypt: ");
    fgets(choice, sizeof(choice), stdin);

    if (choice[0] != 'e' && choice[0] != 'd') {
        printf("Invalid choice.\n");
        return 1;
    }

    char filename[256];
    printf("Enter the filename: ");
    fgets(filename, sizeof(filename), stdin);
    filename[strcspn(filename, "\n")] = '\0';

    FILE *file;
    if (choice[0] == 'e') {
        file = fopen(filename, "w");
        if (!file) {
            perror("Failed to open file for writing");
            return 1;
        }
    } else {
        file = fopen(filename, "r");
        if (!file) {
            perror("Failed to open file for reading");
            return 1;
        }
    }



    char key[MAX_KEY_LENGTH];
    printf("Enter the key: ");
    fgets(key, sizeof(key), stdin);
    key[strcspn(key, "\n")] = '\0';

    if (strlen(key) == 0) {
        fprintf(stderr, "Key cannot be empty.\n");
        fclose(file);
        return 1;
    }

    char buffer[4096];
    size_t bytes_read;

    if (choice[0] == 'e') {
        printf("Enter the string to encrypt: ");
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = '\0';

        xor_encrypt_decrypt(buffer, strlen(buffer), key);

        fwrite(buffer, 1, strlen(buffer), file);
    } else {
        bytes_read = fread(buffer, 1, sizeof(buffer) - 1, file);
        if (bytes_read == 0) {
            perror("Failed to read from file");
            fclose(file);
            return 1;
        }

        buffer[bytes_read] = '\0';
        xor_encrypt_decrypt(buffer, bytes_read, key);

        printf("%s\n", buffer);
    }

    fclose(file);
    return 0;
}