
#ifndef MANTLE_CORE_HEADER_
#define MANTLE_CORE_HEADER_

#include "CustomTypes.h"

#ifdef MANTLE_GFX

#ifdef NVIDIA_BUNDLE_INCLUDE
	#define NVIDIA_BUNDLE
#endif// NVIDIA_BUNDLE_INCLUDE

#include "VulkanStructsBundles.h"
#include "NvidiaPhysicalDevice.h"
#include "LogicalDevice.h"
#endif // MANTLE_GFX

#endif // !MANTLE_CORE_HEADER_
