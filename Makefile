CC				= g++
CC_FLAGS	= -pedantic -Wall -Wextra -Wconversion
CC_LIBS		= 

DIR_SRC		= src
DIR_INC		= inc
DIR_OBJ		= obj
DIR_BIN		= bin

BIN				= alarm.exe

CMD_CP    = cp -f
CMD_RM    = rm -f
CMD_LN    = ln -sf
CMD_PRINT = @printf

OBJS      = $(DIR_OBJ)/generic.o $(DIR_OBJ)/regexassembly.o $(DIR_OBJ)/entry.o $(DIR_OBJ)/userentry.o $(DIR_OBJ)/logentry.o $(DIR_OBJ)/sensorentry.o $(DIR_OBJ)/table.o $(DIR_OBJ)/usertable.o $(DIR_OBJ)/logtable.o $(DIR_OBJ)/sensortable.o $(DIR_OBJ)/logger.o $(DIR_OBJ)/misc.o $(DIR_OBJ)/main.o

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

  $(DIR_OBJ)/logger.o: $(DIR_SRC)/logger.cpp $(DIR_INC)/logger.h
	$(CC) $(CC_FLAGS) -I$(DIR_INC) -c $< -o $@

$(DIR_OBJ)/sensortable.o: $(DIR_SRC)/sensortable.cpp $(DIR_INC)/sensortable.h
	$(CC) $(CC_FLAGS) -I$(DIR_INC) -c $< -o $@

$(DIR_OBJ)/logtable.o: $(DIR_SRC)/logtable.cpp $(DIR_INC)/logtable.h
	$(CC) $(CC_FLAGS) -I$(DIR_INC) -c $< -o $@
  
$(DIR_OBJ)/usertable.o: $(DIR_SRC)/usertable.cpp $(DIR_INC)/usertable.h
	$(CC) $(CC_FLAGS) -I$(DIR_INC) -c $< -o $@

$(DIR_OBJ)/table.o: $(DIR_SRC)/table.cpp $(DIR_INC)/table.h
	$(CC) $(CC_FLAGS) -I$(DIR_INC) -c $< -o $@

$(DIR_OBJ)/sensorentry.o: $(DIR_SRC)/sensorentry.cpp $(DIR_INC)/sensorentry.h
	$(CC) $(CC_FLAGS) -I$(DIR_INC) -c $< -o $@

$(DIR_OBJ)/logentry.o: $(DIR_SRC)/logentry.cpp $(DIR_INC)/logentry.h
	$(CC) $(CC_FLAGS) -I$(DIR_INC) -c $< -o $@

$(DIR_OBJ)/userentry.o: $(DIR_SRC)/userentry.cpp $(DIR_INC)/userentry.h
	$(CC) $(CC_FLAGS) -I$(DIR_INC) -c $< -o $@

$(DIR_OBJ)/entry.o: $(DIR_SRC)/entry.cpp $(DIR_INC)/entry.h
	$(CC) $(CC_FLAGS) -I$(DIR_INC) -c $< -o $@

$(DIR_OBJ)/regexassembly.o: $(DIR_SRC)/regexassembly.cpp $(DIR_INC)/regexassembly.h
	$(CC) $(CC_FLAGS) -I$(DIR_INC) -c $< -o $@
  
$(DIR_OBJ)/generic.o: $(DIR_SRC)/generic.cpp $(DIR_INC)/generic.h
	$(CC) $(CC_FLAGS) -I$(DIR_INC) -c $< -o $@
