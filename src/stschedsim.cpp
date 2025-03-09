#include <iostream>
#include <deque>
#include <memory>
#include <process.h>
#include <scheduler.h>

int
main(int argc, char *argv[]) {
  SchedulerSimulator* sim {new FCFSSimulator()};

  sim->addProcess(new Process(0, 3));
  sim->addProcess(new Process(2, 6));
  sim->addProcess(new Process(4, 4));
  sim->addProcess(new Process(6, 5));
  sim->addProcess(new Process(8, 2));

  sim->start();
  sim->end();

  return 0;
}
