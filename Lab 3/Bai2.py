from flask import Flask, request, jsonify

app = Flask(__name__)

@app.route('/nhom8', methods=['POST'])
def calculate_lights():
    print(response)  
    data = request.get_json()  
    
    light_intensity = data.get('data', {}).get('light')
    distance_value = data.get('data', {}).get('distance')
    if distance_value or light_intensity is None:
        response = {
            "error": True,
            "message": "Invalid data format. Value is missing.",
            "data": {} }    
    else:
        if distance_value > 50:
            light_count = 0
        else:
            if (light_intensity < 100):
                light_count = 3
            elif (light_intensity < 400):
                light_count = 2
            elif (light_intensity < 1000):
                light_count = 1
        response = {
            "error": False,
            "message": "this is a message of API",
            "data": {
                "light": light_count,
            }
        }
    return jsonify(response)

if __name__ == '__main__':
    app.run(host='0.0.0.0')