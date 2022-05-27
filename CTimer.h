#ifndef __CTIMER_H
#define __CTIMER_H

class CTimer{
public:
 CTimer();
~CTimer();
 void Reset();
 void Start();
 void Stop();    
 void Delay(int iMilliseconds);
 unsigned int GetElapsed();
 unsigned int GetCumulated();
 unsigned int GetNow();
 unsigned int GetFirst();
 unsigned int GetFinal();

 private:        
  unsigned int FirstTime,FinalTime;
  unsigned int TimeCumulated;
  unsigned int TimeElapsed;
 };
#endif