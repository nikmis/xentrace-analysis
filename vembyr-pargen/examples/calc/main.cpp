#include <iostream>
#include "calc.h"

using namespace std;

main()
{
	char expr_string[512];
	
	cin >> expr_string;
	cout << (long) Parser::parse(expr_string) << endl;
	        
	return 0;
}
