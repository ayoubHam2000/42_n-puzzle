Main = \
	main.cpp

Examples = \
	./examples/draw_maze.cpp

FILES = \
	$(Main)



#-Wall -Wextra -Werror -g
# -fsanitize=address

CFLAGS = -std=c++11 -Wall -Wextra  -g
INCLUDE = -I ./includes/ -I ./classes
LINKING = -L/usr/local/lib -lSDL2 -lSDL2_image -lm

DEPFLAGS = -MMD -MF $(@:.o=.d)

NAME = n-puzzle.out

OBJ_DIR = _OUT/

OBJ = $(addprefix $(OBJ_DIR), $(FILES:.cpp=.o))

deps = $(OBJ:.o=.d)

all : $(NAME)


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

re : fclean all
