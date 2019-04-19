#include <cs50.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int main(int argc, char *argv[])
{
    int n = atoi(argv[1]);

    // ensure proper usage
    if (argc != 4)
    {
        fprintf(stderr, "Usage: ./resize n infile outfile\n");
        return 1;
    }
    else if (n < 0 || n > 100)
    {
        fprintf(stderr, "Number n must be between a positive integer between 0 and 100");
        return 1;
    }

    // save filenames
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

    // declare outfile BITMAPFILEHEADER and BITMAPINFOHEADER
    BITMAPFILEHEADER new_bf = bf;
    BITMAPINFOHEADER new_bi = bi;

    //modifyBITMAPINFOHEADER & BITMAPFILEHEADER
    new_bi.biWidth = bi.biWidth * n;
    new_bi.biHeight = bi.biHeight * n;

    // determine padding for scanlines in original file & new file
    int padding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    int new_padding = (4 - (new_bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    //modifyBITMAPINFOHEADER & BITMAPFILEHEADER
    new_bi.biSizeImage = abs(new_bi.biHeight) * ((sizeof(RGBTRIPLE) * new_bi.biWidth) + new_padding);
    new_bf.bfSize = new_bi.biSizeImage + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);


    // write outfile's BITMAPFILEHEADER
    fwrite(&new_bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&new_bi, sizeof(BITMAPINFOHEADER), 1, outptr);

    // iterate over infile's scanlines
    for (int i = 0, biHeight = abs(bi.biHeight); i < biHeight; i++)
    {
            //array will be pixel length of new width
            RGBTRIPLE row[new_bi.biWidth];
            int x = 0;

            // iterate over pixels in scanline
            for (int j = 0; j < bi.biWidth; j++)
            {
                // temporary storage
                RGBTRIPLE triple;

                // read RGB triple from infile and write to array
                fread(&triple, sizeof(RGBTRIPLE), 1, inptr);
                for (int k = 0; k < n; k++)
                {
                    row[x] = triple;
                    x++;
                }
            }

            // write the row to outfile n times
            for (int l = 0; l < n; l++)
            {
                int num_pix = sizeof(row)/sizeof(row[0]);
                //for (int m = 0; m < num_pix; m++)
                //{
                    fwrite(&row, sizeof(RGBTRIPLE), num_pix, outptr);
                //}
                //write outfile new_padding
                for (int p = 0; p < new_padding; p++)
                {
                    fputc(0x00, outptr);
                }
            }

            // skip over infile padding, if any
            fseek(inptr, padding, SEEK_CUR);
    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    return 0;
}