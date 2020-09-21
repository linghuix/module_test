
#include "BSP.h"

#include "func_can.h"


#define ForceBufferSize 10

struct ForceBuffer{
	float data[ForceBufferSize];
	uint16_t in;
	uint16_t out;
};


TEST ForceCollector_test(void);

TEST ForceCollectExperiment(void);
TEST SendString_test(void);
