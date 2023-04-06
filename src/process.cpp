#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid): pid_nr(pid){}
// TODO: Return this process's ID
int Process::Pid() {
  return pid_nr;
}

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() const {
  return float(LinuxParser::ActiveJiffies(pid_nr)) / float(sysconf(_SC_CLK_TCK)) / float(LinuxParser::UpTime(pid_nr));
}

// TODO: Return the command that generated this process
string Process::Command() {
  return LinuxParser::Command(pid_nr);
}

// TODO: Return this process's memory utilization
string Process::Ram() {
  return LinuxParser::Ram(pid_nr);
}

// TODO: Return the user (name) that generated this process
string Process::User() {
  return LinuxParser::User(pid_nr);
}

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() {
  return LinuxParser::UpTime(pid_nr);
}

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const {
  return a.CpuUtilization() < this->CpuUtilization();
}
