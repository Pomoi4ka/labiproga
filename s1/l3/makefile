main: main.cpp
	$(CXX) -Wall -Wextra -pedantic -std=c++98 -o $@ $<

plot: main
	./main
	sed 's/ //g; s/|/ /g' table.txt > table.csv
	gnuplot plot.gnuplot
	feh *.svg
