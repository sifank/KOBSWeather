// File:        KOBSweather.h
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

#pragma once

//#include "locale_compat.h"
#include <memory>
#include <cstring>
#include <string>
#include <unistd.h>
#include <mysql/mysql.h>
#include <sys/wait.h>
#include "indiweather.h"
#include "config.h"

using namespace std;

class OpenWeatherMap : public INDI::Weather
{
  public:
    OpenWeatherMap();
    virtual ~OpenWeatherMap();

    //  Generic indi device entries
    bool Connect() override;
    bool Disconnect() override;
    const char *getDefaultName() override;

    virtual bool initProperties() override;
    virtual void ISGetProperties(const char *dev) override;
    virtual bool ISNewText(const char *dev, const char *name, char *texts[], char *names[], int n) override;

  protected:
    virtual IPState updateWeather() override;
    virtual bool saveConfigItems(FILE *fp) override;

  private:
    //mysql db fields
    enum {
        time,
        roofTemp,
        roofHum,
        roofDp,
        roofPres,
        ambwindspd,
        ambwinddeg,
        ambwindGust,
        ambRain,
        ambPressure,
        ambTemp,
        ambHum,
        ambDp,
        ambOTAd,
        ambOBSd,
        ambCrd,
        skyTemp,
        OTAtemp,
        OTAhum,
        OTAdp,
        ambOBSt,
        ambOBSh,
        ambOTAt,
        ambOTAh,
        DOMtemp,
        DOMhum,
        DOMdp,
        DOMpres,
        OTA1temp,
        OTA1hum,
        OTA1dp,
        OTAskyTemp,
        OTA1skyTemp,
        pme10,
        pme25,
        pme100,
        pm3,
        pm5,
        pm10,
        pm25,
        pm50,
        pm100,
        isRain,
        lux,
        OTApres,
        OTA1pres,
    };

    enum {
        Host,
        User,
        Pwd,
        DataBase,
        Mysql_N,
    };
      
    IText MysqlT[Mysql_N];
    ITextVectorProperty MysqlTP;
    
    bool getDeviceInfo();
    double owmLat, owmLong;
    bool LastParseSuccess = false;
    int Raining;
};

