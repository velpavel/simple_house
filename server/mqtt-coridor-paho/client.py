import paho.mqtt.client as mqtt
import sqlite3

from configs import db_file

# Подключение MQTT
def on_connect(client, userdata, flags, rc):
    print("Connected with result code "+str(rc))
    client.subscribe("#")

# The callback for when a PUBLISH message is received from the ESP8266.
def on_message(client, userdata, message):
    #print(message.topic + '->' + str(message.payload.decode("utf-8")) + " |with QoS " + str(message.qos))

    conn = sqlite3.connect(db_file)
    c = conn.cursor()
    c.execute('''insert into log (topic,payload,qos) values(?,?,?)''',
              (message.topic, message.payload, message.qos))
    conn.commit()
    sql_fd = '''select id, comment from devices where topic_status=?'''
    sql_ic = '''insert or replace into current_status (device_id, current_val) values (?, ?)'''
    c.execute(sql_fd, (message.topic,))
    result = c.fetchall()
    if result:
        #print(result[0][1])
        c.execute(sql_ic, (result[0][0], message.payload.decode("utf-8")))
        conn.commit()
    conn.close()

mqttc=mqtt.Client(client_id='RPI_db_daemon')
mqttc.on_connect = on_connect
mqttc.on_message = on_message
mqttc.connect("192.168.88.9",1883,60)
#mqttc.loop_start()
mqttc.loop_forever()