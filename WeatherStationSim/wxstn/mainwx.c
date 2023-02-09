#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <time.h>
#include "wx.h"

int main (void)
{
    double wdata[WDATASZ];
    //InitializeWeatherStation();
    int dunits = InitializeWeatherStation(wdata);
    time_t now;
    int logged;

    while (1)
    {
     now = time(0);
     UpdateWeatherStation(wdata);
     logged = LogWeatherData("weatherdata.txt", now, dunits, wdata);
     DisplayWeatherData(dunits, now, wdata);
     SpeakWeatherData(dunits, now, wdata);
     msDelay(WXDELAY);
    }
   return 0;
}


 /*  UpdateWeatherStation(); taken out 13.3.2020
        printf("\nEnter m for metric units, b for British units, or q to quit");
        test = getch();
//InitializeWeatherStation();
        if (test == 'b')
        {
        dunits = BRITISH;
        }

        if(test == 'q')
        {
        return 1;
        }
         DisplayWeatherData(dunits);*/
