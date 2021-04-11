// File:        KOBSweather.cpp
// Version:     20210308
// Author:      Sifan Kahale
// Desc:        INDI Weather driver for the KOBS allsky station

/*******************************************************************************
  Copyright(c) 2015 Jasem Mutlaq. All rights reserved.

  INDI Weather Underground (TM) Weather Driver

  Modified for OpenWeatherMap API by Jarno Paananen
  Modified for KOBSweather by Sifan Kahale

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

// We declare an auto pointer to KOBSweather.
std::unique_ptr<KOBSweather> openWeatherMap(new KOBSweather());

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
KOBSweather::KOBSweather()
{
    setVersion(KW_VERSION_MAJOR, KW_VERSION_MINOR);

    setWeatherConnection(CONNECTION_NONE);
}

/***********************************************************************/
KOBSweather::~KOBSweather() {}

const char *KOBSweather::getDefaultName()
{
    return (const char *)"KOBS Weather";
}

/***********************************************************************/
bool KOBSweather::Connect()
{

    return true;
}

bool KOBSweather::Disconnect()
{
    return true;
}

/***********************************************************************/
bool KOBSweather::initProperties()
{
    INDI::Weather::initProperties();

    IUFillText(&EphemT[0], "CURRSUN", "Sun Eph", "");
    IUFillText(&EphemT[1], "CURRMOON", "Moon Eph", "");
    IUFillText(&EphemT[2], "CURRCOND", "Current Conditions", "");
    IUFillTextVector(&EphemTP, EphemT, 3, getDeviceName(), "Conditions", "Conditions", MAIN_CONTROL_TAB, IP_RO, 0, IPS_OK);
    
    addParameter("WEATHER_TEMPERATURE", "Temperature (F)", 20, 80, 1);
    addParameter("WEATHER_HUMIDITY", "Humidity (%)", 0, 95, 1);
    addParameter("WEATHER_DEW_POINT", "Dew Point (F)", 0, 100, 0);
    addParameter("WEATHER_DP_DEP", "DP Depresion", 3, 0, 1);
    addParameter("WEATHER_PRESSURE", "Pressure (inMg)", 27, 32, 1);
    addParameter("WEATHER_WIND_SPEED", "Wind (mph)", 0, 6, 1);
    addParameter("WEATHER_WIND_GUST", "Gust (mph)", 0, 10, 1);
    addParameter("WEATHER_RAIN_HOUR", "Daily Rain (in)", 0, 0, 1);
    addParameter("WEATHER_IS_RAIN", "Raining (1=yes)", 0, 1, 0);
    addParameter("WEATHER_SKY_TEMP", "Sky Temp (c)", -40, -10, 1);
    addParameter("WEATHER_LUX", "Sky LUX (lux*10K)", 0, 1, 1);
    
    setCriticalParameter("WEATHER_TEMPERATURE");
    setCriticalParameter("WEATHER_WIND_SPEED");
    setCriticalParameter("WEATHER_IS_RAIN");
    setCriticalParameter("WEATHER_SKY_TEMP");
    setCriticalParameter("WEATHER_HUMIDITY");
    
    IUFillText(&ScriptsT[EphemScript], "EPHEMERIS_SCRIPT_NAME", "Ephemeris Script", "KOBSephemeris.py");
    IUFillText(&ScriptsT[CurWeather], "WEATHER_SCRIPT_NAME", "Weather Script", "KOBSweather.py");
    IUFillTextVector(&ScriptsTP, ScriptsT, Script_N, getDeviceName(), "SCRIPTS", "Scripts", OPTIONS_TAB, IP_RW, 100, IPS_IDLE);
    addDebugControl();

    return true;
}

/*****************************************************************************************/
bool KOBSweather::updateProperties()
{
    INDI::Weather::updateProperties();

    if (isConnected())
    {
        defineText(&ScriptsTP);
        defineText(&EphemTP);
        updateWeather();
    }
    else
    {
        deleteProperty(ScriptsTP.name);
        deleteProperty(EphemTP.name);
    }

    return true;
}


/***********************************************************************/
void KOBSweather::ISGetProperties(const char *dev)
{
    INDI::Weather::ISGetProperties(dev);
    loadConfig(true, ScriptsTP.name);
}

/***********************************************************************/
bool KOBSweather::saveConfigItems(FILE *fp)
{
    INDI::Weather::saveConfigItems(fp);
    IUSaveConfigText(fp, &ScriptsTP);

    return true;
}

/***********************************************************************/
bool KOBSweather::ISNewText(const char *dev, const char *name, char *texts[], char *names[], int n)
{
    if (dev != nullptr && strcmp(dev, getDeviceName()) == 0)
    {
        if (strcmp(name, ScriptsTP.name) == 0)
        {
            ScriptsTP.s = IPS_OK;
            IUUpdateText(&ScriptsTP, texts, names, n);
            
            // this was set if there was an error, reset it when the script name has changed so we can try it again
            LastParseSuccess = true;
            
            // update client display
            IDSetText(&ScriptsTP, nullptr);
            return true;
        }

    }
    
    return INDI::Weather::ISNewText(dev, name, texts, names, n);
}

/***********************************************************************/
IPState KOBSweather::updateWeather()
{
    // if LastParseSucess was false, means we had error with script, no need repeating this
    if(LastParseSuccess) {
        
        char cmd[51];
        snprintf(cmd, 50, "/usr/share/indi/scripts/%s", ScriptsT[CurWeather].text);
        
        if (access(cmd, F_OK|X_OK) == -1) {
            LOGF_ERROR("Cannot use script [%s], check its existence and permissions", cmd);
            LastParseSuccess = false;
            return IPS_ALERT;
        }

        LOGF_DEBUG("Run script: %s", cmd);
        FILE *handle = popen(cmd, "r");
        if (handle == nullptr) {
            LOGF_ERROR("Failed to run script [%s]", strerror(errno));
            LastParseSuccess = false;
            return IPS_ALERT;
        }
        
        byte_count = fread(buf, 1, BUFSIZ - 1, handle);
        fclose(handle);
        buf[byte_count] = 0;
        if (byte_count == 0) {
            LOGF_ERROR("Got no output from script [%s]", cmd);
            LastParseSuccess = false;
            return IPS_ALERT;
        }
    
        LOGF_DEBUG("Read %d bytes output [%s]", byte_count, buf);
    
        rc = sscanf(buf, "%f,%f,%f,%f,%f,%f,%f,%f,%f,%f", &roofTemp, &roofHum, &roofDp, &ambPressure, &ambwindspd, &ambwindGust, &ambRain, &Raining, &skyTemp, &lux);
    
        if (rc < 1)  {
            LOGF_ERROR("Failed to parse input %s", buf);
            LastParseSuccess = false;
            return IPS_ALERT;
        }
    
        setParameterValue("WEATHER_TEMPERATURE", roofTemp);
        setParameterValue("WEATHER_HUMIDITY", roofHum);
        setParameterValue("WEATHER_DEW_POINT", roofDp);
        setParameterValue("WEATHER_DP_DEP", roofTemp - roofDp);
        setParameterValue("WEATHER_PRESSURE", ambPressure);
        setParameterValue("WEATHER_WIND_SPEED", ambwindspd);
        setParameterValue("WEATHER_WIND_GUST", ambwindGust);
        setParameterValue("WEATHER_RAIN_HOUR", ambRain);
        setParameterValue("WEATHER_IS_RAIN", Raining);
        setParameterValue("WEATHER_SKY_TEMP", skyTemp);
        setParameterValue("WEATHER_LUX", lux * 10000);
        
        // Fill in current weather string for copy/paste
        time_t ts = std::time(0);
        std::tm* now = std::localtime(&ts);
        sprintf(CurrentCond, "[Time: %02i:%02i] Temp: %.1f Hum: %.1f DpDep %.1f Gust %.1f SkyTemp %.1f LUX %.1f", now->tm_hour, now->tm_min, roofTemp, roofHum, roofTemp - roofDp, ambwindGust, skyTemp, lux);
        EphemT[current].text = CurrentCond;
        
        // Read in current sun/moon ephemeris - just once 
        if (!EphemSet) {
            KOBSweather::getEphemeris();
            EphemSet = true;
        }
        
        return IPS_OK;
    }
    
    return IPS_ALERT;
}

/***********************************************************************/
bool KOBSweather::getEphemeris()
{
    if(LastParseSuccess2) {
        
        char cmd[51];
        snprintf(cmd, 50, "/usr/share/indi/scripts/%s", ScriptsT[EphemScript].text);
        
        if (access(cmd, F_OK|X_OK) == -1) {
            LOGF_ERROR("Cannot use script [%s], check its existence and permissions", cmd);
            LastParseSuccess2 = false;
            return false;
        }

        LOGF_DEBUG("Run script: %s", cmd);
        FILE *handle = popen(cmd, "r");
        if (handle == nullptr) {
            LOGF_ERROR("Failed to run script [%s]", strerror(errno));
            LastParseSuccess2 = false;
            return false;
        }
        
        byte_count = fread(buf, 1, BUFSIZ - 1, handle);
        fclose(handle);
        buf[byte_count] = 0;
        if (byte_count == 0) {
            LOGF_ERROR("Got no output from script [%s]", cmd);
            LastParseSuccess2 = false;
            return false;
        }
    
        LOGF_DEBUG("Read %d bytes output [%s]", byte_count, buf);
        
        stringstream iss(buf);
        
        string SunEphem;
        getline(iss, SunEphem, '\n');
        LOGF_DEBUG("SunEphem: %s", SunEphem.c_str());
        char *SUN = new char[SunEphem.size()+1];
        strcpy(SUN, SunEphem.c_str());
        EphemT[sun].text = SUN;
            
        string MoonEphem;
        getline(iss, MoonEphem, '\n');
        LOGF_DEBUG("Line: %s", MoonEphem.c_str());
        char *MOON = new char[MoonEphem.size()+1];
        strcpy(MOON, MoonEphem.c_str());
        
        EphemT[moon].text = MOON;
    }
    
    return true;
}

