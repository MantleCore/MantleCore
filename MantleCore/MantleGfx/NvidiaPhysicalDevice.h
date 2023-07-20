
#ifndef MANTLE_NVIDIA_PHYSICAL_DEVICE_HEADER_
#define MANTLE_NVIDIA_PHYSICAL_DEVICE_HEADER_

#include "CustomTypes.h"
#include "VulkanStructsBundles.h"

#include <vector>
#include <map>

namespace MantleGfx {

	namespace {
		using ActiveNvidiaMap = std::multimap<VkPhysicalDevice, MgPhysicalDeviceInfoBundle>;
	}

	class MANTLE_GFX_API MgNvidiaPhysicalDevice {
	public:
		MgNvidiaPhysicalDevice();
		~MgNvidiaPhysicalDevice();

		bool								mgDestroyHandles();

		u32									mgGetInstanceVersion();
		VkResult							mgCreateApplicationInstance(const std::vector<const char*>& layers, const std::vector<const char*>& extensions);
		VkResult							mgCreateDebugUtils();
		void								mgCreateWindowSurfaceCallbackFunc(void (*createWindowSurface)(VkInstance, VkSurfaceKHR*));
		VkInstance							mgGetInstance() const;

		std::vector<VkLayerProperties>		mgGetInstanceLayerProperties();
		std::vector<VkExtensionProperties>	mgGetInstanceExtensionProperties();
		std::vector<VkExtensionProperties>	mgGetDeviceExtensionProperties(VkPhysicalDevice& physicalDevice);
		std::vector<VkLayerProperties>		mgGetDeviceLayerProperties(VkPhysicalDevice& physicalDevice);

		std::vector<VkPhysicalDevice>		mgGetPhyscalDeviceList() const;
		ActiveNvidiaMap						mgGetPhysicalDeviceInfoMap() const;
		VkSurfaceKHR						mgGetSurface() const;
		void								mgSelectPhysicalDevice(VkPhysicalDevice& physicalDevice);
		u32									mgFindQueueFamilies(VkQueueFlagBits queueFlag);

	private:
		class Internal;
		std::unique_ptr<Internal> pInternal;
	};
} // !MantleGfx

#endif // !MANTLE_NVIDIA_PHYSICAL_DEVICE_HEADER_