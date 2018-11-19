HDR_DIR =header
HEADER = $(addprefix -I,$(HDR_DIR))
CCOPTION = Wall std=gnu++14 D__USE_MINGW_ANSI_STDIO=1 pthread
FLAGS = $(addprefix -,$(CCOPTION))
CC = g++
OBJ_DIR =obj
OBJ = link.o data_operations.o net.o neuron.o main.o 
BUILD_DIR =bin
SRC_DIR =src

$(BUILD_DIR)/a.out: $(addprefix $(OBJ_DIR)/,$(OBJ)) 
	 $(CC) $(FLAGS) $(addprefix $(OBJ_DIR)/,$(OBJ)) -o $(BUILD_DIR)/a.out

$(OBJ_DIR)/link.o: $(SRC_DIR)/link.cpp 
	 $(CC) $(FLAGS) -c $(SRC_DIR)/link.cpp -o $(OBJ_DIR)/link.o $(HEADER)

$(OBJ_DIR)/data_operations.o: $(SRC_DIR)/data_operations.cpp 
	 $(CC) $(FLAGS) -c $(SRC_DIR)/data_operations.cpp -o $(OBJ_DIR)/data_operations.o $(HEADER)

$(OBJ_DIR)/net.o: $(SRC_DIR)/net.cpp 
	 $(CC) $(FLAGS) -c $(SRC_DIR)/net.cpp -o $(OBJ_DIR)/net.o $(HEADER)

$(OBJ_DIR)/neuron.o: $(SRC_DIR)/neuron.cpp 
	 $(CC) $(FLAGS) -c $(SRC_DIR)/neuron.cpp -o $(OBJ_DIR)/neuron.o $(HEADER)

$(OBJ_DIR)/main.o: $(SRC_DIR)/main.cpp 
	 $(CC) $(FLAGS) -c $(SRC_DIR)/main.cpp -o $(OBJ_DIR)/main.o $(HEADER)


clean: 
	rm $(OBJ_DIR)/*.o
	rm $(BUILD_DIR)/*.out