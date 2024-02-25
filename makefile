
SRCS := $(shell find ./src -name '*.c')
HEADERS := $(shell find ./src -name '*.h')
OBJS := $(SRCS:.c=.o)


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
