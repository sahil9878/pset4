#include <stdio.h>
#include <stdint.h>

int main(int argc, char* argv[])
{
    // Ensure proper usage
    if (argc != 2)
    {
        fprintf (stderr,"Error! Usage: ./recover forensic image\n");
        return 1;
    }
    
    // Opening the card file in read mode
    FILE *fp = fopen(argv[1],"r");

    if (fp == NULL)
    {
        fprintf (stderr,"Error! file %s can't be opened\n",argv[1]);
        return 2;
    }
    // Declare a file pointer to place the JPEG's pixel in new file
    FILE* pic = NULL;
    int count = 0;
    // Declaring a buffer of size 512
    unsigned char buffer[512];
 
     // To read 512 blocks in one go and doing it till we get EOF
    while ( fread(buffer, sizeof(unsigned char), 512, fp) == 512) 
    {
        // To detect JPEG's signature
        if ((buffer[0] == 0xff) && 
            (buffer[1] == 0xd8) && 
            (buffer[2] == 0xff) && 
            ((buffer[3] & 0xf0) == 0xe0)) 
            {
            if (count != 0)
            {
                fclose(pic);
            }
            
            // Declaring a string to determmine JPEG's name
            
            char filename[10];
            sprintf(filename, "%03i.jpg", count);
            
            // Increment count by 1 on detecting a JPEG
            
            count++;
            
             // Opening a new file in write mode to place JPEG's pixel
             
            pic = fopen(filename, "w");
            if (pic == NULL) 
            {
                fprintf(stderr,"Error! Can't write in file %s\n",filename);
                return 3;
            }
            
            // Writing to the file 512 bytes in one go
            
            fwrite(buffer, sizeof(unsigned char), 512, pic);
            
    }
    
        }
    // closing file
    fclose(filept);

}