OS := $(shell uname)

all:
ifeq ($(OS), Darwin)
	@echo "OS" $(OS)
	cmake -H. -B_project -G "Xcode" -DCMAKE_INSTALL_PREFIX="_install"
else
	@echo "OS" $(OS)
	@echo "Generate Release Makefiles"
	cmake -H. -B_project -DCMAKE_BUILD_TYPE=Release -G "Unix Makefiles" -DCMAKE_INSTALL_PREFIX="_install"
	cd _project && make
endif

debug:
	@echo "OS" $(OS)
	@echo "Generate Debug Makefiles"
	cmake -H. -B_project -DCMAKE_BUILD_TYPE=Debug -G "Unix Makefiles" -DCMAKE_INSTALL_PREFIX="_install"

release:
	@echo "OS" $(OS)
	@echo "Generate Release Makefiles"
	cmake -H. -B_project -DCMAKE_BUILD_TYPE=Release -G "Unix Makefiles" -DCMAKE_INSTALL_PREFIX="_install"

clean:
	rm -r _project & rm -r _install