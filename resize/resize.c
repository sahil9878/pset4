/**
 *resizes the input file by a given factor
 */
       
#include <stdio.h>
#include <stdlib.h>
#include<ctype.h>

#include "bmp.h"

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        fprintf(stderr, "Usage: ./copy (n) infile outfile\n");
        return 1;
    }

    // remember filenames and factor
    int factor = atoi(argv[1]);
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
     if (factor < 1 || factor > 100)
    {
        fprintf(stderr, "Error! range of n must be = [1,100]\n ");
        return 4;
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
        return 5;
    }
    
    // preserving original dimensions
    LONG biwidth_in = bi.biWidth;
    LONG biheight_in = bi.biHeight;
    // For infile
    int padding_in = (4 - (biwidth_in * sizeof(RGBTRIPLE)) % 4) % 4;
    
    // Changing dimensions for outfile
    bi.biWidth = factor* bi.biWidth; 
    bi.biHeight =factor * bi.biHeight;
    // determine padding for scanlines
    int padding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    bi.biSizeImage = ((bi.biWidth * sizeof(RGBTRIPLE)) + padding) * abs(bi.biHeight);
    bf.bfSize = bi.biSizeImage + sizeof (BITMAPFILEHEADER) + sizeof (BITMAPINFOHEADER);
    
     // write outfile's BITMAPFILEHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);

    // allocate memory for temp to hold scanline
    RGBTRIPLE *temp = malloc(sizeof(RGBTRIPLE) * (bi.biWidth));
  
    // iterate over infile's scanlines
    for (int i = 0, biHeight = abs(biheight_in); i < biHeight; i++)
    {
        //sets counter to 0 after each row
        int counter = 0;
        // iterate over pixels in scanline
        for (int j = 0; j < biwidth_in; j++)
        {
        
    
            // temporary storage
            RGBTRIPLE triple;

            // read RGB triple from infile
            fread(&triple, sizeof(RGBTRIPLE), 1, inptr);
            //for writing pixel to temp n times
            for(int trp = 0; trp < factor; trp++)
            {
                *( temp + counter ) = triple;
                counter++;
            }
        
        }
        
        // skip over padding, if any
        fseek(inptr, padding_in, SEEK_CUR);

        // write RGB triple to outfile
          for(int trp = 0; trp < factor; trp++)
          {
        fwrite(temp, sizeof(RGBTRIPLE), bi.biWidth, outptr);
        
        // then add  padding back (to demonstrate how)
      
        for (int k = 0; k < padding; k++)
        {
            fputc(0x00, outptr);
        }
          }
    }
  
    // success
    return 0;

}