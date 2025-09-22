#pragma once

#include <vector>
#include <memory>
#include <process.hpp>
#include <scheduler.hpp>

class SRTSimulator : public SchedulerSimulator {
private:
  std::vector<Process*> readyProcess;
protected:
  void receiveArrivedProcess(Process* arrivedProcess) override;
public:
  SRTSimulator();
  void scheduler() override;
  bool allProcessCompleted() override;
};

