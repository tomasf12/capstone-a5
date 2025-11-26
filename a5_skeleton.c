/*
Student Name: Tomas Fickert
Student ID: 1295844
Course: CIS*1300 | Assignment 5
Due Date: November 25th, at 11:59pm

By including this statement in this header comment, I certify that I have read and understood the University policy on academic integrity.

I assert that this work is 100% my own. 
I assert that I did not copy code from another person.
I assert that I did not copy code from AI.
I understand that I will receive a grade of 0 if I copy code from another person or from AI, as this constitutes academic dishonesty.

I have appropriately acknowledged all materials (code, data, images, ideas or words) that I have used, whether directly quoted or paraphrased.

Furthermore, I certify that this assignment was prepared by me specifically for the CIS*1300 Fall 2025 course.
*/  

#include "a5.h"

#define MAX_LINE_LENGTH 5096 

/*
 * Function: load_database
 * 
 * Opens the CSV file, counts the records, dynamically allocates an array
 * of DNARecord structs, and parses the file content into the array.
 *
 * filename: The name of the CSV file to read.
 * record_count: Pointer to an integer to store the number of records found.
 *
 * returns: A pointer to the dynamically allocated array of records,
 * or NULL if the file cannot be opened or memory allocation fails.
 */
struct DNARecord *load_database(const char *filename, int *record_count) {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Error: Could not open file %s\n", filename);
        *record_count = 0;
        return NULL;
    }

    // Step 1: Count the number of lines (records) in the file
    int count = 0;
    char buffer[MAX_LINE_LENGTH];
    while (fgets(buffer, MAX_LINE_LENGTH, fp) != NULL) {
        // Only count lines that are not empty
        if (strlen(buffer) > 1) {
            count++;
        }
    }

    *record_count = count;

    if (count == 0) {
        fclose(fp);
        return NULL;
    }

    // Step 2: Allocate memory for the array of structs
    struct DNARecord *db = malloc(sizeof(struct DNARecord) * count);
    if (db == NULL) {
        printf("Error: Memory allocation failed for database array.\n");
        fclose(fp);
        return NULL;
    }

    // Step 3: Rewind file and parse data
    rewind(fp);
    int i = 0;
    while (fgets(buffer, MAX_LINE_LENGTH, fp) != NULL && i < count) {
        // Remove trailing newline character
        buffer[strcspn(buffer, "\n")] = 0;

        // Parse ID
        char *token = strtok(buffer, ",");
        if (token != NULL) {
            strncpy(db[i].id, token, 10);
            db[i].id[9] = '\0'; // Ensure null-termination
        }

        // Parse Classification
        token = strtok(NULL, ",");
        if (token != NULL) {
            strncpy(db[i].classification, token, 50);
            db[i].classification[49] = '\0'; // Ensure null-termination
        }

        // Parse Sequence (Dynamically allocated)
        token = strtok(NULL, ",");
        if (token != NULL) {
            db[i].sequence = malloc(strlen(token) + 1);
            if (db[i].sequence == NULL) {
                printf("Error: Memory allocation failed for sequence at record %d.\n", i);
                // Cleanup would ideally happen here, but for simplicity we continue or exit
            } else {
                strcpy(db[i].sequence, token);
            }
        } else {
            // Handle case where sequence might be missing
            db[i].sequence = malloc(1);
            db[i].sequence[0] = '\0';
        }

        i++;
    }

    fclose(fp);
    return db;
}

/*
 * Function: find_longest_sequence
 * 
 * Iterates through the database to find the record with the longest DNA sequence.
 *
 * database: Pointer to the array of records.
 * record_count: The number of records in the database.
 * longest_length: Pointer to an integer to store the length found.
 *
 * returns: The ID of the record with the longest sequence, or NULL if empty.
 */
char *find_longest_sequence(struct DNARecord *database, int record_count, int *longest_length) {
    if (database == NULL || record_count == 0) {
        *longest_length = 0;
        return NULL;
    }

    int max_len = -1;
    int max_index = -1;

    for (int i = 0; i < record_count; i++) {
        if (database[i].sequence != NULL) {
            int current_len = strlen(database[i].sequence);
            if (current_len > max_len) {
                max_len = current_len;
                max_index = i;
            }
        }
    }

    if (max_index != -1) {
        *longest_length = max_len;
        return database[max_index].id;
    }

    return NULL;
}

/*
 * Function: count_by_classification
 * 
 * Counts how many records match a specific classification (case-sensitive).
 *
 * database: Pointer to the array of records.
 * record_count: The number of records.
 * classification: The classification string to search for.
 *
 * returns: The integer count of matching records.
 */
int count_by_classification(struct DNARecord *database, int record_count, const char *classification) {
    if (database == NULL || record_count == 0) {
        return 0;
    }

    int count = 0;
    for (int i = 0; i < record_count; i++) {
        if (strcmp(database[i].classification, classification) == 0) {
            count++;
        }
    }

    return count;
}

/*
 * Function: calculate_average_sequence_length
 * 
 * Calculates the average length of all DNA sequences in the database.
 *
 * database: Pointer to the array of records.
 * record_count: The number of records.
 *
 * returns: The average length as a double. Returns 0.0 if empty.
 */
double calculate_average_sequence_length(struct DNARecord *database, int record_count) {
    if (database == NULL || record_count == 0) {
        return 0.0;
    }

    long total_length = 0;
    for (int i = 0; i < record_count; i++) {
        if (database[i].sequence != NULL) {
            total_length += strlen(database[i].sequence);
        }
    }

    return (double)total_length / record_count;
}

/*
 * Function: add_record
 * 
 * Adds a new record to the database by reallocating the array to be larger.
 *
 * db_ptr: Pointer to the database array pointer (allows modification of the array ptr).
 * record_count: Pointer to the record count (will be incremented).
 * id: New record ID.
 * classification: New record classification.
 * sequence: New DNA sequence.
 *
 * returns: 1 on success, 0 on failure.
 */
int add_record(struct DNARecord **db_ptr, int *record_count, const char *id, const char *classification, const char *sequence) {
    int new_count = *record_count + 1;

    // Attempt to resize the array
    struct DNARecord *temp = realloc(*db_ptr, sizeof(struct DNARecord) * new_count);
    if (temp == NULL) {
        printf("Error: Failed to reallocate memory for new record.\n");
        return 0; // Failure
    }

    // Update the main pointer to the new block
    *db_ptr = temp;

    // Access the new last element
    struct DNARecord *new_record = &(*db_ptr)[*record_count];

    // Copy static fields
    strncpy(new_record->id, id, 10);
    new_record->id[9] = '\0'; 

    strncpy(new_record->classification, classification, 50);
    new_record->classification[49] = '\0';

    // Allocate and copy the dynamic sequence
    new_record->sequence = malloc(strlen(sequence) + 1);
    if (new_record->sequence == NULL) {
        printf("Error: Failed to allocate memory for new sequence.\n");
       
        return 0;
    }
    strcpy(new_record->sequence, sequence);

    // Update the count
    *record_count = new_count;

    return 1; // Success
}

/*
 * Function: delete_record
 * 
 * Removes a record with the specified ID and shrinks the array.
 *
 * db_ptr: Pointer to the database array pointer.
 * record_count: Pointer to the record count (will be decremented).
 * id_to_delete: The ID of the record to remove.
 *
 * returns: 1 on success (record found and deleted), 0 on failure (not found).
 */
int delete_record(struct DNARecord **db_ptr, int *record_count, const char *id_to_delete) {
    if (*db_ptr == NULL || *record_count == 0) {
        return 0;
    }

    int index_to_delete = -1;

    // Find the record index
    for (int i = 0; i < *record_count; i++) {
        if (strcmp((*db_ptr)[i].id, id_to_delete) == 0) {
            index_to_delete = i;
            break;
        }
    }

    if (index_to_delete == -1) {
        return 0; // Record not found
    }

    // Free the sequence memory for the deleted record
    free((*db_ptr)[index_to_delete].sequence);

    // Shift subsequent records down to fill the gap
    for (int i = index_to_delete; i < *record_count - 1; i++) {
        (*db_ptr)[i] = (*db_ptr)[i + 1];
    }

    // Decrement count
    (*record_count)--;

    // Resize the array to save memory
    if (*record_count > 0) {
        struct DNARecord *temp = realloc(*db_ptr, sizeof(struct DNARecord) * (*record_count));
        if (temp != NULL) {
            *db_ptr = temp;
        }
        // If realloc fails when shrinking, it's usually okay to keep the old larger block
        // so we don't return error here.
    } else {
        // If count is 0, free the array entirely
        free(*db_ptr);
        *db_ptr = NULL;
    }

    return 1; // Success
}

/*
 * Function: free_database
 * 
 * Frees all dynamically allocated memory (individual sequences and the main array).
 *
 * database: Pointer to the database array.
 * record_count: The number of records.
 */
void free_database(struct DNARecord *database, int record_count) {
    if (database == NULL) {
        return;
    }

    // Free each dynamically allocated sequence string
    for (int i = 0; i < record_count; i++) {
        if (database[i].sequence != NULL) {
            free(database[i].sequence);
        }
    }

    // Free the array of structs
    free(database);
}