#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::stol;
using std::stoi;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  string total, free, buffers, line;
  string memTotal;
  float memTotalF;
  string memFree;
  float memFreeF;
//   string memBuffers;
//   float memBuffersF;
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()){
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> total >> memTotal;
    try
    {
      memTotalF = stof(memTotal);
    }
    catch(...)
    {
      memTotalF = 0;
    }

    std::getline(filestream, line);
    std::istringstream linestream1(line);
    linestream1 >> free >> memFree;
    try
    {
      memFreeF = stof(memFree);
    }
    catch(...)
    {
      memFreeF = 0;
    }

//     std::getline(filestream, line);

//     std::getline(filestream, line);
//     std::istringstream linestream2(line);
//     linestream2 >> buffers >> memBuffers;
//     try
//     {
//       memBuffersF = stof(memBuffers);
//     }
//     catch(...)
//     {
//       memBuffersF = 0;
//     }
  }
  // Memory Utilization = 1.0 - (free_memory / (total_memory - buffers))
  return 1.0 - memFreeF / memTotalF;
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() {
  string line;
  long uptime;
  string uptimeS;
  std::ifstream filestream(kProcDirectory + kUptimeFilename);
  if (filestream.is_open()){
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> uptimeS;
    try
    {
      uptime = stol(uptimeS);
    }
    catch(...)
    {
      uptime = 0;
    }
  }
  return uptime;
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() {
  return ActiveJiffies() + IdleJiffies();
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) {
  string line, nth;
  long activeJ{0};
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
  if (filestream.is_open()){
    std::getline(filestream, line);
    std::istringstream linestream(line);
    for (int i = 0; i < 14; ++i){
      linestream >> nth;
    }
    try
    {
      activeJ += stol(nth);
    }
    catch(...)
    {
      activeJ += 0;
    }
    for (int i = 0; i < 3; i++){
      linestream >> nth;
      try
      {
        activeJ += stol(nth);
      }
      catch(...)
      {
        activeJ += 0;
      }
    }
  }
  return activeJ;
}

long LinuxParser::TimeElapsed(int pid) {
  string line, nth;
  long piduptime = UpTime(pid);
  long systemuptime{0};
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
  if (filestream.is_open()){
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> nth;
    try
    {
      systemuptime += stol(nth);
    }
    catch(...)
    {
      systemuptime += 0;
    }
  }
  return systemuptime - piduptime;
}


// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
  string line, nth;
  long activeJ{0};
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()){
    std::getline(filestream, line);
    std::istringstream linestream(line);
    for (int i = 0; i < 3; ++i){
      linestream >> nth;
      try
      {
        activeJ += stol(nth);
      }
      catch(...)
      {
        activeJ += 0;
      }
    }
    for (int i = 0; i < 2; ++i){
      linestream >> nth;
    }
    for (int i = 0; i < 3; ++i){
      linestream >> nth;
      try
      {
        activeJ += stol(nth);
      }
      catch(...)
      {
        activeJ += 0;
      }
    }
    linestream >> nth;

  }
  return activeJ;
}



// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
  string line, nth;
  long idle, iowait;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()){
    std::getline(filestream, line);
    std::istringstream linestream(line);
    for (int i = 0; i < 4; ++i){
      linestream >> nth;
    }
    try
    {
      idle = stol(nth);
    }
    catch(...)
    {
      idle = 0;
    }
    linestream >> nth;
    try
    {
      iowait = stol(nth);
    }
    catch(...)
    {
      iowait = 0;
    }
  }
  return idle + iowait;
}

// TODO: Read and return CPU utilization
float LinuxParser::CpuUtilization() {
//   long previdle = IdleJiffies();
//   long prevactive = ActiveJiffies();
//   long prevtotal = previdle + prevactive;
//   sleep(1);
  long idle = IdleJiffies();
  long active = ActiveJiffies();
  long total = idle + active;

//   long deltatotal = total - prevtotal;
//   long deltaidle = idle - previdle;
//   return (deltatotal - deltaidle) / deltatotal;
  return (total - idle) / total;
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "processes") {
          try
          {
            return stoi(value);
          }
          catch(...)
          {
            return 0;
          }
        }
      }
    }
  }
  return 0;
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "procs_running") {
          try
          {
            return stoi(value);
          }
          catch(...)
          {
            return 0;
          }
        }
      }
    }
  }
  return 0;
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) {
  string line;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if (filestream.is_open()){
    std::getline(filestream, line);
  }
  return line;
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) {
  string line;
  string key;
  string value;
  long value_in_mb;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "VmSize:") {
          value_in_mb = stol(value) / 1024;
          return to_string(value_in_mb);
        }
      }
    }
  }
  return value;
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) {
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "Uid:") {
          return value;
        }
      }
    }
  }
  return value;
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) {
  string u_id, key, oneLetter, value, line;
  u_id = Uid(pid);
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> oneLetter >> value) {
        if (value == u_id) {
          return key;
        }
      }
    }
  }
  return key;
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) {
  string line, nth;
//   long up_time, start_time;
  long start_time;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
  if (filestream.is_open()){
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> nth;
//     try
//     {
//       up_time = stol(nth);
//     }
//     catch(...)
//     {
//       up_time = 0;
//     }
    for (int i = 0; i < 21; ++i){
      linestream >> nth;
    }
    try
    {
      start_time = stol(nth);
    }
    catch(...)
    {
      start_time = 0;
    }
  }
  return start_time / sysconf(_SC_CLK_TCK);
//   return up_time - start_time / sysconf(_SC_CLK_TCK);
}
