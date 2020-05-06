//
//  main.cpp
//  Vulkan_Playground
//
//  Created by apple on 2020/4/23.
//  Copyright © 2020 National Taiwan University. All rights reserved.
//

#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <vulkan/vulkan.h>

#include "vulkanplaygroundapplication.h"

int main(int argc, const char * argv[]) {
    VulkanPlaygroundApplication app;
    
    try {
        app.run();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
