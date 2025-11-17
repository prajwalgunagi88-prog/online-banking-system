#include<stdio.h>
#include<stdlib.h>
int main()
{
    int n,i;
    int *arr;
    printf("enter the number of elements\n");
    scanf("%d",&n);
    arr=(int*)malloc(n*sizeof(int));
    if(arr==NULL)
    {
        printf("memory not allocated");
        return 1;
    }
    printf("enter the array elements are \n");
    for(i=0;i<n;i++)
    scanf("%d",&arr[i]);
    printf("\n array elements are:\t");
    for(i=0;i<n;i++)
    printf("%d",arr[i]);
    printf("\n");
    free(arr);
    return 0;
}