export

CC=g++

COMMON_CXXFLAGS := -Wl -Wall -O2 -fno-common -Wwrite-strings 
COMMON_LDFLAGS := 

ifdef DEBUG
COMMON_CXXFLAGS += -DDEBUG
endif
export COMMON_CXXFLAGS
export COMMON_LDFLAGS

CXXFLAGS := $(COMMON_CXXFLAGS) 
LDFLAGS := $(COMMON_LDFLAGS) 



SUBDIRS := protos common src

OBJS := protos/protos.o common/common.o src/server.o 

server: $(patsubst %, _dir_%, $(SUBDIRS))
	$(CC) -static -o $@ ${LDFLAGS} $(OBJS) 

$(patsubst %, _dir_%, $(SUBDIRS)):
	$(MAKE) -C $(patsubst _dir_%, %, $@) $(TGT)



all: app server

app:
	$(MAKE) -C apps

client: 
	$(CC) client.cpp proto/query.pb.cc $(COMMON_LDFLAGS) -lprotobuf -lz -o client


cleanall: clean cleanapp

cleanapp:
	$(MAKE) -C apps clean
clean:
	@$(MAKE) TGT=clean $(patsubst %, _dir_%, $(SUBDIRS))
	-rm -f server

