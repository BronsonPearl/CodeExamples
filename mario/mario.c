#include <stdio.h>
#include <cs50.h>




int main(void)
{

    // init variables
    int lines = 0;
    char brick = '#';
    int currentLine = 0;

    //prompts for input until number entered is 1-8
    while (true)
    {
        lines =  get_int("height: ");

        if ((lines < 9) && (lines > 0))
        {
            break;
        }
    }

    for (int i = 0; i < lines; i++)
    {

        //print starting spaces to invert half pyramid
        for (int j = lines - 1; j > i; j--)
        {
            printf(" ");

        }
        //print left half
        for (int z = 0; z <= i; z++)
        {
            printf("%c", brick);
        }

        // print middle gap
        for (int y = 2; y > 0; y--)
        {
            printf(" ");
        }
        // print right half
        for (int x = 0; x <= i; x++)
        {
            printf("%c", brick);
        }

        printf("\n");
    }

    return 0;

}


