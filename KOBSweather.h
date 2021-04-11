// File:        KOBSweather.h
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

#pragma once

//#include "locale_compat.h"
#include <memory>
#include <cstring>
#include <string>
#include <unistd.h>
#include <sys/wait.h>
#include "indiweather.h"
#include <stdint.h>
#include "config.h"
#include <ctime>

using namespace std;

class KOBSweather : public INDI::Weather
{
  public:
    KOBSweather();
    virtual ~KOBSweather();

    //  Generic indi device entries
    bool Connect() override;
    bool Disconnect() override;
    const char *getDefaultName() override;

    virtual bool initProperties() override;
    virtual bool updateProperties() override;
    virtual void ISGetProperties(const char *dev) override;
    virtual bool ISNewText(const char *dev, const char *name, char *texts[], char *names[], int n) override;

  protected:
    virtual IPState updateWeather() override;
    virtual bool saveConfigItems(FILE *fp) override;

  private:
    enum {
        EphemScript,
        CurWeather,
        Script_N,
    };
    IText ScriptsT[Script_N] {};
    ITextVectorProperty ScriptsTP;
    
    enum {
        sun,
        moon,
        current,
        Ephem_N,
    };
    IText EphemT[Ephem_N] {};
    ITextVectorProperty EphemTP;
    
    bool getDeviceInfo();
    bool getEphemeris();
    bool EphemSet = false;
    bool LastParseSuccess = true;
    bool LastParseSuccess2 = true;
    char buf[BUFSIZ], CurrentCond[BUFSIZ];
    size_t byte_count;
    int rc;
    
    float roofTemp, roofHum, roofDp, ambPressure, ambwindspd, ambwindGust, ambRain, Raining, skyTemp, lux;
};

