# CarND-Controls-PID


## Reflection
Here I describe in some detail the PID controller and the hyperparameters I chose, twiddled and froze for the final submission.

### Effect of P.I.D. components:
Each component had a major role to play in the fully functional controller.

#### P - Proportionality
This component was the crux of the controller as explained in the vidoes. The proportionality component makes the controller steer in proportion to the error. However as the video shows, this introduces ringing and overshooting because it is impossible to reach zero steering angle even on a relatively straight road. This is illustrated in the video link below:

![Effect of using only P control](Videos/P-Steer.mov)

#### D - Deravative
This component controls the 2nd order effect. i.e., this acts on the rate of change of cross-track error. What this ensures is more order in the steering control by making sure that the controller doesn't overreact by oversteering when the curvature of the road is high. Applying steers twice hurts badly. So the D control is used in conjunction with the P controller to reward more steering only when the change is appreciable. It also helps in understeering. The effect of P+D control is illustrated in the video link below:

![Effect of PD control](Videos/PD-Steer.mov)

#### I - Integral
The Integral component as the name suggests applies control action based on the duration of the persistent input signal being observed. These are particularly useful when there is an offset between the desired result and the observed result. The PID controller illustrated in the video link below:

![Effect of PID control](Videos/PID-Steer.mov)

The effects of each of the components look fairly evident in the videos

### Tuning - Hyperparameter choice
In order to tune this algorithm I followed the sequence described below:
1. I implemented twiddling.
2. Following this, I initialized the parameters as the video lectures suggested. 
3. The above step didn't yeild the results I was expecting. My issue was that I was overshooting significantly and on later examination, my intergral component was way too high. 
4. Also, the 'dp' for the tuning of the parameters were too high. 
5. After a log-binary search type manual parameter tuning, I was able to arrive at parameters that were satisfactory.

