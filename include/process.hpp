#pragma once

enum ProcessState { NEW, READY, RUNNING, TERMINATED };

class Process {
private:
  static unsigned int nextId;
  unsigned int id;
  unsigned int arrive;
  unsigned int service;
  unsigned int execute;
  unsigned int waiting;
  unsigned int terminated;
  ProcessState state;
public:
  Process(unsigned int arrive,
	  unsigned int service);
  
  void update();
  void update(ProcessState newState, unsigned int currentTime);
  void render();

  unsigned int getId() const;
  unsigned int getArriveTime() const;

  unsigned int getServiceTime() const;
  unsigned int getExecutedTime() const;

  unsigned int getWaitingTime() const;
  unsigned int getTurnaroundTime() const;
  unsigned int getRemainingServiceTime() const;
};

