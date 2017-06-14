#include "Service\ServiceUtils.h"
#include "Service.h"

int WINAPI ServiceMain(ServiceBaseInfo *ServiceInfo)
{
	return ServiceMainLib(ServiceInfo);
}