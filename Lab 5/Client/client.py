from flask import Flask, render_template, request
import requests
import base64
import cv2
import time
app = Flask(__name__)
camera = cv2.VideoCapture(0)


# Function to encode image to Base64
def encode_image_to_base64(image):
    _, buffer = cv2.imencode('.jpg',image)
    base64_image = base64.b64encode(buffer).decode('utf-8')
    return base64_image

@app.route('/')
def index():
    return render_template('index.html')

@app.route('/send_image', methods=['GET'])
def send_image():
    ret, frame = camera.read()
    url = 'http://192.168.137.99:8000/recog'
    encoded_image_data = encode_image_to_base64(frame)

    data = {
        'image': encoded_image_data,
        'w': 160,  #  width
        'h': 160   #  height
    }
    start_time = time.time()
    response = requests.post(url, data=data)
    elapsed_time = time.time() - start_time
    print("Elapsed time: {:.2f} seconds".format(elapsed_time))

    if response.status_code == 200:
        result = response.text
        if result=='caothi':result="Cao Thi"
        elif result=="baongan": result="Bảo Ngân"
        else: result="Không thể nhận diện"
        print('Kết quả nhận diện:', result)
    else:
        print('Lỗi khi gửi dữ liệu ảnh lên máy chủ.')

    return {
        "delay":elapsed_time,
        "image":encoded_image_data,
        "result":result
    }

if __name__ == '__main__':
    app.run(port=8080)