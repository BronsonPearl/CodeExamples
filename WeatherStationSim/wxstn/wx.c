#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "wx.h"


void UpdateWeatherStation(double wreadings[WDATASZ])
{
    printf("\nUpdating Weather Station\n");

    wreadings[TEMPERATURE] = GetTemperature();
    wreadings[HUMIDITY] = GetHumidity();
    wreadings[PRESSURE] = GetPressure();
    wreadings[WINDSPEED] = GetWindspeed();
    wreadings[WINDDIRECTION] = GetWindDirection();
}

int InitializeWeatherStation(double wd[WDATASZ])
{
    int iunits, i;
    char test;

    for(i=0; i < WDATASZ; i++)
    {
        wd[i]= 0.0;
    }


	printf("Bronson's Tech104 Weather Station\n\n");
	srand((unsigned)time(NULL));

	// ask which unit they want
	printf("\nEnter m for metric units, b for British units, or q to quit");
    test = getch();

    if (test == 'b')
    {
    iunits = BRITISH;
    }
    if (test == 'm')
    {
    iunits = METRIC;
    }
    else if(test == 'q')
    {
    exit(0);
    }

    return iunits;
}

/*  temperature = 25.1;
    humidity = 55.4;
    pressure = 101.7;
    windspeed = 15.5;
    direction = 320.0;
    taken out 24.2.2020*/

double GetTemperature(void)
{
    return GetRandom(UPPERT-LOWERT)+LOWERT;
}
double GetHumidity(void)
{
    return GetRandom(UPPERH-LOWERH)+LOWERH;
}
double GetPressure(void)
{
    return GetRandom(UPPERP-LOWERP)+LOWERP;
}
double GetWindspeed(void)
{
    return GetRandom(UPPERWS-LOWERWS)+LOWERWS;
}
double GetWindDirection(void)
{
    return GetRandom(UPPERWD-LOWERWD)+LOWERWD;
}

void DisplayWeatherData(int units,time_t wtime, double wreads[WDATASZ])
{
    double /*temperature,humidity,pressure,windspeed,direction,*/ bt, bp, bw;
    double creads[WDATASZ];
    int i;

    printf("\n%s",ctime(&wtime));

    for(i=0; i<WDATASZ; i++)
    {
    creads[i] = wreads[i];
    }

    if(units == METRIC)
    {
    printf("\nTemperature: %4.1lf C\tHumidity: %3.0lf%%\tPressure: %4.1lf kPa\n",creads[TEMPERATURE],creads[HUMIDITY],creads[PRESSURE]);
    printf("Windspeed: %3.0lf km/hr\tDirection: %3.0lf Degrees\n\n",creads[WINDSPEED],creads[WINDDIRECTION]);
    }
    else
    {
    bt = (creads[TEMPERATURE] * 9.0 / 5.0) + 32.0;
    bp = creads[PRESSURE] * KPATOINHG;
    bw = creads[WINDSPEED] * KMTOMPH;
    printf("\nTemperature: %4.1lf F\tHumidity: %3.0lf%%\tPressure: %4.1lf inHg\n",bt,creads[HUMIDITY],bp);
    printf("Windspeed: %3.0lf mph\tDirection: %3.0lf Degrees\n\n",bw,creads[WINDDIRECTION]);
    }

}

int GetRandom(int range)
{
 return rand() % range;
}

void msDelay(int ms)
{
    long pause;
    clock_t now,then;

    pause = ms*(CLOCKS_PER_SEC/1000);
    now = then = clock();
    while( (now-then) < pause )
    {
        now = clock();
    }
}

int LogWeatherData(char * fname, time_t wtime, int units, double wreads[WDATASZ])
{
    FILE *fp;
    char ltime[TDATASZ];

    fp = fopen(fname,"a");

    if(fp==NULL)
    {
        printf("\ncannot open file. Data not retrieved.");
        return 0;
    }

    strcpy(ltime, ctime(&wtime));

    ltime[3] = ',';
    ltime[7] = ',';
    ltime[10]= ',';
    ltime[19]= ',';
    ltime[24] = '\0';

    fprintf(fp,"%.24s,%d,%lf,%lf,%lf,%lf,%lf\n",
			ltime,
	        units,
			wreads[TEMPERATURE],
			wreads[HUMIDITY],
			wreads[PRESSURE],
			wreads[WINDSPEED],
			wreads[WINDDIRECTION] );


    fclose(fp);

    return 1;
}

int SpeakWeatherData(int units, time_t wtime, double wreads[WDATASZ])
{
    FILE *fp;
	char ltime[TSTRINGSZ];
	struct tm *info;

	fp = fopen("wspeak.txt","a");

    if(fp==NULL)
    {
        printf("\ncannot open file. Data not retrieved.");
        return 0;
    }

    info = localtime(&wtime);

    strftime(ltime,TDATASZ,"%A %B %Y %I %M %p", info);

    if(units == METRIC)
    {
     fprintf(fp,"%s temperature %3.1lf degrees celsius humidity %3.0lf per cent pressure %4.1lf kilo pascals windspeed %3.0lf kilometers per hour wind direction %3.0lf degrees\n",
             ltime,
			 wreads[TEMPERATURE],
			 wreads[HUMIDITY],
			 wreads[PRESSURE],
			 wreads[WINDSPEED],
			 wreads[WINDDIRECTION] );
    }

    else
    {
     fprintf(fp,"%s temperature %3.1lf degrees fahrenheit humidity %3.0lf per cent pressure %4.1lf inches of mercury windspeed %3.0lf miles per hour wind direction %3.0lf degrees\n",
            ltime,
			wreads[TEMPERATURE],
			wreads[HUMIDITY],
			wreads[PRESSURE],
			wreads[WINDSPEED],
			wreads[WINDDIRECTION] );
    }

    fclose(fp);

    system("espeak -f wspeak.txt");

    return 1;

}
