INCS = cmdline.h expr.h Parse.h Val.h Pointers.h Env.h Cont.h Step.h

OBJSLIB = cmdline.o expr.o Parse.o Val.o Function.o Env.o Cont.o Step.o

OBJS = main.o $(OBJSLIB)

CXXFLAGS = --std=c++14 -O2 -fsanitize=undefined -fno-sanitize-recover=undefined

msdscript: $(OBJS)
	$(CXX) $(CXXFLAGS) -o msdscript $(OBJS)

msdscriptlib.a: $(OBJSLIB)
	ar -ruv msdscriptlib.a $(OBJSLIB)

.PHONY: test
test: msdscript
	./msdscript --test

main.o: main.cpp $(INCS)
	$(CXX) $(CXXFLAGS) -c $<

cmdline.o: cmdline.cpp $(INCS)
	$(CXX) $(CXXFLAGS) -c $<

expr.o: expr.cpp $(INCS)
	$(CXX) $(CXXFLAGS) -c $<

Parse.o: Parse.cpp $(INCS)
	$(CXX) $(CXXFLAGS) -c $<

Val.o: Val.cpp $(INCS)
	$(CXX) $(CXXFLAGS) -c $<

Function.o: Function.cpp $(INCS)
	$(CXX) $(CXXFLAGS) -c $<

Env.o: Env.cpp $(INCS)
	$(CXX) $(CXXFLAGS) -c $<

Cont.o: Cont.cpp $(INCS)
	$(CXX) $(CXXFLAGS) -c $<

Step.o: Step.cpp $(INCS)
	$(CXX) $(CXXFLAGS) -c $<

INCS2 = ../test_msdscript/exec.h
OBJS2 = ../test_msdscript/exec.o ../test_msdscript/main_exec.o

test_msdscript: $(OBJS2)
	$(CXX) $(CXXFLAGS) -o test_msdscript $(OBJS2)

main_exec.o: ../test_msdscript/main_exec.cpp $(INCS2)
	$(CXX) $(CXXFLAGS) -c $<

exec.o: ../test_msdscript/exec.cpp $(INCS2)
	$(CXX) $(CXXFLAGS) -c $<

