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

#include <vector>

class GLFWwindow;

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
    
private:
    GLFWwindow* _window;
    VkInstance _instance;
    VkSurfaceKHR _surface;
};

#endif /* vulkanplaygroundapplication_h */
