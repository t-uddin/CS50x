#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>


//Prompt user for cipher key in command-line argument

int main(int argc, string argv[])
{

    //Print error message and return 1 if zero or multiple command-line arguments
    if (argc != 2)
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }

    //Iterate over provided argument to check each character is a digit
    bool key_valid = true;
    int len = strlen(argv[1]);
    for (int i = 0; i < len; i++)
    {
        if (isdigit(argv[1][i]) == false)
        {
            printf("Usage: ./caesar key\n");
            key_valid = false;
            return 1;
        }
    }

    //If key valid, convert command-line argument to integer and prompt user for plain text
    int key = 0;
    if (key_valid == true)
    {
        key = atoi(argv[1]);
    }

    //prompt user for plain text
    string text = get_string("plaintext: ");


    //Convert plain text to cypher text character by character
    printf("ciphertext: ");
    int n = strlen(text);

    for (int i = 0; i < n; i++)
    {
        //Rotate and print A-Z characters
        if (text[i] >= 65 && text[i] <= 90)
        {
            int upper = text[i] - 65;
            upper = (upper + key) % 26;
            printf("%c", (char) upper + 65);
        }

        //Rotate and print a-z characters
        else if (text[i] >= 97 && text[i] <= 122)
        {
            int lower = text[i] - 97;
            lower = (lower + key) % 26;
            printf("%c", (char) lower + 97);
        }
        
        //Print non-alphabetic characters as is
        else
        {
            printf("%c", text[i]);
        }
    }

    //Print new line
    printf("\n");
    return 0;

}