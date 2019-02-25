SOURCES := src
HEADERS := inc
BUILD := build
OBJECTS := $(BUILD)/obj
ifndef PROJECT
PROJECT := sdl.exe

IMGUI_LIB := $(BUILD)/libimgui.a

IMGUI_CPP := $(wildcard libraries/imgui/*.cpp)
IMGUI_OBJ := $(patsubst libraries/imgui/%.cpp, $(OBJECTS)/%.o,$(IMGUI_CPP))
endif
OUTPUT := $(BUILD)/$(PROJECT)

CXX := g++
ifndef PLATFORM
ifeq ($(OS),Windows_NT)
PLATFORM :=__WIN32
else
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
PLATFORM :=__LINUX
endif
ifeq ($(UNAME_S),Darwin)
PLATFORM :=__OSX
endif
endif
CXXFLAGS := -g -I$(HEADERS) -Ilibraries/imgui -Wall -Wextra -Wno-unused-parameter -mwindows -D$(PLATFORM)
ifndef RELEASE
CXXFLAGS += -D__DEBUG
endif
endif
LINKFLAGS :=-LC:/MinGW/lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -static-libstdc++ -Lbuild -limgui
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
HPPFILES := $(filter-out $(HEADERS)/$(STUB).hpp, $(wildcard $(HEADERS)/*.hpp))
OBJFILES := $(patsubst $(SOURCES)/%.cpp, $(OBJECTS)/%.o,$(CPPFILES))

###############################################################

.PHONY: all
all: project docs

.PHONY: project
project: $(OUTPUT)

.PHONY: setup
setup: $(SOURCES) $(HEADERS) $(BUILD)

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
$(OUTPUT): $(OBJECTS) $(OBJFILES) $(IMGUI_LIB)
ifeq ($(suffix $@), "a")
	$(AR) rvs $(OUTPUT) $(OBJFILES) $(ARFLAGS)
else
	$(CXX) $(OBJFILES) $(LINKFLAGS) -o $(OUTPUT)
	cp C:/MinGW/bin/zlib1.dll $(BUILD)/
	cp C:/MinGW/bin/SDL2.dll $(BUILD)/
	cp C:/MinGW/bin/SDL2_Image.dll $(BUILD)/
endif

$(OBJECTS)/%.o: $(SOURCES)/%.cpp
	$(CXX) -c $(CXXFLAGS) $< -o $@

$(OBJECTS): $(BUILD)
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
$(IMGUI_LIB): $(IMGUI_OBJ)
	$(AR) rvs $(IMGUI_LIB) $(IMGUI_OBJ) $(ARFLAGS)

$(OBJECTS)/%.o: libraries/imgui/%.cpp
	$(CXX) -c $(CXXFLAGS) $< -o $@

###############################################################

.PHONY: clean
clean:
	rm -rf $(BUILD)

.PHONY: docs
docs: doxygen

.PHONY: doxygen
doxygen:
	@doxygen Doxyfile

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
	@cloc $(SOURCES) $(HEADERS) Makefile --quiet
