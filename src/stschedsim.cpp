#include <filesystem>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <process.hpp>
#include <scheduler.hpp>
#include <fcfssched.hpp>
#include <rrsched.hpp>
#include <getopt.h>

void loadProcess(SchedulerSimulator&, const char *);

int
main(int argc, const char *argv[]) {

  const char *filename;
  
  if (argc == 2) {
    filename = argv[1];
  }
  
  SchedulerSimulator* sim {getSchedulerAlgorithm(RR)};

  loadProcess(*sim, filename);

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
  case Feedback:
  default:
    ret = nullptr;
    break;
  }

  return ret;
}

void
loadProcess(SchedulerSimulator& sim, const char *processFilename) {
  std::string filename { processFilename };
  std::ifstream file(filename);
  std::string line;

  std::cout << "Process File: " << processFilename << std::endl;

  if (file.is_open()) {
    while (std::getline(file, line)) {
      std::istringstream inbuf(line);
      int arriveTime, serviceTime;
      inbuf >> arriveTime >> serviceTime;
      sim.addProcess(new Process(arriveTime, serviceTime));
    }
  }
  else {
    std::cerr << "File: " << processFilename << " doesn't exist" << std::endl;
    std::cerr << "Current directory: " << std::filesystem::current_path() << std::endl;
  }
  
  file.close();
}
