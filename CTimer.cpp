#include "CTimer.h"
#include <windows.h>

CTimer::CTimer()
{
 Reset();
}

CTimer::~CTimer()
{
 
}
void CTimer::Reset()
{
 FirstTime=FinalTime=0;
 TimeCumulated=0;
 TimeElapsed=0;
}

void CTimer::Start()
{
 FirstTime=timeGetTime();
}

void CTimer::Stop()
{
 FinalTime=timeGetTime();
 TimeElapsed=FinalTime-FirstTime;
 TimeCumulated+=TimeElapsed;
}

unsigned int CTimer::GetElapsed()
{
 return TimeElapsed;
}

unsigned int CTimer::GetCumulated()
{
 return TimeCumulated;
}

unsigned int CTimer::GetNow()
{
 return timeGetTime();
}

unsigned int CTimer::GetFirst()
{
 return FirstTime;
}

unsigned int CTimer::GetFinal()
{
 return FinalTime;
}

void CTimer::Delay(int iMilliseconds)
{
 Sleep(iMilliseconds);
}