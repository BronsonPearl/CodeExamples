/** main program structure and function call order for Gh controller
 * @file GHC.c
 */
#include "ghcontrol.h"

int main(void)
{
	struct setpoints sets = {0};
	struct readings creadings ={0};
	struct controls ctrl = {0};
	time_t now;
	char test;
	int logged;
	alarmlimit_s alimits = { 0 };
    alarm_s warn[NALARMS] = { 0 };


	time(NULL);

	sets = GhSetTargets(sets);
	alimits = GhSetAlarmsLimits();

	GhControllerInit();

	while(1)
	{
		now = time(NULL);

		creadings = GhGetReadings();
		logged = GhLogData("ghdata.txt",creadings);
		ctrl = GhSetControls(sets,creadings);
        GhDisplayAlarms(warn);
		GhDisplayAll(creadings,sets);
		GhDisplayReadings(creadings);
		GhDisplayTargets(sets);
		GhDisplayControls(ctrl);
		GhDelay(GHUPDATE);

	}

	test =  fgetc(stdin);

	if(test =='')
	{
		return EXIT_SUCCESS;
	}

}

