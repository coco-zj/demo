export

CC=g++

COMMON_CXXFLAGS := -Wall -O2 -fno-common -Wwrite-strings 
COMMON_LDFLAGS := -L/usr/lib/x86_64-linux-gnu/ #-L./apps/redis-2.8.13/deps/hiredis/ -L./apps/libevent-2.0.21-stable/.libs/

ifdef DEBUG
COMMON_CXXFLAGS += -DDEBUG
endif
export COMMON_CXXFLAGS
export COMMON_LDFLAGS


CXXFLAGS := $(COMMON_CXXFLAGS) #-I./apps/redis-2.8.13/deps/hiredis/ -I./apps/libevent-2.0.21-stable/include/
LDFLAGS := $(COMMON_LDFLAGS) #-lhiredis -levent -lprotobuf

LIBS := -lz

SUBDIRS := common src

OBJS := common/common.o src/server.o ./apps/libevent-2.0.21-stable/.libs/libevent.so ./apps/redis-2.8.13/deps/hiredis/libhiredis.a \
		./apps/protobuf-2.5.0/src/.libs/libprotobuf.so

server: $(patsubst %, _dir_%, $(SUBDIRS))
	$(CC) -o $@ ${LDFLAGS} $(OBJS)

$(patsubst %, _dir_%, $(SUBDIRS)):
	$(MAKE) -C $(patsubst _dir_%, %, $@) $(TGT)


#IFSERVER_OBJS: src/InterfaceServer.cpp
#	$(CC) -c $@ ${CXXFLAGS} ${LDFLAGS} src/InterfaceServer.cpp

#server: src/InterfaceServer.o src/main.o
#	$(CC) -o $@ src/main.o src/InterfaceServer.o ${CXXFLAGS} ${LDFLAGS} src/main.o src/InterfaceServer.o


all: app server

app:
	$(MAKE) -C apps

client: 
	$(CC) client.cpp common/query.pb.cc $(COMMON_LDFLAGS) -lprotobuf -lz -o client


cleanall: clean cleanapp

cleanapp:
	$(MAKE) -C apps clean
clean:
	@$(MAKE) TGT=clean $(patsubst %, _dir_%, $(SUBDIRS))
	-rm -f src/*.o
	-rm -f server

