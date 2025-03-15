#include <hrrnsched.hpp>
#include <algorithm>

static double ratio(const Process *p);

HRRNSimulator::HRRNSimulator()
  : SchedulerSimulator() {
}

void
HRRNSimulator::scheduler() {
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
	if (readyProcess.size() > 0) {
	  std::vector<Process*>::iterator result =
	    std::max_element(readyProcess.begin(),
			     readyProcess.end(),
			     [](Process* p, Process* q) {
			       return ratio(p) < ratio(q);
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
HRRNSimulator::allProcessCompleted() {
    
  return runProcess == nullptr;
}

double
ratio(const Process *p) {
  return (p->getWaitingTime() + p->getServiceTime()) / p->getServiceTime();
}
