#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{

    //Ensure valid input
    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./recover image\n");
        return 1;
    }

    //Open input file
    FILE *memory = fopen(argv[1], "r");
    if (memory == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", argv[1]);
        return 1;
    }


    //Declare counter variable
    int count = 0;

    //Declare filenam variable - ensure enough memory/characters for full name
    char filename[8];

    //Create buffer and allocate memory
    unsigned char *buffer = malloc(512);

    //Declare image variable where new jpegs are written to
    FILE *recovered_image;

    //Scan file for beginning of a JPEG, iterate in bytes of 512
    while (fread(buffer, 512, 1, memory))
    {

        //If found, open new JPEG file and keep writing data to file in 512 byte chunks until next JPEG found

        if (buffer[0] == 0xff &&
            buffer[1] == 0xd8 &&
            buffer[2] == 0xff &&
            (buffer[3] & 0xf0) == 0xe0)
        
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
                fclose(memory);
                return 3;
            }

            //Write jpeg into the new file
            //fwrite(buffer, 512, 1, recovered_image);

            count++;

        }


        fwrite(buffer, 512, 1, recovered_image);
    }

    //Close files and free memory
    fclose(memory);
    //fclose(recovered_image);
    free(buffer);
    return 0;

}
