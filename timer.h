#ifndef TIMER_H
#define TIMER_H

class Timer {
 public:
  Timer(unsigned long interval) : interval_(interval), previous_millis_(0) {}

  bool Elapsed();

 private:
  unsigned long interval_;
  unsigned long previous_millis_;
};

#endif  // TIMER_H
