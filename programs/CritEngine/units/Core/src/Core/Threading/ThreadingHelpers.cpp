#include "ThreadingHelpers.h"

unsigned int getThreadCountEstimate()
{
    return std::thread::hardware_concurrency();
}
