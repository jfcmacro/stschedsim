#include <process.hpp>
#include <iostream>

Process::Process(unsigned int arrive,
		 unsigned int service) :
    id(nextId++),
    arrive(arrive),
    service(service),
    execute(0),
    waiting(0),
    state(NEW) {
}

void
Process::update() {
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


void
Process::update(ProcessState newState, unsigned int currentTime) {
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

void
Process::render() {
  if (state == NEW) return;
  std::cout << "Process id: " << id
	    << " arrive: " << arrive
	    << " service: " << service
	    << " execute: " << execute
	    << " waiting: " << waiting
	    << " turnaround: " << getTurnaroundTime()
	    << " terminated: " << terminated
	    << std::endl;
}

unsigned int
Process::getArriveTime() const {
  return arrive;
}

unsigned int
Process::getServiceTime() const {
  return service;
}

unsigned int
Process::getExecutedTime() const {
  return execute;
}

unsigned int
Process::getTurnaroundTime() const {
  return service + waiting;
}

unsigned int
Process::getRemainingServiceTime() const {
  return service - execute;
}

unsigned int Process::nextId = 0;
