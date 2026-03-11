NAME =	ft_containers
SRCS =	main.cpp

OBJDIR = .objs
OBJ = $(addprefix $(OBJDIR)/, $(SRCS:.cpp=.o))

CXXFLAGS = -Wall -Wextra -Werror -std=c++98 -Iinclude
CXX = c++

# Colors
GREEN = \033[1;32m
CYAN = \033[1;36m
YELLOW = \033[1;33m
RED = \033[1;31m
RESET = \033[0m

all : $(NAME)

$(NAME):	$(OBJ)
	@$(CXX) $(CXXFLAGS) $(OBJ) -o $(NAME)
	@printf "$(GREEN)✔ $(NAME) compiled successfully$(RESET)\n"

$(OBJDIR)/%.o: %.cpp
	@mkdir -p $(OBJDIR)
	@$(CXX) $(CXXFLAGS) -c $< -o $@
	@printf "$(CYAN)  Compiling $(YELLOW)$<$(RESET)\n"

clean:
	@$(RM) -r $(OBJDIR)
	@printf "$(RED)✗ Object files removed$(RESET)\n"

fclean: clean
	@$(RM) $(NAME)
	@printf "$(RED)✗ $(NAME) removed$(RESET)\n"

re: fclean all

.PHONY: re bonus clean fclean all
