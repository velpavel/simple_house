import time
import RPi.GPIO as GPIO
GPIO.setmode(GPIO.BCM)

lamp_gpio_list=[23,24]
btn_gpio=22

GPIO.setup(lamp_gpio_list, GPIO.OUT, initial=GPIO.LOW)
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
      GPIO.output(lamp_gpio_list, not GPIO.input(lamp_gpio_list[0]))
    time.sleep(0.01)
except Exception as e:
  print(e)
finally:
  GPIO.cleanup(lamp_gpio_list+[btn_gpio])
