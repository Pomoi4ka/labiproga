.PHONY: all
all: v1 v2

v1 v2: main.cpp
	$(CXX) -ggdb -fno-exceptions -Wall -Wextra -pedantic -std=c++98 -DVERSION=$(subst v,,$@) -o $@ $<

TESTS = $(patsubst %.txt,%,$(shell ls control/))

.PHONY: control $(TESTS)
control: $(TESTS)

$(TESTS):
	@echo -n 'Ввод: '; cat control/$@.txt
	@ln -sf control/$@.txt input.txt
	@rm output.txt 2>/dev/null || :
	./$(word 1,$(subst _t, ,$@)) || :
	@if [ -f output.txt ]; then echo -n 'Вывод: '; cat output.txt; fi
