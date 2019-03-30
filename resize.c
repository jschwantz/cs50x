// Copies a BMP file

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "bmp.h"

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        fprintf(stderr, "Usage: ./resize f infile outfile\n");
        return 1;
    }

    if (atof(argv[1]) == 0)
    {
        fprintf(stderr, "Usage: ./resize f infile outfile\n");
        return 1;
    }

    float f = atof(argv[1]);

    if (f < 0 || f > 100)
    {
        fprintf(stderr, "Usage: ./resize f infile outfile\n");
        return 1;
    }

    if (f > 0 && f < 1)
    {
    //    f = 1/(round(1/f));
    }
    else
    {
        f = round(f);
    }

    // remember filenames
    char *infile = argv[2];
    char *outfile = argv[3];

    // open input file
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 ||
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }

    //fprintf(stderr, "%f\n", f);
    //fprintf(stderr, "%u\n", bi.biSizeImage);
    //fprintf(stderr, "%u\n", bi.biWidth);
    //fprintf(stderr, "%u\n", abs(bi.biHeight));

    // determine padding for scanlines
    int oldpad = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    // Need to update outfile's BITMAPINFOHEADER
    long oldwidth = bi.biWidth;
    bi.biWidth *= f;
    long newwidth = bi.biWidth;

    double fdr = (double)(1/f);
    long newheight;

    if (f < 1)
    {
        // Doing rounding here to get a good width that rounds with 2 well; don't need if you're not doing anything besides .5
        //bi.biWidth += (bi.biWidth % 2);
        newheight = abs(bi.biHeight) * f;
        //f = (float)bi.biWidth / oldwidth;
    }
    long oldheight = abs(bi.biHeight);
    bi.biHeight *= f;
    long oldsize = bi.biSizeImage;

    // determine padding for resized image
    int newpad = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    // Separate padding to get the size of the answer we're looking for, not even sized image
    int nrpad = (4 - (newwidth * sizeof(RGBTRIPLE)) % 4) % 4;

    if (f >= 1)
    {
        bi.biSizeImage = (bi.biWidth * sizeof(RGBTRIPLE) + newpad) * abs(bi.biHeight);
    }
    else
    {
        bi.biSizeImage = (newwidth * sizeof(RGBTRIPLE) + nrpad) * newheight;
    }

    // Need to update outfile's BITMAPFILEHEADER
    bf.bfSize = sizeof(bf) + sizeof(bi) + bi.biSizeImage;

    // write outfile's BITMAPFILEHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);


    // iterate over infile's scanlines; iterate over old height
    for (int i = 0, h = 0, biHeight = abs(bi.biHeight); i < biHeight; i++)
    {
        // iterate over pixels in scanline
        for (int j = 0; j < oldwidth; j++)
        {
            // temporary storage
            RGBTRIPLE triple;

            // read RGB triple from infile
            fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

            // write RGB triple to outfile; this will only run for f > 1
            if (f >= 1)
            {
                for (int z = 0; z < f; z++)
                {
                    fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
                }
            }

            // if less than 1, only print appropriate characters
            else if (f < 1 && (fmodf((j+1),fdr) > 0))
            {
                fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);

            }
        }

        // skip over padding, if any
        fseek(inptr, oldpad, SEEK_CUR);

        for (int k = 0; k < newpad; k++)
        {
            fputc(0x00, outptr);
        }

        // for size increase, go back and pull the same line again
        if (f > 1)
        {
            float chk = fmodf(i+1,f);
            if (chk > 0 || (i == 0))
            {
                //fprintf(stderr,"%i, go back\n",i);
                fseek(inptr, -(oldwidth*sizeof(RGBTRIPLE) + oldpad), SEEK_CUR);
            }
        }
        // for size decrease, jump forward a line
        else if (f < 1)
        {
            if (fmodf((h+1),fdr) > 0)
            {
                //fprintf(stderr,"%i, go forward\n",i);
                fseek(inptr, (oldwidth*sizeof(RGBTRIPLE) + oldpad), SEEK_CUR);
                h++;
            }
        }
    h++;
    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;
}
