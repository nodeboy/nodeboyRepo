/*
 * DataStore.cpp
 *
 *  Created on: Nov 23, 2012
 *      Author: mxx
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "DataStore.h"

bool DataStore::bConnected = false;
MYSQL DataStore::mysql;

DataStore::DataStore()
{
}

DataStore::~DataStore()
{

}

void DataStore::CloseDB(void)
{
	if (bConnected)
	{
		mysql_close(&mysql);
		bConnected = false;
	}
}

bool DataStore::connectDB(void)
{
	if (!bConnected)
	{
		mysql_init(&mysql);
		bConnected = (mysql_real_connect(&mysql, "localhost", "root", "lyq850114",
				"imformation", 3306, NULL, 0) != NULL);
		if (!bConnected)
		{
		//	printf("connect mysql error:%d!\n", mysql_errno(&mysql));
		}
		else
			mysql_autocommit(&mysql, 1);
	//	printf("connect %s!\n", bConnected ? "OK" : "FAIL");
	}
	return bConnected;
}

bool DataStore::QueryPermissions(const char* szUsr, const char* szPwd,char table)
{
	//printf("User:%s!Pwd:%s!\n", szUsr, szPwd);
	permUser.strUser = "FALSE";
	permUser.strPwd = "FALSE";
	permUser.strFactory = "FALSE";
	string strSQL;
	if (!bConnected)
	{
		connectDB();
		if (!bConnected)
		{
			return false;
		}
	}
	string strUsr = szUsr;
	string strPwd = szPwd;
	if(table)
	{
		strSQL = "SELECT * FROM `testtable` WHERE `carb` LIKE '";

	}
	else
	{
		strSQL = "SELECT * FROM `usrtable` WHERE `usr` LIKE '";
	}
	strSQL.append(szUsr);
	strSQL.append("' ");
	//printf("Query:%s!\n", strSQL.c_str());

	MYSQL_RES *result;
	MYSQL_ROW row;
	mysql_set_character_set(&mysql, "utf8");
	if (mysql_query(&mysql, strSQL.c_str()) != 0)
	{
		printf("areyouko");
	//	printf("Query fail!Usr Fac:%s!\n", permUser.strFactory.c_str());
		return false;
	}
	result = mysql_store_result(&mysql);
	int i;
	while (row = mysql_fetch_row(result))
	{
		for (i = 0; i < mysql_num_fields(result); i++)
		{
			if (i == 0)
			{

				if(table)
				{
					carData.strPlateNo = row[i];
				}
				else
				{
					permUser.strUser = row[i];
				}

			}
			else if (i == 1)
			{
				if(table)
				{
					carData.iDevID = row[i];
				}
				else
				{
					if (strcmp(row[i], szPwd))
					{
						break;
					}
					permUser.strPwd = row[i];
				}
			}
			else if (i == 2)
			{
				if(table)
				{
					carData.strDriver = row[i];
				}
				else
				{
					permUser.strFactory = row[i];
				}
			}
			else if(i==3)
			{
				if(table)
				{
					carData.timeInstallTime = row[i];
				}
			}
		}


	}
	if((table==0)&&(strcmp(permUser.strFactory.c_str(), "FALSE")==0))
	{
		CloseDB();
		return false;

	}
	mysql_free_result(result);
	CloseDB();
	return true;
}

bool DataStore::QueryCarData(const char* szSQL)
{
	printf("Query:%s!\n", szSQL);
	MYSQL_RES *result;
	MYSQL_ROW row;
	mysql_set_character_set(&mysql, "utf8");
	if (mysql_query(&mysql, szSQL) != 0)
	{
		printf("Query fail!%s!\n", szSQL);
		return false;
	}
	result = mysql_store_result(&mysql);
	int i;
	while (row = mysql_fetch_row(result))
	{
		for (i = 0; i < mysql_num_fields(result); i++)
		{
			printf("%s ", row[i]);
		}
	}

	mysql_free_result(result);
	return true;
}

void DataStore::InsertData(CarData& car)
{
	mysql_set_character_set(&mysql, "utf8");
	string sql =
			"INSERT INTO `VehicleInfo`(`GpsDevID`, `PlateNo`, `EngineNo`, `DriverName`, `Unit`,`ContactInfo`,`DetectionFactory`,`AnnualRecords`,`MaintenanceRecords`,`InstallTime`,`SerStartTime`, `ServerRecTime`) VALUES (";
	sql += "'" + car.iDevID + "'" + ",";
	sql += "'" + car.strPlateNo + "'" + ",";
	sql += "'" + car.strEngineNo + "'" + ",";
	sql += "'" + car.strDriver + "'" + ",";
	sql += "'" + car.strUnit + "'" + ",";
	sql += "'" + car.strContactInfo + "'" + ",";
	sql += "'" + car.strDetectionFactory + "'" + ",";
	sql += "'" + car.strAnnualRecords + "'" + ",";
	sql += "'" + car.strMaintenanceRecords + "'" + ",";

	char buf[64];
	//sprintf(buf, "%lld", car.timeInstallTime);
	sql += "'";
	sql += buf;
	sql += "',";
	buf[0] = '\0';
	sprintf(buf, "%lld", car.timeSerStartTime);
	sql += "'";
	sql += buf;
	sql += "',";

	sql += "CURRENT_TIMESTAMP);";
	printf("%s", sql.c_str());
	if (mysql_query(&mysql, sql.c_str()))
	{
		printf("mysql error:%s", mysql_error(&mysql));
		throw exception();
	}
	mysql_commit(&mysql);
	printf("committed");
}

bool DataStore::IsConnect(void)
{
	return bConnected;
}
