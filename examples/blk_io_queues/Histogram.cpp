#include <iostream>
#include <map>

#include "Histogram.h"

using namespace std;

Histogram::Histogram():	bucket(1) { }

void Histogram::setBucket(unsigned long long & tmpBucket)
{
	bucket = tmpBucket;
}

void Histogram::bucketData(unsigned long long & waitTime)
{
	unsigned long long key = waitTime/bucket;		

	MapWaitTimes::iterator it;

	if((it = mapWaitTimes.find(key)) == mapWaitTimes.end())
	{
		mapWaitTimes.insert( pair<unsigned long long, unsigned long long>(key, 1) );
	}
	else
	{
		(it->second)++;
	}
}

void Histogram::printHisto(void)
{
	MapWaitTimes::iterator it;

	for(it = mapWaitTimes.begin(); it != mapWaitTimes.end(); it++)
	{
		//cout << (*it).first << " => " << (*it).second << endl;

		cout << (*it).first*bucket/(1E6) << " (ms) " 
			<< "- " << ((*it).first + 1)*bucket/(1E6) <<  " (ms) " 
			<< " => " << (*it).second << endl;
	}

	cout << endl;
}
