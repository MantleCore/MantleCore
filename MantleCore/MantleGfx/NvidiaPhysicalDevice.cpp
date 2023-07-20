
#include "NvidiaPhysicalDevice.h"

#include "Logger.h"
#include "Interfaces/IPhysicalDevice.h"

// vkGetPhysicalDeviceCooperativeMatrixPropertiesNV
// vkGetPhysicalDeviceSupportedFramebufferMixedSamplesCombinationsNV
// vkGetPhysicalDeviceOpticalFlowImageFormatsNV
// vkGetPhysicalDeviceExternalImageFormatPropertiesNV
// VkExternalImageFormatPropertiesNV 

namespace MantleGfx {
	
	class MgNvidiaPhysicalDevice::Internal : private IPhysicalDevice {
	public:
	
		Internal() :
			mInstance(VK_NULL_HANDLE),
			mSurface(VK_NULL_HANDLE)
		{}
	
		~Internal() 
		{}

		void constructPropertiesChain(MgPhysicalDevicePropertiesBundle& propertyBundle) {

		}

		void constructFeaturesChain(MgPhysicalDeviceFeaturesBundle& featureBundle) {

		}

		void constructQueueFamilyPropertiesChain(MgVkQueueFamilyPropertiesBundle& propertyBundle) {

		}

		void constructMemoryPropertiesChain(MgMemoryPropertiesBundle& propertyBundle) {

		}

		// Inherited from IPhysicalDevice
		bool destroyHandles() override {
	
			std::string message = "";
			bool isSuccessful = true;
	
			if ((mInstance != VK_NULL_HANDLE) && (mSurface != VK_NULL_HANDLE)) {
				message += "\nCan destroy surface";
			}
			else {
				message += "\nSurface is null, cannot destroy";
				isSuccessful = false;
			}
	
			if (mInstance != VK_NULL_HANDLE) {
				message += "\nCan destroy instance";
			}
			else {
				message += "\nInstance is null, cannot destroy";
				isSuccessful = false;
			}
	
			// If there are no null handles we can safely destroy them
			if (isSuccessful) {
				vkDestroySurfaceKHR(mInstance, mSurface, nullptr);
				vkDestroyInstance(mInstance, nullptr);
	
				message += "\nSuccessfully destroyed all handles";
				MantleLogger::logMessage(MantleLogger::MG_LOG_LEVEL_INTERNAL, message, FILENAME, FUNC, LINE);
	
			}
			else {
				message += "\nFailed to destroy handle(s)";
				MantleLogger::logMessage(MantleLogger::MG_LOG_LEVEL_INTERNAL_ERROR, message, FILENAME, FUNC, LINE);
			}
	
			return isSuccessful;
		}
	
		u32 getInstanceVersion() override {
			u32 apiVersion = 0;
	
			if (vkEnumerateInstanceVersion(&apiVersion) != VK_SUCCESS) {
				return -1;
			}
	
			return apiVersion;
		}
	
		VkResult createInstance() override {
	
			VkApplicationInfo applicationInfo = {};
			applicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
			applicationInfo.pNext = nullptr;
			applicationInfo.pApplicationName = "Mantle Graphics";
			applicationInfo.applicationVersion = VK_MAKE_VERSION(0, 0, 1);
			applicationInfo.pEngineName = "Mantle Graphics Engine";
			applicationInfo.engineVersion = VK_MAKE_VERSION(0, 0, 1);
			applicationInfo.apiVersion = VK_API_VERSION_1_3;
	
			VkInstanceCreateInfo instanceCreateInfo = {};
			instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
			instanceCreateInfo.pNext = nullptr;
			instanceCreateInfo.flags = 0;
			instanceCreateInfo.pApplicationInfo = &applicationInfo;
			instanceCreateInfo.enabledLayerCount = static_cast<u32>(mInstanceLayers.size());
			instanceCreateInfo.ppEnabledLayerNames = mInstanceLayers.data();
			instanceCreateInfo.enabledExtensionCount = static_cast<u32>(mInstanceExtensions.size());
			instanceCreateInfo.ppEnabledExtensionNames = mInstanceExtensions.data();
	
			VkResult result = vkCreateInstance(&instanceCreateInfo, nullptr, &mInstance);
	
			std::string message = "";
			if (result != VK_SUCCESS) {
				message = "Failed to create a Vulkan instance";
				MantleLogger::logMessage(MantleLogger::MG_LOG_LEVEL_INTERNAL_ERROR, message, FILENAME, FUNC, LINE);
	
				return result;
			}
	
			message = "Success in creating a Vulkan instance";
			MantleLogger::logMessage(MantleLogger::MG_LOG_LEVEL_INTERNAL, message, FILENAME, FUNC, LINE);
	
			uint32_t numberOfGpus = 0;
			result = vkEnumeratePhysicalDevices(mInstance, &numberOfGpus, nullptr);
	
			if (result != VK_SUCCESS && result != VK_INCOMPLETE) {
				message = "Failed to gather physical device count";
				MantleLogger::logMessage(MantleLogger::MG_LOG_LEVEL_INTERNAL_ERROR, message, FILENAME, FUNC, LINE);
	
				return result;
			}
	
			mNvidiaGpusList.resize(numberOfGpus);
			result = vkEnumeratePhysicalDevices(mInstance, &numberOfGpus, mNvidiaGpusList.data());
	
			if (result != VK_SUCCESS && result != VK_INCOMPLETE) {
				message = "Failed to gather a list of physical devices";
				MantleLogger::logMessage(MantleLogger::MG_LOG_LEVEL_INTERNAL_ERROR, message, FILENAME, FUNC, LINE);
	
				return result;
			}
	
			// Filter NVIDIA GPUs
			const u32 NVIDIA_ID = 0x10DE;
			mNvidiaGpusList.erase(std::remove_if(mNvidiaGpusList.begin(), mNvidiaGpusList.end(), [NVIDIA_ID](VkPhysicalDevice gpu) {
	
				VkPhysicalDeviceProperties properties;
				vkGetPhysicalDeviceProperties(gpu, &properties);
	
				return properties.vendorID != NVIDIA_ID;
	
				}), mNvidiaGpusList.end());
	
			return result;
		}
	
		VkResult createDebugUtils() override {
			return VkResult();
		}
	
		void createWindowSurfaceCallbackFunc(void (*createWindowSurface)(VkInstance, VkSurfaceKHR*)) override {
			createWindowSurface(mInstance, &mSurface);
	
			std::string message = "Success in creating a vulkan surface";
			MantleLogger::logMessage(MantleLogger::MG_LOG_LEVEL_INTERNAL, message, FILENAME, FUNC, LINE);
		}
	
		std::vector<VkLayerProperties> getInstanceLayerProperties() override {
			u32 propertyCount = 0;
			VkResult result;
			std::string message = "";
	
			result = vkEnumerateInstanceLayerProperties(&propertyCount, nullptr);
	
			if (result != VK_SUCCESS && result != VK_INCOMPLETE) {
				message = "Failed to gather instance layer count";
				MantleLogger::logMessage(MantleLogger::MG_LOG_LEVEL_INTERNAL_ERROR, message, FILENAME, FUNC, LINE);
			}
	
			std::vector<VkLayerProperties> LayerProperties(propertyCount);
			result = vkEnumerateInstanceLayerProperties(&propertyCount, LayerProperties.data());
	
			if (result != VK_SUCCESS && result != VK_INCOMPLETE) {
				message = "Failed to gather instance layer list";
				MantleLogger::logMessage(MantleLogger::MG_LOG_LEVEL_INTERNAL_ERROR, message, FILENAME, FUNC, LINE);
			}
	
			return LayerProperties;
		}
	
		std::vector<VkExtensionProperties> getInstanceExtensionProperties() override {
			u32 propertyCount = 0;
			VkResult result;
			std::string message = "";
	
			result = vkEnumerateInstanceExtensionProperties(nullptr, &propertyCount, nullptr);
	
			if (result != VK_SUCCESS && result != VK_INCOMPLETE) {
				message = "Failed to gather instance extension count";
				MantleLogger::logMessage(MantleLogger::MG_LOG_LEVEL_INTERNAL_ERROR, message, FILENAME, FUNC, LINE);
			}
	
			std::vector<VkExtensionProperties> extensionProperties(propertyCount);
			result = vkEnumerateInstanceExtensionProperties(nullptr, &propertyCount, extensionProperties.data());
	
			if (result != VK_SUCCESS && result != VK_INCOMPLETE) {
				message = "Failed to gather instance extension list";
				MantleLogger::logMessage(MantleLogger::MG_LOG_LEVEL_INTERNAL_ERROR, message, FILENAME, FUNC, LINE);
			}
	
			return extensionProperties;
		}
	
		std::vector<VkExtensionProperties> getDeviceExtensionProperties(VkPhysicalDevice& physicalDevice) override {
			u32 propertyCount = 0;
			VkResult result;
			std::string message = "";
	
			result = vkEnumerateDeviceExtensionProperties(physicalDevice, NULL, &propertyCount, nullptr);
	
			if (result != VK_SUCCESS && result != VK_INCOMPLETE) {
				message = "Failed to gather physical device extension count";
				MantleLogger::logMessage(MantleLogger::MG_LOG_LEVEL_INTERNAL_ERROR, message, FILENAME, FUNC, LINE);
			}
	
			std::vector<VkExtensionProperties> extensionProperties(propertyCount);
			result = vkEnumerateDeviceExtensionProperties(physicalDevice, NULL, &propertyCount, extensionProperties.data());
	
			if (result != VK_SUCCESS && result != VK_INCOMPLETE) {
				message = "Failed to gather physical device extension list";
				MantleLogger::logMessage(MantleLogger::MG_LOG_LEVEL_INTERNAL_ERROR, message, FILENAME, FUNC, LINE);
			}
	
			return extensionProperties;
		}
	
		std::vector<VkLayerProperties> getDeviceLayerProperties(VkPhysicalDevice& physicalDevice) override {
			u32 propertyCount = 0;
			VkResult result;
			std::string message = "";

			result = vkEnumerateDeviceLayerProperties(physicalDevice, &propertyCount, nullptr);

			if (result != VK_SUCCESS && result != VK_INCOMPLETE) {
				message = "Failed to gather physical device layer count";
				MantleLogger::logMessage(MantleLogger::MG_LOG_LEVEL_INTERNAL_ERROR, message, FILENAME, FUNC, LINE);
			}

			std::vector<VkLayerProperties> LayerProperties(propertyCount);
			result = vkEnumerateDeviceLayerProperties(physicalDevice, &propertyCount, LayerProperties.data());

			if (result != VK_SUCCESS && result != VK_INCOMPLETE) {
				message = "Failed to gather physical device layer list";
				MantleLogger::logMessage(MantleLogger::MG_LOG_LEVEL_INTERNAL_ERROR, message, FILENAME, FUNC, LINE);
			}

			return LayerProperties;
		}
	
		void selectPhysicaldevice(VkPhysicalDevice& physicalDevice) override {
			MgPhysicalDeviceInfoBundle infoBundle = {};

			constructPropertiesChain(infoBundle.mgPropertiesBundle);
			constructFeaturesChain(infoBundle.mgFeaturesBundle);
			constructQueueFamilyPropertiesChain(infoBundle.mgQueueFamilyBundle);
			constructMemoryPropertiesChain(infoBundle.mgMemoryBundle);

			mActiveNvidiaGpusMap.insert(std::make_pair(physicalDevice, infoBundle));
		}

		u32 findQueueFamilies(VkQueueFlagBits queueFlag) override {
			return u32();
		}
	
	public:
		VkInstance							mInstance;
		VkSurfaceKHR						mSurface;

		std::vector<VkPhysicalDevice>		mNvidiaGpusList;
		ActiveNvidiaMap						mActiveNvidiaGpusMap;
	
		std::vector<const char*>			mInstanceLayers; 
		std::vector<const char*>			mInstanceExtensions;
	
	};// !MgNvidiaPhysicalDevice::Internal
	
	// MgNvidiaPhysicalDevice ==========================================================================
	
	MgNvidiaPhysicalDevice::MgNvidiaPhysicalDevice() :
		pInternal(std::make_unique<MgNvidiaPhysicalDevice::Internal>()) 
	{
	}
	
	MgNvidiaPhysicalDevice::~MgNvidiaPhysicalDevice() {
	}
	
	bool MgNvidiaPhysicalDevice::mgDestroyHandles() {
		return pInternal->destroyHandles();
	}
	
	u32 MgNvidiaPhysicalDevice::mgGetInstanceVersion() {
		return pInternal->getInstanceVersion();
	}

	VkResult MgNvidiaPhysicalDevice::mgCreateApplicationInstance(const std::vector<const char*>& layers, const std::vector<const char*>& extensions) {
		pInternal->mInstanceLayers = layers;
		pInternal->mInstanceExtensions = extensions;
		return pInternal->createInstance();
	}

	VkResult MgNvidiaPhysicalDevice::mgCreateDebugUtils() {
		return pInternal->createDebugUtils();
	}
	
	void MgNvidiaPhysicalDevice::mgCreateWindowSurfaceCallbackFunc(void (*createWindowSurface)(VkInstance, VkSurfaceKHR*)) {
		pInternal->createWindowSurfaceCallbackFunc(createWindowSurface);
	}

	VkInstance MgNvidiaPhysicalDevice::mgGetInstance() const {
		return pInternal->mInstance;
	}

	std::vector<VkLayerProperties> MgNvidiaPhysicalDevice::mgGetInstanceLayerProperties() {
		return pInternal->getInstanceLayerProperties();
	}

	std::vector<VkExtensionProperties> MgNvidiaPhysicalDevice::mgGetInstanceExtensionProperties() {
		return pInternal->getInstanceExtensionProperties();
	}

	std::vector<VkExtensionProperties> MgNvidiaPhysicalDevice::mgGetDeviceExtensionProperties(VkPhysicalDevice& physicalDevice) {
		return pInternal->getDeviceExtensionProperties(physicalDevice);
	}
	std::vector<VkLayerProperties> MgNvidiaPhysicalDevice::mgGetDeviceLayerProperties(VkPhysicalDevice& physicalDevice) {
		return pInternal->getDeviceLayerProperties(physicalDevice);
	}
	
	std::vector<VkPhysicalDevice> MgNvidiaPhysicalDevice::mgGetPhyscalDeviceList() const {
		return pInternal->mNvidiaGpusList;
	}

	ActiveNvidiaMap MgNvidiaPhysicalDevice::mgGetPhysicalDeviceInfoMap() const {
		return pInternal->mActiveNvidiaGpusMap;
	}

	VkSurfaceKHR MgNvidiaPhysicalDevice::mgGetSurface() const {
		return pInternal->mSurface;
	}
	
	void MgNvidiaPhysicalDevice::mgSelectPhysicalDevice(VkPhysicalDevice& physicalDevice) {
		return pInternal->selectPhysicaldevice(physicalDevice);
	}

	u32 MgNvidiaPhysicalDevice::mgFindQueueFamilies(VkQueueFlagBits queueFlag) {
		return pInternal->findQueueFamilies(queueFlag);
	}

}// !MantleGfx