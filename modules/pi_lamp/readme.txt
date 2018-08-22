Для автозапуска
sudo nano /etc/rc.local
в блок перед exit 0
screen -d -m -S lamp python3 /home/pi/simple_home/lamp/btn_control.py
