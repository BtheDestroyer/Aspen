SOURCES := ./src
HEADERS := ./inc
BUILD := ./build
OBJECTS := $(BUILD)/obj
ifndef PROJECT
PROJECT := aspen
endif
LIBRARY := Aspen

IMGUI_LIB := $(BUILD)/libimgui.a
IMGUI_CPP := $(wildcard libraries/imgui/*.cpp)
IMGUI_OBJ := $(patsubst libraries/imgui/%.cpp, $(OBJECTS)/%.o,$(IMGUI_CPP))
IMGUI_SDL_CPP := $(filter-out libraries/imgui_sdl/example.cpp,$(wildcard libraries/imgui_sdl/*.cpp))
IMGUI_SDL_OBJ := $(patsubst libraries/imgui_sdl/%.cpp, $(OBJECTS)/%.o,$(IMGUI_SDL_CPP))

ifndef PLATFORM
ifeq ($(OS),Windows_NT)
PLATFORM :=__WIN32
PROJECT := $(PROJECT).exe
else
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
PLATFORM :=__LINUX
endif
ifeq ($(UNAME_S),Darwin)
PLATFORM :=__OSX
endif
endif
endif
OUTPUT := $(BUILD)/$(PROJECT)

CXX := g++.exe
CXXFLAGS := -g -I$(HEADERS) \
						-Ilibraries/imgui \
						-Ilibraries/imgui_sdl \
						-Wall -Wextra -Wno-unused-parameter \
						-std=c++14 \
						-D$(PLATFORM)
LINKFLAGS :=-LC:\mingw32\mingw32\i686-w64-mingw32\lib -Lbuild\
						-lmingw32 \
						-limgui \
						-lSDL2main -lSDL2 \
						-lSDL2_image -lSDL2_ttf -lSDL2_mixer \
						-static-libstdc++
ifdef RELEASE
CXXFLAGS += -O2
else
CXXFLAGS += -D__DEBUG
endif
ifeq ($(OS),Windows_NT)
CXXFLAGS += -mwindows -Dmain=SDL_main
LINKFLAGS += -mwindows
ifndef RELEASE
CXXFLAGS += -mconsole
LINKFLAGS += -mconsole
endif
endif

AR := ar
ARFLAGS :=

STUB := Stub
STUB_CAP := $(shell tr '[:lower:]' '[:upper:]' <<< $(STUB))
STUB_LOW := $(shell tr '[:upper:]' '[:lower:]' <<< $(STUB))
ifdef NEWFILE
NEWFILE_CAP := $(shell tr '[:lower:]' '[:upper:]' <<< $(NEWFILE))
NEWFILE_LOW := $(shell tr '[:upper:]' '[:lower:]' <<< $(NEWFILE))
endif

CPPFILES := $(filter-out $(SOURCES)/$(STUB).cpp, $(wildcard $(SOURCES)/*.cpp))
ifeq ($(suffix $(PROJECT)),.a)
CPPFILES := $(filter-out $(SOURCES)/main.cpp, $(CPPFILES))
endif
HPPFILES := $(filter-out $(HEADERS)/$(STUB).hpp, $(wildcard $(HEADERS)/*.hpp))
RCFILES := $(wildcard $(SOURCES)/*.rc)
OBJFILES := $(filter-out $(OBJECTS)/main.o,$(patsubst $(SOURCES)/%.cpp, $(OBJECTS)/%.o,$(CPPFILES)))
ifeq ($(suffix $(PROJECT)),.exe)
#OBJFILES := $(OBJFILES) $(patsubst $(SOURCES)/%.rc, $(OBJECTS)/%.o,$(RCFILES))
endif

###############################################################

.PHONY: all
all: project docs

.PHONY: project
project: $(OUTPUT)
	@echo Built: $(OUTPUT)

.PHONY: library
library:
	$(MAKE) project PROJECT=lib$(LIBRARY).a

.PHONY: setup
setup: $(SOURCES) $(HEADERS) $(OBJECTS)

.PHONY: newfile
newfile: newcpp newhpp

.PHONY: newcpp
newcpp: $(SOURCES) $(SOURCES)/$(STUB).cpp
ifdef NEWFILE
ifeq (,$(wildcard $(SOURCES)/$(NEWFILE).cpp))
	@echo "Making new cpp: $(NEWFILE).cpp"
	@cat $(SOURCES)/$(STUB).cpp |\
		sed "s/$(STUB)/$(NEWFILE)/g" |\
		sed "s/$(STUB_CAP)/$(NEWFILE_CAP)/g" |\
		sed "s/$(STUB_LOW)/$(NEWFILE_LOW)/g" \
		> $(SOURCES)/$(NEWFILE).cpp
else
	@echo "$(NEWFILE).cpp already exists"
endif
else
	@echo "Can't make new cpp! NEWFILE is not defined"
	@echo "Call 'make' with the argument 'NEWFILE=filename'"
endif

$(SOURCES)/$(STUB).cpp:
	@touch $@

.PHONY: newhpp
newhpp: $(HEADERS) $(HEADERS)/$(STUB).hpp
ifdef NEWFILE
ifeq (,$(wildcard $(HEADERS)/$(NEWFILE).hpp))
	@echo "Making new hpp: $(NEWFILE).hpp"
	@cat $(HEADERS)/$(STUB).hpp |\
		sed "s/$(STUB)/$(NEWFILE)/g" |\
		sed "s/$(STUB_CAP)/$(NEWFILE_CAP)/g" |\
		sed "s/$(STUB_LOW)/$(NEWFILE_LOW)/g" \
		> $(HEADERS)/$(NEWFILE).hpp
else
	@echo "$(NEWFILE).hpp already exists"
endif
else
	@echo "Can't make new hpp! NEWFILE is not defined"
	@echo "Call 'make' with the argument 'NEWFILE=filename'"
endif

$(HEADERS)/$(STUB).hpp:
	@touch $@

.NOTPARALLEL: $(OUTPUT)
ifeq ($(suffix $(PROJECT)),.a)
$(OUTPUT): $(OBJFILES)
	$(AR) rvs $@ $^ $(ARFLAGS)
else
$(BUILD)/lib$(LIBRARY).a: $(OBJFILES)
	$(AR) rvs $@ $^ $(ARFLAGS)

$(OUTPUT): $(BUILD)/lib$(LIBRARY).a $(OBJECTS)/main.o $(IMGUI_LIB)
	$(CXX) $(OBJECTS)/main.o -l$(LIBRARY) $(LINKFLAGS) -o $(OUTPUT)
	#cp C:/MinGW/bin/libpng16-16.dll $(BUILD)/
	#cp C:/MinGW/bin/zlib1.dll $(BUILD)/
	#cp C:/MinGW/bin/SDL2.dll $(BUILD)/
	#cp C:/MinGW/bin/SDL2_Image.dll $(BUILD)/
	#cp C:/MinGW/bin/SDL2_ttf.dll $(BUILD)/
	#cp C:/MinGW/bin/SDL2_mixer.dll $(BUILD)/
endif

$(OBJECTS)/%.o: $(SOURCES)/%.cpp
	$(CXX) -c $(CXXFLAGS) $< -o $@

$(OBJECTS)/%.o: $(SOURCES)/%.rc
ifeq ($(PLATFORM),__WIN32)
#	windres $< -o $@
endif

$(OBJFILES) : | $(OBJECTS)

$(OBJECTS): | $(BUILD)
	@mkdir -p $@

$(SOURCES):
	@mkdir -p $@

$(HEADERS):
	@mkdir -p $@

$(BUILD):
	@mkdir -p $@

###############################################################

.PHONY: imgui
imgui: $(IMGUI_LIB)

.NOTPARALLEL: $(IMGUI_LIB)
$(IMGUI_LIB): $(IMGUI_OBJ) $(IMGUI_SDL_OBJ)
	$(AR) rvs $(IMGUI_LIB) $(IMGUI_OBJ) $(IMGUI_SDL_OBJ) $(ARFLAGS)

$(OBJECTS)/%.o: libraries/imgui/%.cpp
	$(CXX) -c $(CXXFLAGS) $< -o $@

$(OBJECTS)/%.o: libraries/imgui_sdl/%.cpp
	$(CXX) -c $(CXXFLAGS) -IC:/MinGW/include/SDL2 $< -o $@

###############################################################

.PHONY: clean
clean:
	rm -rf $(BUILD)

.PHONY: docs
docs: doxygen

.PHONY: doxygen
doxygen:
	doxygen Doxyfile

.PHONY: run
run: $(OUTPUT)
	$(OUTPUT)

.PHONY: test-mem
test-mem: $(OUTPUT)
	drmemory $(OUTPUT)

.PHONY: debug
debug: $(OUTPUT)
	gdb $(OUTPUT)

.PHONY: count
count:
	cloc $(SOURCES) $(HEADERS) Makefile --quiet
