/*
 * stall.cpp
 *
 *  Created on: Dec 7, 2013
 *      Author: konlese
 */

#include <stdlib.h>
#include <stdio.h>
#include <strings.h>
#include <string.h>
#include "DataStore.h"
//#include "TraceLog.h"

int main()
{
	char c;
	int i,j=0;
	char password[10];
	char usrname[10];
	memset(usrname,0,10);
	memset(password,0,10);
	DataStore dataStore;
	printf("%s","Content-type:text/html;charset=UTF-8;\n\n");
	if(getenv("CONTENT_LENGTH"))
	{
		int n=atoi(getenv("CONTENT_LENGTH"));
		for (i=0; i<n;i++)
		{
			c=getchar();
			switch (c)
			{
				case '=':
					j=1;
					break;
				default:
					if(j==0)
					{
						usrname[i]=c;
					}
					else
					{
						password[j-1]=c;
						j++;
					}
					break;
			}
		}
		if(dataStore.QueryPermissions(usrname, password))
		{
			printf("%s",dataStore.permUser.strFactory.c_str());
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





