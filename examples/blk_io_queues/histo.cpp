#include <iostream>
#include <cstdio>
#include <cstdlib>

#include "Histogram.h"

using namespace std;

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
		cout << "Usage: ./Histogram <bucket size in nano seconds>" << endl;
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
