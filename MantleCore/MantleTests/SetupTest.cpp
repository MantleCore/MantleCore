
#include "include/SDL.h"
#include "include/SDL_vulkan.h"
#undef main

#define MANTLE_GFX
#include "MantleCore.h"
#include "Logger.h"


const int WIDTH = 800;
const int HEIGHT = 600;

SDL_Window* window;

void createSDLSurfaceCallbackFunc(VkInstance instance, VkSurfaceKHR* surface) 
{
    if (!SDL_Vulkan_CreateSurface(window, instance, surface)) {
        throw std::runtime_error("failed to create surface");
    }
}

int main() {
    SDL_Init(SDL_INIT_VIDEO);

    // Create an SDL window
    window = SDL_CreateWindow("Mantle Graphics - Vulkan Renderer",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        WIDTH, HEIGHT,
        SDL_WINDOW_VULKAN);

    std::string message = "This is a test Message";
    MantleLogger::logMessage(MantleLogger::MG_LOG_LEVEL_FATAL, message, FILENAME, FUNC, LINE);
    MantleLogger::logMessage(MantleLogger::MG_LOG_LEVEL_ERROR, message, FILENAME, FUNC, LINE);
    MantleLogger::logMessage(MantleLogger::MG_LOG_LEVEL_WARNING, message, FILENAME, FUNC, LINE);
    MantleLogger::logMessage(MantleLogger::MG_LOG_LEVEL_INFO, message, FILENAME, FUNC, LINE);
    MantleLogger::logMessage(MantleLogger::MG_LOG_LEVEL_DEBUG, message, FILENAME, FUNC, LINE);
    MantleLogger::logMessage(MantleLogger::MG_LOG_LEVEL_INTERNAL, message, FILENAME, FUNC, LINE);
    MantleLogger::logMessage(MantleLogger::MG_LOG_LEVEL_INTERNAL_ERROR, message, FILENAME, FUNC, LINE);
    MantleLogger::logMessage(MantleLogger::MG_LOG_LEVEL_INTERNAL_FATAL, message, FILENAME, FUNC, LINE);
    std::cout << "\n\n";


    MantleGfx::MgNvidiaPhysicalDevice nvGpu;
    std::vector<const char*> layers;

    u32 extensionCount = 0;
    SDL_Vulkan_GetInstanceExtensions(window, &extensionCount, nullptr);
    std::vector<const char*> extensions(extensionCount);
    SDL_Vulkan_GetInstanceExtensions(window, &extensionCount, extensions.data());

    nvGpu.mgCreateApplicationInstance(layers, extensions);

    std::cout << "\n\nEXT Properties-----------------------" << std::endl;
    auto extProperties = nvGpu.mgGetInstanceExtensionProperties();
    for (int i = 0; i < extProperties.size(); i++) {
        std::cout << "EXT: " << extProperties[i].extensionName << std::endl;
        std::cout << "EXT: " << extProperties[i].specVersion << std::endl;
    }

    std::cout << "\n\nLayer Properties---------------------" << std::endl;
    auto layProperties = nvGpu.mgGetInstanceLayerProperties();
    for (int i = 0; i < layProperties.size(); i++) {
        std::cout << "Layer: " << layProperties[i].description << std::endl;
        std::cout << "Layer: " << layProperties[i].implementationVersion << std::endl;
        std::cout << "Layer: " << layProperties[i].layerName << std::endl;
        std::cout << "Layer: " << layProperties[i].specVersion << std::endl;
    }

    std::cout << "\n\nAPI Version: " << nvGpu.mgGetInstanceVersion() << std::endl;

    nvGpu.mgCreateWindowSurfaceCallbackFunc(createSDLSurfaceCallbackFunc);
    
    auto gpuList = nvGpu.mgGetPhyscalDeviceList();
    nvGpu.mgSelectPhysicalDevice(gpuList.at(0));

    // Main loop
    bool quit = false;
    while (!quit) {

        SDL_Event event;
        while (SDL_PollEvent(&event)) {

            if ((event.type == SDL_QUIT) && nvGpu.mgDestroyHandles()) {
                quit = true;
            }
        }
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
}