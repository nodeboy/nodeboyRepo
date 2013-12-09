/*
 * DataStore.h
 *
 *  Created on: Nov 23, 2012
 *      Author: mxx
 */

#ifndef DATASTORE_H_
#define DATASTORE_H_
#include <mysql/mysql.h>
#include <string>
using namespace std;

class DataStore
{
public:

	typedef struct
	{
		string strUser;
		string strPwd;
		string strFactory;
	} Permissions;

	typedef struct
	{
		string iDevID;
		string strPlateNo;
		string strEngineNo;
		string strDriver;
		string strUnit;
		string strContactInfo;
		string strDetectionFactory;
		string strAnnualRecords;
		string strMaintenanceRecords;
		string timeInstallTime;
		long long timeSerStartTime;
	} CarData;

	DataStore();
	virtual ~DataStore();
	static void CloseDB(void);
	bool IsConnect(void);
	bool QueryCarData(const char* szSQL);
	bool QueryPermissions(const char* szUsr, const char* szPwd,char table);
	void InsertData(CarData& car);

	Permissions permUser;
	CarData carData;
protected:
	static MYSQL mysql;
	static bool bConnected;
	static bool connectDB(void);

};

#endif /* DATASTORE_H_ */
