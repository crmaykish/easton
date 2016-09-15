#ifndef JOYSTICK_H_
#define JOYSTICK_H_

struct Joystick_Axis {
    int pin;
    int min;
    int max;
    int mid;
    int val;
};

struct Joystick_Switch {
    int pin;
    bool val;
};

struct Joystick {
    Joystick_Axis* x;
    Joystick_Axis* y;
    Joystick_Switch* sw;
};

void update_joystick(Joystick* j);
int map_x(Joystick* j);
int map_y(Joystick* j);

#endif
