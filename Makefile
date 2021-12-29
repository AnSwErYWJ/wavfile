VPATH ?= .


#
# build target
#
MODULE 			:= wavtool
EXE_P2W 		:= pcm2wav
EXE_W2P 		:= wav2pcm
EXE_WI 			:= wavinfo
STATIC_LIBRARY  := lib$(MODULE).a
SHARE_LIBRARY   := lib$(MODULE).so
ALL 			:= $(EXE_P2W) $(EXE_W2P) $(EXE_WI) $(STATIC_LIBRARY) $(SHARE_LIBRARY)


#
# toolchains
#
CC  	:= gcc
LD  	:= ld
CPP 	:= cpp
CXX 	:= g++
AR  	:= ar
AS  	:= as
NM  	:= nm
STRIP 	:= strip
OBJCOPY := objcopy
RM 		:= -rm -rf


#
# files or dirs
#
SRC_FILES := src/wav.c
INCLUDE_DIRS := -Iinclude
P2W_FILES := $(SRC_FILES) apps/pcm2wav.c
W2P_FILES := $(SRC_FILES) apps/wav2pcm.c
WI_FILES  := $(SRC_FILES) apps/wavinfo.c


#
# flags
#
CFLAGS   := -Os -std=gnu99 -fvisibility=hidden -D_GNU_SOURCE -fPIC
CFLAGS   += -Wall -Wno-variadic-macros -Wno-format-zero-length -fstack-protector-all -ffunction-sections -fdata-sections
CFLAGS   +=  $(INCLUDE_DIRS)
#CFLAGS  += -fPIE

CXXFLAGS := $(subst -std=gnu99,,$(CFLAGS))

LDFLAGS  := -Wl,--gc-sections
#LDFLAGS  += -pie


#
# rules
#
%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o : %.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

%.o : %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@


#
# objects
#
OBJECTS = $(subst .c,.o,$(subst .cpp,.o,$(subst .cc,.o,$(SRC_FILES))))
P2W_OBJECTS = $(subst .c,.o,$(subst .cpp,.o,$(subst .cc,.o,$(P2W_FILES))))
W2P_OBJECTS = $(subst .c,.o,$(subst .cpp,.o,$(subst .cc,.o,$(W2P_FILES))))
WI_OBJECTS = $(subst .c,.o,$(subst .cpp,.o,$(subst .cc,.o,$(WI_FILES))))


#
# goal all
#
.PHONY: all
all: $(ALL)

$(EXE_P2W) : $(P2W_OBJECTS)
	$(CXX) $^ $(LDFLAGS) -o $@
	$(STRIP) --strip-unneeded $@

$(EXE_W2P) : $(W2P_OBJECTS)
	$(CXX) $^ $(LDFLAGS) -o $@
	$(STRIP) --strip-unneeded $@

$(EXE_WI) : $(WI_OBJECTS)
	$(CXX) $^ $(LDFLAGS) -o $@
	$(STRIP) --strip-unneeded $@

$(STATIC_LIBRARY) : $(OBJECTS)
	$(AR) crv $@ $^
	$(LD) -r $^ -o lib$(MODULE)_default.o
	$(STRIP) --strip-unneeded lib$(MODULE)_default.o
	$(OBJCOPY) --localize-hidden lib$(MODULE)_default.o lib$(MODULE).o
	$(AR) crv $@ lib$(MODULE).o
	$(RM) lib$(MODULE)_default.o lib$(MODULE).o

$(SHARE_LIBRARY) : $(OBJECTS)
	$(CXX) $^ -shared $(LDFLAGS) -o $@
	$(STRIP) --strip-unneeded $@

#
# goal: clean
#
.PHONY: clean 
clean:
	$(RM) $(OBJECTS) $(P2W_OBJECTS) $(W2P_OBJECTS) $(WI_OBJECTS) $(ALL)