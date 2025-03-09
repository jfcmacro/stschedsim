#include <iostream>
#include <deque>
#include <memory>

enum ProcessState { NEW, READY, RUNNING, TERMINATED };

class Process {
private:
  static unsigned int nextId;
  unsigned int id;
  unsigned int arrive;
  unsigned int service;
  unsigned int execute;
  unsigned int start;
  unsigned int waiting;
  unsigned int terminated;
  ProcessState state;
public:
  Process(unsigned int arrive,
	  unsigned int service) :
    id(nextId++),
    arrive(arrive),
    service(service),
    execute(0),
    start(0),
    waiting(0),
    state(NEW) { }
  void update() {
    switch (state) {
    case NEW:
      break;
    case READY:
      waiting++;
      break;
    case RUNNING:
      execute++;
      break;
    case TERMINATED:
      break;
    }
  }

 void update(ProcessState newState, unsigned int currentTime) {
    switch(state) {
    case NEW:
      if (newState == READY) {
	arrive = currentTime;
      }
      break;
    case READY:
      // if (newState == RUNNING) {
      // }
      break;
    case RUNNING:
      switch(newState) {
      // case READY:
      // 	break;
      case TERMINATED:
	terminated = currentTime;
      }
      break;
    case TERMINATED:
      break;
    }
    state = newState;
  }

  void render() {
    if (state == NEW) return;
    std::cout << "Process id: " << id
	      << " state: " << state
	      << " arrive: " << arrive
	      << " service: " << service
	      << " execute: " << execute
	      << " waiting: " << waiting
	      << " turnaround: " << (service + waiting)
	      << " terminated: " << terminated
	      << std::endl;
  }

  unsigned int getArriveTime() const {
    return arrive;
  }

  unsigned int getServiceTime() const {
    return service;
  }

  unsigned int getExecutedTime() const {
    return execute;
  }

  unsigned int getTurnaroundTime() const {
    return service + waiting;
  }
};

unsigned int Process::nextId = 0;

class SchedulerSimulator {
protected:
  std::deque<Process*> newProcess;
  Process* runProcess;
  std::deque<Process*> terminatedProcess;
  bool running;
  unsigned int currentTime;
public:
  SchedulerSimulator() : running(false), runProcess(nullptr) { }
  void addProcess(Process* process) {
    newProcess.push_back(process);
  }
  void start() {
    running = true;
    currentTime = 0;
    while(running) {
      scheduler();
      if (!runProcess) break;
      running = !allProcessCompleted();
      if (running) currentTime++;
    }
  }
  void end() {
    for (auto it = terminatedProcess.begin(); it != terminatedProcess.end(); ) {
      (*it)->render();
      it++;
    }
  }
  virtual void scheduler() = 0;
  virtual bool allProcessCompleted() = 0;
};

class FCFSSimulator : public SchedulerSimulator {
private:
  std::deque<Process*> readyProcess;
public:
  FCFSSimulator() : SchedulerSimulator() { }
  void scheduler() override {
    // Check if newer processes have just arrived
    int i = 0;
    for (auto it = newProcess.begin(); it != newProcess.end();i++) {

      if (!(*it)) break;
      if ((*it)->getArriveTime() == currentTime) {
	(*it)->update(READY, currentTime);
	readyProcess.push_back(*it);
	newProcess.erase(newProcess.begin() + i);
      }	
      it++;
    }

    if (!runProcess) { // There is not running process, we must choose the first one

      if (readyProcess.size() > 0) {
	runProcess = readyProcess.front();
	readyProcess.pop_front();
	runProcess->update(RUNNING, currentTime);
      }
    }
    else {

      // If a running process we check it that process has already terminated, first
      // update its statistics and then check if already ended.
      runProcess->update();
      
      if ((runProcess->getServiceTime() - runProcess->getExecutedTime()) == 0) {
	
	runProcess->update(TERMINATED, currentTime);
	terminatedProcess.push_back(runProcess);
	runProcess = nullptr;
	if (readyProcess.size() > 0) {
	  runProcess = readyProcess.front();
	  readyProcess.pop_front();
	  runProcess->update(RUNNING, currentTime);
	}
      }
    }

    // UpdateWaitingProcess
    for (auto& process : readyProcess) {
      process->update();
    }
  }

  bool allProcessCompleted() override {
    // return newProcess.size() == 0 and
    //   readyProcess.size() == 0;
    return runProcess == nullptr;
  }
};

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
