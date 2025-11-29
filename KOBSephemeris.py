#!/usr/bin/python3
# Name:     ephemeris
# Author:   sifan
# Version:  20251128
# Desc:     Calculates todays ephemeris for the Sun and Moon

import ephem
import datetime

utc = datetime.datetime.now(datetime.UTC)

ko = ephem.Observer()
ko.lon = '-124.040438'
ko.lat = '44.867813'
ko.elevation = 16
ko.epoch = '2025'
ko.date = ephem.now()

moon = ephem.Moon(ko)
sun = ephem.Sun(ko)
ko.pressure = 0
ko.horizon = '-12'  # nautical twilight
print("Sun Nautical Evening: %s, Morning: %s" % (ephem.localtime(ko.next_setting(sun, use_center=True)).ctime(),ephem.localtime(ko.next_rising(sun, use_center=True)).ctime()))
print("  Moon Rise: %s, Set: %s, Percent: %d%%" % (ephem.localtime(ko.next_rising(moon)).ctime(),ephem.localtime(ko.next_setting(moon)).ctime(),moon.phase))

