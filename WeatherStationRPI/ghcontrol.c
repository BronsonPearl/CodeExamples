/** function definitions and in depth structure for Gh main code
 * @file ghcontrol.c
 */

#include "ghcontrol.h"
#define ALARMNMSZ 18

/**generates a random number within a range
 * @version 01 June 2020
 * @author Paul Moggach
 * @author Bronson Pearl
 * @param range integer mod division to random number
 * @return int
 */
int GhGetRandom(int range)
{
	return rand()%range;
}

/**creates a timed delay for the clock
 * @version 01 June 2020
 * @author Paul Moggach
 * @author Bronson Pearl
 * @param integer delay measured in milliseconds
 * @return void
 */
void GhDelay(int milliseconds)
{
	long int wait;
	clock_t  now,start;

	wait = milliseconds * (CLOCKS_PER_SEC/1000);
	start = clock();
	now = start;

	while( (now-start) < wait)
	{
		now = clock();
	}
}

/** sets parameters by calling srand for number generation, sets targets, and diplays controllers name. initialization of the Ghcontroller
 * @version 06 July 2020
 * @author Paul Moggach
 * @author Bronson Pearl
 * @param void
 * @return void
 */
void GhControllerInit(void)
{
	srand((unsigned)time(NULL));
	GhDisplayHeader("Bronson Pearl");
	#if SENSEHAT
		ShInit();
	#endif
}

/** generates simulated measurement for humidity
 * @version 06 July 2020
 * @author Paul Moggach
 * @author Bronson Pearl
 * @param void
 * @return double
 */
double GhGetHumidity(void)
{

#if SIMTEMPERATURE
	return GhGetRandom(USHUMID-LSHUMID) + LSHUMID;
#else
	return 25.0;
#endif
}

/** generates simulated measurement for wind pressure
 * @version 06 July 2020
 * @author Paul Moggach
 * @author Bronson Pearl
 * @param void
 * @return double
 */
double GhGetPressure(void)
{
#if SIMTEMPERATURE
	return GhGetRandom(USPRESS-LSPRESS) + LSPRESS;
#else
	return 25.0;
#endif
}

/** generates simulated measurement for temperature
 * @version 06 July 2020
 * @author Paul Moggach
 * @author Bronson Pearl
 * @param void
 * @return double
 */
double GhGetTemperature(void)
{
#if SIMTEMPERATURE
	return GhGetRandom(USTEMP-LSTEMP) + LSTEMP;
#else
	return 25.0;
#endif
}

/** assigns simulated measurements to the appropraite array location
 * @version 06 July 2020
 * @author Paul Moggach
 * @author Bronson Pearl
 * @param type double readings[SENSORS]
 * @return void
 */
struct readings GhGetReadings(void)
{
	struct readings now = {0};
	now.rtime = time(NULL);
	now.temperature = GhGetTemperature();
	now.humidity = GhGetHumidity();
	now.pressure = GhGetPressure();
	return now;
}

/** sets contorl points for simulation
 * @version 06 july 2020
 * @author Paul Moggach
 * @author Bronson Pearl
 * @param type void
 * @return cpoints
 */
struct setpoints GhSetTargets(struct setpoints spts)
{
	struct setpoints cpoints = GhRetrieveSetpoints("setpoints.dat");

	if( (cpoints.temperature = 0) )
	{
	cpoints.temperature = STEMP;
	cpoints.humidity = SHUMID;
	GhSaveSetpoints("setpoints.dat", cpoints);
	}
	return cpoints;
}

/** controls the heater and humidifier for green house
 * @version 15 June 2020
 * @author Paul Moggach
 * @author Bronson Pearl
 * @param heater pointer humidifier pointer and creadings array
 * @return void
 */
struct controls GhSetControls(struct setpoints target, struct readings rdata)
{
	struct controls cset = {0};

	if( rdata.temperature < STEMP )
	{
		 cset.heater = ON;
	}
	else
	{
		 cset.heater = OFF;
	}

	if( rdata.humidity < SHUMID)
	{
		 cset.humidifier = ON;
	}
	else
	{
		 cset.humidifier = OFF;
	}

	return cset;
}

/** Writes data to a file to log simulated weather readings
 * @version 13 July 2020
 * @author Paul Moggach
 * @author Bronson Pearl
 * @param fname string with file name, ghdata
 * @return 1
 */
int GhLogData(char * fname, struct readings ghdata)
{
	FILE *fp;
	char ltime[25];

	fp = fopen(fname,"a");

	if(fp==NULL)
	{
	return 0;
	}

	strcpy(ltime, ctime(&ghdata.rtime));
	ltime[3] = ',';
	ltime[7] = ',';
	ltime[10]= ',';
	ltime[19]= ',';


 	fprintf(fp,"%.24s,%5.1lf,%5.1lf,%6.1lf\n", ltime, ghdata.temperature, ghdata.humidity, ghdata.pressure);

	fclose(fp);

	return 1;
}

/* save setpoints to file
 * @version 13 July 2020
 * @author Paul Moggach
 * @author Bronson Pearl
 * @param fname string with file name, setpoint struct
 * @return 1
 */
int GhSaveSetpoints(char * fname, struct setpoints spts)
{
	FILE *fp;
	fp = fopen(fname,"w");

	if(fp==NULL)
	{
	return 0;
	}

	fwrite(&spts,sizeof(struct setpoints),1, fp);

	fclose(fp);

	return 1;
}

/* retrieves file
 * @version 13 July 2020
 * @author Paul Moggach
 * @author Bronson Pearl
 * @param fname string with file name
 * @return spts
 */
struct setpoints GhRetrieveSetpoints(char * fname)
{
	struct setpoints spts = {0.0};

	FILE *fp;

	fp = fopen(fname,"r");

	if(fp==NULL)
	{
	return spts;
	}

	fread(&spts, sizeof(struct setpoints),1,fp);

	fclose(fp);

	return spts;
}

alarmlimit_s GhSetAlarmsLimits(void)
{
    alarmlimit_s calarm;
    calarm.hight = UPPERATEMP;
    calarm.lowt = LOWERATEMP;
    calarm.highh = UPPERAHUMID;
    calarm.lowh = LOWERAHUMID;
    calarm.highp = UPPERAPRESS;
    calarm.lowp = LOWERAPRESS;

    return calarm;
}

void GhSetAlarms(alarm_s calarm[NALARMS],alarmlimit_s alarmpt, struct readings rdata)
{

int i;
for(i=0; i<7; i++)
{
    calarm[i].code = NOALARM;
}

if(rdata.temperature >= alarmpt.hight)
{
    calarm[HTEMP].code = HTEMP;
    calarm[HTEMP].atime = rdata.rtime;
    calarm[HTEMP].value = rdata.temperature;
}

if(rdata.temperature <= alarmpt.lowt)
{
    calarm[LTEMP].code = LTEMP;
    calarm[LTEMP].atime = rdata.rtime;
    calarm[LTEMP].value = rdata.temperature;
}

if(rdata.humidity >= alarmpt.highh)
{
    calarm[HHUMID].code = HHUMID;
    calarm[HHUMID].atime = rdata.rtime;
    calarm[HHUMID].value = rdata.temperature;
}

if(rdata.humidity <= alarmpt.lowh)
{
    calarm[LHUMID].code = LHUMID;
    calarm[LHUMID].atime = rdata.rtime;
    calarm[LHUMID].value = rdata.temperature;
}

if(rdata.pressure >= alarmpt.highp)
{
    calarm[HPRESS].code = HPRESS;
    calarm[HPRESS].atime = rdata.rtime;
    calarm[HPRESS].value = rdata.temperature;
}

if(rdata.pressure <= alarmpt.lowp)
{
    calarm[LPRESS].code = LPRESS;
    calarm[LPRESS].atime = rdata.rtime;
    calarm[LPRESS].value = rdata.temperature;
}

}

alarm_s GhDisplayAlarms(alarm_s alrm[NALARMS])
{
    // Alarm Message Array
const char alarmnames[NALARMS][ALARMNMSZ] =
        {"No Alarms","High Temperature","Low Temperature","High Humidity",
          "Low Humidity","High Pressure","Low Pressure"};

          for(int i = 1; i<7; i++)
          {
            if('->' != NOALARM)
            {
                printf("%s %s", alarmnames['->'],ctime(&alrm[i].atime));
            }
          }

}

//Dispaly functions
/** displays the on and off status of humidifier and heater based on current temperatures compared to target temperatures
 * @version 06 July 2020
 * @author Paul Moggach
 * @author Bronson Pearl
 * @param heater pointer humidifier pointer
 * @return void
 */
void GhDisplayControls(struct controls ctrl)
{
	fprintf(stdout,"\nControls: \theater status: %d\t humidifier status: %d\n",ctrl.heater, ctrl.humidifier);
}

/** prints the readings of units measured: temp, humidity, wind pressure.
 * @version 08 June 2020
 * @author Paul Moggach
 * @author Bronson Pearl
 * @param rtime and array dreads passed to display current time and appropriate measurements
 * @return void
 */
void GhDisplayReadings(struct readings rdata)
{
	fprintf(stdout,"\n%sReadings\tT:%5.lfC\t H:%5.1lf%%\t P:% 6.1lfPa",ctime(&rdata.rtime),rdata.temperature,rdata.humidity,rdata.pressure);
}

/** displays set targets for humdifier and heater
 * @version 15 June 2020
 * @author Paul Moggach
 * @author Bronson Pearl
 * @param void
 * @return void
 */
void GhDisplayTargets(struct setpoints spts)
{
	fprintf(stdout,"\nSetpoints:  T: %5.lfC\t H: %5.lf%%", STEMP, SHUMID);
}

void GhDisplaySetpoints(void)
{
}

/**prints GH controller title
 * @version 06 MAY 2018
 * @author Paul Moggach
 * @author Bronson Pearl
 * @param sname string with Operator's name
 * @return void
 * */

void GhDisplayHeader(const char * sname)
{
	fprintf(stdout, "%s's CENG153 Greenhouse Controller\n",sname);
}

void GhDisplayAll(struct readings rd,struct setpoints sd)
{
	int rv, sv, ach, avl;
    fbpixel_s pxc={0};

    ShClearMatrix();

	rv = (NUMPTS * (((rd.temperature-LSTEMP) / (USTEMP-LSTEMP)) +0.05) -1);
    sv = (NUMPTS * (((sd.temperature - LSTEMP) / (USTEMP-LSTEMP)) +0.05) -1);
        pxc.red = 0x00;
        pxc.green = 0xFF;
        pxc.blue = 0x00;
        ShSetVerticalBar(TBAR,pxc,rv);
        pxc.red = 0xF0;
        pxc.green = 0x0F;
        pxc.blue = 0xF0;
        ShSetPixel(TBAR,sv,pxc);

	rv = (NUMPTS * (((rd.humidity-LSHUMID) / (USHUMID-LSHUMID)) +0.05) -1);
    sv = (NUMPTS * (((sd.humidity - LSHUMID) / (USHUMID-LSHUMID)) +0.05) -1);
        pxc.red = 0x00;
        pxc.green = 0xFF;
        pxc.blue = 0x00;
        ShSetVerticalBar(HBAR,pxc,rv);
        pxc.red = 0xF0;
        pxc.green = 0x0F;
        pxc.blue = 0xF0;
        ShSetPixel(HBAR,sv,pxc);

	rv = (NUMPTS * (((rd.pressure-LSPRESS) / (USPRESS-LSPRESS)) +0.05) -1);
        pxc.red = 0x00;
        pxc.green = 0xFF;
        pxc.blue = 0x00;
        ShSetVerticalBar(PBAR,pxc,rv);


}
