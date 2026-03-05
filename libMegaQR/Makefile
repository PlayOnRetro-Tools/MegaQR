.ONESHELL:

PREFIX  	:= m68k-elf

BIN 		:= $(GDK)/bin
INCLUDE_LIB := $(GDK)/inc
RES_LIB 	:= $(GDK)/res

RM 			:= rm
CP 			:= cp
MKDIR 		:= mkdir
ECHO 		:= echo
FIND 		:= find

AR 			:= $(PREFIX)-gcc-ar
CC 			:= $(PREFIX)-gcc
LD			:= $(PREFIX)-ld
OBJCPY 		:= $(PREFIX)-objcopy
BINTOS 		:= $(PREFIX)-bintos
RESCOMP 	:= java -jar $(GDK)/bin/rescomp.jar

SRC 		:= src
INCLUDE 	:= inc
RES 		:= res

RES_RES 	:= $(wildcard $(RES)/*.res)
SOURCES 	:= $(shell $(FIND) $(SRC) -type f)
SRC_C 		:= $(filter %.c,$(SOURCES))
OBJS 		:= $(addprefix build/out/, $(RES_RES:.res=.o))
OBJS		+= $(addprefix build/out/, $(SRC_C:.c=.o))
DEPS 		:= $(addprefix build/out/, $(SRC_C:.c=.d))

INCS		:=  -I$(INCLUDE) -I$(SRC) -I$(INCLUDE_LIB) -I$(RES_LIB) -I$(RES)
FLAGS 		:=  -m68000 -Wall -Wextra -Wno-shift-negative-value \
				-Wno-main -Wno-unused-parameter -fno-builtin -fms-extensions $(INCS) \
				-fdiagnostics-color=always -ffunction-sections -fdata-sections \
				-finline-small-functions -O3 -fuse-linker-plugin \
				-fno-web -fno-gcse -fno-unit-at-a-time -fomit-frame-pointer -flto -g
AFLAGS  	:= -x assembler-with-cpp -Wa,--register-prefix-optional,--bitwise-or $(FLAGS)

.PHONY: all clean cleanall pre-build test

all: pre-build build/lib/libmegaqr.a

clean:
	@$(RM) -rf build

cleanall: clean

pre-build:
	@$(MKDIR) -p build/lib build/out build/dep build/out/res

build/lib/libmegaqr.a: $(OBJS)
	@$(ECHO) "  LD $(notdir $@)"
	@$(AR) rcs $@ $^

build/out/%.o: %.c
	@$(ECHO) "  CC  $(notdir $<)"
	@$(MKDIR) -p $(dir $@)
	@$(CC) $(FLAGS) -MMD -c $< -o $@

build/out/%.o: %.rs
	@$(CC) -x assembler-with-cpp -Wa,--register-prefix-optional,--bitwise-or \
	 $(FLAGS) -c $*.rs -o $@
	@$(CP) $*.d build/out/$*.d
	@$(RM) $*.d

%.rs: %.res
	@$(RESCOMP) $*.res $*.rs -dep build/out/$*.o

-include $(DEPS)
