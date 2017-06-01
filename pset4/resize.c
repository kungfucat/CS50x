/**
 * resize.c
 *
 * Computer Science 50
 * Problem Set 4
 *
 * Resizes a BMP piece by piece, just because.
 */
       
#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int main(int argc, char* argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        printf("Usage: ./resize resizing-factor infile outfile\n");
        return 1;
    }

    // remember filenames
    char* infile = argv[2];
    char* outfile = argv[3];
    
    // Remembered factor to resize
    int factor=atoi(argv[1]);
    if(factor<1||factor>100)
    {
        printf(" resizing-factor must be an integer between 1 and 100\n");
        return 1;
    }

    // open input file 
    FILE* inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        printf("Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE* outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf,out_bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi,out_bi;
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
    
    //missed this earlier. Needed to copy all members first and then update
    out_bi=bi;
    out_bf=bf;
    
    //calculated padding of infile, to skip it during the loops
    int inpad= (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    
    //calculated padding of outfile, to add after every row finishes
    int outpad= (4 - (bi.biWidth*factor*sizeof(RGBTRIPLE)) % 4) % 4;
    
    //Updated Outfile's Headers
    out_bi.biWidth=bi.biWidth*factor;
    out_bi.biHeight=bi.biHeight*factor;
    out_bi.biSizeImage=(out_bi.biWidth*sizeof(RGBTRIPLE)+outpad)*abs(out_bi.biHeight);
    out_bf.bfSize=bf.bfSize+(out_bi.biSizeImage-bi.biSizeImage);

    // write outfile's BITMAPFILEHEADER
    fwrite(&out_bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&out_bi, sizeof(BITMAPINFOHEADER), 1, outptr);
    
    // temporary storage
    RGBTRIPLE triple;

    // iterate over infile's scanlines
    for (int i = 0, biHeight = abs(bi.biHeight); i < biHeight; i++)
    {
        for(int j=0;j<factor;j++)
        {
           // iterate over pixels in scanline
           for (int k = 0; k < bi.biWidth; k++)
           {
               // read RGB triple from infile
               fread(&triple, sizeof(RGBTRIPLE), 1, inptr);
               
               for(int h=0;h<factor;h++)
               {
                  // write RGB triple to outfile
                  fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
               }
           }
           
           //insert Padding at the the end
           for (int k = 0; k < outpad; k++)
           {
            fputc(0x00, outptr);
           }
           
           if(j<factor-1)
           {
               fseek(inptr,-bi.biWidth*sizeof(RGBTRIPLE),SEEK_CUR);
           }
      }   
           
           // skip over padding, if any
           fseek(inptr, inpad, SEEK_CUR);
   }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // that's all folks
    return 0;
}
