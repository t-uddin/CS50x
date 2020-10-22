#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{

    //Ensure valid input
    if (argc != 2)
    {
        printf("Usage: ./recover image\n");
        return 1;
    }

    //Open input file
    FILE *memory = fopen(argv[1], "r");
    if (argv[1] == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", argv[1]);
        return 1;
    }


    //Declare counter variable
    int count = 0;

    //Declare filenam variable - ensure enough memory/characters for full name
    char filename[7];

    //Create buffer and allocate memory
    unsigned char *buffer = malloc(251);

    //Declare image variable where new jpegs are written to
    FILE *recovered_image = NULL;

    //Scan file for beginning of a JPEG, iterate in bytes of 512
    while (fread(buffer, 512, 1, memory))
    {

        //If found, open new JPEG file and keep writing data to file in 512 byte chunks until next JPEG found

        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            //Close current jpeg file if exists
            if (count > 0)
            {
                fclose(recovered_image);
            }

            //Open new JPEG file and write to it in increments of 512
            //Create new file name
            sprintf(filename, "%03i.jpg", count);

            //Open the new file
            recovered_image = fopen(filename, "w");

            if (recovered_image == NULL)
            {
                free(buffer);
                return 3;
            }
            else
            {
                //Write jpeg into the new file
                fwrite(buffer, 512, 1, recovered_image);
            }

            count++;

        }

        else
        {
            fwrite(buffer, 512, 1, recovered_image);
        }

    }

    //Close input file and free memory
    fclose(memory);
    free(buffer);

}
