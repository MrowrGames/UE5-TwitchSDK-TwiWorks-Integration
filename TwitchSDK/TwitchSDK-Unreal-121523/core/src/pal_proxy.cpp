#include "pal.hpp"
#include "marshalling.hpp"

using namespace R66;
using namespace R66::PAL;
using namespace R66::Marshalling;

ProxyPAL::ProxyPAL(void* parent, ProxyPALImpl impl)
	: Parent(parent)
	, Impl(impl)
{
}

void ProxyPAL::Dispose()
{
	Parent = nullptr;
}
