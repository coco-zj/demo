
CXXFLAGS= $(COMMON_CFLAGS) -Wwrite-strings  -I. -I../apps/libevent-2.0.21-stable/include -I../apps/redis-2.8.13/deps/hiredis/ -I../apps/protobuf-2.5.0/src/
LDFLAGS= $(COMMON_LDFLAGS) -L../apps/redis-2.8.13/deps/hiredis/ -L../apps/libevent-2.0.21-stable/.libs/ -L../apps/protobuf-2.5.0/src/.libs/ -L/usr/lib/x86_64-linux-gnu/ 

LIBS= -lz -lhiredis -levent -lprotobuf

SOURCES=$(wildcard *.cpp)
OBJS = $(SOURCES:%.cpp=%.o)

server.o: $(OBJS) DbOperator.o
	$(LD) $(LDFLAGS) -r -o $@ $(OBJS) $(LIBS)
$(OBJS):
	$(CC) $(CXXFLAGS) -c $(patsubst %.o, %.cpp, $@)

ifneq ($(MAKECMDGOALS),clean)
-include $(SOURCES:%.cpp=%.d)
%.d:$.cpp
	$(CC) -MM $(CXXFLAGS) $< > %@
	@sed -i 's/^\(.*\)\.o[: ]*/\1\.o $@: /' $@
endif

clean:
	rm -rf $(OBJS) server.o 
	rm -f *.d

