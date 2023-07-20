
#ifndef MANTLE_INTERFACE_PHYSICAL_DEVICE_HEADER_
#define MANTLE_INTERFACE_PHYSICAL_DEVICE_HEADER_

#include "CustomTypes.h"

#include <vector>

namespace MantleGfx {

class IPhysicalDevice {
public:

	virtual ~IPhysicalDevice() = default;
	
	/**
	* Destroy's Instance and Device related handles
	* 
	* @param Void
	* @returns True if all handles destroyed, otherwise false
	*/
	virtual bool destroyHandles() = 0;

	/**
	* Calls vkEnumerateInstanceVersion.
	* 
	* @param Void
	* @returns Success:
	* @returns - VK_SUCCESS 
	* @return ---------------------------------
	* @returns Failure:
	* @returns - VK_ERROR_OUT_OF_HOST_MEMORY
	*/
	virtual u32 getInstanceVersion() = 0;

	/**
	 * Creates the Vulkan Instance and collects all Nvidia PhysicalDevices.
	 *
	 * @param Void
	 * 
	 * @returns A u32 version of the Instance.
	 */
	virtual VkResult createInstance() = 0;

	/**
	* Creates all the validation and debug tools from vulkan
	* 
	* @param Void
	* 
	* @return Success
	* @return ---------------------------------
	* @return Failure
	*/
	virtual VkResult createDebugUtils() = 0;

	/**
	* Generic call back function for using different types
	* of windows.
	* 
	* @param createWindowSurface: Function pointer with the parameters
	*		 to pass in to create the surface for
	*		 your desired window.
	* 
	* @returns Void
	*/
	virtual void createWindowSurfaceCallbackFunc(void (*createWindowSurface)(VkInstance, VkSurfaceKHR*)) = 0;

	/**
	* Calls vkEnumerateInstanceLayerProperties.
	*
	* @param Void
	*
	* @returns A list of VkLayerProperties
	*/
	virtual std::vector<VkLayerProperties> getInstanceLayerProperties() = 0;

	/**
	* Calls vkEnumerateInstanceExtensionProperties.
	*
	* @param Void
	*
	* @returns A list of VkExtensionProperties
	*/
	virtual std::vector<VkExtensionProperties> getInstanceExtensionProperties() = 0;

	/**
	* Calls vkEnumerateDeviceExtensionProperties.
	*
	* @param Void
	*
	* @returns A list of VkLayerProperties
	*/
	virtual std::vector<VkExtensionProperties> getDeviceExtensionProperties(VkPhysicalDevice& physicalDevice) = 0;

	/**
	* Calls vkEnumerateDeviceLayerProperties.
	*
	* @param Void
	*
	* @returns A list of VkExtensionProperties
	*/
	virtual std::vector<VkLayerProperties> getDeviceLayerProperties(VkPhysicalDevice& physicalDevice) = 0;

	/**
	* User selects the physical device to work on.
	*
	* @param physicalDevice: reference to the user's selected physical device.
	*
	* @returns Void
	*/
	virtual void selectPhysicaldevice(VkPhysicalDevice& physicalDevice) = 0;

	/**
	* Find the queue family index based on the desired flag
	* 
	* @param queueFlag: Specificy queue type
	* 
	* @returns Queue family index
	*/
	virtual u32	findQueueFamilies(VkQueueFlagBits queueFlag) = 0;
};

} // !MantleGfx
#endif // !MANTLE_INTERFACE_PHYSICAL_DEVICE_HEADER_


