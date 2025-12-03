# Lot of useful features learnt and implemented from here https://makefiletutorial.com/#implicit-rules
# some code snippets for the OBJS and SRCS are from the bottom of the webpage

TARGET_EXEC := demo

LIBS := -lm

# Directories
BUILD_DIR := ./build
SRC_DIR :=  ./src
INC_DIRS := $(shell find $(SRC_DIR) -type d)

# SRCS
SRCS_MATRIX := $(shell find $(SRC_DIR)/MatrixMath -name '*.c')
SRCS_RNN := $(shell find $(SRC_DIR)/RNN -name '*.c')
SRCS_TEST := $(shell find $(SRC_DIR)/Test -name '*.c')
SRCS_DEMO := $(shell find $(SRC_DIR)/Demo -name '*.c')

# objs
OBJS_MATRIX := $(SRCS_MATRIX:%=$(BUILD_DIR)/%.o)
OBJS_RNN := $(SRCS_RNN:%=$(BUILD_DIR)/%.o)
OBJS_TEST := $(SRCS_TEST:%=$(BUILD_DIR)/%.o)
OBJS_DEMO := $(SRCS_DEMO:%=$(BUILD_DIR)/%.o)

# include
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

# flags
CPPFLAGS := -Wall $(INC_FLAGS)

# BUILDS
all: demo test

demo: $(OBJS_MATRIX) $(OBJS_RNN) $(OBJS_DEMO)
	$(CC) $(OBJS_MATRIX) $(OBJS_RNN) $(OBJS_DEMO) $(LIBS) -o $(BUILD_DIR)/$@


test: $(OBJS_MATRIX) $(OBJS_TEST)
	$(CC) $(OBJS_MATRIX) $(OBJS_TEST) $(LIBS) -o $(BUILD_DIR)/$@


$(BUILD_DIR)/%.c.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@


.PHONY: clean
clean:
	-rm -r $(BUILD_DIR)

