#include <stdio.h>
#include <stdlib.h>
#include <cs50.h>
#include <stdint.h>

typedef uint8_t  BYTE;

int main(int argc, char *argv[])
{
    // check for correct number of arguments
    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./recover image\n");
        return 1;
    }
    char *infile = argv[1];

    // open infile
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    // set buffer size for 512
    // This wasn't allowing sizeof(buff) to be 512, so that was fun; it was 8
    BYTE *buff = malloc(sizeof(BYTE) * 512);

    int fstarted = 0;
    int filenum = 0;

    // create initial filename to be used
    char filename[8];
    sprintf(filename, "%03d.jpg", filenum);
    filenum++;

    FILE *outptr = fopen(filename, "wb");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", filename);
        return 3;
    }
    // Loop until the buffer no longer reads a full 512 bytes
    while (fread(buff, 1, 512, inptr) == 512)
    {
        // check and see if this is the start of our jpeg
        if (buff[0] == 0xff &&
            buff[1] == 0xd8 &&
            buff[2] == 0xff &&
            (buff[3] & 0xf0) == 0xe0)
        {
            // first time through where file exists but hasn't started
            if (fstarted == 0)
            {
                fwrite(buff, 512, 1, outptr);
                fstarted = 1;
            }
            // if a file has started writing, end it here and start a new one
            else if (fstarted == 1)
            {
                // close current file and start new file
                fclose(outptr);

                // lets create the file, and start writing
                fstarted = 1;

                // dynamically create our new file name
                sprintf(filename, "%03d.jpg", filenum);

                outptr = fopen(filename, "wb");
                if (outptr == NULL)
                {
                    fclose(inptr);
                    fprintf(stderr, "Could not create %s.\n", filename);
                    return 3;
                }
                fwrite(buff, 512, 1, outptr);
                filenum++;
            }
        }
        else
        {
            // keep plugging away with current file
            if (fstarted == 1)
            {
                // keep writing
                fwrite(buff, 512, 1, outptr);
            }
        }
    }

    // one last write for the remainder characters
    int last = fread(buff, 1, 512, inptr);
    fwrite(buff, last, 1, outptr);

    free(buff);
    fclose(outptr);
    fclose(inptr);

    return 0;
}
