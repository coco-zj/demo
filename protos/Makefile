CXXFLAGS= $(COMMON_CFLAGS) -I .

LDFLAGS= $(COMMON_LDFLAGS)

SOURCES = $(wildcard *.cc)
OBJS = $(SOURCES:.cc=.o)

protos.o: $(OBJS)
	$(LD) $(LDFLAGS) -r -o $@ $(OBJS)

$(OBJS):
	$(CC) $(CXXFLAGS) -c $(patsubst %.o, %.cc, $@)

ifneq ($(MAKECMDGOALS),clean)
-include $(SOURCES:%.cc=%.d)
%.d: %.cc
	$(CC) -MM $(CXXFLAGS) $< > $@
	@sed -i 's/^\(.*\)\.o[: ]*/\1\.o $@: /' $@
endif

clean:
	rm -f $(OBJS) protos.o
	rm -f *.d


