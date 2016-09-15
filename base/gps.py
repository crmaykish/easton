import serialsensor

class GPS(serialsensor.SerialSensor):
    time = None
    latitude = None
    longitude = None
    speed = 0
    altitude = 0
    satellites = 0
    fix_quality = 0;

    def __init__(self, port, baud, timeout):
        super().__init__(port, baud, timeout)

    def parse_time(self, rmc_data):
        time = rmc_data[1]
        hours = time[0:2]
        minutes = time[2:4]
        seconds = time[4:6]
        return hours + ":" + minutes + ":" + seconds

    def parse_latitude(self, rmc_data):
        latitude = rmc_data[3]
        decimal = 0.0
        try:
            degrees = float(latitude[:2])
            minutes = float(latitude[2:])
            decimal = degrees + minutes / 60
        except ValueError:
            print("Value error for: " + latitude)

        return decimal

    def parse_longitude(self, rmc_data):
        longitude = rmc_data[5]
        decimal = 0.0
        try:
            degrees = float(longitude[:3])
            minutes = float(longitude[3:])
            decimal = degrees + minutes / 60
        except ValueError:
            print("Value error for: " + longitude)

        if rmc_data[6] == "W":
            decimal *= -1

        return decimal

    def parse_rmc(self, rmc_data):
        self.time = self.parse_time(rmc_data)
        self.latitude = self.parse_latitude(rmc_data)
        self.longitude = self.parse_longitude(rmc_data)
        try:
            self.speed = float(rmc_data[7])
        except ValueError:
            print("Value error for: " + rmc_data[7])

    def parse_gga(self, gga_data):
        self.fix_quality = int(gga_data[6])
        self.satellites = int(gga_data[7])
        try:
            self.altitude = float(gga_data[9])
        except ValueError:
            print("Value error for: " + gga_data[9])

    def parse(self):
        reading = self.ser.readline().decode("utf-8").split(',')
        if (reading[0] == "$GPRMC"):
            self.parse_rmc(reading)
        elif (reading[0] == "$GPGGA"):
            self.parse_gga(reading)

    def get_json(self):
        return {
            'rmc': {
                'time': self.time,
                'latitude': self.latitude,
                'longitude': self.longitude,
                'speed': self.speed,
            },
            'gga': {
                'satellites': self.satellites,
                'altitude': self.altitude,
                'fix_quality': self.fix_quality
            }
        }