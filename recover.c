#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

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
    //Define BYTE
    //typedef uint8_t BYTE;
    //Create buffer and allocate memory
    unsigned char *buffer = malloc(251);
    //Declare image variable where new jpegs are written to
    FILE *image = NULL;

    //Scan file for beginning of a JPEG, iterate in bytes of 512
    fread(buffer, 512, 1, image);

        //If found, open new JPEG file and keep writing data to file in 512 byte chunks until next JPEG found

        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            //Close current jpeg file if exists
            if (count > 0)
            {
                fclose(image);
            }

            //Open new JPEG file and write to it in increments of 512
            //Create new file name
            count++;
            sprintf(filename, "%03i.jpg", count);

            //Open the new file
            image = fopen(filename, "w");

            //Write jpeg into the new file
            fwrite(buffer, 512, 1, image);
        }

        //else
        //{
        //    continue;
        //}
        //Close previous JPEG file, start writing next JPEG file
        //Repeat until new JPEG marker or end of file
    //Repeat until end of file reached



    //Close input file and free memory
    fclose(memory);
    fclose(image);
    free(buffer);


}
