#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization();  // TODO: See src/processor.cpp
  Processor();

  // TODO: Declare any necessary private members
 private:
    long jiffies_;
    long activeJiffs_;
    long prevTotal_;
    long prevActive_;

};

#endif