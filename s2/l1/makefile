CXXFLAGS=-Wall -Wextra -pedantic -std=c++98 -ggdb -Wno-cast-function-type
ifeq ($(CXX), clang++)
	CXXFLAGS+=-fsanitize=memory,undefined
endif
main: main.cpp
	$(CXX) $(CXXFLAGS) -o $@ main.cpp

tests/out%.txt: tests/in%.txt main
	@echo 'Ввод: '; cat $<
	@ln -sf $< input.txt
	@echo 1 | ./main >/dev/null
	@mv output.txt $@
	@echo 'Вывод: '; cat $@

.PHONY: tests
tests: $(addprefix tests/,$(subst in,out,$(shell ls tests/)))
