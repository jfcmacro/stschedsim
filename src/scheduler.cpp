#include <scheduler.hpp>
#include <algorithm>

SchedulerSimulator::SchedulerSimulator()
  : running(false), runProcess(nullptr) {
}

void
SchedulerSimulator::addProcess(Process* process) {
  newProcess.push_back(process);
}

void
SchedulerSimulator::start() {
  running = true;
  currentTime = 0;

  while(running) {

    scheduler();
    if (!runProcess) {
      break;
    }

    running = !allProcessCompleted();

    if (running) {
      currentTime++;
    }
  }
}

void
SchedulerSimulator::end() {

  std::sort(terminatedProcess.begin(),terminatedProcess.end(),
	    [](Process* pa, Process* pb) { return pa->getId() < pb->getId(); });
  
  for (auto it = terminatedProcess.begin(); it != terminatedProcess.end(); ) {
    
    (*it)->render();
    it++;
  }
}


unsigned int SchedulerSimulator::quantum = 1;

unsigned int SchedulerSimulator::getQuantum() {
  return SchedulerSimulator::quantum;
}

void SchedulerSimulator::setQuantum(unsigned int quantum) {
  SchedulerSimulator::quantum = quantum;
}
