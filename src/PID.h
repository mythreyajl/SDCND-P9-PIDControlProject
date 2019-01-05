#ifndef PID_H
#define PID_H

#include <vector>

class PID {
public:
  /*
  * Errors
  */
  double p_error;
  double i_error;
  double d_error;

  /*
  * Coefficients
  */ 
  double Kp;
  double Ki;
  double Kd;
    
  /*
  * Twiddle parameters
  */
  enum eTwiddleState {
    STATE_WAIT,
    STATE_BEGIN, // Initial state, delta errors are just initialized
    STATE_CHANGE1, // Delta error factor was increased: d_error *= 1.1
    STATE_CHANGE2 // Error was decreased
  };
  
  // Timing
  int timestamp;
  int t_settle = 100;
  int t_evaluate = 100;
  
  // Error
  double best_error = __DBL_MAX__;
  double current_error;
  double error_tolerance;
  int current_param = 0;
  eTwiddleState state = eTwiddleState::STATE_BEGIN;
  std::vector<double> params, delta_errors;

  /*
  * Constructor
  */
  PID();

  /*
  * Destructor.
  */
  virtual ~PID();

  /*
  * Initialize PID.
  */
  void Init(double Kp, double Ki, double Kd);

  /*
  * Update the PID error variables given cross track error.
  */
  void UpdateError(double cte);

  /*
  * Calculate the total PID error.
  */
  double TotalError();
  
  /*
   * Assign params to vector for convenience
   */
  void AssignParamsToVec() {
    params = {Kp, Ki, Kd};
  }
  
  /*
   * Assign params to vector for convenience
   */
  void AssignVecToParams() {
    Kp = params[0];
    Ki = params[1];
    Kd = params[2];
  }
  
  
};

#endif /* PID_H */
