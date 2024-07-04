import paho.mqtt.client as mqtt
import json

mqtt_broker = "192.168.137.234"
mqtt_port = 1883
mqtt_topic = "cq21/2/nhom8/dht/value"
mqtt_detected_topic = "cq21/2/nhom8/dht/detected"

def on_connect(client, userdata, flags, reason_code, properties):
    print("Kết nối thành công đến MQTT broker!")
    client.subscribe(mqtt_topic)

def on_message(client, userdata, msg):
    data = msg.payload.decode("utf-8")
    print(data)
    process_sensor_data(client, data)

def process_sensor_data(client, data):
    data_value = json.loads(data)
    if "Tem" in data_value and "Hum" in data_value:
        temperature = data_value["Tem"]
        humidity = data_value["Hum"]
        if temperature < 25 or temperature > 28 or humidity < 40 or humidity > 70:     
            client.publish(mqtt_detected_topic, "1")
        else: client.publish(mqtt_detected_topic, "0")

mqttc = mqtt.Client(mqtt.CallbackAPIVersion.VERSION2)
mqttc.on_connect = on_connect
mqttc.on_message = on_message

mqttc.username_pw_set(username="group08", password="123456")
mqttc.connect(mqtt_broker, mqtt_port, 60)
mqttc.loop_forever()