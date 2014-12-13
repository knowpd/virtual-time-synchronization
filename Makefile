#SUBDIRS = virtualagent/net
#SUBDIRS = virtualagent/shared
SUBDIRS = virtualagent

.PHONY: subdirs $(SUBDIRS)

subdirs: $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) -C $@

all: subdirs

clean:
	for n in $(SUBDIRS);\
	do $(MAKE) -C $$n clean;\
	done