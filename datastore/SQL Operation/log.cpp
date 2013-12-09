/*
 * DataStore_test.cpp
 *
 *  Created on: Nov 23, 2012
 *      Author: mxx
 */

#include <stdlib.h>
#include <stdio.h>
#include <strings.h>
#include <string.h>
#include "DataStore.h"
//#include "TraceLog.h"

int main()
{
#if 1
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
		if(dataStore.QueryPermissions(usrname, password,0))
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
#endif
#if 0
	DataStore dataStore;
	printf("test normal!\n");
//	dataStore.QueryPermissions("admin1", "88888888");
	//dataStore.QueryPermissions("admin2", "88888888");
	if(dataStore.QueryPermissions("admin1", "88888888"))
	{
		printf("%s",dataStore.permUser.strFactory.c_str());
	}
	printf("\ntest user error!\n");
	dataStore.QueryPermissions("admin4", "88888888");
	printf("\ntest pwd error!\n");
	dataStore.QueryPermissions("admin1", "66666666");
	return 0;
#endif
}
