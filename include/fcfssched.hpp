#pragma once

#include <deque>
#include <memory>
#include <process.hpp>
#include <scheduler.hpp>

class FCFSSimulator : public SchedulerSimulator {
private:
  std::deque<Process*> readyProcess;
public:
  FCFSSimulator();
  void scheduler() override;
  bool allProcessCompleted() override;
};

