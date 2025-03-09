#pragma once

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
    
    return runProcess == nullptr;
  }
};
