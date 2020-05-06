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
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensions);
    creationInfo.ppEnabledExtensionNames = glfwExtensions;
    creationInfo.enabledExtensionCount = glfwExtensionCount;
    creationInfo.enabledLayerCount = 0;
    
    if (vkCreateInstance(&creationInfo, nullptr, &_instance) != VK_SUCCESS) {
        throw std::runtime_error("Fail to create vulkan instance.");
    }
    
    // Create Window surfaces
    createSurface();
    
    // Create Frame buffers
    
}

void VulkanPlaygroundApplication::postCleanUp() {
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
