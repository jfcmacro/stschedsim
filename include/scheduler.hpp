#pragma once
#include <deque>
#include <memory>
#include <process.hpp>

enum SchedulerAlgorithm {
  FCFS,
  RR,
  SPN,
  SRT,
  HRRN,
  FEEDBACK,
  SCHED_ALGO_UNKNOWN};

class SchedulerSimulator {
private:
  static unsigned int quantum;
protected:
  std::deque<Process*> newProcess;
  Process* runProcess;
  std::deque<Process*> terminatedProcess;
  bool running;
  unsigned int currentTime;
  void checkArrivedProcess();
  virtual void receiveArrivedProcess(Process* process) = 0;
public:
  SchedulerSimulator();
  void addProcess(Process* process);
  void start();
  void end();
  virtual void scheduler() = 0;
  virtual bool allProcessCompleted() = 0;
  static unsigned int getQuantum();
  static void setQuantum(unsigned int quantum);
};

SchedulerSimulator*
getSchedulerAlgorithm(SchedulerAlgorithm);
