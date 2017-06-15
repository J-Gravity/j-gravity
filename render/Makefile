
NAME	= grav

SRC		= vector.cpp matrix.cpp camera.cpp main.cpp color.cpp render.cpp
OBJ		= $(addprefix $(OBJDIR),$(SRC:.cpp=.o))

CC		= clang++
CGLAGS	= -O3 -I ./frameworks/SDL2.framework/Headers/
LDFLAGS	= -Wl -rpath ./frameworks/ -F ./frameworks/ -framework SDL2

SRCDIR	= ./src/
INCDIR	= ./includes/
OBJDIR	= ./obj/

all: obj $(NAME)

obj:
	mkdir -p $(OBJDIR)

$(OBJDIR)%.o: $(SRCDIR)%.cpp
	$(CC) $(CGLAGS) -I $(INCDIR) -o $@ -c $<

$(NAME): $(OBJ)
	$(CC) $(LDFLAGS) $(OBJ) -o $(NAME)

clean:
	rm -rf $(OBJDIR)

fclean: clean
	rm -rf $(NAME)

f: re
	./$(NAME)

re: fclean all
