#include<stdio.h>
#include<string.h>
#include<stdlib.h>
char str1[],str2[],result[],dest[],src[];
int length();
int copy();
int concat();
int compare();
int main()
{
    int choice;
    while(choice)
    {
        printf("1:length\n");
        printf("2:copy\n");
        printf("3:concat\n");
        printf("4:compare\n");
        scanf("%d",&choice);
        switch(choice)
        {
            case 1:length();
                   break;
            case 2:copy();
                   break;
            case 3:concat();
                   break;
            case 4:compare();
                   break;
            case 5:exit(0);
                   break;
            default:printf("invalid choice\n");
                    break;                                   
        }
    }
    return 0;
}
int length(char str[])
{
    int i=0;
    while(str[i]!='\0')
    i++;
    return 1;

}
int copy(char dest[],char src[])
{
int i=0;
while(src[i]!='\0')
{
    dest[i]=src[i];
    i++;
}
dest[i]='\0';
}
int concat(char result[],char str1[],char str2[])
{
    int i=0;j=0;
    while(str1[i]!='\0')
    {
        result[i]=str1[i];
        i++;   
    }
    while(str2[j]!='\0')
    {
        result[i]=str2[j];
        i++;
        j++;
    }
    result[i]='\0';
}
int compare(char str1[],char str2[])
{
    int i=0;
    while(str1[i]!='\0' && str2[i]='\0')
    {
        if(str1[i]!=str2[i])
        i++;
    }
    if(str1[i]=='\0' && str2[i]=='\0')
    return 0;
    else 
    return 1;
}
