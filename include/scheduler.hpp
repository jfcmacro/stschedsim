#pragma once
#include <deque>
#include <memory>
#include <process.hpp>

class SchedulerSimulator {
protected:
  std::deque<Process*> newProcess;
  Process* runProcess;
  std::deque<Process*> terminatedProcess;
  bool running;
  unsigned int currentTime;
public:
  SchedulerSimulator();
  void addProcess(Process* process);
  void start();
  void end();
  virtual void scheduler() = 0;
  virtual bool allProcessCompleted() = 0;
};

