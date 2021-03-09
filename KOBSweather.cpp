// File:        KOBSweather.cpp
// Version:     20210308
// Author:      Sifan Kahale
// Desc:        INDI Weather driver for the KOBS allsky station

/*******************************************************************************
  Copyright(c) 2015 Jasem Mutlaq. All rights reserved.

  INDI Weather Underground (TM) Weather Driver

  Modified for OpenWeatherMap API by Jarno Paananen

  This program is free software; you can redistribute it and/or modify it
  under the terms of the GNU General Public License as published by the Free
  Software Foundation; either version 2 of the License, or (at your option)
  any later version.

  This program is distributed in the hope that it will be useful, but WITHOUT
  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
  more details.

  You should have received a copy of the GNU Library General Public License
  along with this library; see the file COPYING.LIB.  If not, write to
  the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
  Boston, MA 02110-1301, USA.

  The full GNU General Public License is included in this distribution in the
  file called LICENSE.
*******************************************************************************/

#include "KOBSweather.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#pragma GCC diagnostic ignored "-Wformat-truncation="

// We declare an auto pointer to OpenWeatherMap.
std::unique_ptr<OpenWeatherMap> openWeatherMap(new OpenWeatherMap());

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string *)userp)->append((char *)contents, size * nmemb);
    return size * nmemb;
}

void ISGetProperties(const char *dev)
{
    openWeatherMap->ISGetProperties(dev);
}

void ISNewSwitch(const char *dev, const char *name, ISState *states, char *names[], int n)
{
    openWeatherMap->ISNewSwitch(dev, name, states, names, n);
}

void ISNewText(const char *dev, const char *name, char *texts[], char *names[], int n)
{
    openWeatherMap->ISNewText(dev, name, texts, names, n);
}

void ISNewNumber(const char *dev, const char *name, double values[], char *names[], int n)
{
    openWeatherMap->ISNewNumber(dev, name, values, names, n);
}

void ISNewBLOB(const char *dev, const char *name, int sizes[], int blobsizes[], char *blobs[], char *formats[],
               char *names[], int n)
{
    INDI_UNUSED(dev);
    INDI_UNUSED(name);
    INDI_UNUSED(sizes);
    INDI_UNUSED(blobsizes);
    INDI_UNUSED(blobs);
    INDI_UNUSED(formats);
    INDI_UNUSED(names);
    INDI_UNUSED(n);
}
void ISSnoopDevice(XMLEle *root)
{
    openWeatherMap->ISSnoopDevice(root);
}

/***********************************************************************/
OpenWeatherMap::OpenWeatherMap()
{
    setVersion(KW_VERSION_MAJOR, KW_VERSION_MINOR);

    owmLat  = -1000;
    owmLong = -1000;

    setWeatherConnection(CONNECTION_NONE);
}

/***********************************************************************/
OpenWeatherMap::~OpenWeatherMap() {}

const char *OpenWeatherMap::getDefaultName()
{
    return (const char *)"KOBS Weather";
}

/***********************************************************************/
bool OpenWeatherMap::Connect()
{

    return true;
}

bool OpenWeatherMap::Disconnect()
{
    return true;
}

/***********************************************************************/
bool OpenWeatherMap::initProperties()
{
    INDI::Weather::initProperties();

    addParameter("WEATHER_TEMPERATURE", "Temperature (F)", 20, 80, 1);
    addParameter("WEATHER_HUMIDITY", "Humidity (%)", 0, 95, 1);
    addParameter("WEATHER_DEW_POINT", "Dew Point (F)", 0, 100, 0);
    addParameter("WEATHER_DP_DEP", "DP Depresion", 3, 0, 1);
    addParameter("WEATHER_PRESSURE", "Pressure (inMg)", 27, 32, 1);
    addParameter("WEATHER_WIND_SPEED", "Wind (mph)", 0, 6, 1);
    addParameter("WEATHER_WIND_GUST", "Gust (mph)", 0, 10, 1);
    addParameter("WEATHER_RAIN_HOUR", "Daily Rain (in)", 0, 0, 1);
    addParameter("WEATHER_IS_RAIN", "Raining", 0, 1, 1);
    addParameter("WEATHER_SKY_TEMP", "Sky Temp (c)", -40, -10, 1);
    addParameter("WEATHER_LUX", "Sky LUX (lux*10K)", 0, 1, 1);
    
    setCriticalParameter("WEATHER_TEMPERATURE");
    setCriticalParameter("WEATHER_WIND_SPEED");
    setCriticalParameter("WEATHER_IS_RAIN");
    setCriticalParameter("WEATHER_SKY_TEMP");
    setCriticalParameter("WEATHER_HUMIDITY");
    
    IUFillText(&MysqlT[Host], "MYSQL_HOST", "Mysql Hostname", "Please Set");
    IUFillText(&MysqlT[User], "MYSQL_User", "Mysql Username", "Please Set");
    IUFillText(&MysqlT[Pwd], "MYSQL_Pwd", "Mysql Password", "Please Set");
    IUFillText(&MysqlT[DataBase], "MYSQL_DB", "Mysql Database", "Please Set");
    IUFillTextVector(&MysqlTP, MysqlT, Mysql_N, getDeviceName(), "MYSQL", "Script", OPTIONS_TAB, IP_RW, 60, IPS_IDLE);

    defineText(&MysqlTP);
    
    addDebugControl();

    return true;
}

/***********************************************************************/
void OpenWeatherMap::ISGetProperties(const char *dev)
{
    INDI::Weather::ISGetProperties(dev);

    loadConfig(true, MysqlTP.name);
}

/***********************************************************************/
bool OpenWeatherMap::saveConfigItems(FILE *fp)
{
    INDI::Weather::saveConfigItems(fp);

    IUSaveConfigText(fp, &MysqlTP);

    return true;
}

/***********************************************************************/
bool OpenWeatherMap::ISNewText(const char *dev, const char *name, char *texts[], char *names[], int n)
{
    if (dev != nullptr && strcmp(dev, getDeviceName()) == 0)
    {
        if (strcmp(name, MysqlTP.name) == 0)
        {
            MysqlTP.s = IPS_OK;
            IUUpdateText(&MysqlTP, texts, names, n);
            // update client display
            IDSetText(&MysqlTP, nullptr);
            return true;
        }

    }
    
    return INDI::Weather::ISNewText(dev, name, texts, names, n);
}

/***********************************************************************/
IPState OpenWeatherMap::updateWeather()
{
    // MYSQL connection
    static unsigned int opt_port_num = 3306; /* PORT */
    static char *opt_socket_name = NULL; /* SOCKET NAME, DO NOT CHANGE */
    static unsigned int opt_flags = 0; /* CONNECTION FLAGS, DO NOT CHANGE */
    
    MYSQL *conn; /* pointer to connection handler */
    MYSQL_RES *res; /* holds the result set */
    MYSQL_ROW row;
    conn = mysql_init (NULL);
    
    /* THIS CONNECTS TO SERVER, DO NOT CHANGE ANYTHING HERE */
    //mysql_real_connect (conn, opt_host_name, opt_user_name, opt_password,
    //opt_db_name, opt_port_num, opt_socket_name, opt_flags);
    mysql_real_connect (conn, MysqlT[Host].text, MysqlT[User].text, MysqlT[Pwd].text, MysqlT[DataBase].text, opt_port_num, opt_socket_name, opt_flags);
    
    /* show tables in the database (test for errors also) */
    mysql_query(conn, "SELECT * FROM sensordata ORDER BY time DESC LIMIT 1");
    res = mysql_store_result(conn);
    
    // get the number of the columns
    int num_fields = mysql_num_fields(res);
    
    row = mysql_fetch_row(res);
    
    setParameterValue("WEATHER_TEMPERATURE", stof(row[roofTemp]));
    setParameterValue("WEATHER_HUMIDITY", stof(row[roofHum]));
    setParameterValue("WEATHER_DEW_POINT", stof(row[roofDp]));
    setParameterValue("WEATHER_DP_DEP", stof(row[roofTemp]) - stof(row[roofDp]));
    setParameterValue("WEATHER_PRESSURE", stof(row[ambPressure]));
    setParameterValue("WEATHER_WIND_SPEED", stof(row[ambwindspd]));
    setParameterValue("WEATHER_WIND_GUST", stof(row[ambwindGust]));
    setParameterValue("WEATHER_RAIN_HOUR", stof(row[ambRain]));
    if (row[isRain] == "y")
        Raining = 1;
    else
        Raining = 0;
    setParameterValue("WEATHER_IS_RAIN", Raining);
    setParameterValue("WEATHER_SKY_TEMP", stof(row[skyTemp]));
    setParameterValue("WEATHER_LUX", stof(row[lux]) * 10000);
    
    if(res != NULL)
       mysql_free_result(res);

    /* disconnect from server */
    mysql_close (conn);

    return IPS_OK;
}


