CC				:= clang
CFLAGS			:= -Iinclude -Ilibft/include -g
LD				:= $(CC)
LD_FLAGS		:= -Llibft -lft

RM				:= rm -rf

TARGET			:= pipex

SRC				:= $(addprefix src/, main.c paths.c cmd.c stat.c here_doc.c exec_normal.c exec_here_doc.c argv.c error.c parsecl.c)
OBJ				:= $(SRC:%.c=%.o)

all: libft libft/libft.a $(TARGET)

libft:
	git clone https://github.com/aurelien-brabant/libft

libft/libft.a:
	make -C libft

norm:
	make norm -C libft
	norminette include
	norminette src

$(TARGET): $(OBJ)
	$(LD) $(OBJ) -o $(TARGET) $(LD_FLAGS)

clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(TARGET)

re: fclean all

%.o:%.c
	$(CC) $(CFLAGS) $< -c -o $@
