#include <filesystem>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <locale>
#include <algorithm>
#include <process.hpp>
#include <scheduler.hpp>
#include <fcfssched.hpp>
#include <rrsched.hpp>
#include <getopt.h>
#include <stdlib.h>

void loadProcess(SchedulerSimulator&, std::filesystem::path&);
void usage(std::filesystem::path&);
SchedulerAlgorithm getSchedulerAlgorithm(std::string&);

int
main(int argc, const char *argv[]) {

  
  const char *filename;
  std::filesystem::path progname {argv[0]};
  
  if (argc == 2) {
    filename = argv[1];
  }
  else {
    usage(progname);
  }
  
  SchedulerSimulator* sim {getSchedulerAlgorithm(RR)};

  std::filesystem::path schedulerFile { filename };

  loadProcess(*sim, schedulerFile);

  sim->start();
  sim->end();

  return 0;
}

SchedulerSimulator*
getSchedulerAlgorithm(SchedulerAlgorithm schedAlg) {
  SchedulerSimulator* ret;
  switch (schedAlg) {
  case FCFS:
    ret = new FCFSSimulator();
    break;
    
  case RR:
    ret = new RRSimulator(SchedulerSimulator::getQuantum());
    break;

  case SPN:
  case SRT:
  case HRRN:
  case FEEDBACK:
  default:
    ret = nullptr;
    break;
  }

  return ret;
}

SchedulerAlgorithm
getSchedulerAlgorithm(std::string& schedulerStr) {
  std::string str { schedulerStr};

  // std::transform(str.begin(), str.end(), str.begin(), std::tolower);
  std::for_each(str.begin(), str.end(), [](char& c) { c = std::tolower(c); });

  if (str == "fcfs") return SchedulerAlgorithm::FCFS;
  if (str == "rr") return SchedulerAlgorithm::RR;
  if (str == "spn") return SchedulerAlgorithm::SPN;
  if (str == "srt") return SchedulerAlgorithm::SRT;
  if (str == "hrrn") return SchedulerAlgorithm::HRRN;
  if (str == "feedback") return SchedulerAlgorithm::FEEDBACK;
  return SchedulerAlgorithm::SCHED_ALGO_UNKNOWN;
}

void
loadProcess(SchedulerSimulator& sim,
	    std::filesystem::path& schedulerFile) {
  std::ifstream file(schedulerFile);
  std::string line;

  if (file.is_open()) {
    while (std::getline(file, line)) {
      std::istringstream inbuf(line);
      int arriveTime, serviceTime;
      inbuf >> arriveTime >> serviceTime;
      sim.addProcess(new Process(arriveTime, serviceTime));
    }
  }
  else {
    std::cerr << "Scheduler File: "
	      << schedulerFile
	      << " doesn't exist" << std::endl;
    std::cerr << "Current directory: "
	      << std::filesystem::current_path()
	      << std::endl;
  }
  
  file.close();
}

void
usage(std::filesystem::path& progname) {
  std::cerr << "Usage: "
	    << progname.filename().c_str()
	    << " <process_scheduler_filename>"
	    << std::endl;
  _Exit(EXIT_FAILURE);
}
