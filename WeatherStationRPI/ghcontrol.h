/** GH control constants, structures, function prototypes for Gh control code
 * @file ghcontrol.h
 */

#ifndef GHCONTROL_H
#define GHCONTROL_H

//Includes

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "pisensehat.h"

//Contants

#define ON 1
#define OFF 0
#define STEMP 25.0
#define SHUMID 55.0
#define SIMULATE 1
#define USTEMP 50
#define LSTEMP -10
#define USHUMID 100
#define LSHUMID 0
#define USPRESS 1016
#define LSPRESS 975
#define GHUPDATE 2000
#define SENSORS 3
#define TEMPERATURE 0
#define HUMIDITY 1
#define PRESSURE 2
#define SIMTEMPERATURE 1
#define SIMHUMIDITY 1
#define SIMPRESSURE 1
#define NUMBARS 8
#define NUMPTS 8.0
#define TBAR 7
#define HBAR 5
#define PBAR 3
#define SENSEHAT 1
#define NALARMS 7
#define UPPERATEMP 30
#define LOWERATEMP 10
#define LOWERAHUMID 25
#define UPPERAHUMID 70
#define LOWERAPRESS 985
#define UPPERAPRESS 1016

// Enumerated Types
typedef enum { NOALARM,HTEMP,LTEMP,HHUMID,LHUMID,HPRESS,LPRESS } alarm_e;

//Data Structures

struct readings
{
	time_t rtime;
	double temperature;
	double humidity;
	double pressure;
};

struct setpoints
{
	double temperature;
	double humidity;
};

struct controls
{
	int heater;
	int humidifier;
};

typedef struct alarmlimits
{
    double hight;
    double lowt;
    double highh;
    double lowh;
    double highp;
    double lowp;
}alarmlimit_s;

typedef struct alarms
{
    alarm_e code;
    time_t atime;
    double value;
}alarm_s;

///@cond INTERNAL
//Function Prototypes

void GhDisplayHeader(const char * sname);
int GhGetRandom(int range);
void GhDelay(int milliseconds);
void GhControllerInit(void);
void GhDisplayControls(struct controls ctrl);
void GhDisplayReadings(struct readings rdata);
void GhDisplaySetpoints(void);
void GhDisplayTargets(struct setpoints spts);
void GhDisplayAll(struct readings rd,struct setpoints sd);
struct controls GhSetControls(struct setpoints target, struct  readings rdata);
struct setpoints GhSetTargets(struct setpoints spts);
double GhGetHumidity(void);
double GhGetPressure(void);
double GhGetTemperature(void);
struct readings GhGetReadings(void);
int GhLogData(char * fname, struct readings ghdata);
int GhSaveSetpoints(char * fname, struct setpoints spts);
struct setpoints GhRetrieveSetpoints(char *fname);
int ShSetVerticalBar(int bar, fbpixel_s px, uint8_t value);
alarmlimit_s GhSetAlarmsLimits(void);
void GhSetAlarms(alarm_s calarm[NALARMS],alarmlimit_s alarmpt, struct readings rdata);
alarm_s GhDisplayAlarms(alarm_s alrm[NALARMS]);

///@endcond

#endif



