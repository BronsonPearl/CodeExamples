
#ifndef WX_H_INCLUDED
#define WX_H_INCLUDED

// Constants
#define KMTOMPH 	0.62137119
#define KPATOINHG 	0.2953
#define METRIC 0
#define BRITISH 1

#define UPPERT 50
#define LOWERT -50
#define UPPERH 100
#define LOWERH 0
#define UPPERP 115
#define LOWERP 85
#define UPPERWS 130
#define LOWERWS 0
#define UPPERWD 360
#define LOWERWD 0

#define WXDELAY 5000

#define TEMPERATURE 0
#define HUMIDITY 1
#define PRESSURE 2
#define WINDSPEED 3
#define WINDDIRECTION 4
#define WDATASZ 5

#define TDATASZ 26
#define TSTRINGSZ  80

//function prototype
void UpdateWeatherStation(double wreadings[WDATASZ]);
int InitializeWeatherStation(double wd[WDATASZ]);
void DisplayWeatherData(int units,time_t wtime, double wreads[WDATASZ]);
double GetTemperature(void);
double GetHumidity(void);
double GetPressure(void);
double GetWindspeed(void);
double GetWindDirection(void);
int GetRandom(int range);
void msDelay(int ms);
int LogWeatherData(char * fname, time_t wtime, int units, double wreads[WDATASZ]);
int SpeakWeatherData(int units, time_t wtime, double wreads[WDATASZ]);

#endif // WX_H_INCLUDED
