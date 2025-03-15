#pragma once

#include <vector>
#include <memory>
#include <process.hpp>
#include <scheduler.hpp>

class HRRNSimulator : public SchedulerSimulator {
private:
  std::vector<Process*> readyProcess;
public:
  HRRNSimulator();
  void scheduler() override;
  bool allProcessCompleted() override;
};

