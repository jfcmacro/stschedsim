#include <iostream>
#include <deque>
#include <memory>

enum ProcessState { NEW, READY, RUNNING, TERMINATED };

class Process {
private:
  unsigned int id;
  unsigned int arrive;
  unsigned int service;
  unsigned int execute;
  unsigned int start;
  unsigned int waiting;
  unsigned int terminated;
  ProcessState state;
public:
  Process(unsigned int id,
	  unsigned int arrive,
	  unsigned int service) : id(id),
			    arrive(arrive),
			    service(service),
			    execute(0),
			    start(0),
			    waiting(0) { }
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
      if (newState == RUNNING) {
      }
      break;
    case RUNNING:
      switch(newState) {
      case READY:
	break;
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
};

class SchedulerSimulator {
protected:
  std::deque<Process*> newProcess;
  Process* runProcess;
  std::deque<Process*> terminatedProcess;
  bool running;
  unsigned int currentTime;
public:
  SchedulerSimulator() : running(false), runProcess(NULL) { }
  void addProcess(Process* process) {
    newProcess.push_back(process);
  }
  void start() {
    running = true;
    currentTime = 0;
    while(running) {
      scheduler();
      running = !allProcessCompleted();
      if (running) currentTime++;
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
    // Check if a New Process has just arrived
    Process* arrivedProcess = NULL;
    int i = 0;
    for (auto it = newProcess.begin(); it != newProcess.end();i++) {
      if ((*it)->getArriveTime() == currentTime) {
	arrivedProcess = *it;
	// it = it.erase(it);
	newProcess.erase(newProcess.begin() + i);
	break;
      }
      else {
	it++;
      }
    }

    if (arrivedProcess) {
      arrivedProcess->update(READY, currentTime);
      readyProcess.push_back(arrivedProcess);
    }
    // if there is possible to commute to new process
    if (!runProcess) {
      runProcess = readyProcess.front();
      readyProcess.pop_front();
    }
    else {
      runProcess->update();
      if ((runProcess->getServiceTime() - runProcess->getExecutedTime()) == 0) {
	runProcess->update(TERMINATED, currentTime);
	terminatedProcess.push_back(runProcess);
	runProcess = readyProcess.front();
	runProcess->update(RUNNING, currentTime);
	readyProcess.pop_front();
      }
    }
    // UpdateWaitingProcess
    for (auto& process : readyProcess) {
      process->update();
    }
  }
  
  bool allProcessCompleted() override {
    return runProcess == NULL;
  }
};

int
main(int argc, char *argv[]) {
  SchedulerSimulator* sim = new FCFSSimulator();
  sim->addProcess(new Process(0, 0, 3));
  sim->addProcess(new Process(1, 2, 6));
  sim->addProcess(new Process(2, 4, 4));
  sim->addProcess(new Process(3, 6, 5));
  sim->addProcess(new Process(4, 8, 2));
  sim->start();
  
  return 0;
}
