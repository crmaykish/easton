#include "joystick.h"
#include <Arduino.h>

void update_joystick(Joystick* j) {
    j->x->val = analogRead(j->x->pin);
    j->y->val = analogRead(j->y->pin);
    j->sw->val = analogRead(j->sw->pin);
}

int map_x(Joystick* j) {
    return map(j->x->val, j->x->min, j->x->max, -255, 255);
}

int map_y(Joystick* j) {
    return map(j->y->val, j->y->min, j->y->max, -255, 255);
}
