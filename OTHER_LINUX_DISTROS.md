##### Debian Bullseye

_Note:_ Only tested in a Docker environment.

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

##### Automatic Git Clone and Build via Commandline

```
git clone https://github.com/cg-tuwien/VulkanLaunchpadStarter.git && \
	cd VulkanLaunchpadStarter && \
	git checkout initialize-repo && \
	mkdir out && cd out && \
	cmake -G Ninja .. && \
	cmake --build . --config Debug
```