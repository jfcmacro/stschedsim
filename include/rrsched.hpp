#pragma once

#include <deque>
#include <memory>
#include <process.hpp>
#include <scheduler.hpp>

class RRSimulator : public SchedulerSimulator {
private:
  std::deque<Process*> readyProcess;
  unsigned int quantum;
  unsigned int remainingQuantum;
protected:
  void receiveArrivedProcess(Process* arrivedProcess) override;
public:
  RRSimulator(unsigned int quantum);
  void scheduler() override;
  bool allProcessCompleted() override;
};

