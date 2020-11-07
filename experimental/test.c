#include <stdio.h>
#include <stdlib.h>
#include "_string.h"

int main()
{
	string str;
	set_string(&str, "Hello, my name is Darshan.");
	printf("size of string is %i.\nstring: %s\n", str.length, str.str);
}
