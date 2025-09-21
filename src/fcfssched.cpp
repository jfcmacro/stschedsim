#include <fcfssched.hpp>

FCFSSimulator::FCFSSimulator()
  : SchedulerSimulator() {
}

void
FCFSSimulator::scheduler() {
    // Check if newer processes have just arrived
    for (auto it = newProcess.begin(); it != newProcess.end();) {

      // if (!(*it)) break;
      if ((*it)->getArriveTime() == currentTime) {
	(*it)->update(READY, currentTime);
	readyProcess.push_back(*it);
	it = newProcess.erase(it);
      }
      else {
	it++;
      }
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

bool
FCFSSimulator::allProcessCompleted() {
    
  return runProcess == nullptr;
}
