#include<cmath>

extern "C" {

  double cos_wrapper(const double x){
    return std::cos(x);
  }
  
}
