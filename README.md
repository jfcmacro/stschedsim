# stschedsim

A short-time scheduler simulator

## Execute

There are three way to execute it:

### Show the options

```bash
$ stschedsim --help
```

Or

```bash
$ stschedsim -h
```

### Execute some schedulers algorithms without quantum time:

In particular:

* FCFS (First Come First Server).
* SPN (Shortest Process Next).
* HRRN (High Ration Response Next).
* SRT (Shortest Remaining Time)

```bash
$ stschedsim [-s|--sched] [fcfs|spn|srt|hrrn] <proces_scheduler_filename>
```

### Execute some schedulers algorithms using quantum time and Round Robin algorithm

```bash
$ stschedsim [-s|--sched] [rr|fb] [[-q|--quantum] <quantum_value>] <process_scheduler_filename
```

### Process Scheduler File

This process consist on several lines with two fields each line. First
field, process arrive time, and the second field service time:


Here we have 5 process:

```txt
0 3
2 6
4 4
6 5
8 2
```

## Compile

We are using cmake.

To compile, on the directory where the project was clonned:

```bash
$ mkdir build
$ cd build
$ cmake ..
$ cmake --build .
```
