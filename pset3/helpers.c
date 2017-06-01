/**
 * helpers.c
 *
 * Computer Science 50
 * Problem Set 3
 *
 * Helper functions for Problem Set 3.
 */
       
#include <cs50.h>

#include "helpers.h"

/**
 * Returns true if value is in array of n values, else false.
 */
bool search(int value, int values[], int n)
{
    int mid,beg=0,end=n-1;
    if(n<0)
       return false;
    else
    {
        while(beg<=end)
        {
            mid=(beg+end)/2;
            if(values[mid]==value)
               return true;
            else if(values[mid]<value)
               beg=mid+1;
            else
               end=mid-1;
        }
        return false;
    }
}

/**
 * Sorts array of n values.
 */
void sort(int values[], int n)
{
    int i,j;
    for(i=0;i<n;i++)
    {
        for(j=0;j<n-1;j++)
        {
            if(values[j]>values[j+1])
            {
                values[j+1]=values[j]+values[j+1];
                values[j]=values[j+1]-values[j];
                values[j+1]=values[j+1]-values[j];
            }
        }
    }
    return;
}
