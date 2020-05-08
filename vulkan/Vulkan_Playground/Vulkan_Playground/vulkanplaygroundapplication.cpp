//
//  vulkanplaygroundapplication.cpp
//  Vulkan_Playground
//
//  Created by apple on 2020/4/24.
//  Copyright © 2020 National Taiwan University. All rights reserved.
//

#include "vulkanplaygroundapplication.h"

#include <GLFW/glfw3.h>
#include <iostream>

void VulkanPlaygroundApplication::run() {
    initWindow();
    initVulkan();
    mainLoop();
    postCleanUp();
}

void VulkanPlaygroundApplication::initWindow() {
    glfwInit();
    
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    _window = glfwCreateWindow(800, 600, "Vulkan Playground", nullptr, nullptr);
}

void VulkanPlaygroundApplication::mainLoop() {
    while (!glfwWindowShouldClose(_window)) {
        glfwPollEvents();
    }
}

void VulkanPlaygroundApplication::initVulkan() {
    // Init VKInstance
    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "vulkan_playground";
    appInfo.applicationVersion = 1;
    appInfo.pEngineName = "No_Engine";
    appInfo.engineVersion = 1;
    appInfo.apiVersion = VK_API_VERSION_1_0;
    
    // Fill in VkCreationInfo
    VkInstanceCreateInfo creationInfo{};
    creationInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    creationInfo.pApplicationInfo = &appInfo;
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
    creationInfo.ppEnabledExtensionNames = glfwExtensions;
    creationInfo.enabledExtensionCount = glfwExtensionCount;
    creationInfo.enabledLayerCount = 0;
    
    if (vkCreateInstance(&creationInfo, nullptr, &_instance) != VK_SUCCESS) {
        throw std::runtime_error("Fail to create vulkan instance.");
    }
    
    // Create Window surface
    createSurface();
    
    // Init a physical device
    initPhysicalDevice();
    
    // Create logical devices and queue
    createLogicalDevice();
    
    // Create Swap Chain
    createSwapChain();
    
    // Create Swap Chain Image views
    
}

void VulkanPlaygroundApplication::postCleanUp() {
    for (auto imageView : _swapChainImageViews) {
        vkDestroyImageView(_logicalDevice, imageView, nullptr);
    }
    for (auto image : _swapChainImages) {
        vkDestroyImage(_logicalDevice, image, nullptr);
    }
    vkDestroySwapchainKHR(_logicalDevice, _swapChain, nullptr);
    vkDestroyDevice(_logicalDevice, nullptr);
    vkDestroySurfaceKHR(_instance, _surface, nullptr);
    vkDestroyInstance(_instance, nullptr);
    glfwDestroyWindow(_window);
    glfwTerminate();
}

void VulkanPlaygroundApplication::createSurface() {
    if (glfwCreateWindowSurface(_instance, _window, nullptr, &_surface) != VK_SUCCESS) {
        throw std::runtime_error("Fail to create window surface.");
    }
}

void VulkanPlaygroundApplication::initPhysicalDevice() {
    _device = VK_NULL_HANDLE;
    // Get all devices
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(_instance, &deviceCount, nullptr);
    std::cout << deviceCount << " devices found." << std::endl;
    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(_instance, &deviceCount, devices.data());
    if (deviceCount == 0) {
        throw std::runtime_error("Failed to find GPU with Vulkan support");
    }
    
    // Check devices one by one
    int bestScore = 0;
    VkPhysicalDevice pickedDevice = VK_NULL_HANDLE;
    for (const auto& phyDevice : devices) {
        int curScore = rateDevice(phyDevice);
        if (curScore > bestScore) {
            pickedDevice = phyDevice;
            bestScore = curScore;
            break;
        }
    }
    
    // Throw an error when no device is found
    if (pickedDevice == VK_NULL_HANDLE) {
        throw std::runtime_error("Fail to find a suitable GPU");
    } else {
        _device = pickedDevice;
        std::cout << "a device is found." << std::endl;
    }
}

void VulkanPlaygroundApplication::createLogicalDevice() {
    QueueFamilyIndicies indicies = findQueueFamilies(_device);
    
    VkDeviceQueueCreateInfo queueCreateInfo{};
    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueFamilyIndex = indicies.graphicsFamily.value();
    queueCreateInfo.queueCount = 1;
    float queuePriority = 1.0;
    queueCreateInfo.pQueuePriorities = &queuePriority;
    
    VkPhysicalDeviceFeatures deviceFeatures{};
    VkDeviceCreateInfo deviceCreateInfo{};
    deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceCreateInfo.pQueueCreateInfos = &queueCreateInfo;
    deviceCreateInfo.queueCreateInfoCount = 1;
    deviceCreateInfo.pEnabledFeatures = &deviceFeatures;
    deviceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
    deviceCreateInfo.ppEnabledExtensionNames = deviceExtensions.data();
    
    if (vkCreateDevice(_device, &deviceCreateInfo, nullptr, &_logicalDevice) !=
        VK_SUCCESS) {
        throw std::runtime_error("Fail to create logical devices.");
    }
    
    vkGetDeviceQueue(_logicalDevice, indicies.graphicsFamily.value(), 0, &_graphicsQueue);
    vkGetDeviceQueue(_logicalDevice, indicies.presentFamily.value(), 0, &_presentQueue);
}

void VulkanPlaygroundApplication::createSwapChain() {
    SwapChainSupportDetails details;
    
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(_device, _surface, &details.capabilities);
    
    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(_device, _surface, &formatCount, nullptr);
    if (formatCount != 0) {
        details.formats.resize(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(_device, _surface, &formatCount, details.formats.data());
    }
    
    uint32_t presentModeCount = 0;
    vkGetPhysicalDeviceSurfacePresentModesKHR(_device, _surface, &presentModeCount, nullptr);
    if (presentModeCount != 0) {
        details.presentModes.resize(presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(_device, _surface, &presentModeCount, details.presentModes.data());
    }
    
    VkSurfaceFormatKHR surfaceFormat = chooseSurfaceFormat(details);
    VkPresentModeKHR presentMode = choosePresentMode(details);
    VkExtent2D extent = chooseSwapExtent(details);
    
    uint32_t imageCount = details.capabilities.minImageCount + 1;
    if (imageCount > details.capabilities.maxImageCount && details.capabilities.maxImageCount > 0) {
        imageCount = details.capabilities.maxImageCount;
    }
    
    VkSwapchainCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = _surface;
    createInfo.imageFormat = surfaceFormat.format;
    createInfo.minImageCount = imageCount;
    createInfo.imageExtent = extent;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    
    if (vkCreateSwapchainKHR(_logicalDevice, &createInfo, nullptr, &_swapChain) != VK_SUCCESS) {
        throw std::runtime_error("Fail to create a swap chain.");
    }
    
    std::cout << "Swap chain created." << std::endl;
    
    uint32_t vkimageCount = 0;
    vkGetSwapchainImagesKHR(_logicalDevice, _swapChain, &vkimageCount, nullptr);
    if (imageCount > 0) {
        _swapChainImages.resize(imageCount);
        vkGetSwapchainImagesKHR(_logicalDevice, _swapChain, &vkimageCount, _swapChainImages.data());
    }
    _swapChainImageFormat = surfaceFormat.format;
    _swapChainExtent = extent;
}

void VulkanPlaygroundApplication::creatImageViews() {
    _swapChainImageViews.resize(_swapChainImages.size());
    for (size_t i = 0; i < _swapChainImageViews.size(); ++i) {
        VkImageViewCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        createInfo.image = _swapChainImages[i];
        createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        createInfo.format = _swapChainImageFormat;
        createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        createInfo.subresourceRange.baseMipLevel = 0;
        createInfo.subresourceRange.levelCount = 1;
        createInfo.subresourceRange.baseArrayLayer = 0;
        createInfo.subresourceRange.layerCount = 1;
        
        if (vkCreateImageView(_logicalDevice, &createInfo, nullptr, &_swapChainImageViews[i]) != VK_SUCCESS) {
            throw std::runtime_error("failed to create image views!");
        }
    }
    
    std::cout << "Image Views created." << std::endl;
}

int VulkanPlaygroundApplication::rateDevice(VkPhysicalDevice device) const {
    int score = 0;
    
    VkPhysicalDeviceProperties properties;
    VkPhysicalDeviceFeatures features;
    vkGetPhysicalDeviceProperties(device, &properties);
    vkGetPhysicalDeviceFeatures(device, &features);
    
    if (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
        score += 1000;
    }
    
    score += properties.limits.maxImageDimension2D;
    
    QueueFamilyIndicies indicies = findQueueFamilies(device);
    if (!indicies.isComplete()) {
        return 0;
    }
    
    return score;
}

QueueFamilyIndicies VulkanPlaygroundApplication::findQueueFamilies(VkPhysicalDevice device) const {
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());
    
    QueueFamilyIndicies indicies;
    uint32_t i = 0;
    for (const auto& queueFamily : queueFamilies) {
        if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            indicies.graphicsFamily = i;
            
            VkBool32 presentSupport = false;
            vkGetPhysicalDeviceSurfaceSupportKHR(device, i, _surface, &presentSupport);
            if (presentSupport) {
                indicies.presentFamily = i;
            }
        }
        ++i;
    }
    
    return indicies;
}

VkSurfaceFormatKHR VulkanPlaygroundApplication::chooseSurfaceFormat(const SwapChainSupportDetails& details) const {
    for (auto& curFormat : details.formats) {
        if (curFormat.format == VK_FORMAT_B8G8R8_SRGB &&
            curFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
            return curFormat;
        }
    }
    
    return details.formats[0];
}

VkPresentModeKHR VulkanPlaygroundApplication::choosePresentMode(const SwapChainSupportDetails& details) const {
    for (auto& curPresentMode : details.presentModes) {
        if (curPresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
            return curPresentMode;
        }
    }
    
    return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D VulkanPlaygroundApplication::chooseSwapExtent(const SwapChainSupportDetails& details) const {
    VkExtent2D actualExtent = {
        800,
        600
    };
    return actualExtent;
}
