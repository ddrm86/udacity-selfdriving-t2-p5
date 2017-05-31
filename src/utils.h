#ifndef UTILS_H
#define UTILS_H

#include "Eigen-3.3/Eigen/Core"
#include "Eigen-3.3/Eigen/QR"
#include <cppad/cppad.hpp>
#include <cppad/ipopt/solve.hpp>
#include <math.h>
#include <vector>

using std::vector;
using CppAD::AD;

namespace utils {
  // Evaluate a polynomial.
  double polyeval(Eigen::VectorXd coeffs, double x);
  
  AD<double> polyeval(Eigen::VectorXd coeffs, AD<double> x);
  
  // Fit a polynomial.
  // Adapted from
  // https://github.com/JuliaMath/Polynomials.jl/blob/master/src/Polynomials.jl#L676-L716
  Eigen::VectorXd polyfit
    (Eigen::VectorXd xvals, Eigen::VectorXd yvals, int order);
  
  /**
  * From https://discussions.udacity.com/t/not-able-to-display-trajectory-and-reference-paths-in-the-simulator/248545/9?u=david_29805351951199
  * psi - car's heading in map coordinates
  * (x_c, y_c) - car's position in map coordinates
  * (x_p, y_p) - point position in map coordinates
  * returns {x', y'} of the point's coordinate in car coordinates
  */  
  vector<double> toCarCoords
    (double psi, double x_c, double y_c, double x_p, double y_p);    
    
  // From https://forum.kde.org/viewtopic.php?f=74&t=94839
  Eigen::VectorXd vectorToEigen(vector<double> v);
}

#endif

