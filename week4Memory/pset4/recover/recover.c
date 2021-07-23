#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    // Check for invalid usage
    if (argc != 2)
    {
        printf("Usage: ./recover image\n");
        return 1;
    }

    // Open input file
    FILE *infile = fopen(argv[argc - 1], "r");
    if (infile == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", argv[argc - 1]);
        return 1;
    }

    //count the numer of jpg have found
    int count = 0;

    //use to create a new jpg file
    FILE *oufile = NULL;

    //store the block
    BYTE bf[512];

    while (fread(&bf, sizeof(BYTE), 512, infile))
    {
        //determine if the block is a new jpg file or not
        if (bf[0] == 0xff && bf[1] == 0xd8 && bf[2] == 0xff && (bf[3] & 0xf0) == 0xe0)
        {
            // if this block is a new jpg, close the previous jpg file
            if (count != 0)
            {
                fclose(oufile);
            }

            //store the jpg file name
            char *name = malloc(8);
            sprintf(name, "%03i.jpg", count);

            oufile = fopen(name, "w");
            count++;
            free(name);
        }
        //if new block is not a new jpg file, continue to write the opened jpg file
        if (count != 0)
        {
            fwrite(&bf, sizeof(BYTE), 512, oufile);
        }
    }

    //close file
    fclose(oufile);
    fclose(infile);

    return 0;

}