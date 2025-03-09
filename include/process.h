#pragma once

enum ProcessState { NEW, READY, RUNNING, TERMINATED };

class Process {
private:
  static unsigned int nextId;
  unsigned int id;
  unsigned int arrive;
  unsigned int service;
  unsigned int execute;
  // unsigned int start;
  unsigned int waiting;
  unsigned int terminated;
  ProcessState state;
public:
  Process(unsigned int arrive,
	  unsigned int service) :
    id(nextId++),
    arrive(arrive),
    service(service),
    execute(0),
      //    start(0),
    waiting(0),
    state(NEW) { }
  void update() {
    switch (state) {
    case NEW:
      break;
    case READY:
      waiting++;
      break;
    case RUNNING:
      execute++;
      break;
    case TERMINATED:
      break;
    }
  }

 void update(ProcessState newState, unsigned int currentTime) {
    switch(state) {
    case NEW:
      if (newState == READY) {
	arrive = currentTime;
      }
      break;
    case READY:
      break;
    case RUNNING:
      switch(newState) {
      case TERMINATED:
	terminated = currentTime;
      }
      break;
    case TERMINATED:
      break;
    }
    state = newState;
  }

  void render() {
    if (state == NEW) return;
    std::cout << "Process id: " << id
	      << " state: " << state
	      << " arrive: " << arrive
	      << " service: " << service
	      << " execute: " << execute
	      << " waiting: " << waiting
	      << " turnaround: " << (service + waiting)
	      << " terminated: " << terminated
	      << std::endl;
  }

  unsigned int getArriveTime() const {
    return arrive;
  }

  unsigned int getServiceTime() const {
    return service;
  }

  unsigned int getExecutedTime() const {
    return execute;
  }

  unsigned int getTurnaroundTime() const {
    return service + waiting;
  }
};

unsigned int Process::nextId = 0;
