#include <process.hpp>
#include <scheduler.hpp>
#include <fcfssched.hpp>
#include <rrsched.hpp>
#include <getopt.h>

int
main(int argc, const char *argv[]) {
  
  SchedulerSimulator* sim {new RRSimulator(1)};

  sim->addProcess(new Process(0, 3));
  sim->addProcess(new Process(2, 6));
  sim->addProcess(new Process(4, 4));
  sim->addProcess(new Process(6, 5));
  sim->addProcess(new Process(8, 2));

  sim->start();
  sim->end();

  return 0;
}
