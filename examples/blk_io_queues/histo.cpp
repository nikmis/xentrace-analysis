#include <iostream>
#include <map>
#include <cstdio>
#include <cstdlib>

using namespace std;

typedef struct Histo
{
	unsigned type;
	unsigned long long waitTime;
} Histo;

typedef map<unsigned long long, unsigned long long> MapWaitTimes;

void printHisto(MapWaitTimes & waitTimes, unsigned long long & bucket);
void bucketData(MapWaitTimes & waitTimes, Histo & tmp, unsigned long long & bucket);


int main(int argc, char *argv[])
{
	FILE *fp;

	if((fp = fopen("out.dat", "r")) == NULL)
	{
		printf("Can't open file\n");
		exit(0);
	}
	
	if(argc < 2)
	{
		cout << "Usage: ./a.out <bucket size in nano seconds" << endl;
		exit(0);
	}

	char *end;
	unsigned long long bucket = strtoll(argv[1], &end, 10);
	
	MapWaitTimes lostWait, noLostWait;

	while(!feof(fp))
	{
		Histo tmp;

		fread(&tmp, sizeof(Histo), 1, fp);

		//cout << tmp.type << " " << tmp.waitTime << endl;

		(tmp.type == 1) ? bucketData(lostWait, tmp, bucket) : bucketData(noLostWait, tmp, bucket);
	}


	cout << "Lost Wait Times Histogram" << endl;
	printHisto(lostWait, bucket);

	cout << "Clean Wait Times Histogram" << endl;
	printHisto(noLostWait, bucket);

	return 0;
}
		
void bucketData(MapWaitTimes & waitTimes, Histo & tmp, unsigned long long & bucket)
{
	unsigned long long key = tmp.waitTime/bucket;		

	//cout << tmp.type << " " << key << endl;

	MapWaitTimes::iterator it;

	if((it = waitTimes.find(key)) == waitTimes.end())
	{
		waitTimes.insert( pair<unsigned long long, unsigned long long>(key, 1) );
	}
	else
	{
		(it->second)++;
	}
}

void printHisto(MapWaitTimes & waitTimes, unsigned long long & bucket)
{
	MapWaitTimes::iterator it;

	for(it = waitTimes.begin(); it != waitTimes.end(); it++)
	{
		//cout << (*it).first << " => " << (*it).second << endl;

		cout << (*it).first*bucket/(1E6) << " (ms) " << "- " << ((*it).first + 1)*bucket/(1E6) <<  " (ms) " << " => " << (*it).second << endl;
	}

	cout << endl;
}
