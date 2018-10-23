CC				= g++
CC_FLAGS	= -pedantic -Wall -Wextra -Wconversion
CC_LIBS		= 

DIR_INC		= inc
DIR_SRC		= src
DIR_OBJ		= obj
DIR_BIN		= bin

BIN				= alarm.exe

CMD_CP    = cp -f
CMD_RM    = rm -f
CMD_LN    = ln -sf
CMD_PRINT = @printf

OBJS      = $(DIR_OBJ)/generic.o $(DIR_OBJ)/RegexAssembly.o $(DIR_OBJ)/Entry.o $(DIR_OBJ)/UserEntry.o $(DIR_OBJ)/LogEntry.o $(DIR_OBJ)/SensorEntry.o $(DIR_OBJ)/Table.o $(DIR_OBJ)/UserTable.o $(DIR_OBJ)/LogTable.o $(DIR_OBJ)/SensorTable.o $(DIR_OBJ)/Logger.o $(DIR_OBJ)/misc.o $(DIR_OBJ)/main.o

.PHONY: all
all: $(DIR_BIN)/$(BIN)
	$(CMD_LN) $(DIR_BIN)/$(BIN) $(BIN)

.PHONY: clean
clean:
	$(CMD_RM) $(BIN)
	$(CMD_RM) $(DIR_BIN)/$(BIN)
	$(CMD_RM) $(DIR_OBJ)/*.o

$(DIR_BIN)/$(BIN): $(OBJS)
	$(CC) $(DIR_OBJ)/*.o $(CC_LIBS) -o $(DIR_BIN)/$(BIN)

$(DIR_OBJ)/main.o: $(DIR_SRC)/main.cpp
	$(CC) $(CC_FLAGS) -I$(DIR_INC) -c $< -o $@
  
$(DIR_OBJ)/misc.o: $(DIR_SRC)/misc.cpp $(DIR_INC)/misc.h
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
  
$(DIR_OBJ)/generic.o: $(DIR_SRC)/generic.cpp $(DIR_INC)/generic.h
	$(CC) $(CC_FLAGS) -I$(DIR_INC) -c $< -o $@
