NAME = PmergeMe

CXX = c++
CXXFLAGS = -std=c++98 -Wall -Wextra -Werror -MMD -MP
LDFLAGS =

SRCS = main.cpp PmergeMe.cpp

OBJSDIR = objs
OBJS = $(addprefix $(OBJSDIR)/, $(SRCS:%.cpp=%.o))
DEPENDS = $(OBJS:.o=.d)

all: $(NAME)

-include $(DEPENDS)

debug: CXXFLAGS += -DDEBUG -g -fsanitize=address -fsanitize=undefined -fsanitize=leak -DUNIT_TEST
debug: LDFLAGS += -fsanitize=address -fsanitize=undefined -fsanitize=leak
debug: fclean all
	./PmergeMe `shuf -i 1-1000000 -n 50000 | tr '\n' ' '`

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
