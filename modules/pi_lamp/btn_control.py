#!/usr/bin/env python3
# -*- coding: utf-8 -*-
import sys
import configparser
import time
import RPi.GPIO as GPIO
from lamp_control import lamp_change

config = configparser.ConfigParser()
config.read("/home/pi/simple_home/global_config.conf")         #считаем конфиг
btn_gpio = config.getint("pins", 'btn_gpio') #пина из конфига присвоем переменной 


GPIO.setmode(GPIO.BCM)
GPIO.setwarnings(False)

btn_gpio=22

GPIO.setup(btn_gpio, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)
btn_status=GPIO.input(btn_gpio)

def is_btn_pressed():
  global btn_status 
  if GPIO.input(btn_gpio) != btn_status:
    time.sleep(.005)
    btn_status=GPIO.input(btn_gpio)
    return btn_status
  return 0

try:
  while True:
    if is_btn_pressed():
      lamp_change()
    time.sleep(0.02)
except Exception as e:
  print(e)
finally:
  GPIO.cleanup(btn_gpio)
