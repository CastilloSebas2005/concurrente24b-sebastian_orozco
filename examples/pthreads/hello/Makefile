# target: prerequisites
#	command to build target

hello: hello.c
	cc -g -Wall -Wextra hello.c -o hello -pthread

.PHONY: lint
lint:
	cpplint hello.c

.PHONY: memcheck
memcheck: hello
	valgrind --tool=memcheck --leak-check=full ./hello

.PHONY: helgrind
helgrind: hello
	valgrind --tool=helgrind ./hello

.PHONY: asan # invalid access & memory leaks
asan: hello.c
	cc -g -Wall -Wextra -fsanitize=address hello.c -o hello -pthread

.PHONY: msan # uninitialized memory
msan: hello.c
	clang -g -Wall -Wextra -fsanitize=memory hello.c -o hello -pthread

.PHONY: tsan # thread sanitizer
tsan: hello.c
	cc -g -Wall -Wextra -fsanitize=thread hello.c -o hello -pthread

.PHONY: ubsan # undefined behavior
ubsan: hello.c
	cc -g -Wall -Wextra -fsanitize=undefined hello.c -o hello -pthread

.PHONY: clean
clean:
	rm -f hello