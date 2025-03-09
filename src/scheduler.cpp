#include <scheduler.hpp>

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

  for (auto it = terminatedProcess.begin(); it != terminatedProcess.end(); ) {

    (*it)->render();
    it++;
  }
}
