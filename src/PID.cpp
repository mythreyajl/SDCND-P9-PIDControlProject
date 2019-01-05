#include "PID.h"
#include <vector>
#include <iostream>

using namespace std;

/*
* TODO: Complete the PID class.
*/

PID::PID() {
  
  // Set initial values for the delta error factors
  p_error = 0.0;
  i_error = 0.0;
  d_error = 0.0;
  current_error = 0;
  error_tolerance = 0.2;
  
  state = eTwiddleState::STATE_WAIT;
  params.resize(3);
  delta_errors.resize(3);
  
  delta_errors = {0.1, 0.001, 1};
}

PID::~PID() {}

void PID::Init(double Kp, double Ki, double Kd) {

  // Set initial values for the parameters
  this->Kp = Kp;
  this->Ki = Ki;
  this->Kd = Kd;
}

void PID::UpdateError(double cte) {
  
  // Setting up errors
  if(timestamp == 0)
    p_error = cte;
  
  d_error = cte - p_error;
  p_error = cte;
  i_error += cte;
  
  AssignParamsToVec();
  
  // Update error only when allowed
  if(timestamp > t_settle)
    current_error += cte*cte;
  
  //
  if(TotalError() > error_tolerance) {
    
    if(timestamp == 0) {
      
      switch(state) {
        
        case eTwiddleState::STATE_WAIT: break;
        case eTwiddleState::STATE_BEGIN:
        {
          params[current_param] += delta_errors[current_param];
          state = eTwiddleState::STATE_CHANGE1;
          break;
        };
        case eTwiddleState::STATE_CHANGE1:
        {
          if(current_error < best_error) {
            
            best_error = current_error;
            std::cout << "Error reduced: " << best_error << std::endl;
            delta_errors[current_param] *= 1.1;
            current_param = (++current_param)%3;
            state =  eTwiddleState::STATE_BEGIN;
          } else {
            
            params[current_param] -= 2 * delta_errors[current_param];
            state = eTwiddleState::STATE_CHANGE2;
          }
          break;
        };
        case eTwiddleState::STATE_CHANGE2:
        {
          if(current_error < best_error) {
            
            best_error = current_error;
            std::cout << "Error reduced: " << best_error << std::endl;
            delta_errors[current_param] *= 1.1;
            current_param = (++current_param)%3;
          } else {
            
            params[current_param] += delta_errors[current_param];
            delta_errors[current_param] /= 1.1;
            current_param = (++current_param)%3;
          }
          state = eTwiddleState::STATE_BEGIN;
          break;
        };
        default: state = eTwiddleState::STATE_BEGIN; break;
      }
    }
  }
  
  AssignVecToParams();
  
  timestamp++;
  if(timestamp == t_evaluate + t_settle - 1) {
    timestamp = 0;
    current_error = 0.0;
    i_error = 0.0;
    if(state == eTwiddleState::STATE_WAIT)
      state = eTwiddleState::STATE_BEGIN;
  }
  std::cout << " Timestamp: " << timestamp << "; Error: " << current_error << "; p: {" << Kp << ", " << Ki << ", " << Kd << "}, dp: {" << p_error << ", " << i_error << ", " << d_error << "}" << std::endl;
}

double PID::TotalError() {
  
  return p_error + i_error + d_error;
}

