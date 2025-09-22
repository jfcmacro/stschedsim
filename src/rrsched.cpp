#include <rrsched.hpp>

RRSimulator::RRSimulator(unsigned int quatum)
  : SchedulerSimulator(), quantum(quatum), remainingQuantum(1) {
}

void
RRSimulator::receiveArrivedProcess(Process* arrivedProcess) {
  readyProcess.push_back(arrivedProcess);
}

void
RRSimulator::scheduler() {
  // Check if newer processes have just arrived
  checkArrivedProcess();

  if (!runProcess) { // There is not running process, we must choose the first one
    
    if (readyProcess.size() > 0) {
      
      runProcess = readyProcess.front();
      readyProcess.pop_front();
      runProcess->update(RUNNING, currentTime);
      remainingQuantum = quantum;
    }
  }
  else {

    if (remainingQuantum > 0) {
      remainingQuantum--;
    }
      
    runProcess->update();
      
    if (runProcess->getRemainingServiceTime() == 0) {
	
      runProcess->update(TERMINATED, currentTime);
      terminatedProcess.push_back(runProcess);
      runProcess = nullptr;
    }
    else if (remainingQuantum == 0) {
	
      runProcess->update(READY, currentTime);
      readyProcess.push_back(runProcess);
      runProcess = nullptr;
    }

    if (runProcess == nullptr) {
      
      if (readyProcess.size() > 0) {
	
	runProcess = readyProcess.front();
	readyProcess.pop_front();
	runProcess->update(RUNNING, currentTime);
      }
	
      remainingQuantum = quantum;
    }
  }

  // UpdateWaitingProcess
  for (auto& process : readyProcess) {
      
    process->update();
  }
}

bool
RRSimulator::allProcessCompleted() {
    
  return runProcess == nullptr;
}
