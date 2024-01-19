#include "pch.h"
#include "BindableResourceManager.h"

namespace bind
{
	ResourceManager& ResourceManager::Get()
	{
		static ResourceManager RM;
		return RM;
	}
}