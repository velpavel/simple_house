#!/usr/bin/env python3
# -*- coding: utf-8 -*-
import sys
import RPi.GPIO as GPIO
import configparser
 
def output_pin_init(pin_name):
    config = configparser.ConfigParser()
    config.read("/home/pi/simple_home/global_config.conf")         #считаем конфиг
    pin_number = config.getint("pins", pin_name) #пина из конфига присвоем переменной pin_number
     
    print("use pin:"+str(pin_number))
     
    GPIO.setmode(GPIO.BCM)
    GPIO.setwarnings(False)            
    GPIO.setup(pin_number, GPIO.OUT)   #устанавливаем пин на выходной сигнал
    return pin_number
        
def output_pin_set(pin_name, pin_state):
    pin_number = output_pin_init(pin_name)
    if pin_state:
        GPIO.output(pin_number, GPIO.HIGH)
    else:
        GPIO.output(pin_number, GPIO.LOW)
        
def output_pin_read(pin_name):
    pin_number = output_pin_init(pin_name)
    return GPIO.input(pin_number)
        
def lamp_on():
    print('Включаю')
    output_pin_set('lamp1_gpio', 1)
    output_pin_set('lamp2_gpio', 1)
    
def lamp_off():
    print('Выключаю')
    output_pin_set('lamp1_gpio', 0)
    output_pin_set('lamp2_gpio', 0)

def lamp_change():
    print('Меняю')
    state = output_pin_read('lamp1_gpio')
    output_pin_set('lamp1_gpio', not state)
    output_pin_set('lamp2_gpio', not state)
    
def lamp_half():
    print('Включаю')
    output_pin_set('lamp1_gpio', 1)
    output_pin_set('lamp2_gpio', 0)
    
def lamp_half2():
    print('Включаю')
    output_pin_set('lamp1_gpio', 0)
    output_pin_set('lamp2_gpio', 1)
    
    
if __name__ == '__main__':
    if len (sys.argv) > 1:
        if sys.argv[1] == '1': lamp_on()
        elif sys.argv[1] == '0': lamp_off()
        elif sys.argv[1] == '2': lamp_half()
        elif sys.argv[1] == '3': lamp_half2()
        else: print(sys.argv[1])
    else:
        print('Паремтры 1 - включить, 0 - выключить')