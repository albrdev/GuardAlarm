CC				:= g++
CC_FLAGS	:= -pedantic -Wall -Wextra -Wconversion
CC_LIBS		:= 

DIR_INC		:= inc
DIR_SRC		:= src
DIR_OBJ		:= obj
DIR_BIN		:= bin

BIN				:= alarm.exe

CMD_CP    := cp -f
CMD_RM    := rm -f
CMD_LN    := ln -sf
CMD_PRINT := @printf

INCS	:= $(wildcard $(DIR_INC)/*.h)
SRCS	:= $(wildcard $(DIR_SRC)/*.cpp)
OBJS	:= $(patsubst $(DIR_SRC)/%.cpp,$(DIR_OBJ)/%.o,$(SRCS))

.PHONY: all
all: $(DIR_BIN)/$(BIN)

.PHONY: clean
clean:
	$(CMD_RM) $(DIR_OBJ)/*.o

.PHONY: distclean
distclean: clean
	$(CMD_RM) $(DIR_BIN)/$(BIN)

.PHONY: run
run: $(DIR_BIN)/$(BIN)
	./$<

$(DIR_BIN)/$(BIN): $(OBJS)
	$(CC) $(OBJS) $(CC_LIBS) -o $(DIR_BIN)/$(BIN)

$(DIR_OBJ)/Main.o: $(DIR_SRC)/Main.cpp
	$(CC) $(CC_FLAGS) -I$(DIR_INC) -c $< -o $@
  
$(DIR_OBJ)/Misc.o: $(DIR_SRC)/Misc.cpp $(DIR_INC)/Misc.h
	$(CC) $(CC_FLAGS) -I$(DIR_INC) -c $< -o $@

$(DIR_OBJ)/Logger.o: $(DIR_SRC)/Logger.cpp $(DIR_INC)/Logger.h
	$(CC) $(CC_FLAGS) -I$(DIR_INC) -c $< -o $@

$(DIR_OBJ)/SensorTable.o: $(DIR_SRC)/SensorTable.cpp $(DIR_INC)/SensorTable.h
	$(CC) $(CC_FLAGS) -I$(DIR_INC) -c $< -o $@

$(DIR_OBJ)/LogTable.o: $(DIR_SRC)/LogTable.cpp $(DIR_INC)/LogTable.h
	$(CC) $(CC_FLAGS) -I$(DIR_INC) -c $< -o $@
  
$(DIR_OBJ)/UserTable.o: $(DIR_SRC)/UserTable.cpp $(DIR_INC)/UserTable.h
	$(CC) $(CC_FLAGS) -I$(DIR_INC) -c $< -o $@

$(DIR_OBJ)/Table.o: $(DIR_SRC)/Table.cpp $(DIR_INC)/Table.h
	$(CC) $(CC_FLAGS) -I$(DIR_INC) -c $< -o $@

$(DIR_OBJ)/SensorEntry.o: $(DIR_SRC)/SensorEntry.cpp $(DIR_INC)/SensorEntry.h
	$(CC) $(CC_FLAGS) -I$(DIR_INC) -c $< -o $@

$(DIR_OBJ)/LogEntry.o: $(DIR_SRC)/LogEntry.cpp $(DIR_INC)/LogEntry.h
	$(CC) $(CC_FLAGS) -I$(DIR_INC) -c $< -o $@

$(DIR_OBJ)/UserEntry.o: $(DIR_SRC)/UserEntry.cpp $(DIR_INC)/UserEntry.h
	$(CC) $(CC_FLAGS) -I$(DIR_INC) -c $< -o $@

$(DIR_OBJ)/Entry.o: $(DIR_SRC)/Entry.cpp $(DIR_INC)/Entry.h
	$(CC) $(CC_FLAGS) -I$(DIR_INC) -c $< -o $@

$(DIR_OBJ)/RegexAssembly.o: $(DIR_SRC)/RegexAssembly.cpp $(DIR_INC)/RegexAssembly.h
	$(CC) $(CC_FLAGS) -I$(DIR_INC) -c $< -o $@
  
$(DIR_OBJ)/Generic.o: $(DIR_SRC)/Generic.cpp $(DIR_INC)/Generic.h
	$(CC) $(CC_FLAGS) -I$(DIR_INC) -c $< -o $@
