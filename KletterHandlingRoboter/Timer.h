#ifndef Timer_h
#define Timer_h

#include <Arduino.h>

class Timer {

  public:
	  bool lastq;
	  unsigned long startzeit;
	  
	  Timer();
	  void in(bool& in);
	  void q(bool& q);
	  void pt(unsigned long& pt);
	  void et(unsigned long& et);

	private:
		bool _in = false;
		bool _q = false;
		unsigned long _pt = 0;
		unsigned long _et = 0;
};
#endif
