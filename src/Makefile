




## The executable name
EXE = BluebearEngine

OBJS = bitboards.o evaluate.o main.o moves.o position.o

FLAGS = -O3

all: $(EXE); rm $(OBJS)

$(EXE): $(OBJS)
	g++ -o $@ $(OBJS) $(FLAGS)

## 
%.o: %.cpp
	g++ -c -o $@ $< $(FLAGS)






