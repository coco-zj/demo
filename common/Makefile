CXXFLAGS= $(COMMON_CFLAGS) -I .

LDFLAGS= $(COMMON_LDFLAGS)

SOURCES = $(wildcard *.cpp)
OBJS = $(SOURCES:.cpp=.o)

common.o: $(OBJS)
	$(LD) $(LDFLAGS) -r -o $@ $(OBJS)

$(OBJS):
	$(cpp) $(CXXFLAGS) -c $(patsubst %.o, %.cpp, $@)

ifneq ($(MAKECMDGOALS),clean)
-include $(SOURCES:%.cpp=%.d)
%.d: %.cpp
	$(cpp) -MM $(CXXFLAGS) $< > $@
	@sed -i 's/^\(.*\)\.o[: ]*/\1\.o $@: /' $@
endif

clean:
	rm -f $(OBJS) common.o
	rm -f *.d


