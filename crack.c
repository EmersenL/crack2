#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "md5.h"

const int PASS_LEN=20;        // Maximum any password will be
const int HASH_LEN=33;        // Length of MD5 hash strings

int tryguess(char *hash, char *guess);
char ** read_dictionary(char *filename, int *size);
char **argv;

int main(int argc, char *argv[])
{
    if (argc < 3) 
    {
        printf("Usage: %s hash_file dict_file\n", argv[0]);
        exit(1);
    }

    // Read the dictionary file into an array of strings.
    int dlen;
    char **dict = read_dictionary(argv[2], &dlen);

    // Open the hash file for reading.
    FILE *hashes = fopen(argv[1], "r");
    if (!hashes)
    {
        printf("Can't open %s for writing\n", argv[1]);
        exit(1);
    }

    // For each hash, try every entry in the dictionary.
    // Print the matching dictionary entry.
    // Need two nested loops.
    char hashchar[50];
    while (fgets(hashchar, 50, hashes) != NULL)
    {
        hashchar[strlen(hashchar)-1] = '\0';
        for (int i = 0; i < dlen; i++)
        {
            if (tryguess( hashchar, dict[i]) == 1)
            {
                printf("%s", dict[i]);
            }
        }
    }
    free(dict[0]);
    free(dict);
}

// Given a hash and a plaintext guess, return 1 if
// the hash of the guess matches the given hash.
// That is, return 1 if the guess is correct.
int tryguess(char *hash, char *guess)
{
    // Hash the guess using MD5
    char *guesshash = md5(guess, strlen(guess));
    // Compare the two hashes
    if (strcmp(guesshash, hash) == 0) return 1;
    // Free any malloc'd memory
    free(guesshash);
    return 0;
}

// Read in the dictionary file and return the array of strings
// and store the length of the array in size.
// This function is responsible for opening the dictionary file,
// reading from it, building the data structure, and closing the
// file.
char ** read_dictionary(char *filename, int *size)
{
    struct stat info;
    if (stat(filename, &info) == -1) *size = 0;
    else *size = info.st_size;
    
    char *contents = malloc(*size);
    
    FILE *f = fopen(argv[2], "r");
    if (!f)
    {
        printf("can't open %s for reading\n", "fruit.txt");
        exit(1);
    }
    //read entire file into memory all at once
    fread(contents, 1, *size, f);
    fclose(f);
    
    int numpass = 0;
    
    //replace newlines with null characters
    for (int i = 0; i < *size; i++)
    {
        if (contents[i] == '\n')
        {
            numpass++;
            contents[i] = '\0';
        }
    }
    
    char **passwords = malloc(numpass*sizeof(char *));
    
    //fill in each address
    passwords[0] = contents;
    int j = 1;
    
    for (int i = 0; i < *size - 1; i++)
    {
        if (contents[i] == '\0')
        {
            passwords[j] = &contents[i+1];
            j++;
        }
    }
    
    return passwords;
    
    free(passwords);
    free(contents);
}
