
################################################
# 	COMPILER AND COMPILE FLAG SETTINGS
################################################
CXX	= g++
ifeq ($(OS),Windows_NT)
    $(error Use of make not supported in Windows!)
endif
UNAME := $(shell uname -s)
ifeq ($(UNAME),Darwin)
    $(info "Compiling for Apple / MacOS ...")
    INC_FLAGS = -I/usr/X11/include -I. -I./include/
    CXXFLAGS = -O0 -g -std=c++11 -DGL_GLEXT_PROTOTYPES \
                -DGL_DO_NOT_WARN_IF_MULTI_GL_VERSION_HEADERS_INCLUDED -DOSX \
                -Wno-deprecated-register -Wno-deprecated-declarations \
                -Wno-shift-op-parentheses -Wno-parentheses-equality $(INC_FLAGS)
    LDFLAGS = -framework GLUT -framework OpenGL \
               -L"/System/Library/Frameworks/OpenGL.framework/Libraries" \
               -lGL -lGLU -lm -lstdc++
else ifeq ($(UNAME),Linux)
    $(info "Compiling for Linux ...")
    INC_FLAGS = -I. -I./include
    CXXFLAGS = -O0 -g3 -std=c++11 -Wall -Wno-deprecated $(INC_FLAGS)
    LDFLAGS = -L/usr/X11R6/lib -lGLEW -lglut -lGLU -lGL -lX11
else
    $(info "Compiling for unknown OS assuming Linux-like  ...")
    INC_FLAGS = -I. -I./include
    CXXFLAGS = -O0 -g3 -std=c++11 -Wall -Wno-deprecated $(INC_FLAGS)
    LDFLAGS = -L/usr/X11R6/lib -lGLEW -lglut -lGLU -lGL -lX11
endif
		
################################################
# 	SOURCE and OBJECT FILES
################################################
# Add inputs and outputs from these tool invocations to the build variables 
CPPS += \
src/exercise3.cpp \
src/cgCamera.cpp \
src/cgMesh.cpp \
src/cgShader.cpp \
src/scenes.cpp \
src/utils.cpp \
src/validation.cpp

# All .o files go to obj dir.
OBJS = $(CPPS:src/%.cpp=obj/%.o)

# General rule to generate .o files from .cpp files
# The -MMD flags additionaly creates a .d file with
# the same name as the .o file.
obj/%.o : src/%.cpp
	@echo 'Building file: $<'
	$(CXX) $(CXXFLAGS) -MMD -c $< -o $@
	@echo 'Finished building: $<'
	@echo ' '

################################################
# 	MAKE TARGETS
################################################

# Add inputs and outputs from these tool invocations to the build variables 

# All Target 
all: exercise3

# Tool invocations
exercise3: $(OBJS)
	@echo 'Building target: $@'
	$(CXX) -o "./$@" $(OBJS) $(LDFLAGS)
	@echo 'Finished building target: $@'
	@echo ' '

# Gcc/Clang will create these .d files containing dependencies.
CPP_DEPS = $(OBJS:%.o=%.d)
# Include all .d files
-include $(CPP_DEPS)

	
# Other Targets
# This should remove all generated files.
.PHONY : clean
clean :
	-rm ./exercise3 $(OBJS) $(CPP_DEPS)
	@echo ' '
