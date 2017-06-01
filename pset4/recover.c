/**
 * recover.c
 *
 * Computer Science 50
 * Problem Set 4
 *
 * Recovers JPEGs from a forensic image.
 */

#include<stdio.h>

int main()
{
    //opened files
    FILE* file=fopen("card.raw","r");
    FILE* jpeg=NULL;
    int counter=0;
    unsigned char buffer[512];
    char name[8];
    
    if(file==NULL)
    {
        printf("File could not be opened");
        fclose(file);
        return 1;
    }    
        while(fread(&buffer,sizeof(buffer),1,file)==1)
        {
            //to check if this is the beginning of a jpeg
            if(buffer[0]==0xff&&buffer[1]==0xd8&&buffer[2]==0xff&&(buffer[3]==0xe0||buffer[3]==0xe1))
            {
                //close previous jpeg, if it existed
                if(counter!=0)
                   fclose(jpeg);
                sprintf(name,"%03d.jpg",counter);   
                jpeg=fopen(name,"a");
                counter++;
                // even if it is the beginning, write it into the current file opened
                fwrite(buffer,sizeof(buffer),1,jpeg);
            }
            else 
            {
                // if it is not the beginning, still write it in the current image file, if it exists
                if(counter!=0)
                   fwrite(buffer,sizeof(buffer),1,jpeg);
            }
        }
        //will close the jpeg file, even if it existed 
        if(counter!=0)
           fclose(jpeg);
        fclose(file);
    return 0;
}
