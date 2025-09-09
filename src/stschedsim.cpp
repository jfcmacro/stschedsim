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
#include <spnsched.hpp>
#include <srtsched.hpp>
#include <hrrnsched.hpp>
#include <getopt.h>
#include <stdlib.h>

void loadProcess(SchedulerSimulator&, std::filesystem::path&);
void usage(std::filesystem::path&);
SchedulerAlgorithm getSchedulerAlgorithm(const char*);

int
main(int argc, char* const argv[]) {

  const char *filename;
  std::filesystem::path progname {argv[0]};

  static struct option long_options[] = {
    {"sched",   required_argument, 0, 's'},
    {"quantum", required_argument, 0, 'q'},
    {0,         0,                 0,  0 }
  };

  int long_index = 0;
  int opt;
  SchedulerAlgorithm schedAlgo = SchedulerAlgorithm::RR;
  bool isSetQuantum = false;

  while ((opt = getopt_long(argc, argv, "s:q:",
			    long_options, &long_index)) != -1) {
    switch(opt) {
    case 's':
      schedAlgo = getSchedulerAlgorithm(optarg);

      if (schedAlgo == SchedulerAlgorithm::SCHED_ALGO_UNKNOWN) {

	usage(progname);
      }

      break;

    case 'q':
      SchedulerSimulator::setQuantum(std::stoul(optarg));
      isSetQuantum = true;

      break;

    case '?':
      usage(progname);

      break;
    }
  }

  if (schedAlgo != SchedulerAlgorithm::RR and
      isSetQuantum) {
    usage(progname);
  }

  if (optind == argc) {

    usage(progname);
  }
  else {

    filename = argv[optind];
  }

  SchedulerSimulator* sim { getSchedulerAlgorithm(schedAlgo) };

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
    ret = new SPNSimulator();
    break;

  case SRT:
    ret = new SRTSimulator();
    break;

  case HRRN:
    ret = new HRRNSimulator();
    break;
    
  case FEEDBACK:
  default:
    ret = nullptr;
    break;
  }

  return ret;
}

SchedulerAlgorithm
getSchedulerAlgorithm(const char* schedulerStr) {
  std::string str { schedulerStr };

  std::for_each(str.begin(), str.end(), [](char& c) { c = std::tolower(c); });

  if (str == "fcfs") return SchedulerAlgorithm::FCFS;
  if (str == "rr")   return SchedulerAlgorithm::RR;
  if (str == "spn")  return SchedulerAlgorithm::SPN;
  if (str == "srt")  return SchedulerAlgorithm::SRT;
  if (str == "hrrn") return SchedulerAlgorithm::HRRN;
  if (str == "fb")   return SchedulerAlgorithm::FEEDBACK;

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
  const unsigned int SPACES = 5;
  
  std::cerr << "Usage: "
	    << std::endl
	    << std::string(SPACES, ' ')
	    << progname.filename().c_str()
	    << " [[-s|--sched] [fcfs|spn|srt|hrrn|fb]] <process_scheduler_filename>"
	    << std::endl
	    << std::string(SPACES, ' ')
	    << progname.filename().c_str()
	    << " [[-s|--sched] rr]] [[-q|--quantum] <quantum_value>] <process_scheduler_filename>"
	    << std::endl;

  _Exit(EXIT_FAILURE);
}
