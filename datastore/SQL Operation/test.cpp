/*
 * test.cpp
 *
 *  Created on: Dec 7, 2013
 *      Author: konlese
 */

#include <stdlib.h>
#include <stdio.h>
#include <strings.h>
#include <string.h>
#include "DataStore.h"

int main()
{
	char c;
	int i,j=0;
	char carnb[20];
	memset(carnb,0,20);
	DataStore dataStore;
	printf("%s","Content-type:text/html;charset=UTF-8;\n\n");
	if(getenv("CONTENT_LENGTH"))
	{
		int n=atoi(getenv("CONTENT_LENGTH"));
		for (i=0;i<n;i++)
		{
			c=getchar();
			carnb[i]=c;


		}
		if(dataStore.QueryPermissions(carnb, "abc",1))
		{
			printf("%s,",dataStore.carData.strDriver.c_str());
			printf("%s,",dataStore.carData.iDevID.c_str());
			printf("%s,",dataStore.carData.strPlateNo.c_str());
			printf("%s",dataStore.carData.timeInstallTime.c_str());
			//printf("%s","FLASE");
		}
		else
		{
			printf("%s","FLASE");
		}
	}
	else
	{
		printf("%s","FLASE");
	}
}




