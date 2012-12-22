src := $(wildcard *.cc)
obj := ${src:.cc=.o}

CPPFLAGS += -Wall

.PHONY: pareto clean

pareto: $(obj)
	$(LINK.cc) $(obj) -o $@

clean:
	@- $(RM) $(obj)
