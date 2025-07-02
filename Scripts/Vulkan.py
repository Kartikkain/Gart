import os
import subprocess
import sys
import platform
from pathlib import Path

import Utils
import shutil

from io import BytesIO
from  urllib.request import urlopen
from zipfile import ZipFile


def get_vulkan_sdk_url():
    base = 'https://sdk.lunarg.com/sdk/download'
    version = GART_VULKAN_VERSION
    if platform.machine().endswith('64'):
        return f'{base}/{version}/windows/VulkanSDK-{version}-Installer.exe'
    else:
        return f'{base}/{version}/windows/vulkan_sdk.exe'



VULKAN_SDK = os.environ.get('VULKAN_SDK')
GART_VULKAN_VERSION ='1.3.275.0' if platform.machine().endswith('64') else '1.2.170.0'
VULKAN_SDK_INSTALLER_URL = get_vulkan_sdk_url()
VULKAN_SDK_EXE_PATH = 'Gart/vendor/VulkanSDK/VulkanSDK.exe'


def InstallVulkanSDK():
    print('Downloading  {} to {}'.format(VULKAN_SDK_INSTALLER_URL,VULKAN_SDK_EXE_PATH))
    Utils.DownaloadFile(VULKAN_SDK_INSTALLER_URL,VULKAN_SDK_EXE_PATH)
    print("Done!")
    print("Running Vulkan SDK installer...")
    os.startfile(os.path.abspath(VULKAN_SDK_EXE_PATH))
    print("Re-run this script after installation")

def InstallVulkanPrompt():
    print("Would you like to install the Vulkan SDK?")
    install = Utils.YesOrNo()
    if(install):
        InstallVulkanSDK()
        quit()

def CheckVulkanSDK():
    if(VULKAN_SDK is None):
        print("You don't have the Vulkan SDK installed!")
        InstallVulkanPrompt()
        return False
    elif (GART_VULKAN_VERSION not in VULKAN_SDK):
        print(f"Located Vulkan SDK at {VULKAN_SDK}")
        print(f"You don't have the correct Vulkan SDK version! (Hazel requires {GART_VULKAN_VERSION})")
        InstallVulkanPrompt()
        return False
    
    print(f"Correct Vulkan SDK located at {VULKAN_SDK}")
    return True



VulkanSDKDebugLibsURL = f'{VULKAN_SDK}'
OutputDirectory = "Gart/vendor/VulkanSDK"


def CheckVulkanSDKDebugLibs():
    shadercdLib = Path(f"{OutputDirectory}/Lib/shaderc_sharedd.lib")

    if(not shadercdLib.exists()):
        print(f"No Vulkan SDK debug libs found. (Checked {shadercdLib})")
        print("Coping to folder", OutputDirectory)
        CopyVulkanFolder(VulkanSDKDebugLibsURL,OutputDirectory)
        return False
    
    print("All file exists")
    return True

def CopyVulkanFolder(installDir, DestDir):
    folders_to_copy = ["Bin","Lib"]
    for folder in folders_to_copy:
        src_path = os.path.join(installDir,folder)
        des_path = os.path.join(DestDir,folder)
        if os.path.exists(des_path):
            print(f'Debug lib folder {folder} already exists in {DestDir}')
        else:
            if os.path.exists(src_path):
                shutil.copytree(src_path,des_path)
                print(f'Debug lib folder {folder} copied to {DestDir}')
            else:
                print(f' The {src_path} does not exist!')
