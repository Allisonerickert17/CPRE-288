/*
 * movement.h
 *
 *  Created on: Feb 1, 2024
 *      Author: seshenou
 */

#ifndef MOVEMENT_H_
#define MOVEMENT_H_
#include "open_interface.h"


void move_forward (oi_t*sensor, double distance_mm);

void turn_right(oi_t *sensor, double degrees);

void turn_left(oi_t *sensor, double degrees);

void bumpRight(oi_t*sensor);

void bumpLeft(oi_t*sensor);

void moveBack(int mm, oi_t*sensor);

#endif /* MOVEMENT_H_ */
