.PHONY: config configure build release clean rebuild run lldb debug doc windows scripting

BUILD_SYSTEM = Ninja
XCODE_BUILD_SYSTEM = Xcode
BACKUP_BUILD_SYSTEM = 'Unix Makefiles'
WINDOWS_BUILD_SYSTEM = 'MinGW Makefiles'
BUILD_FOLDER = build/bin
BINARY_NAME = supergoon
BINARY_PATH = $(BUILD_FOLDER)/$(BINARY_NAME)

all: build run

configure:
	@cmake . -B build -D CMAKE_BUILD_TYPE=Debug -G $(BUILD_SYSTEM)

xconfigure:
	@cmake . -B build -D CMAKE_BUILD_TYPE=Debug -G $(XCODE_BUILD_SYSTEM)

bconfigure:
	@cmake . -B build -D CMAKE_BUILD_TYPE=Debug -G $(BACKUP_BUILD_SYSTEM)

wconfigure:
	@cmake . -B build -D CMAKE_PREFIX_PATH=/c/cmake -G $(WINDOWS_BUILD_SYSTEM)

build:
	@cmake --build build

install:
	@cmake --install build

clean:
	@ - rm -rf build

rebuild: clean configure build install scripting
brebuild: clean bconfigure build install
wrebuild: clean wconfigure build install

scripting:
	@cd ./goon/src/GoonScripting && csc /target:library /doc:../../../$(BUILD_FOLDER)/goon.xml /out:../../../$(BUILD_FOLDER)/goon.dll InternalCalls.cs

run:
	@cd ./$(BUILD_FOLDER) && ./$(BINARY_NAME)

lldb:
	@cd ./$(BUILD_FOLDER) && lldb ./$(BINARY_NAME)

debug: rebuild lldb

doc:
	@cd ./goon/doc/; ./update_doc.sh; cd -

test:
	@cd ./build && ctest --output-on-failure; cd -
vtest:
	@cd ./build && ctest --output-on-failure --verbose; cd -
