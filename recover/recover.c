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
    // Declaring a buffer of size 512
    unsigned char buffer[512];
   int count = 0; 
     // To read 512 blocks in one go and doing it till we get EOF
    while ( fread(buffer, sizeof(unsigned char), 512, fp) == 512) 
    {
        // To detect JPEG's signature
        if ((buffer[0] == 0xff) && 
            (buffer[1] == 0xd8) && 
            (buffer[2] == 0xff) && 
            ((buffer[3] & 0xf0) == 0xe0)) 
              count++;  
    }
    {printf("%d jpeg found\n",count);
        }
   

}