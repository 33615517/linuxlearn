.PHONY: all
all: testsig mykill





testsig:testSig.cc
	g++ -o $@ $^ -std=c++11
mykill:mykill.cc
	g++ -o $@ $^ -std=c++11


.PHONY: clean
clean:
	rm -f testsig mysill