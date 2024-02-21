
COMMON_PATH := src

SRC_FILES := main.c utility.c db_utility.c statements.c shells/magazzino.c
HEADER_FILES := $(SRC_FILES:.c=.h) common.h
OBJ_FILES := $(SRC_FILES:.c=.o)

SRCS := $(addprefix $(COMMON_PATH)/,$(SRC_FILES))
HEADERS := $(addprefix $(COMMON_PATH)/,$(HEADER_FILES))
OBJS := $(addprefix $(COMMON_PATH)/,$(OBJ_FILES))



LINKER_OPT := -lcrypt -pie $$(mariadb_config --include --libs)
COMPILER_OPT :=



TARGETS := client



all: $(TARGETS) light-clean

client: $(OBJS)
	$(CC) $(OBJS) -o $@ $(LINKER_OPT)

$(OBJS): $(SRCS) $(HEADERS)
	$(CC) $(@:.o=.c) -c -o $@ $(COMPILER_OPT)

clean:
	rm -f $(TARGETS) $(OBJS)

light-clean:
	rm -f $(OBJS)
