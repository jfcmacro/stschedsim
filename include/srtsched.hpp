#pragma once

#include <vector>
#include <memory>
#include <process.hpp>
#include <scheduler.hpp>

class SRTSimulator : public SchedulerSimulator {
private:
  std::vector<Process*> readyProcess;
public:
  SRTSimulator();
  void scheduler() override;
  bool allProcessCompleted() override;
};

