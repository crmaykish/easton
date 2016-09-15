import serialsensor

class Motor(serialsensor.SerialSensor):
    l_speed = 0
    r_speed = 0
    heading = 0.0

    def __init__(self, port, baud, timeout):
        super().__init__(port, baud, timeout)

    def parse(self):
        reading = self.ser.readline().decode("utf-8").split(',')

        if (reading[0] == "speed"):
            try:
                self.l_speed = int(reading[1])
                self.r_speed = int(reading[2])
            except ValueError:
                print("Value error for speed")
        elif (reading[0] == "compass"):
            try:
                self.heading = float(reading[1])
            except ValueError:
                print("Value error for: " + reading[1])
        elif (reading[0] == "log"):
            print("Motor board log: " + reading[1])

    def command(self, command):
        tosend = (command + '\n').encode('ascii');
        self.ser.write(tosend)

    def drive(self, side, speed):
        command = "d,"
        if side == "left":
            command += "0,"
        elif side == "right":
            command += "1,"
        command += str(speed)

        self.command(command)

    def get_json(self):
        return {
            "l_speed": self.l_speed,
            "r_speed": self.r_speed,
            "heading": self.heading
        }
