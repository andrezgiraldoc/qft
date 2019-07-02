#include <stdio.h>
#include <iostream>
#include <vector>
#include <complex>
#include "qregister.hpp"

using namespace std;

int main() {
  QRegister qreg({{0,1}, {1,0}, {1/sqrt(2), 1/sqrt(2)}, {1/sqrt(2), -1/sqrt(2)}, {1/sqrt(2), {0,1/sqrt(2)}}});
  vector <complex<double> > result;
  result = qreg.getStateVector();
  qreg.applyQFT();

  result = qreg.getStateVector();
  cout<<"Final states: "<<endl;
  for (int i = 0; i < result.size(); i++)
    cout<<"result["<<i<<"] = "<<result[i]<<endl;
  
  return 0;
}