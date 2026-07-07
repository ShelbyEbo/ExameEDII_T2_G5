NAME    = exame
CC      = cc
CFLAGS  = -Wall -Wextra -Werror -I includes

SRCS    = srcs/auth.c        \
          srcs/avl.c         \
          srcs/user.c        \
          srcs/file.c        \
          srcs/heap.c        \
          srcs/huffman.c     \
          srcs/free.c     \
          srcs/storage.c     \
          srcs/chat.c        \
          srcs/graph.c       \
          srcs/report.c      \
          srcs/menus.c \
          srcs/menus_aux.c \
          srcs/menu_chat_aux.c \
          srcs/menu_graphs_aux.c \
          srcs/main.c

OBJS    = $(SRCS:srcs/%.c=obj/%.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

obj/%.o: srcs/%.c
	@mkdir -p obj
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf obj

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
