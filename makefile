NAME = gravity_ga
CC = g++

CF = -Wall -Werror -Wextra -pedantic
CF += -std=c++11
CF += `sdl2-config --cflags`
CF += -lSDL2_image
#-I/usr/local/include
#CF += -D_REENDTRANT -D_THREAD_SAFE

LD = `sdl2-config --libs`
#-L/usr/local/lib -lSDL2 -pthread
#-Wl,-rpath=/usr/local/lib

CSRC = main.cpp
CSRC += Drawable.cpp Circle.cpp Isosceles.cpp Line.cpp
CSRC += ObserverNotifier.cpp World.cpp UserModel.cpp UserEvent.cpp ViewClient.cpp

OBJS = ${CSRC:%.cpp=%.o}

vpath %.h Drawable
vpath %.cpp Drawable
vpath %.h ObserverNotifier
vpath %.cpp ObserverNotifier

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) -o $(NAME) $(OBJS) $(LD) $(CF)

%.o: %.cpp
	$(CC) -c $< $(CF)

run: $(NAME)
	./$(NAME)

clean:
	-rm $(NAME) a.out *.core *.o *~ .*~

depend:
	$(CC) -E -MM $(CSRC) > .depend
