CC				:= clang
CFLAGS			:= -Iinclude -g
LD				:= $(CC)
LD_FLAGS		:= 

RM				:= rm -rf

TARGET			:= pipex

LIB				:= $(addprefix lib/, ft_strdup.c ft_strtok.c ft_strlcpy.c ft_strlen.c ft_strncmp.c)
SRC				:= $(addprefix src/, main.c paths.c) $(LIB)
OBJ				:= $(SRC:%.c=%.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(LD) $(LD_FLAGS) $(OBJ) -o $(TARGET)

clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(TARGET)

re: fclean all

%.o:%.c
	$(CC) $(CFLAGS) $< -c -o $@