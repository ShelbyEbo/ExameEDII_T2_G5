NAME = exame
CC = cc
CFLAGS = -Wall -Wextra -Werror

SRCS = auth.c \
	avl.c \
	chat.c \
	file.c \
	graph.c \
	heap.c \
	huffman.c \
	main.c \
	presentation.c \
	presentation2.c \
	report.c \
	storage.c \
	user.c

OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re
