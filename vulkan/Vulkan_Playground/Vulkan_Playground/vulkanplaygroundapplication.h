//
//  vulkanplaygroundapplication.h
//  Vulkan_Playground
//
//  Created by apple on 2020/4/24.
//  Copyright © 2020 National Taiwan University. All rights reserved.
//

#ifndef vulkanplaygroundapplication_h
#define vulkanplaygroundapplication_h

#include <vulkan/vulkan.h>

#include <optional>
#include <vector>

class GLFWwindow;

const std::vector<const char *> deviceExtensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

struct QueueFamilyIndicies {
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;
    
    bool isComplete() const {
        return graphicsFamily.has_value() &&
        presentFamily.has_value();
    }
};

struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};

class VulkanPlaygroundApplication {
public:
    VulkanPlaygroundApplication() {}
    
    void run();
    
private:
    void initWindow();
    void initVulkan();
    void mainLoop();
    void postCleanUp();
    
    void createSurface();
    void createSwapChain();
    void initPhysicalDevice();
    void createLogicalDevice();
    void creatImageViews();
    int rateDevice(VkPhysicalDevice device) const;
    QueueFamilyIndicies findQueueFamilies(VkPhysicalDevice device) const;
    
    VkSurfaceFormatKHR chooseSurfaceFormat(const SwapChainSupportDetails&) const;
    VkPresentModeKHR choosePresentMode(const SwapChainSupportDetails&) const;
    VkExtent2D chooseSwapExtent(const SwapChainSupportDetails&) const;
    
private:
    GLFWwindow* _window;
    VkInstance _instance;
    VkPhysicalDevice _device;
    VkSurfaceKHR _surface;
    VkDevice _logicalDevice;
    VkQueue _graphicsQueue;
    VkQueue _presentQueue;
    VkSwapchainKHR _swapChain;
    std::vector<VkImage> _swapChainImages;
    VkFormat _swapChainImageFormat;
    VkExtent2D _swapChainExtent;
    std::vector<VkImageView> _swapChainImageViews;
};

#endif /* vulkanplaygroundapplication_h */
