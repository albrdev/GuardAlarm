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

OBJS      = $(DIR_OBJ)/generic.o $(DIR_OBJ)/regexassembly.o $(DIR_OBJ)/credentials.o $(DIR_OBJ)/database.o $(DIR_OBJ)/logentry.o $(DIR_OBJ)/misc.o $(DIR_OBJ)/main.o

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
  
$(DIR_OBJ)/database.o: $(DIR_SRC)/database.cpp $(DIR_INC)/database.h
	$(CC) $(CC_FLAGS) -I$(DIR_INC) -c $< -o $@

$(DIR_OBJ)/logentry.o: $(DIR_SRC)/logentry.cpp $(DIR_INC)/logentry.h
	$(CC) $(CC_FLAGS) -I$(DIR_INC) -c $< -o $@

$(DIR_OBJ)/credentials.o: $(DIR_SRC)/credentials.cpp $(DIR_INC)/credentials.h
	$(CC) $(CC_FLAGS) -I$(DIR_INC) -c $< -o $@

$(DIR_OBJ)/regexassembly.o: $(DIR_SRC)/regexassembly.cpp $(DIR_INC)/regexassembly.h
	$(CC) $(CC_FLAGS) -I$(DIR_INC) -c $< -o $@
  
$(DIR_OBJ)/generic.o: $(DIR_SRC)/generic.cpp $(DIR_INC)/generic.h
	$(CC) $(CC_FLAGS) -I$(DIR_INC) -c $< -o $@
