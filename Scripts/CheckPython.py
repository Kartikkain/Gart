import subprocess
import sys
from importlib.metadata import packages_distributions

def install(package):
    print(f"Insalling {package} module...")
    subprocess.check_call(['python','-m','pip','install',package])

def is_package_installed(package_name):
    return package_name.lower() in packages_distributions()

def validatePackage(package):
    if not is_package_installed(package):
        install(package)

def validatePackages():
    validatePackage("requests")
    validatePackage("fake-useragent")

 


