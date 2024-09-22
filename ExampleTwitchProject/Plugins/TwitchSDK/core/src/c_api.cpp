#include <cassert>
#include "c_api.hpp"

using namespace R66;
using namespace R66::PAL;
using namespace R66::Marshalling;

std::shared_ptr<ProxyPAL>* ProxyPAL_new(void* parent, ProxyPALImpl impl)
{
	return new std::shared_ptr<ProxyPAL>(std::make_shared<ProxyPAL>(parent, impl));
}

void ProxyPAL_Dispose(std::shared_ptr<ProxyPAL>* pal)
{
	pal->get()->Dispose();
	delete pal;
}

std::shared_ptr<R66Api>* R66Api_new(
	std::shared_ptr<PlatformAbstractionLayer>* pal,
	const char16_t* clientId,
	int32_t useEventSubProxy)
{
	return new std::shared_ptr<R66Api>(R66Api::Initialize(*pal, clientId, useEventSubProxy != 0));
}

void R66Api_Dispose(std::shared_ptr<R66Api>* api)
{
	delete api;
}

const char16_t* R66_GetVersion()
{
	return R66::Version.c_str();
}
