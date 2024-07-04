import cv2
from kafka import KafkaProducer,KafkaConsumer
import time
from datetime import datetime
import json
import threading

topic_name = "send_image"
id_room="20"
kafka_ip="192.168.137.1:9092"

p = KafkaProducer(
    bootstrap_servers=[kafka_ip],
    max_request_size = 9000000,
)

c = KafkaConsumer(
    "receive_result",
    bootstrap_servers = [kafka_ip],
    auto_offset_reset='earliest',
    enable_auto_commit=False,
    fetch_max_bytes = 9000000,
    fetch_max_wait_ms = 10000,
    group_id="my-group2"
)



def function_reci():
    for message in c:
            print("result: ",message.value)
            c.commit()
     

# Tạo một luồng mới với daemon=True
thread1 = threading.Thread(target=function_reci, daemon=True)
thread1.start()

cam = cv2.VideoCapture(0)

start_time = time.time()
while True:
    elapsed_time = time.time() - start_time
    countdown = 20 - int(elapsed_time)
    print(f"Countdown: {countdown} seconds", end="\r")
    ret, frame = cam.read()
    if ret:
        ret, buffer = cv2.imencode('.jpg', frame)
        if countdown <= 0:
            start_time = time.time()
            info={
                "id_room":id_room,
                "timestamp": datetime.fromtimestamp(time.time()).strftime('%Y-%m-%d %H:%M:%S')
            }
            info=json.dumps(info)
            p.send(topic_name, key=info.encode('utf-8'), value=buffer.tobytes())
            p.flush()
            print("Sent!")