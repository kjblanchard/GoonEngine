.PHONY: build

FILES = hello.cs
BUILD_FOLDER = /Users/kevin/git/engine/goon/build/bin
PROJECT_NAME = hello
GOON_DLL_NAME = goon.dll

all: build


build:
	@csc /target:library /reference:$(BUILD_FOLDER)/$(GOON_DLL_NAME) /out:$(BUILD_FOLDER)/$(PROJECT_NAME).dll $(FILES)