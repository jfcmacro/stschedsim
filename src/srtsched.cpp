#include <srtsched.hpp>
#include <algorithm>

SRTSimulator::SRTSimulator()
  : SchedulerSimulator() {
}

void
SRTSimulator::receiveArrivedProcess(Process* arrivedProcess) {
  readyProcess.push_back(arrivedProcess);
}

void
SRTSimulator::scheduler() {
  // Check if newer processes have just arrived
  size_t prevNbrReadyProcess = readyProcess.size();
  checkArrivedProcess();
  bool atLeastOneArrived = false;

  if (prevNbrReadyProcess < readyProcess.size()) {
    atLeastOneArrived = true;
  }
  
  if (!runProcess) { // There is not running process, we must choose the first one

    if (readyProcess.size() > 0) {
      runProcess = readyProcess.front();
      readyProcess.erase(readyProcess.begin());
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
    }
    else if (atLeastOneArrived) {

      runProcess->update(READY, currentTime);
      readyProcess.push_back(runProcess);
      runProcess = nullptr;
    }

    if (!runProcess) {
      if (readyProcess.size() > 0) {

	std::vector<Process*>::iterator result =
	  std::min_element(readyProcess.begin(),
			   readyProcess.end(),
			   [](Process* p, Process* q) {
			     return p->getServiceTime() - p->getExecutedTime()
			       < q->getServiceTime() - q->getExecutedTime();
			   });
	runProcess = *result;
	readyProcess.erase(result);
	runProcess->update(RUNNING, currentTime);
      }
    }
  }

  // UpdateWaitingProcess
  for (auto& process : readyProcess) {
      
    process->update();
  }
}

bool
SRTSimulator::allProcessCompleted() {
    
  return runProcess == nullptr;
}
