#!/usr/bin/python3
# File:         KOBSweather.py
# Version:      20210311
# Authur:       Sifan
# Desc:         retrieves weather data from mysql for the INDI KOBSweather driver

import sys, os
import mysql.connector

MYHost = "kiana"
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
  otaTemp,
  otaHum,
  otaDp,
  otaPres,
  otaLux,
  otaSkyTemp,
  allskyTemp,
  allskyHum,
  allskyDp,
  allskyPres,
  allskyLux,
  allskySkyTemp,
  allskyWind,
  allskyWGust,
  allskyWDir,
  fallskyTemp,
  fallskyHum,
  fallskyDp,
  fallskyPres,
  fallskyLux,
  fallskySkyTemp,
  fallskyWind,
  fallskyWGust,
  fallskyWDir,
  domeTemp,
  domeHum,
  domeDp,
  domePres,
  domeIsRain,
  domePME10,
  domePME25,
  domePME100,
  domePM3,
  domePM5,
  domePM10,
  domePM25,
  domePM50,
  domePM100,
  obsTemp,
  obsHum,
  obsDp,
  obsPres,
  obsWind,
  obsWGust,
  obsWdir,
  obsIsRain,
  obsRain,
  obsRain1Hr,
  obsRain24Hr,
  obsLgtng) in cursor:

  # rain comes in as y or n, but needs to be int
  if (obsIsRain == "y"):
        Raining = 1
  else:
        Raining = 0
        
  print("%s,%s,%s,%s,%s,%s,%s,%s,%s,%s\n" % (allskyTemp, allskyHum, allskyDp, allskyPres, allskyWind, allskyWGust, obsRain, Raining, allskySkyTemp, allskyLux))
    
cursor.close()

