CC=g++
CPP_FLAGS=-std=c++11 -lboost_program_options -lssl -lcrypto -lboost_system -lpthread
OBJ_DIR=obj
AC_SOURCES=TransportLayerProvider PostProvider MailClient
AC_OBJ=$(AC_SOURCES:%=$(OBJ_DIR)/$(AC_DIR)/%.o)
AC_DIR=abstract_client
BT_SOURCES=tls
BT_OBJ=$(BT_SOURCES:%=$(OBJ_DIR)/$(BT_DIR)/%.o)
BT_DIR=boost_tools
PP_SOURCES=pop3
PP_OBJ=$(PP_SOURCES:%=$(OBJ_DIR)/$(PP_DIR)/%.o)
PP_DIR=pp
SOURCES=$(AC_SOURCES:%=$(AC_DIR)/%.cpp) $(BT_SOURCES:%=$(BT_DIR)/%.cpp) $(PP_SOURCES:%=$(PP_DIR)/%.cpp) main.cpp 
OBJECTS=$(SOURCES:%.cpp=$(OBJ_DIR)/%.o)
OBJ_DIRS=$(OBJ_DIR) $(OBJ_DIR)/$(AC_DIR) $(OBJ_DIR)/$(BT_DIR) $(OBJ_DIR)/$(PP_DIR)
EXEC_NAME=pop3_client

all: $(OBJECTS)
	g++ $(OBJECTS) $(CPP_FLAGS) -o $(EXEC_NAME)

debug: $(OBJECTS)
	g++ $(OBJECTS) $(CPP_FLAGS) -g -o $(EXEC_NAME)

$(OBJECTS): $(OBJ_DIR)/%.o : %.cpp
	@mkdir -p $(OBJ_DIRS)
	$(CC) $(CPP_FLAGS) -g -c $< -o $@

clean:
	@rm -rf $(OBJ_DIRS)
	@rm $(EXEC_NAME)
