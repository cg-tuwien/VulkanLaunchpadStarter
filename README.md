# Vulkan Launchpad Starter
Starter template for a project based on Vulkan Launchpad



# Linux

## Dependencies

### Ubuntu 22.04 (officially recommended by us)

```
#!/bin/bash
set -e -o pipefail

sudo apt update && sudo apt upgrade -y && sudo apt install -y wget gpg lsb-release software-properties-common

wget -O - https://apt.kitware.com/keys/kitware-archive-latest.asc 2>/dev/null | gpg --dearmor - | sudo tee /etc/apt/trusted.gpg.d/kitware.gpg >/dev/null
sudo rm /etc/apt/trusted.gpg.d/kitware.gpg && sudo apt-key adv --keyserver keyserver.ubuntu.com --recv-keys 6AF7F09730B3F0A4
sudo apt-add-repository -y "deb https://apt.kitware.com/ubuntu/ $(lsb_release -cs) main"
sudo add-apt-repository -y ppa:ubuntu-toolchain-r/test
sudo add-apt-repository -y ppa:oibaf/graphics-drivers
wget -qO- https://packages.lunarg.com/lunarg-signing-key-pub.asc | tee /etc/apt/trusted.gpg.d/lunarg.asc
wget -qO /etc/apt/sources.list.d/lunarg-vulkan-jammy.list http://packages.lunarg.com/vulkan/lunarg-vulkan-jammy.list

sudo apt update && sudo apt upgrade -y && sudo apt install -y  g++ gdb make ninja-build rsync zip kitware-archive-keyring cmake libassimp-dev g++-11 libvulkan-dev libvulkan1 mesa-vulkan-drivers vulkan-tools vulkan-sdk dpkg-dev libvulkan1-dbgsym vulkan-tools-dbgsym libglfw3 libglfw3-dev libgl1-mesa-dev libglu1-mesa-dev libglew-dev libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev
sudo apt clean all

sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-11 60 --slave /usr/bin/g++ g++ /usr/bin/g++-11

echo ""
echo "Now running \"vulkaninfo\" to see if vulkan has been installed successfully:"
vulkaninfo
```

### Ubuntu 20.04 (should work but we only tested the build in a docker environment, not if it runs!)

```
sudo apt update && sudo apt upgrade -y && sudo apt install -y wget gpg git lsb-release software-properties-common && \
	wget -O - https://apt.kitware.com/keys/kitware-archive-latest.asc 2>/dev/null | gpg --dearmor - | sudo tee /etc/apt/trusted.gpg.d/kitware.gpg >/dev/null && \
	sudo rm /etc/apt/trusted.gpg.d/kitware.gpg && sudo apt-key adv --keyserver keyserver.ubuntu.com --recv-keys 6AF7F09730B3F0A4 && \
	sudo apt-add-repository -y "deb https://apt.kitware.com/ubuntu/ $(lsb_release -cs) main" && \
	sudo add-apt-repository -y ppa:ubuntu-toolchain-r/test && \
	sudo add-apt-repository -y ppa:oibaf/graphics-drivers && \
	wget -qO- https://packages.lunarg.com/lunarg-signing-key-pub.asc | tee /etc/apt/trusted.gpg.d/lunarg.asc && \
	wget -qO /etc/apt/sources.list.d/lunarg-vulkan-focal.list http://packages.lunarg.com/vulkan/lunarg-vulkan-focal.list && \
	sudo apt update && sudo apt upgrade -y && sudo apt install -y g++ gdb make ninja-build rsync zip kitware-archive-keyring cmake libassimp-dev g++-11 libvulkan-dev libvulkan1 mesa-vulkan-drivers vulkan-tools vulkan-sdk dpkg-dev libvulkan1-dbgsym vulkan-tools-dbgsym libglfw3 libglfw3-dev libgl1-mesa-dev libglu1-mesa-dev libglew-dev libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev && \
	sudo apt clean all && \
	sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-11 60 --slave /usr/bin/g++ g++ /usr/bin/g++-11
```

### Linux Mint 21.1 (should work but we only tested the build in a docker environment, not if it runs!)

```
sudo apt update && sudo apt upgrade -y && sudo apt install -y mint-dev-tools build-essential devscripts fakeroot quilt dh-make automake libdistro-info-perl less nano ubuntu-dev-tools python3 \
		wget git gpg lsb-release software-properties-common && \
	wget -O - https://apt.kitware.com/keys/kitware-archive-latest.asc 2>/dev/null | gpg --dearmor - | sudo tee /etc/apt/trusted.gpg.d/kitware.gpg >/dev/null && \
	sudo rm /etc/apt/trusted.gpg.d/kitware.gpg && sudo apt-key adv --keyserver keyserver.ubuntu.com --recv-keys 6AF7F09730B3F0A4 && \
	sudo apt-add-repository -y "deb https://apt.kitware.com/ubuntu/ jammy main" && \
	sudo add-apt-repository -y ppa:ubuntu-toolchain-r/test && \
	sudo add-apt-repository -y ppa:oibaf/graphics-drivers && \
	wget -qO- https://packages.lunarg.com/lunarg-signing-key-pub.asc | tee /etc/apt/trusted.gpg.d/lunarg.asc && \
	wget -qO /etc/apt/sources.list.d/lunarg-vulkan-jammy.list http://packages.lunarg.com/vulkan/lunarg-vulkan-jammy.list && \
	sudo apt update && sudo apt upgrade -y && sudo apt install -y  g++ gdb make ninja-build rsync zip kitware-archive-keyring cmake libassimp-dev g++-11 libvulkan-dev libvulkan1 mesa-vulkan-drivers vulkan-tools vulkan-sdk dpkg-dev libvulkan1-dbgsym vulkan-tools-dbgsym libglfw3 libglfw3-dev libgl1-mesa-dev libglu1-mesa-dev libglew-dev libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev && \
	sudo apt clean all && \
	sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-11 60 --slave /usr/bin/g++ g++ /usr/bin/g++-11
```

### Debian Bullseye (should work but we only tested the build in a docker environment, not if it runs!)

```
sudo apt update && sudo apt upgrade -y && sudo apt install -y wget sudo gpg git lsb-release software-properties-common build-essential checkinstall zlib1g-dev libssl-dev g++ gdb make ninja-build rsync zip bison libx11-xcb-dev libxkbcommon-dev libwayland-dev libxrandr-dev libxcb-randr0-dev autotools-dev libxcb-keysyms1 libxcb-keysyms1-dev libxcb-ewmh-dev pkg-config libglfw3 libglfw3-dev libgl1-mesa-dev libglu1-mesa-dev libglew-dev libxinerama-dev libxcursor-dev libxcb-cursor-dev libxi-dev && \
	wget -q https://github.com/Kitware/CMake/releases/download/v3.25.1/cmake-3.25.1.tar.gz && \
	tar -zxvf cmake-3.25.1.tar.gz && cd cmake-3.25.1 && ./bootstrap && make -j12 && make install

	# Individual Vulkan repos below:
	# 1. https://github.com/KhronosGroup/Vulkan-Loader/blob/master/BUILD.md#building-on-linux
	#apt install -y git build-essential libx11-xcb-dev libxkbcommon-dev libwayland-dev libxrandr-dev
	#cd / && git clone https://github.com/KhronosGroup/Vulkan-Loader.git && cd Vulkan-Loader && mkdir build && cd build && cmake -DUPDATE_DEPS=ON .. && make

	# 2. https://github.com/KhronosGroup/Vulkan-ValidationLayers/blob/master/BUILD.md#building-on-linux
	#apt install -y pkg-config git build-essential libx11-xcb-dev libxkbcommon-dev libwayland-dev libxrandr-dev libegl1-mesa-dev python3-distutils
	#cd / && git clone https://github.com/KhronosGroup/Vulkan-ValidationLayers.git && cd Vulkan-ValidationLayers && mkdir build && cd build && cmake -DUPDATE_DEPS=ON .. && make

	# 3. https://github.com/LunarG/gfxreconstruct/blob/dev/BUILD.md#building-for-linux
	#apt install -y git build-essential libx11-xcb-dev libxcb-keysyms1-dev libwayland-dev libxrandr-dev zlib1g-dev liblz4-dev libzstd-dev
	#git clone https://github.com/LunarG/gfxreconstruct.git && cd gfxreconstruct && git submodule update --init --recursive && mkdir build && cd build && cmake .. && make

	# This one repo includes the ones above as submodules anyways:
	# 4. https://github.com/LunarG/VulkanTools/blob/master/BUILD.md
git clone https://github.com/LunarG/VulkanTools.git && cd VulkanTools && git submodule update --init --recursive && mkdir build && ./update_external_sources.sh && cd build && python3 ../scripts/update_deps.py && cmake -C helper.cmake .. && cmake --build . --parallel
```

Build:

```
git clone https://github.com/cg-tuwien/VulkanLaunchpadStarter.git && \
	cd VulkanLaunchpadStarter && \
	git checkout initialize-repo && \
	mkdir out && cd out && \
	cmake -DVulkan_INCLUDE_DIR=/VulkanTools/build/Vulkan-Headers/build/install/include/ -DVulkan_LIBRARY=/VulkanTools/build/Vulkan-Loader/build/install/lib/libvulkan.so -G Ninja .. && \
	cmake --build . --config Debug
```

## Automatic git clone and build via commandline:

```
git clone https://github.com/cg-tuwien/VulkanLaunchpadStarter.git && \
	cd VulkanLaunchpadStarter && \
	git checkout initialize-repo && \
	mkdir out && cd out && \
	cmake -G Ninja .. && \
	cmake --build . --config Debug
```

