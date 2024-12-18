#include "core/ds.h"
#include "core/log.h"
#include "core/math.h"

#include <cstdint>

using namespace tt;

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
	c_rand r;
	uint32_t r_uint = r.rand_int(1, 5);
	logln("Random number from 1 to 5: {}", r_uint);
	logln("hash of fred is {}", static_cast<uint32_t>("fred"_h));
	return 0;
}
