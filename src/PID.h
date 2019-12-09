#ifndef PID_H
#define PID_H

class PID {
 public:
  /**
   * Constructor
   */
  PID();

  /**
   * Destructor.
   */
  virtual ~PID();

  /**
   * Initialize PID.
   * @param (Kp_, Ki_, Kd_) The initial PID coefficients
   */
  void Init(double Kp_, double Ki_, double Kd_);

  /**
   * Update the PID error variables given cross track error.
   * @param cte The current cross track error
   */
  void UpdateError(double cte);
  

  bool UpdateTwiddleError(double cte);
  /**
   * Calculate the total PID error.
   * @output The total PID error
   */
  double TotalError();

 private:
  void UpdateParam();
  /**
   * PID Errors
   */
  double p_error;
  double i_error;
  double d_error;
  double best_error;
  
  /**
   * PID Coefficients
   */ 
  double Kp;
  double Ki;
  double Kd;
  double dKp;
  double dKi;
  double dKd;

  bool second_run;
  int paramUnderTest;
};

#endif  // PID_H
