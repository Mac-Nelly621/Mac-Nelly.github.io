#include "hash_table.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MIN_USER_ID 1
#define MAX_USER_ID 2139

// Sets every bucket to NULL so the table starts out empty.
void initialise_hash_table(HashTable *ht)
{
    if (ht == NULL)
    {
        return;
    }

    for (int i = 0; i < HASH_TABLE_SIZE; i++)
    {
        ht->buckets[i] = NULL;
    }
}

// Returns the bucket index for a user ID using h(k) = k mod N,
// or -1 if the ID is outside the valid range of 1 to 2139.
int get_bucket_for_user(int user_id)
{
    if (user_id < MIN_USER_ID || user_id > MAX_USER_ID)
    {
        return -1;
    }

    return user_id % HASH_TABLE_SIZE;
}

// Walks the chain in the user's bucket and returns a pointer to the
// matching user, or NULL if the user is not found or inputs are invalid.
User *find_user(const HashTable *ht, int user_id)
{
    if (ht == NULL)
    {
        return NULL;
    }

    int bucket = get_bucket_for_user(user_id);
    if (bucket == -1)
    {
        return NULL;
    }

    User *current = ht->buckets[bucket];
    while (current != NULL)
    {
        if (current->user_id == user_id)
        {
            return current;
        }
        current = current->next;
    }

    return NULL;
}

// Adds a new user at the head of their bucket's chain, or updates the
// password if the user already exists.
int register_user(HashTable *ht, int user_id, char password[])
{
    if (ht == NULL || password == NULL)
    {
        return ERROR;
    }

    int bucket = get_bucket_for_user(user_id);
    if (bucket == -1 || strlen(password) > MAX_PASSWORD_LEN)
    {
        return ERROR;
    }

    // existing user: update their record rather than adding a duplicate
    User *existing = find_user(ht, user_id);
    if (existing != NULL)
    {
        strcpy(existing->password, password);
        return SUCCESS;
    }

    User *new_user = malloc(sizeof(User));
    if (new_user == NULL)
    {
        return ERROR;
    }

    new_user->user_id = user_id;
    strcpy(new_user->password, password);

    // insert at the head of the chain
    new_user->next = ht->buckets[bucket];
    ht->buckets[bucket] = new_user;

    return SUCCESS;
}

// Returns SUCCESS only if the user exists and the password matches.
int is_valid_password(const HashTable *ht, int user_id, char password[])
{
    if (ht == NULL || password == NULL)
    {
        return ERROR;
    }

    User *user = find_user(ht, user_id);
    if (user == NULL)
    {
        return ERROR;
    }

    if (strcmp(user->password, password) == 0)
    {
        return SUCCESS;
    }

    return ERROR;
}

// Frees every user in every chain and resets each bucket to NULL.
void free_buckets(HashTable *ht)
{
    if (ht == NULL)
    {
        return;
    }

    for (int i = 0; i < HASH_TABLE_SIZE; i++)
    {
        User *current = ht->buckets[i];
        while (current != NULL)
        {
            User *next = current->next; // save before freeing
            free(current);
            current = next;
        }
        ht->buckets[i] = NULL;
    }
}

// Reads "user_id password" pairs from a file, registering them in file
// order so that chain ordering is preserved.
int load_users_from_file(HashTable *ht, char filename[])
{
    if (ht == NULL || filename == NULL)
    {
        return ERROR;
    }

    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        return ERROR;
    }

    int user_id;
    char password[MAX_PASSWORD_LEN + 2]; // one extra char to detect overlong passwords
    int fields_read;
    int result = SUCCESS;

    while ((fields_read = fscanf(file, "%d %41s", &user_id, password)) == 2)
    {
        if (register_user(ht, user_id, password) != SUCCESS)
        {
            result = ERROR;
            break;
        }
    }

    // anything other than a clean end of file means malformed data
    if (result == SUCCESS && fields_read != EOF)
    {
        result = ERROR;
    }

    fclose(file);
    return result;
}
