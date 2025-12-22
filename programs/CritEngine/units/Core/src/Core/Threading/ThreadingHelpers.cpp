#include "ThreadingHelpers.h"

unsigned int GetThreadCountEstimate()
{
    return std::thread::hardware_concurrency();
}