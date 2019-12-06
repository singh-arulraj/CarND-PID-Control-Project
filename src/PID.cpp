#include "PID.h"
#include <iostream>
/**
 * TODO: Complete the PID class. You may add any additional desired functions.
 */

PID::PID() {}

PID::~PID() {}
using namespace std;
void PID::Init(double Kp_, double Ki_, double Kd_, bool run_withtwiddle) {
  /**
   * TODO: Initialize PID coefficients (and errors, if needed)
   */
    Kp = Kp_;
    Ki = Ki_;
    Kd = Kd_;
    i_error = 0;
    p_error = 0.0;
    best_error = 99999.0;
    dKp = 1;
    dKi = 1;
    dKd = 1;
    second_run = false;
#ifdef RUN_TWIDDLE
    if (run_withtwiddle) {
    UpdateParam();
    }
#endif
}

void PID::UpdateError(double cte) {
  /**
   * TODO: Update PID errors based on cte.
   */  
  i_error += cte;
  d_error = (cte - p_error);
  p_error = cte;
}

double PID::TotalError() {
  /**
   * TODO: Calculate and return the total error
   */
  return (Kp * p_error + Kd * d_error + Ki * i_error);  // TODO: Add your total error calc here!
}

void PID::UpdateParam() {
    Kp += dKp;
    Kd += dKd;
    Ki += dKi;
}

bool PID::UpdateTwiddleError(double cte) {
    this->UpdateError(cte);
    double current_error = this->TotalError();
    double sum = dKp + dKi + dKd;
    if (sum < 0.001) {
        std::cout << "Kp = " << Kp << ", Ki = " << Ki << ", Kd = " << Kd << "\n";
        return true;
    }
    
    if  (cte < this->best_error) {
        this->best_error = cte;
        dKp *= 1.1;
        dKi *= 1.1;
        dKd *= 1.1;
        second_run = false;
        std::cout << "Updating by 1.1 \n";
        this->UpdateParam();
    } else {
        if (!second_run) {
        Kp -= dKp * 2;
        Ki -= dKi * 2;
        Kd -= dKd * 2;
        second_run = true;
        std::cout << "Updating by 2\n";
        } else {
            this->UpdateParam();
            dKp *= 0.9;
            dKd *= 0.9;
            dKi *= 0.9;
            second_run = false;
            std::cout << "Updating by 0.9\n";
        }
    }
    return false;

}
