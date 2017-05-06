
ifndef CC
	CC = gcc
endif

CCFLAGS=-O3 -lm

LIBS = -lOpenCL -fopenmp

COMMON_DIR = .

SRCS = nbody.c files.c init.c display.c

# Change this variable to specify the device type
# to the OpenCL device type of choice. You can also
# edit the variable in the source.
ifndef DEVICE
	DEVICE = CL_DEVICE_TYPE_DEFAULT
endif

# Check our platform and make sure we define the APPLE variable
# and set up the right compiler flags and libraries
PLATFORM = $(shell uname -s)
ifeq ($(PLATFORM), Darwin)
	LIBS = -framework OpenCL libft.a minilibx_macos/libmlx.a -framework OpenGL -framework AppKit
endif

all: nbody

CCFLAGS += -D DEVICE=$(DEVICE)

minilibx_macos/libmlx.a:
	make -C minilibx_macos

nbody: $(SRCS) $(COMMON_DIR)/wtime.c $(COMMON_DIR)/device_info.c minilibx_macos/libmlx.a
	$(CC) $^ $(CCFLAGS) $(LIBS) -I $(COMMON_DIR) -o $@


clean:
	rm -f nbody
	make -C minilibx_macos clean
