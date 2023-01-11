PROG:=version
SRCS:=main.cc

CXX:= g++ -Wall

OBJS:=$(SRCS:.cc=.o)

all: $(PROG)

$(PROG): $(OBJS)
	$(CXX) -o $@ $(OBJS)

.cc.o:
	$(CXX) -c $<

.cc.s:
	$(CXX) -c -S $<

edit:
	geany -i -s *.h *.cc &

clean:
	@rm -f $(PROG) $(OBJS) core *.s *.gch

# Dependencias
main.o: 