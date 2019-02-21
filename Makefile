SOURCES := src
HEADERS := inc
BUILD := build
OBJECTS := $(BUILD)/obj
PROJECT := sdl.exe
OUTPUT := $(BUILD)/$(PROJECT)

CXX := g++
CXXFLAGS := -g -I$(HEADERS) -mwindows -D__WIN32
ifndef RELEASE
CXXFLAGS += -D__DEBUG
endif
LINKFLAGS :=-LC:/MinGW/lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -static-libstdc++
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

all: project docs

project: $(OUTPUT)

setup: $(SOURCES) $(HEADERS) $(BUILD)

newfile: newcpp newhpp

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

$(OUTPUT): $(OBJECTS) $(OBJFILES)
ifeq ($(suffix $@), "a")
	$(AR) rvs $(OUTPUT) $(OBJFILES) $(ARFLAGS)
else
	$(CXX) $(OBJFILES) $(LINKFLAGS) -o $(OUTPUT)
endif

$(OBJECTS)/%.o: $(SOURCES)/%.cpp
	$(CXX) -c $(CXXFLAGS) $< -o $@

$(OBJECTS): $(BUILD)
	@mkdir -p $(OBJECTS)

$(SOURCES):
	@mkdir -p $(SOURCES)

$(HEADERS):
	@mkdir -p $(HEADERS)

$(BUILD):
	@mkdir -p $(BUILD)

clean:
	rm -rf $(BUILD)

docs: doxygen

doxygen:
	@doxygen Doxyfile

test: $(OUTPUT)
	$(OUTPUT)

test-mem: $(OUTPUT)
	drmemory $(OUTPUT)

test-dbg: $(OUTPUT)
	gdb $(OUTPUT)

count:
	@cloc $(SOURCES) $(HEADERS) Makefile --quiet
