#define _USE_MATH_DEFINES
#include <stdio.h>
#include <iostream>
#include <vector>
#include <complex>
#include <map>
#include <bitset>

using namespace std;

#define THRESHOLD 0.000000001

class Qubit {
  public:
    vector< complex<double> > coefficients;
    Qubit();
    Qubit(complex<double>, complex<double>);
    vector< complex<double> > getCoefficients() { return coefficients; };
    vector<double> getProbabilities();
};

Qubit::Qubit() {
  coefficients.push_back(1);
  coefficients.push_back(0);
}

Qubit::Qubit(complex<double> a, complex<double> b) {
  coefficients.push_back(a);
  coefficients.push_back(b);
}

class QRegister {
  private:
    int qubitsNumber;
    vector<Qubit> qubits;
    vector <complex<double> > stateVector;
  public:
    QRegister();
    QRegister(int);
    QRegister(vector< vector <complex<double> > >);
    int getNumberQubits() { return qubitsNumber; };
    vector<Qubit> getQubits() { return qubits; };
    vector <complex<double> > getStateVector() { return stateVector; };
    void applyQFT();
    vector <complex<double> > tensorProduct(vector<Qubit>);
    void matrixProduct(vector <complex<double> >, vector <complex<double> >);
};

QRegister::QRegister() {
  qubitsNumber = 1;
  Qubit qubit;
  qubits.push_back(qubit);
  stateVector.push_back(qubit.coefficients[0]);
  stateVector.push_back(qubit.coefficients[1]);
}

QRegister::QRegister(int n) {
  qubitsNumber = n;
  Qubit qubit;
  for (int i = 0; i < n; i++)
    qubits.push_back(qubit);
  stateVector = tensorProduct(qubits);
}

QRegister::QRegister(vector< vector <complex<double> > > qubitState) {
  qubitsNumber = qubitState.size();
  for (int i = 0; i < qubitsNumber; i++)
  {
    Qubit qubit(qubitState[i][0],qubitState[i][1]);
    qubits.push_back(qubit);
  }
  stateVector = tensorProduct(qubits);
}

void QRegister::applyQFT() {
  int n = qubitsNumber;
  int N = pow(2, n);
  vector <complex<double> > QFTGate;
  complex<double> wm;
  complex<double> imaginaryValue = {0,1};
  double angle = (2*M_PI)/pow(2,n);
  double realPart, imaginaryPart;
  for (int i = 0; i < N; i++)
  {
    for (int j = 0; j < N; j++)
    {
      if (i==0)
        QFTGate.push_back(1/sqrt(N));
      else {
        if (j==0)
          QFTGate.push_back(1/sqrt(N));
        else {
          realPart = cos(angle*i*j);
          imaginaryPart = sin(angle*i*j);
          if (abs(realPart) < THRESHOLD) realPart = 0;
          if (abs(imaginaryPart) < THRESHOLD) imaginaryPart = 0;
          wm = realPart + imaginaryValue*imaginaryPart;
          QFTGate.push_back(wm/sqrt(N));
        }
      }
    }
  }
  matrixProduct(QFTGate, stateVector);
}

vector <complex<double> > QRegister::tensorProduct(vector<Qubit> initialStatesMatrix) {
  const int MAX_QUBITS = 20;
  int n=initialStatesMatrix.size();
  complex<double> val;
  vector <complex<double> > finalStatesVector;
  string binary;
  for (int i=0; i<pow(2,n); i++)
  {
    binary = bitset<MAX_QUBITS>(i).to_string();
    val={1,0};
    for (int j=0; j<n; j++)
      val=val*initialStatesMatrix[j].getCoefficients()[binary.at((MAX_QUBITS-n)+j)-'0'];
    finalStatesVector.push_back(val);
  }
  return finalStatesVector;
}

void QRegister::matrixProduct(vector <complex<double> > operatorsVector, vector <complex<double> > statesVector) {
  int sqrtOperatorsSize = sqrt(operatorsVector.size());
  int statesSize = statesVector.size();
  vector <complex<double> > resultVector;
  complex<double> val;
  int countStatesVectorTimes = 0;
  int operatorIndex = 0;
  for (int i=0; i<sqrtOperatorsSize; i++) {
    val={0,0};
    for (int j=0; j<statesSize; j++) {
      operatorIndex = j + (countStatesVectorTimes*sqrtOperatorsSize);
      val=val+operatorsVector[operatorIndex]*statesVector[j];
    }
    if (abs(val.real()) < THRESHOLD) val.real(0);
    if (abs(val.imag()) < THRESHOLD) val.imag(0);
    resultVector.push_back(val);
    countStatesVectorTimes++;
  }
  stateVector = resultVector;
}