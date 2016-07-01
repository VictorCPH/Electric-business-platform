#include "stdafx.h"
#include <time.h> 
#include <stdio.h> 
#include "CurrentTime.h"

string getCurTime()
{
	time_t t = time(0);
	char tmp[64];
	strftime(tmp, sizeof(tmp), "%Y/%m/%d %X", localtime(&t));
	return string(tmp);
}
