all: type constexpr static_assert tuple

type:typeInference.cc
	g++ -o $@ $^ -std=c++14

constexpr:constexpr.cc
	g++ -o $@ $^ -std=c++17

static_assert:static_assert.cc
	g++ -o $@ $^ -std=c++11

tuple:tuple.cc
	g++ -o $@ $^ -std=c++11

.PHONY: clean all
clean:
	rm -f type constexpr static_assert tuple