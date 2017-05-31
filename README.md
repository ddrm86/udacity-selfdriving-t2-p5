## Model Predictive Control Project
### David del Río Medina

---

## [Rubric](https://review.udacity.com/#!/rubrics/896/view) Points

#### 1. Student describes their model in detail. This includes the state, actuators and update equations.

In every iteration, the model receives a series of waypoints from the reference path the vehicle should follow, the vehicle position, the heading angle and the speed. Using this data, the model must decide how much throttle and steering to apply, and in which direction. This is done in several steps:

1. The model receives all the data in global map coordinates. Dealing with car coordinates is easier, so the first step is converting the waypoints to local vehicle coordinates.
2. Once the waypoints are converted, an order two polynomial is fitted to these waypoints. This is the path the vehicle will try to follow.
3. The Cross Track Error (CTE) and Orientation Error (EPSI) are calculated using the fitted polynomial.
4. The fitted polynomial and the current state of the car (speed, CTE and EPSI) are passed to the Ipopt solver, that predicts the best steering and throttle values for the next `N` steps of `dt` seconds. The solver finds these values by minimizing a given cost function within the boundaries of a given set of constraints.

##### Cost function

The cost function is the same that was proposed in the lesson 19 quiz. Basically, the total cost is the sum of all the things we want to penalize:

* Deviation from the desired CTE, EPSI and speed. We want both errors to be as close to zero as possible (meaning the car stays on track and heading correctly, and the speed as close as the reference speed (35mph in my case) as possible.
* Excessive use of actuators. We want the vehicle to use the throttle and the steering only when necessary.
* Harsh use of the actuators. We want the vehicle to smooth the changes applied to an actuator, avoiding sudden steering, acceleration and braking.

The problem with this cost function is that it takes into account several variables of different magnitude. In order to get better results, weights were applied to some parts of the cost calculation:

*  CTE is bigger in magnitude than EPSI, so the former is multiplied by 0.1 and the latter by 1.5. I found that this results in a smoother driving, since the car does not wait that long to correct the heading.
* The cost of drifting from the reference speed is lowered by a half (weight of 0.5). Without this, despite penalizing the excessive use of actuators, the vehicle sometimes keeps braking and accelerating, trying hard to keep the reference speed.

All these weights were found by trial and error. 

##### Constraints

The solver needs a set of constraints that model the problem to come up with actual solutions.
Basically, for all the variables that determine the state of the model, we need to establish the initial value, the lower and upper bounds and how the value of each variable changes from one timestep to the next one (i.e. the vehicle movement model).

I have implemented the global kinematic model proposed in lesson 19, adding CTE and EPSI update equations.

#### 2. Student discusses the reasoning behind the chosen N (timestep length) and dt (timestep frequency) values. Additionally the student details the previous values tried.

By trial and error, I found that `N = 10` and `dt = 0.1` works well.
 Smaller values of `N` makes the vehicle react too late to curves, due to lack of predictive power. Bigger values do not seem to make any difference, so it is better to save computational resources.
 
 Bigger values of `dt` have the same effect than a small `N`: lack of reaction speed. Smaller values do not seem to improve driving, maybe due to the 100ms latency. 
 
#### 3. The student implements Model Predictive Control that handles a 100 millisecond latency. Students provide details on how they deal with latency.

With the chosen `N` and `dt` parameters there is no need to take special actions to deal with latency, which makes sense, since the chosen timestep frequency is the same as the latency.

With other `dt` values that I tried, a simple solution is to take advantage of the fact that the solver gives the actuators values for the next `N` timesteps, so we can choose the values the solver finds optimal for the next 2nd to Nth timesteps instead of the current ones.