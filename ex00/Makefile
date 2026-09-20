NAME = whatever
CC = c++
CFLAGS = -Wall -Wextra -Werror -std=c++98 -O2
SRCS = main.cpp 
OBJS = $(SRCS:.cpp=.o)

RESET	= \033[0m
GREEN	= \033[0;32m
YELLOW	= \033[0;33m
RED		= \033[0;31m
CYAN	= \033[0;36m

all: $(NAME)

$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJS)
	@echo "$(GREEN)$(NAME) compiled successfully!$(RESET)"

%.o: %.cpp 
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo "$(CYAN)We are compiling the files, hold on...<$(RESET)"

clean:
	@rm -f $(OBJS)
	@echo "$(YELLOW)We are cleaning the object files, hold on...$(RESET)"

fclean: clean
	@rm -f $(NAME)
	@echo "$(RED)We are cleaning the executable, hold on...$(RESET)"

re: fclean all

.PHONY: all clean fclean re