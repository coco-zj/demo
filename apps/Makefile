#
# Makefile for integrating 3rd party softwares' building process
# 
# Note: Before software added to APP_LIST, make sure it's been
# 		configured or Makefile is available
#

APP_LIST=libevent-2.0.21-stable redis-2.8.13 # to be added


# make each dir as a target
APP_DIRS=$(patsubst %, _dir_%, $(APP_LIST))

all: $(APP_DIRS)
$(APP_DIRS):
	@if [ -f $(patsubst _dir_%, %, $@)/Makefile ] || [ -f $(patsubst _dir_%, %, $@)/makefile ];then \
		make -C $(patsubst _dir_%, %, $@) $(TGT);\
	else \
		echo "'$(patsubst _dir_%, %, $@)/Makefile' is missing!";\
	 	echo "You should configure '$(patsubst _dir_%,%, $@)' first!";false;\
	fi

.PHONY: clean distclean
clean:
	make TGT=clean $(APP_DIRS)
distclean:
	make TGT=distclean $(APP_DIRS)
