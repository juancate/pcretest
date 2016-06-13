CFLAGS=`pcre-config --cflags` -Isrc/
CLIBS=`pcre-config --libs`
OBJS = main.o pattern.o utils.o
CPPS = src/main.cpp src/pattern.cpp src/utils.cpp
BIN = patt

all: obj
	g++ $(CLIBS) -o $(BIN) $(OBJS)

obj: $(CPPS)
	g++ $(CFLAGS) -c $(CPPS)

clean:
	rm $(OBJS)

dist-clean:
	rm $(OBJS) $(BIN)

debug: debug-obj
	g++ $(CLIBS) -o $(BIN) $(OBJS)

debug-obj:
	g++ $(CFLAGS) -c $(CPPS) -DDEBUG
