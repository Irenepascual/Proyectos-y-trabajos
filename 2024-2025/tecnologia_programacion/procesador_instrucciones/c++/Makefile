ifeq ($(OS),Windows_NT)
PROG:=main.exe
else
PROG:=main
endif
SRCS:=main.cc programa.cc instruccion.cc

CXX:= g++ -O2

OBJS:=$(SRCS:.cc=.o)
DEPS:=$(SRCS:.cc=.d)

all: $(PROG)

$(PROG): $(OBJS)
	$(CXX) -o $@ $(OBJS)

%.o: %.cc
	$(CXX) -c -MMD $<

clean:
ifeq ($(OS),Windows_NT)
	-@del /q $(PROG) $(DEPS) $(OBJS)
else
	-@rm -f $(PROG) $(DEPS) $(OBJS) core
endif

# Dependencias
-include $(DEPS)
