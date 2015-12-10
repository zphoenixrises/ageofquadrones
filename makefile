program_NAME := AgeofQuadrones
program_C_SRCS := $(wildcard *.c)
program_CXX_SRCS := $(wildcard *.cpp)
program_CXX_SRCS += raygl/raygl.cpp
program_C_OBJS := ${program_C_SRCS:.c=.o}
program_CXX_OBJS := ${program_CXX_SRCS:.cpp=.o}
#program_CXX_OBJS += raygl/raygl.o
program_OBJS := $(program_C_OBJS) $(program_CXX_OBJS)
program_INCLUDE_DIRS :=/usr/include/GL/ 
program_INCLUDE_DIRS +=glm/
program_INCLUDE_DIRS +=raygl/
program_LIBRARY_DIRS :=/usr/lib64/
program_LIBRARIES := GL GLU glut m

CPPFLAGS += $(foreach includedir,$(program_INCLUDE_DIRS),-I$(includedir))
LDFLAGS += $(foreach librarydir,$(program_LIBRARY_DIRS),-L$(librarydir))
LDFLAGS += $(foreach library,$(program_LIBRARIES),-l$(library))

.PHONY: all clean distclean

all: $(program_NAME)

$(program_NAME): $(program_OBJS)
	$(CXX)  -o $(program_NAME) $(program_OBJS) $(LDFLAGS) -pthread

clean:
	@- $(RM) $(program_NAME)
	@- $(RM) $(program_OBJS)

distclean: clean
