export BASE := $(PWD)
export CC := gcc
export INC := $(BASE)/include/
COMMON := $(wildcard $(BASE)/common/*.c)
export COMMON := $(patsubst %.c,%.o,$(COMMON))
export override CPPFLAGS += -O2 -Wall -Werror -I$(INC)

SUBDIRS := $(wildcard */.)
SUBDIRS += $(wildcard application/*/.)
SUBDIRS := $(filter-out application/., $(SUBDIRS))
SUBDIRS := $(filter-out include/., $(SUBDIRS))

all: $(SUBDIRS)

clean: 
	for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir clean; \
	done

print:
	$(info SUBDIRS="$(SUBDIRS)")
	$(info COMMON="$(COMMON)")

$(SUBDIRS):
	$(MAKE) -C $@

.PHONY: all $(SUBDIRS)
