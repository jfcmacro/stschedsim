#pragma once

#include <vector>
#include <memory>
#include <process.hpp>
#include <scheduler.hpp>

class HRRNSimulator : public SchedulerSimulator {
private:
  std::vector<Process*> readyProcess;
protected:
  void receiveArrivedProcess(Process* arrivedProcess) override;
public:
  HRRNSimulator();
  void scheduler() override;
  bool allProcessCompleted() override;
};

