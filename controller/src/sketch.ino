// Trogdor Controller
#include <SPI.h>
#include "RF24.h"       // https://github.com/TMRh20/RF24
#include "joystick.h"

Joystick_Axis lx = {A1, 0, 1023, 497, 0};
Joystick_Axis ly = {A0, 0, 1023, 512, 0};
Joystick_Switch ls = {A2, LOW};
Joystick left_stick{&lx, &ly, &ls};

Joystick_Axis rx = {A4, 0, 1023, 504, 0};
Joystick_Axis ry = {A3, 0, 1023, 503, 0};
Joystick_Switch rs = {A7, LOW};
Joystick right_stick{&rx, &ry, &rs};

RF24 radio(7, 8);
byte address[6] = "1test";
int radio_buffer[6] = {0, 0, 0, 0, 0, 0};

void setup()
{
    radio.begin();
    radio.openWritingPipe(address);
}

void loop()
{
    update_joystick(&left_stick);
    update_joystick(&right_stick);

    radio_buffer[0] = map_x(&left_stick);
    radio_buffer[1] = map_y(&left_stick);
    radio_buffer[2] = !ls.val;
    radio_buffer[3] = map_x(&right_stick);
    radio_buffer[4] = map_y(&right_stick);
    radio_buffer[5] = !rs.val;

    radio.write(&radio_buffer, sizeof(radio_buffer));
}

void debug() {
    Serial.print("lx: ");
    Serial.print(lx.val);
    Serial.print(" ly: ");
    Serial.print(ly.val);
    Serial.print(" ls: ");
    Serial.print(ls.val);
    Serial.print(" rx: ");
    Serial.print(rx.val);
    Serial.print(" ry: ");
    Serial.print(ry.val);
    Serial.print(" rs: ");
    Serial.println(rs.val);
}
