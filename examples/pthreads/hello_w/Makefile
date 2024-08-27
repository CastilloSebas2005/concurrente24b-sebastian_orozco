#target: prerequisites
#command to build target
APPNAME=$(shell basename $(shell pwd))
APPARGS=10
CFLAGS=-g -Wall -Wextra

$(APPNAME): $(APPNAME).c
	cc $(CFLAGS) $< -o $@ -pthread

.PHONY: lint
lint:
	cpplint --filter=-readability/casting $(APPNAME).c

.PHONY: memcheck
memcheck: $(APPNAME)
	valgrind --tool=memcheck --leak-check=full ./$(APPNAME) $(APPARGS)

.PHONY: helgrind
helgrind: $(APPNAME)
	valgrind --tool=helgrind ./$(APPNAME) $(APPARGS)

.PHONY: asan # invalid access & memory leaks
asan: $(APPNAME).c
	cc $(CFLAGS) -fsanitize=address $(APPNAME).c -o $(APPNAME) -pthread

.PHONY: msan # uninitialized memory
msan: $(APPNAME).c
	clang $(CFLAGS) -fsanitize=memory $(APPNAME).c -o $(APPNAME) -pthread

.PHONY: tsan # thread sanitizer
tsan: $(APPNAME).c
	cc $(CFLAGS) -fsanitize=thread $(APPNAME).c -o $(APPNAME) -pthread

.PHONY: ubsan # undefined behavior
ubsan: $(APPNAME).c
	cc $(CFLAGS) -fsanitize=undefined $(APPNAME).c -o $(APPNAME) -pthread

.PHONY: clean
clean:
	rm -f $(APPNAME)