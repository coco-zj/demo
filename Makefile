CC=g++

COMMON_CXXFLAGS := -Wall#-Wl -Wall -O2 -fno-common -Wwrite-strings 
COMMON_LDFLAGS := 

ifdef DEBUG
COMMON_CXXFLAGS += -DDEBUG
endif
export COMMON_CXXFLAGS
export COMMON_LDFLAGS

CXXFLAGS= $(COMMON_CFLAGS) -D_GNU_SOURCE -Wwrite-strings  -I. -I../apps/libevent-2.0.21-stable/include -I../apps/redis-2.8.13/deps/hiredis/ -I../apps/protobuf-2.5.0/src/
LDFLAGS= $(COMMON_LDFLAGS) -L/usr/lib/x86_64-linux-gnu/ -L../apps/redis-2.8.13/deps/hiredis/ -L../apps/libevent-2.0.21-stable/.libs/ -L../apps/protobuf-2.5.0/src/.libs/ 

LIBS := -lz -lpthread -lhiredis -levent -lprotobuf 

SUBDIRS := protos common src

SRCOBJS := $(patsubst ./src/%.cpp,./src/%.o,$(wildcard ./src/*.cpp))

OBJS := protos/protos.o common/common.o $(SRCOBJS) #src/server.o#$(SRCOBJS)

server: $(patsubst %, _dir_%, $(SUBDIRS))
	$(CC) -o $@ ${LDFLAGS} $(OBJS) $(LIBS)

$(patsubst %, _dir_%, $(SUBDIRS)):
	$(MAKE) -C $(patsubst _dir_%, %, $@) $(TGT)



all: server

app:
	$(MAKE) -C apps

client: 
	$(CC) client.cpp protos/query.pb.cc $(COMMON_LDFLAGS) -lprotobuf -lz -o client


cleanall: clean cleanapp

cleanapp:
	$(MAKE) -C apps clean
clean:
	@$(MAKE) TGT=clean $(patsubst %, _dir_%, $(SUBDIRS))
	-rm -f server

