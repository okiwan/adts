DEFAULT=g++ abc.cpp -o abc -Wno-deprecated
SOURCE=abc.cpp

abc: $(SOURCE)
	$(DEFAULT)
	
abc+perf: $(SOURCE)
	$(DEFAULT) -ltcmalloc -DPERFPROFILE=1

clean:
	rm volcado.*
	rm abc
