#pragma once

// #include <deque>
#include <vector>
#include <memory>
#include <process.hpp>
#include <scheduler.hpp>

class SPNSimulator : public SchedulerSimulator {
private:
  std::vector<Process*> readyProcess;
public:
  SPNSimulator();
  void scheduler() override;
  bool allProcessCompleted() override;
};

