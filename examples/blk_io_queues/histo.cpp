#include <iostream>
#include <cstdio>
#include <cstdlib>

#include "Histogram.h"

using namespace std;

int main(int argc, char *argv[])
{
	if(argc < 3)
	{
		cout 	<< "Usage: $./Histogram <bucket(ns)> <filename>" << endl
			<< "Ex:	   $./Histogram 1000000	out.dat" << endl; 
		exit(0);
	}

	FILE *fp;

	if((fp = fopen(argv[2], "r")) == NULL)
	{
		printf("Can't open file %s\n", argv[2]);
		exit(0);
	}
	
	char *end;
	unsigned long long bucket = strtoll(argv[1], &end, 10);
	
	Histogram histogram;

	histogram.setBucket(bucket);

	while(!feof(fp))
	{
		unsigned long long tmp;

		fread(&tmp, sizeof(unsigned long long), 1, fp);

		histogram.bucketData(tmp);
	}


	cout << "Histogram" << endl;
	histogram.printHisto();

	return 0;
}
