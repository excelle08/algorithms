BASE := $(PWD)
export INC := $(BASE)/include/

export COMMON := $(wildcard $(BASE)/common/*.c)

export override CPPFLAGS += -O2 -Wall -Werror -I$(INC)

SUBDIRS := $(wildcard */.)
SUBDIRS := $(filter-out include/., $(SUBDIRS))
SUBDIRS := $(filter-out common/., $(SUBDIRS))

all: $(SUBDIRS)

print:
	for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir print; \
	done

$(SUBDIRS):
	$(MAKE) -C $@

.PHONY: all $(SUBDIRS)
