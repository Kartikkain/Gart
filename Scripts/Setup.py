import os
import subprocess
import CheckPython

CheckPython.validatePackages()

import Vulkan

os.chdir('../')

if(not Vulkan.CheckVulkanSDK()):
    print("Vulkan SDK not installed.")

if(not Vulkan.CheckVulkanSDKDebugLibs()):
    print("Vulkan SDK debug libs not found.")

print("Running premake...")
subprocess.call(["vendor/bin/premake/premake5.exe","vs2022"])