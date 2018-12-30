CC_C		:= gcc
CC_CPP		:= g++
CC_FLAGS	:= -pedantic -Wall -Wextra -Wconversion
CC_LIBS		:= 

FLAGS_DBG	:= -g
FLAGS_RLS	:= -DNDEBUG

DIR_INC		:= inc
DIR_SRC		:= src
DIR_OBJ		:= obj
DIR_BIN		:= bin

BIN			:= alarm.exe

CMD_CP    := cp -f
CMD_RM    := rm -f
CMD_LN    := ln -sf
CMD_PRINT := @printf

SRCS_C		:= $(wildcard $(DIR_SRC)/*.c)
OBJS_C		:= $(patsubst $(DIR_SRC)/%.c,$(DIR_OBJ)/%.o,$(SRCS_C))

OBJS_C		+= $(DIR_OBJ)/crc.o $(DIR_OBJ)/packet.o

SCRS_CPP	:= $(wildcard $(DIR_SRC)/*.cpp)
OBJS_CPP	:= $(patsubst $(DIR_SRC)/%.cpp,$(DIR_OBJ)/%.o,$(SCRS_CPP))

OBJS		:= $(OBJS_C) $(OBJS_CPP)

.PHONY: all
all: debug

.PHONY: release
release: CC_FLAGS += $(FLAGS_RLS)
release: $(DIR_BIN)/$(BIN)

.PHONY: debug
debug: CC_FLAGS += $(FLAGS_DBG)
debug: $(DIR_BIN)/$(BIN)


.PHONY: clean
clean:
	$(CMD_RM) $(OBJS)

.PHONY: distclean
distclean: clean
	$(CMD_RM) $(DIR_BIN)/$(BIN)

.PHONY: run
run: $(DIR_BIN)/$(BIN)
	./$+

$(DIR_BIN)/$(BIN): $(OBJS)
	$(CC_CPP) $(OBJS) $(CC_LIBS) -o $(DIR_BIN)/$(BIN)

$(DIR_OBJ)/Main.o: $(DIR_SRC)/Main.cpp
	$(CC_CPP) $(CC_FLAGS) -I$(DIR_INC) -D__USE_MINGW_ANSI_STDIO=1 -c $< -o $@
  
$(DIR_OBJ)/Misc.o: $(DIR_SRC)/Misc.cpp $(DIR_INC)/Misc.h
	$(CC_CPP) $(CC_FLAGS) -I$(DIR_INC) -c $< -o $@

$(DIR_OBJ)/SerialPort.o: $(DIR_SRC)/SerialPort.cpp
	$(CC_CPP) $(CC_FLAGS) -I$(DIR_INC) -D__USE_MINGW_ANSI_STDIO=1 -c $< -o $@

$(DIR_OBJ)/Logger.o: $(DIR_SRC)/Logger.cpp $(DIR_INC)/Logger.h
	$(CC_CPP) $(CC_FLAGS) -I$(DIR_INC) -c $< -o $@

$(DIR_OBJ)/SensorTable.o: $(DIR_SRC)/SensorTable.cpp $(DIR_INC)/SensorTable.h
	$(CC_CPP) $(CC_FLAGS) -I$(DIR_INC) -c $< -o $@

$(DIR_OBJ)/LogTable.o: $(DIR_SRC)/LogTable.cpp $(DIR_INC)/LogTable.h
	$(CC_CPP) $(CC_FLAGS) -I$(DIR_INC) -c $< -o $@
  
$(DIR_OBJ)/UserTable.o: $(DIR_SRC)/UserTable.cpp $(DIR_INC)/UserTable.h
	$(CC_CPP) $(CC_FLAGS) -I$(DIR_INC) -c $< -o $@

$(DIR_OBJ)/Table.o: $(DIR_SRC)/Table.cpp $(DIR_INC)/Table.h
	$(CC_CPP) $(CC_FLAGS) -I$(DIR_INC) -c $< -o $@

$(DIR_OBJ)/SensorEntry.o: $(DIR_SRC)/SensorEntry.cpp $(DIR_INC)/SensorEntry.h
	$(CC_CPP) $(CC_FLAGS) -I$(DIR_INC) -c $< -o $@

$(DIR_OBJ)/LogEntry.o: $(DIR_SRC)/LogEntry.cpp $(DIR_INC)/LogEntry.h
	$(CC_CPP) $(CC_FLAGS) -I$(DIR_INC) -c $< -o $@

$(DIR_OBJ)/UserEntry.o: $(DIR_SRC)/UserEntry.cpp $(DIR_INC)/UserEntry.h
	$(CC_CPP) $(CC_FLAGS) -I$(DIR_INC) -c $< -o $@

$(DIR_OBJ)/Entry.o: $(DIR_SRC)/Entry.cpp $(DIR_INC)/Entry.h
	$(CC_CPP) $(CC_FLAGS) -I$(DIR_INC) -c $< -o $@

$(DIR_OBJ)/RegexAssembly.o: $(DIR_SRC)/RegexAssembly.cpp $(DIR_INC)/RegexAssembly.h
	$(CC_CPP) $(CC_FLAGS) -I$(DIR_INC) -c $< -o $@
  
$(DIR_OBJ)/Generic.o: $(DIR_SRC)/Generic.cpp $(DIR_INC)/Generic.h
	$(CC_CPP) $(CC_FLAGS) -I$(DIR_INC) -c $< -o $@

$(DIR_OBJ)/packet.o: ../client/packet.c ../client/packet.h
	$(CC_C) $(CC_FLAGS) -I$(DIR_INC) -c $< -o $@

$(DIR_OBJ)/crc.o: ../client/crc.c ../client/crc.h
	$(CC_C) $(CC_FLAGS) -I$(DIR_INC) -c $< -o $@
