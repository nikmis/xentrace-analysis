CC=clang -Wall -m32
INC=-I./include 

SUBDIRS = examples/blk_io_queues examples/cpu_time_sharing examples/cpu_wait examples/xen_dom_time_sharing examples/xen_stats

vpath %.h include/
vpath %.c src/

all: xa subdirs
	
xa:	Xentrace-analysis.c
	$(CC) $(INC) $< -o bin/xa

.PHONY:	subdirs $(SUBDIRS)

subdirs: $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) -C $@
	
clean: 
	rm -rf bin/xa
