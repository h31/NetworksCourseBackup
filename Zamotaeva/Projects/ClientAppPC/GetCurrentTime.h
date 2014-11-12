#ifndef _GETCURRENTTIME_H
#define _GETCURRENTTIME_H
#include <time.h>

char* getCurrentTime()
{
	time_t rawtime;
	struct tm * timeinfo;

	time ( &rawtime );
	timeinfo = localtime ( &rawtime );
	char * time = asctime (timeinfo);

	for(int i = 0;i<strlen(time);i++)
	{
		if(time[i] == ' '||time[i] == '\n')
		{
			time[i] = '_';
		}
	}
	return time;
}
#endif