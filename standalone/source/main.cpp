#include "ds.h"
#include "log.h"

using namespace stk;

int main()
{
	c_fixed_vector<uint16_t, 3> vec;
	vec.append(1);
	vec.append(2);
	vec.append(3);
	vec.append(4);
	logln("Vector contents:");
	for (uint16_t val : vec)
	{
		logln("     {}", val);
	}
	return 0;
}
