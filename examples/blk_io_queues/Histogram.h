#ifndef __HISTOGRAM_H
#define __HISTOGRAM_H

#include <map>

typedef std::map<unsigned long long, unsigned long long> MapWaitTimes;

class Histogram
{
	public:
		Histogram();

		void setBucket(unsigned long long & bucket);
		void printHisto(void);
		void bucketData(unsigned long long & time);

	private:
		unsigned long long bucket;
		MapWaitTimes mapWaitTimes;
};


#endif
