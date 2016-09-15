import gps
import motor
from threading import Thread
from flask import Flask, render_template
from flask_socketio import SocketIO, emit
import logging
from lib.Xbox import xbox

app = Flask(__name__)
app.config['SECRET_KEY'] = 'secret'
socketio = SocketIO(app, async_mode='threading')

log = logging.getLogger('werkzeug')
log.setLevel(logging.ERROR)

gps_thread = None
motor_thread = None
joystick_thread = None

m = None

def motor_init():
    global m
    if m is None:
        m = motor.Motor('/dev/ttyACM0', 115200, 10);

def motor_loop():
    motor_init()
    while True:
        m.parse()
        socketio.emit('motor', m.get_json())

def gps_loop():
    g = gps.GPS('/dev/ttyUSB0', 9600, 10);
    while True:
        g.parse()
        socketio.emit('gps', g.get_json())

def joystick_loop():
    joy = xbox.Joystick()
    print("Xbox controller connected: " + str(joy.connected()))
    while True:
        left = joy.leftY()
        right = joy.rightY()
        left_mapped = left * 255
        right_mapped = right * 255
        print(str(left_mapped) + ", " + str(right_mapped))

@app.route('/')
def index():
    return render_template('index.html')

@socketio.on('drive')
def drive():
    motor_init()
    m.drive("left", 50)
    m.drive("right", 50)

@socketio.on('stop')
def stop():
    motor_init()
    m.drive("left", 0)
    m.drive("right", 0)

@socketio.on('connect')
def connected():
    print("Client connected.")
    # global gps_thread
    # if gps_thread is None:
    #     gps_thread = socketio.start_background_task(target=gps_loop)
    #
    # global motor_thread
    # if motor_thread is None:
    #     motor_thread = socketio.start_background_task(target=motor_loop)
    global joystick_thread
    if joystick_thread is None:
        joystick_thread = socketio.start_background_task(target=joystick_loop)

if __name__ == '__main__':
    socketio.run(app, host="0.0.0.0", debug=True)
