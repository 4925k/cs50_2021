#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


int main(int argc, char *argv[])
{
    //take only one command line argument
    if (argc != 2)
    {
        printf("Usage: ./recover <image>");
        return 1;
    }


    //open file and check if it NULL or not
    FILE *input = fopen(argv[1], "r");
    if (input == NULL)
    {
        printf("Could not open image");
        return 1;
    }


    FILE *img;
    unsigned char data[512];             //to read data into
    char fileName[8];           //to store imageName
    int fileCount = 0;          //keep count of image number
    bool fileOpen = false;      //to see if file is open or not

    //start reading image
    while (fread(&data, sizeof(int), 1, input))
    {
        //if start of a new jpeg
        if (data[0] == 0xff && data[1] == 0xd8 && data[2] == 255 && ((data[3] & 0xfe) == 0xe0))
        {
            //if file is already open, close it.
            if (fileOpen)
            {
                fclose(img);
                fileOpen = false;    //change file open to false
            }
            sprintf(fileName, "%03i.jpg", fileCount);
            img = fopen(fileName, "w");
            fwrite(&data, sizeof(int), 1, img);
            fileOpen = true;
            fileCount++;
        }
        else
        {
            //write data to file is fileOpen is true.
            if (fileOpen)
            {
                fwrite(&data, sizeof(int), 1, img);
            }
        }
    }

    fclose(input);
    fclose(img);

}