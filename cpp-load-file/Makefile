FLAGS=$(CXXFLAGS) -std=c++11 -Wall -Wextra -pedantic -O3

FILES=1M 2M 4M 8M 16M 32M

run: test $(FILES)
	./test $(FILES)

test: test.cpp
	$(CXX) $(FLAGS) $< -o $@

$(FILES):
	dd if=/dev/zero of=$@ bs=$@ count=1

clean:
	$(RM) test $(FILES)
