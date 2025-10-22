NAME=philo

SRC= philo.c	parsing_utils.c	parsing.c	utils.c	\
		clean_up.c

OBJ_DIR = obj
OBJ= $(addprefix $(OBJ_DIR)/, $(SRC:.c=.o))

CC= cc
CFLAGS= -Wall -Wextra -Werror


define LOADING_BAR_COMP_PHILO
	@printf "\033[0;32mCompiling philo: ["
	@for i in `seq 1 20`; do \
		printf "█"; \
		sleep 0.05; \
	done; \
	printf "] ✔️\033[0m\n\n"
endef
define LOADING_BAR_CLEAN
	@printf "\033[0;31mCleaning objects: ["
	@for i in `seq 1 20`; do \
		printf "█"; \
		sleep 0.05; \
	done; \
	printf "] ✔️\033[0m\n\n"
endef

all: $(NAME)

$(NAME): $(OBJ)
	$(call LOADING_BAR_COMP_PHILO)
	@$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

$(OBJ_DIR)/%.o : %.c | $(OBJ_DIR)
	@$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

clean:
	@rm -rf $(OBJ_DIR)
	$(call LOADING_BAR_CLEAN)

fclean: clean
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re