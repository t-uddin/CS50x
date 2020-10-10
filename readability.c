#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <math.h>


int main(void)
{
    //Prompt reader for text
    string text = get_string("Text: ");
    
    //Count No. of letters, words and sentences in text
    float letters = 0;
    float words = 1;
    float sentences = 0;
    
    for (int i = 0, n = strlen(text); i < n; i++)
    { 
        //letters
        if ((text[i] >= 'a' && text[i] <= 'z') || (text[i] >= 'A' && text[i] <= 'Z' ))
        letters++;
        
        //words
        else if (text[i] == ' ')
        words++;
        
        //sentences
        else if (text[i] == '.' || text[i] == '?' || text [i] == '!')
        sentences++;
    }
    
    //printf("%i letter(s)\n%i word(s)\n%i sentence(s)\n", letters, words, sentences);
    
    //Calculate coleman-Liau index
    float L = (letters / words) * 100;
    float S = (sentences / words) * 100;
    
    float index = 0.0588 * L - 0.296 * S - 15.8;

    //Round Coleman-Liau formula result to nearest integer
    int grade = round(index);

    //Output readability grade
    if (grade >= 16)
    {
        printf("Grade 16+\n");
    }
    
    else if (grade <1)
    {
        printf("Before Grade 1\n");
    }
    
    else
    {
        printf("Grade %i\n", grade);
    }

}