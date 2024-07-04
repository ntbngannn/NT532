from __future__ import absolute_import
from __future__ import division
from __future__ import print_function
import tensorflow as tf
import argparse
import facenet
import os
import sys
import math
import pickle
import align.detect_face
import numpy as np
import cv2
import collections
from sklearn.svm import SVC
import base64
from kafka import KafkaConsumer, KafkaProducer
import matplotlib.pyplot as plt
import json
import time
from datetime import datetime
import requests

MINSIZE = 20
THRESHOLD = [0.6, 0.7, 0.7]
FACTOR = 0.709
IMAGE_SIZE = 182
INPUT_IMAGE_SIZE = 160
CLASSIFIER_PATH = './facemodel.pkl'
FACENET_MODEL_PATH = './20180402-114759.pb'

# Load The Custom Classifier
with open(CLASSIFIER_PATH, 'rb') as file:
    model, class_names = pickle.load(file)
print("Custom Classifier, Successfully loaded")

with tf.Graph().as_default():

    # Cai dat GPU neu co
    gpu_options = tf.compat.v1.GPUOptions(per_process_gpu_memory_fraction=0.6)
    sess = tf.compat.v1.Session(config=tf.compat.v1.ConfigProto(gpu_options=gpu_options, log_device_placement=False))

    with sess.as_default():
        # Load the model
        print('Loading feature extraction model')
        facenet.load_model(FACENET_MODEL_PATH)
        images_placeholder = tf.compat.v1.get_default_graph().get_tensor_by_name("input:0")
        embeddings = tf.compat.v1.get_default_graph().get_tensor_by_name("embeddings:0")
        phase_train_placeholder = tf.compat.v1.get_default_graph().get_tensor_by_name("phase_train:0")
        embedding_size = embeddings.get_shape()[1]

        pnet, rnet, onet = align.detect_face.create_mtcnn(sess, "./align")


def predict_img(image):
    # base 64
    name="Unknown"
    frame = image
    bounding_boxes, _ = align.detect_face.detect_face(frame, MINSIZE, pnet, rnet, onet, THRESHOLD, FACTOR)
    faces_found = bounding_boxes.shape[0]
    if faces_found > 0:
        det = bounding_boxes[:, 0:4]
        bb = np.zeros((faces_found, 4), dtype=np.int32)
        for i in range(faces_found):
            bb[i][0] = det[i][0]
            bb[i][1] = det[i][1]
            bb[i][2] = det[i][2]
            bb[i][3] = det[i][3]
            # cropped = frame
            cropped = frame[bb[i][1]:bb[i][3], bb[i][0]:bb[i][2], :]
            scaled = cv2.resize(cropped, (INPUT_IMAGE_SIZE, INPUT_IMAGE_SIZE),
                                interpolation=cv2.INTER_CUBIC)
            scaled = facenet.prewhiten(scaled)
            scaled_reshape = scaled.reshape(-1, INPUT_IMAGE_SIZE, INPUT_IMAGE_SIZE, 3)
            feed_dict = {images_placeholder: scaled_reshape, phase_train_placeholder: False}
            emb_array = sess.run(embeddings, feed_dict=feed_dict)
            predictions = model.predict_proba(emb_array)
            best_class_indices = np.argmax(predictions, axis=1)
            best_class_probabilities = predictions[
                np.arange(len(best_class_indices)), best_class_indices]
            best_name = class_names[best_class_indices[0]]
            print("Name: {}, Probability: {}".format(best_name, best_class_probabilities))

            if best_class_probabilities > 0.5:
                name = class_names[best_class_indices[0]]
            else:
                name = "Unknown"
    return name


kafka_ip = "localhost:9092"

topic_name = "send_image"
c = KafkaConsumer(
    topic_name,
    bootstrap_servers = [kafka_ip],
    auto_offset_reset='earliest',
    enable_auto_commit=False,
    fetch_max_bytes = 9000000,
    fetch_max_wait_ms = 10000,
    group_id="my-group"
)

topic_name_out = "receive_result"
p = KafkaProducer(
    bootstrap_servers=[kafka_ip],
    max_request_size = 9000000,
)

for message in c:
    c.commit()
    image = message.value
    info = message.key.decode('utf-8')
    info = json.loads(info)
    image = np.frombuffer(image, dtype=np.uint8)
    image = cv2.imdecode(image, cv2.IMREAD_COLOR)
    # Display the image
    plt.imshow(cv2.cvtColor(image, cv2.COLOR_BGR2RGB))
    plt.show()
    try: 
        result = predict_img(image)
    except: 
        result = "Unknown"
    if result == "baongan": result = "Bảo Ngân"
    elif result == "caothi": result = "Cao Thi"
    message = {
        "timestamp": info["timestamp"],
        "id_room": info["id_room"],
        "name": result

    }
    message=json.dumps(message)
    print("Result: ", message)
    p.send(topic_name_out, value=message.encode('utf-8'))
    requests.post("http://172.174.189.9:8080/api/v1/RYbVjEaQwPRejiXODKLx/telemetry", data=message)
    p.flush()