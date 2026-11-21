# tool macros
CC ?= gcc
COBJFLAGS := $(CFLAGS) -c

# path macros
OBJ_PATH := $(PWD)
SRC_PATH := $(PWD)
BIN_PATH := $(PWD)

# compile macros
TARGET_NAME := spellsuggest
TARGET := $(PWD)/$(TARGET_NAME)

# src files & obj files
SRC := $(foreach x, $(SRC_PATH), $(wildcard $(addprefix $(x)/*,.c*)))
OBJ := $(addprefix $(OBJ_PATH)/, $(addsuffix .o, $(notdir $(basename $(SRC)))))

# non-phony targets
$(TARGET): $(OBJ)
	$(CC) -o $@ $(OBJ) $(CFLAGS)

$(OBJ_PATH)/%.o: $(SRC_PATH)/%.c*
	$(CC) $(COBJFLAGS) -o $@ $<

.PHONY: all
all: $(TARGET)
	@rm $(OBJ)

.PHONY: clean
clean:
	@echo CLEAN $(CLEAN_LIST)
	@rm -f $(CLEAN_LIST)
	@rm -f $(OBJ)
