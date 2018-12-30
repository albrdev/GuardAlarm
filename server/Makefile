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
DIR_CLI		:= ../client

BIN			:= gad.exe

CMD_CP		:= cp -f
CMD_RM		:= rm -f
CMD_LN		:= ln -sf
CMD_PRINT	:= @printf

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

$(DIR_OBJ)/main.o: $(DIR_SRC)/main.cpp
	$(CC_CPP) $(CC_FLAGS) -I$(DIR_INC) -I$(DIR_CLI) -D__USE_MINGW_ANSI_STDIO=1 -c $< -o $@
  
$(DIR_OBJ)/misc.o: $(DIR_SRC)/misc.cpp $(DIR_INC)/misc.hpp
	$(CC_CPP) $(CC_FLAGS) -I$(DIR_INC) -c $< -o $@

$(DIR_OBJ)/SerialPort.o: $(DIR_SRC)/SerialPort.cpp $(DIR_INC)/SerialPort.hpp
	$(CC_CPP) $(CC_FLAGS) -I$(DIR_INC) -D__USE_MINGW_ANSI_STDIO=1 -c $< -o $@

$(DIR_OBJ)/Logger.o: $(DIR_SRC)/Logger.cpp $(DIR_INC)/Logger.hpp
	$(CC_CPP) $(CC_FLAGS) -I$(DIR_INC) -c $< -o $@

$(DIR_OBJ)/SensorTable.o: $(DIR_SRC)/SensorTable.cpp $(DIR_INC)/SensorTable.hpp
	$(CC_CPP) $(CC_FLAGS) -I$(DIR_INC) -c $< -o $@

$(DIR_OBJ)/LogTable.o: $(DIR_SRC)/LogTable.cpp $(DIR_INC)/LogTable.hpp
	$(CC_CPP) $(CC_FLAGS) -I$(DIR_INC) -c $< -o $@
  
$(DIR_OBJ)/UserTable.o: $(DIR_SRC)/UserTable.cpp $(DIR_INC)/UserTable.hpp
	$(CC_CPP) $(CC_FLAGS) -I$(DIR_INC) -c $< -o $@

$(DIR_OBJ)/Table.o: $(DIR_SRC)/Table.cpp $(DIR_INC)/Table.hpp
	$(CC_CPP) $(CC_FLAGS) -I$(DIR_INC) -c $< -o $@

$(DIR_OBJ)/SensorEntry.o: $(DIR_SRC)/SensorEntry.cpp $(DIR_INC)/SensorEntry.hpp
	$(CC_CPP) $(CC_FLAGS) -I$(DIR_INC) -c $< -o $@

$(DIR_OBJ)/LogEntry.o: $(DIR_SRC)/LogEntry.cpp $(DIR_INC)/LogEntry.hpp
	$(CC_CPP) $(CC_FLAGS) -I$(DIR_INC) -c $< -o $@

$(DIR_OBJ)/UserEntry.o: $(DIR_SRC)/UserEntry.cpp $(DIR_INC)/UserEntry.hpp
	$(CC_CPP) $(CC_FLAGS) -I$(DIR_INC) -c $< -o $@

$(DIR_OBJ)/Entry.o: $(DIR_SRC)/Entry.cpp $(DIR_INC)/Entry.hpp
	$(CC_CPP) $(CC_FLAGS) -I$(DIR_INC) -c $< -o $@

$(DIR_OBJ)/RegexAssembly.o: $(DIR_SRC)/RegexAssembly.cpp $(DIR_INC)/RegexAssembly.hpp
	$(CC_CPP) $(CC_FLAGS) -I$(DIR_INC) -c $< -o $@
  
$(DIR_OBJ)/Generic.o: $(DIR_SRC)/Generic.cpp $(DIR_INC)/Generic.hpp
	$(CC_CPP) $(CC_FLAGS) -I$(DIR_INC) -c $< -o $@

$(DIR_OBJ)/packet.o: ../client/packet.c ../client/packet.h
	$(CC_C) $(CC_FLAGS) -I$(DIR_INC) -c $< -o $@

$(DIR_OBJ)/crc.o: $(DIR_CLI)/crc.c $(DIR_CLI)/crc.h
	$(CC_C) $(CC_FLAGS) -I$(DIR_INC) -c $< -o $@
