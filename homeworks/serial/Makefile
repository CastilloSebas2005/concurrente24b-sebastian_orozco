include ../../common/Makefile

FLAG += -pthread

main: main.c
	cc -g -Wall -Wextra main.c -o main -pthread

.PHONY: lint
lint:
	cpplint main.c

.PHONY: memcheck
memcheck: main
	valgrind --tool=memcheck --leak-check=full ./main

.PHONY: helgrind
helgrind: main
	valgrind --tool=helgrind ./main

.PHONY: asan # invalid access & memory leaks
asan: main.c
	cc -g -Wall -Wextra -fsanitize=address main.c -o main -pthread

.PHONY: msan # uninitialized memory
msan: main.c
	clang -g -Wall -Wextra -fsanitize=memory main.c -o main -pthread

.PHONY: tsan # thread sanitizer
tsan: main.c
	cc -g -Wall -Wextra -fsanitize=thread main.c -o main -pthread

.PHONY: ubsan # undefined behavior
ubsan: main.c
	cc -g -Wall -Wextra -fsanitize=undefined hello.c -o hello -pthread

.PHONY: clean
clean:
	rm -f main