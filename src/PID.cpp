#include "PID.h"

using namespace std;

/*
* TODO: Complete the PID class.
*/

PID::PID() {}

PID::~PID() {}

void PID::Init(double Kp, double Ki, double Kd) {
  this->Kp = Kp;
  this->Ki = Ki;
  this->Kd = Kd;

  // _frame_cnt = 0;

}


void PID::UpdateError(double cte) {
  // https://discussions.udacity.com/t/trying-to-understand-the-interaction-of-pid-and-the-car/337260/2?u=sun.pochin
  // https://discussions.udacity.com/t/car-oscillating-out-of-control-even-with-kp/350071/6?u=sun.pochin
  // d = current cte - previous cte.
  d_error = (cte - p_error) / 1; // 1 == time_delta
  // p = current cte.
  p_error = cte;
  // i = integral/sum of cte.
  i_error = i_error + cte;
  // ++_frame_cnt;
}

double PID::TotalError() {
  double _steer = -p_error * Kp - d_error * Kd - i_error * Ki;
  return _steer;
}
