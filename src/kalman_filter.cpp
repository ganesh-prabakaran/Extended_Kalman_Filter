#include "kalman_filter.h"

using Eigen::MatrixXd;
using Eigen::VectorXd;

// Please note that the Eigen library does not initialize 
// VectorXd or MatrixXd objects with zeros upon creation.

KalmanFilter::KalmanFilter() {}

KalmanFilter::~KalmanFilter() {}

void KalmanFilter::Init(VectorXd &x_in, MatrixXd &P_in, MatrixXd &F_in,
                        MatrixXd &H_in, MatrixXd &R_in, MatrixXd &Q_in) {
  x_ = x_in;
  P_ = P_in;
  F_ = F_in;
  H_ = H_in;
  R_ = R_in;
  Q_ = Q_in;
}

void KalmanFilter::Predict() {
  /**
    * predict the state
  */
  
  x_ = F_ * x_; 
  /*
  if (fabs(x_[0]) < 0.00001) {
    x_[0] = 0.00001;
  }
  
  if (fabs(x_[1]) < 0.00001) {
    x_[1] = 0.00001;
  }
  */
  P_ = F_ * P_ * F_.transpose() + Q_;
}

void KalmanFilter::Update(const VectorXd &z) {
  /**
    * update the state by using Kalman Filter equations
  */
  VectorXd z_pred = H_ * x_;
  VectorXd y = z - z_pred;
  MatrixXd Ht = H_.transpose();
  MatrixXd S = H_ * P_ * Ht + R_;
  MatrixXd Si = S.inverse();
  MatrixXd K = P_ * Ht * Si;
  

  //new estimate
  x_ = x_ + (K * y);
  /*
  if (fabs(x_[0]) < 0.00001) {
    x_[0] = 0.00001;
  }
  
  if (fabs(x_[1]) < 0.00001) {
    x_[1] = 0.00001;
  }
  */
  long x_size = x_.size();
  MatrixXd I = MatrixXd::Identity(x_size, x_size);
  P_ = (I - K * H_) * P_;

}

void KalmanFilter::UpdateEKF(const VectorXd &z) {
  /**
   * update the state by using Extended Kalman Filter equations
  */
  float xt = x_[0];
  float yt = x_[1];
  float vx = x_[2];
  float vy = x_[3];

  float rho = sqrt(xt * xt + yt * yt );
  float theta = atan2(yt,xt);
  
/*   if (rho < 0.0001) {
       rho = 0.0001;
  } */
  float ro_dot = (xt*vx+yt*vy)/rho;
  VectorXd z_pred = VectorXd(3);
  z_pred << rho, theta, ro_dot;
  
  VectorXd y = z - z_pred;
  y(1) = atan2(sin(y(1)),cos(y(1)));
  
  MatrixXd Ht = H_.transpose();
  MatrixXd S = H_ * P_ * Ht + R_;
  MatrixXd Si = S.inverse();
  MatrixXd K = P_ * Ht * Si;

  //new estimate
  x_ = x_ + (K * y);
  /*
    if (fabs(x_[0]) < 0.00001) {
    x_[0] = 0.00001;
  }
  
  if (fabs(x_[1]) < 0.00001) {
    x_[1] = 0.00001;
  }
  */
  long x_size = x_.size();
  MatrixXd I = MatrixXd::Identity(x_size, x_size);
  P_ = (I - K * H_) * P_;

}

