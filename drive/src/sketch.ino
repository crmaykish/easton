// Trogdor Motor Drive and Radio Board
#include <SPI.h>
#include "RF24.h"

#define BAUD  115200

// Motors
const int L_FORWARD = 5;
const int L_REVERSE = 6;
const int R_FORWARD = 9;
const int R_REVERSE = 10;

// Radio
RF24 radio(7,8);
const byte address[6] = "1test";
int radio_buffer[6] = {0, 0, 0, 0, 0, 0};

// Serial buffer
const int COMMAND_LENGTH = 16;
char serial_buffer[COMMAND_LENGTH + 1];
char serial_buffer_size = 0;

// Timers
unsigned long report_timer = 0;
const long report_timer_max = 100;

// Telemetry
float heading = 0.0;
int l_speed = 0;
int r_speed = 0;

void setup() {
    // Motor pins
    pinMode(L_FORWARD, OUTPUT);
    pinMode(L_REVERSE, OUTPUT);
    pinMode(R_FORWARD, OUTPUT);
    pinMode(R_REVERSE, OUTPUT);

    // Communication
    Serial.begin(BAUD);
    radio.begin();
    radio.openReadingPipe(1,address);
    radio.startListening();
}

void loop() {
    if (radio.available()) {
        radio.read(&radio_buffer, sizeof(radio_buffer));
        set_motor(0, radio_buffer[0]);
        set_motor(1, radio_buffer[1]);
    }
    // TODO: shut motors off on a radio timeout

    // Check for incoming commands
    // read_commands();

    // Send reports on a timer
    if (millis() - report_timer >= report_timer_max) {
        report_speed();
        report_timer = millis();
    }

    // Set motor speeds and directions
    // motors();
}

void motors() {
    if (r_speed >= 0) {
        analogWrite(R_REVERSE, 0);
        analogWrite(R_FORWARD, r_speed);
    } else {
        analogWrite(R_FORWARD, 0);
        analogWrite(R_REVERSE, r_speed * -1);
    }

    if (l_speed >= 0) {
        analogWrite(L_REVERSE, 0);
        analogWrite(L_FORWARD, l_speed);
    } else {
        analogWrite(L_FORWARD, 0);
        analogWrite(L_REVERSE, l_speed * -1);
    }
}

void read_commands() {
    if (Serial.available() > 0) {
        serial_buffer[serial_buffer_size] = Serial.read();

        if (serial_buffer[serial_buffer_size] == '\r' || serial_buffer[serial_buffer_size] == '\n' || serial_buffer_size == COMMAND_LENGTH) {
            // Process command
            char* token = strtok(serial_buffer, ",");

            if (strcmp(token, "d") == 0) {
                // Drive command: look for two more values
                int motor = atoi(strtok(NULL, ","));
                int speed = atoi(strtok(NULL, ","));
                set_motor(motor, speed);
            }

            // Clear buffer
            for (int i = 0; i < serial_buffer_size; i++) {
                serial_buffer[i] = (byte) 0;
            }
            serial_buffer_size = 0;
        } else {
            serial_buffer_size++;
        }
    }
}

void report_speed() {
    Serial.print("speed,");
    Serial.print(l_speed);
    Serial.print(",");
    Serial.println(r_speed);
}

void report_compass() {
    Serial.print("compass,");
    Serial.println(heading);
}

void set_motor(int motor, int speed) {
    // Make sure parameters are in bounds
    if ((motor != 0 && motor != 1) || speed < -255 || speed > 255) {
        return;
    }

    if (motor == 0) {
        l_speed = speed;
    } else {
        r_speed = speed;
    }
}
