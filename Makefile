Main = \
	src/classes/PuzzleState.cpp \
	src/classes/Configuration.cpp \
	src/a_star.cpp \
	src/solve.cpp \
	src/is_solvable.cpp \
	src/main.cpp

Generator = \
	./examples/draw_maze.cpp

FILES = \
	$(Main)



#-Wall -Wextra -Werror -g
# -fsanitize=address

CFLAGS = -std=c++11 -Wall -Wextra  -g
INCLUDE = -I ./includes/
LINKING = -L/usr/local/lib -lSDL2 -lSDL2_image -lm

DEPFLAGS = -MMD -MF $(@:.o=.d)

NAME = n-puzzle.out
GENERATOR_NAME = npuzzle-gen.out

OBJ_DIR = _OUT/

OBJ = $(addprefix $(OBJ_DIR), $(FILES:.cpp=.o))

deps = $(OBJ:.o=.d)

all : $(NAME)

generator : $(GENERATOR_NAME)

$(GENERATOR_NAME): src/make_puzzle.c
	gcc -Wall -Wextra -Werror src/make_puzzle.c -o $(GENERATOR_NAME)

$(NAME) : $(OBJ)
	g++ $(OBJ) $(LINKING) -o $(NAME)

$(OBJ) : $(OBJ_DIR)%.o : %.cpp
	mkdir -p $(dir $@)
	g++ $(CFLAGS) $(INCLUDE) -c $< $(DEPFLAGS) -o $@

-include $(deps)

clean :
	rm -rf $(OBJ_DIR)

fclean : clean
	rm -f $(NAME)
	rm -f $(GENERATOR_NAME)

re : fclean all
