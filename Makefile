CC        = g++
NAME      = parser_generator
CPPFLAGS  = -std=c++1z -lstdc++fs
CFLAGS    = -Wall -Wextra -Wpedantic -Wundef -g
CFLAGS_R  = -Wall -Wextra -Wpedantic -Wundef -O2
LDLIBS    =
RM        = rm -fv
MKDIR     = mkdir -p

MAIN      = src/main.cpp

SRC       = src/config.cpp            \
            src/node.cpp              \
            src/cpp_jinja/context.cpp \
            src/cpp_jinja/jinja.cpp   \
            src/cpp_jinja/token.cpp   \
            src/cpp_jinja/util.cpp    \

CONFIG_DIR = config

all: $(NAME)

release: $(NAME)_R

$(NAME): $(SRC)
	$(MKDIR) $(CONFIG_DIR)
	$(CC) -o $(NAME) $(MAIN) $(SRC) $(CFLAGS) $(CPPFLAGS) \
	$(LDLIBS)

$(NAME)_R: $(SRC)
	$(MKDIR)  $(CONFIG_DIR)
	$(CC) -o /$(NAME) $(MAIN) $(SRC) $(CFLAGS_R) \
	$(CPPFLAGS) $(LDLIBS)

clean:
	$(RM) $(BUILD_DIR)/*
	rmdir $(BUILD_DIR)

