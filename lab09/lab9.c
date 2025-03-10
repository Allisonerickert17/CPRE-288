#include "Timer.h"
#include "adc.h"
#include "cybot_uart.h"
#include "lcd.h"
#include "math.h"
#include "ping.h"
#include "ping_distance.h"
#include "servo.h"
#include "stdbool.h"


int start_angle = 0;
int stop_angle = 0;
int object_distance = 0;
/*
 * lab9.c
 *
 *  Created on: Apr 29, 2020
 *      Author: Seth Braddock
 */

int get_start_angle() { return start_angle; }
int get_stop_angle() { return stop_angle; }
int get_object_distance() { return object_distance; }

void send_String(char str[]) {
  int j = 0;
  while (str[j] != '\0') {
    cyBot_sendByte(str[j]);
    j++;
  }
}

void send_to_putty(float data) {
  char val[4];
  sprintf(val, "%f", data);
  send_String(val);
  cyBot_sendByte('\t');
}

float ir_convert() {
  double numerator = 3.3 * adc_read();
  double voltage = numerator / 4095;
  float ir_dist = 39.606 * pow(voltage, -1.874);

  return ir_dist;
}

void scan_init() {
  cyBot_uart_init_clean(); // Clean UART initialization, before running your
                           // UART GPIO init code

  SYSCTL_RCGCGPIO_R |= 0b00000010; // system clock
  while ((SYSCTL_PRGPIO_R & 0b10) == 0) {
  };

  GPIO_PORTB_AFSEL_R |= 0x03; // select UART Rx and Tx
  GPIO_PORTB_PCTL_R &= 0xFFFFFF00;
  GPIO_PORTB_PCTL_R |= 0x00000011;

  GPIO_PORTB_DEN_R |= 0b00000011; // pin direction and digital enable
  GPIO_PORTB_DIR_R &= 0xFFFFFF00;
  GPIO_PORTB_DIR_R |= 0b00000010;

  cyBot_uart_init_last_half(); // Complete the UART device configuration
}

void sensor_sweep() {

  // send header labels
  char header[] = "Angle\tPING Distance\tIR raw value";
  send_String(header);

  cyBot_sendByte('\n');
  cyBot_sendByte('\r');

  int temp = 0; // vars for comparing IR values
  int current = 0;

  int i;
  for (i = 0; i <= 180; i += 2) // Scan and print
  {
    servo_move(i); // position servo

    // send position
    send_to_putty(i);

    // CALLS PING DISTANCE TO DISPLAY ON LCD
    float radius = ping_distance();
    bool object_detected = false;

    // Send ping value
    send_to_putty(radius);

    // CONVERT RAW IR VALUE
    float ir_dist = ir_convert();

    // Send IR value
    send_to_putty(ir_dist);

    // Newline and return
    cyBot_sendByte('\n');
    cyBot_sendByte('\r');

    //
    // CHECKS TO UPDATE IF OBJECT THRESHOLD PRESENT

    if (i == 0) // compare current to temp
    {
      current = ir_dist;

    } else {
      temp = current;
      current = ir_dist;
    }

    if (current - temp >= 10 || current - temp <= -10) {
      if (!object_detected) // 10 cm threshold value
      {
        object_detected = true;
        start_angle = i;
        object_distance = radius;
      } else {
        object_detected = false;
        stop_angle = i;
      }
    }
  }
}

float convert_distance(float angle, float radius) {
  float angle_rad = angle * (3.14 / 180);

  float linear_distance = angle_rad * radius;

  return linear_distance;
}
