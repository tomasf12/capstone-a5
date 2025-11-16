/*
Student Name: 
Student ID: 
Course: CIS*1300 | Assignment 5
Due Date: November 21st, at 11:59pm

By including this statement in this header comment, I certify that I have read and understood the University policy on academic integrity.

I assert that this work is 100% my own. 
I assert that I did not copy code from another person.
I assert that I did not copy code from AI.
I understand that I will receive a grade of 0 if I copy code from another person or from AI, as this constitutes academic dishonesty.

I have appropriately acknowledged all materials (code, data, images, ideas or words) that I have used, whether directly quoted or paraphrased.

Furthermore, I certify that this assignment was prepared by me specifically for the CIS*1300 Fall 2025 course.
*/      

#include "a5.h"
#include<stdlib.h>

#define MAX_DNA_LENGTH 5096

/**
 * @brief Loads a DNA database from a CSV file into a dynamic array.
 *
 * Opens the file, counts records, allocates an array of struct DNARecord,
 * and populates it. The caller is responsible for freeing the
 * returned array.
 *
 * @param[in]  filename     Path to the CSV database file.
 * @param[out] record_count Pointer to an int to store the total record count.
 *
 * @return Pointer to the allocated DNARecord array on success, 
 * NULL on file or memory error.
 */
struct DNARecord *load_database(const char *filename, int *record_count){
    FILE *fp;
    char s[1024];
    int linecount = 0;

    fp = fopen(filename, "r");

    while(fgets(s, sizeof s, fp) != NULL){
        linecount++;
        strcpy(my_record.id, "ID_from_file");
    }
    printf("%d\n", linecount);
    fclose(fp);
}
