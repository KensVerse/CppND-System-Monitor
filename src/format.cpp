#include <string>
#include<cmath>
#include "format.h"

using std::string;
using std::remainderl;
using std::to_string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) {
  long minute, hour, second;
  long remainder1;
  string hString, mString, sString;
  hour = seconds / 3600;
  remainder1 = seconds % 3600;
  minute = remainder1 / 60;
  second = remainder1 % 60;
  hString = to_string(hour);
  if (minute < 10){
    mString = '0' + to_string(minute);
  } else {
    mString = to_string(minute);
  }
  if (second < 10){
    sString = '0' + to_string(second);
  } else {
    sString = to_string(second);
  }
  return hString + ':' + mString + ':' + sString;
}
