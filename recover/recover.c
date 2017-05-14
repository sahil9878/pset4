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
}
    