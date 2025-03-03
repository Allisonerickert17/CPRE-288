/*
 * movement.c
 *
 *  Created on: Feb 1, 2024
 *      Author: seshenou
 */

#include "open_interface.h"
#include "lcd.h"
#include "Timer.h"




void turn_right(oi_t *sensor, double degrees);
void turn_left(oi_t *sensor, double degrees);
void bumpRight(oi_t*sensor);
void bumpLeft(oi_t*sensor);
void moveBack(int mm, oi_t*sensor);

void move_forward (oi_t*sensor, double distance_mm){
    oi_setWheels(500, 500);
    double sum1 = 0;
    while (sum1 < distance_mm){
        oi_update(sensor);
        sum1 += sensor -> distance; // use -> notation since pointer
        if (sensor->bumpLeft) {
            bumpLeft(sensor);
            move_forward( sensor, (distance_mm - sum1+ 250));



       }


        else if (sensor->bumpRight) {
               bumpRight(sensor);
               move_forward( sensor, (distance_mm - sum1+250));

          }


}
    oi_setWheels(0,0);
}



void moveBack(int mm, oi_t*sensor){

    oi_setWheels(0,0); // stop
    double amount = 0;
    oi_setWheels(-500, -500);
    while(amount<mm){

        oi_update(sensor);
        amount -= sensor ->distance;
    }
    oi_setWheels(0,0);
}

void turn_right(oi_t *sensor, double degrees){
    oi_setWheels(-250, 250);
    double sum3 = 0;

    while(sum3 < fabs(degrees - 10)){// sum3 < abs(degrees)
        oi_update(sensor);
        sum3 += fabs(sensor -> angle); // use -> notation since pointer || -=

    }
    oi_setWheels(0,0);
}

void turn_left(oi_t *sensor, double degrees){
    oi_setWheels(250, -250);
      double sum2 = 0;

      while(sum2 < degrees-10){
          oi_update(sensor);
          sum2 += fabs(sensor -> angle); // use -> notation since pointer
          }
      oi_setWheels(0,0);

}

void bumpLeft(oi_t*sensor){
    moveBack(150, sensor);
    turn_right(sensor, 90.0);
    move_forward(sensor, 250);
    turn_left(sensor, 90.0);
}

void bumpRight(oi_t*sensor){
    moveBack(150, sensor);
    turn_left(sensor, 90.0);
    move_forward(sensor, 250);
    turn_right( sensor, 90.0);

}








