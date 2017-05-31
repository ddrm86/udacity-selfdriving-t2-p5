#include "utils.h"

namespace utils {

double polyeval(Eigen::VectorXd coeffs, double x) {
  double result = 0.0;
  for (int i = 0; i < coeffs.size(); i++) {
    result += coeffs[i] * pow(x, i);
  }
  return result;
}

AD<double> polyeval(Eigen::VectorXd &p, AD<double> x) {
  AD<double> ret = 0.0;
  AD<double> xpow = 1.0;
  for (int i=0; i<p.size(); i++) {
    ret += p(i) * xpow;
    xpow *= x;
  }
  return ret;
}

Eigen::VectorXd polyfit
  (Eigen::VectorXd xvals, Eigen::VectorXd yvals, int order) {
  
  assert(xvals.size() == yvals.size());
  assert(order >= 1 && order <= xvals.size() - 1);
  Eigen::MatrixXd A(xvals.size(), order + 1);

  for (int i = 0; i < xvals.size(); i++) {
    A(i, 0) = 1.0;
  }

  for (int j = 0; j < xvals.size(); j++) {
    for (int i = 0; i < order; i++) {
      A(j, i + 1) = A(j, i) * xvals(j);
    }
  }

  auto Q = A.householderQr();
  auto result = Q.solve(yvals);
  return result;
}

vector<double> toCarCoords
    (double psi, double x_c, double y_c, double x_p, double y_p) {
  double x_ptr = (x_p - x_c) * cos(psi) + (y_p - y_c) * sin(psi);
  double y_ptr = (y_p - y_c) * cos(psi) - (x_p - x_c) * sin(psi);
  return {x_ptr, y_ptr};
}

Eigen::VectorXd vectorToEigen(vector<double> v) {
  double* v_pointer = &v[0];
  Eigen::Map<Eigen::VectorXd> eigen_v(v_pointer, v.size());
  return eigen_v;
}

}

