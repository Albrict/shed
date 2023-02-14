OBJS = main.cpp shed.cpp screen.cpp error.cpp text_field.cpp event.cpp file.cpp input_handler.cpp

CC = g++

COMPILER_FLAGS = -Wall -W -std=c++17 -O0 -ffunction-sections -fdata-sections

LINKER_FLAGS = -lncursesw

OBJ_NAME = shed

shed: $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)
