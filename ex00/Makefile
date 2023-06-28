NAME = btc

CXX = c++
CXXFLAGS = -std=c++98 -Wall -Wextra -Werror -MMD -MP
CPPFLAGS = -Iincludes
LDFLAGS =

SRCS = main.cpp
VPATH = srcs

OBJSDIR = objs
OBJS = $(addprefix $(OBJSDIR)/, $(SRCS:%.cpp=%.o))
DEPENDS = $(OBJS:.o=.d)

all: $(NAME)

-include $(DEPENDS)

debug: LDFLAGS += -fsanitize=address -fsanitize=undefined -fsanitize=leak
debug: CXXFLAGS += -DDEBUG -g -fsanitize=address -fsanitize=undefined -fsanitize=leak
debug: fclean all
	./btc

unit-test: CXXFLAGS += -DUNIT_TEST
unit-test: fclean all
	./btc

$(NAME): $(OBJSDIR) $(OBJS)
	$(CXX) $(LDFLAGS) -o $(NAME) $(OBJS)

$(OBJSDIR)/%.o: %.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -o $@ -c $<

objs:
	mkdir -p objs

clean:
	$(RM) -rf $(OBJSDIR)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re debug test
