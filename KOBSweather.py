#!/usr/bin/python3
# File:         KOBSweather.py
# Version:      20210311
# Authur:       Sifan
# Desc:         retrieves weather data from mysql for the INDI KOBSweather driver

import sys, os
import mysql.connector

MYHost = "mingshan"
MYUser = "sifan"
MYPwd = "all4Sky"
MYDb = "kahaleobs"
MYTbl = "sensordata"

#--------------------------------------------------------
# Main - initialize
#--------------------------------------------------------
# open up mysql connector
try:
    conn = mysql.connector.connect (host=MYHost, user=MYUser, passwd=MYPwd, db=MYDb)
    cursor = conn.cursor()
except mysql.connector.Error as e:
    print("DB Error opening db %d:  %s" % (e.args[0], e.args[1]))
    sys.exit(1)

#------------------------------------------------------
# Create data file and print html page
#------------------------------------------------------
# read in from db
try:
    cursor.execute ("SELECT * FROM %s ORDER BY time DESC LIMIT 1" % MYTbl)
except:
    print("DB Error reading servers table")
    sys.exit(1)

# read db and create csv records
for (time,
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
    OTA1pres) in cursor:

    # add this row to the DpDep table
    ambDD = ambTemp - ambDp
    domeDD = DOMtemp - DOMdp
    otisDD = OTAtemp - OTAdp
    taftDD = OTA1temp - OTA1dp
    
    # rain comes in as y or n, but needs to be int
    if (isRain == "y"):
        Raining = 1
    else:
        Raining = 0
        
    print("%s,%s,%s,%s,%s,%s,%s,%s,%s,%s\n" % (roofTemp, roofHum, roofDp, ambPressure, ambwindspd, ambwindGust, ambRain, Raining, skyTemp, lux))
    
cursor.close()

