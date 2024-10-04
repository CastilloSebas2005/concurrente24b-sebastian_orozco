include ../../../common/Makefile

FLAG+=-pthread
PERF=perf stat -B -e cache-references,cache-misses,cycles,instructions,branches,faults,migrations

.PHONY: perf
perf:
	$(PERF) $(EXEFILE) 0
	$(PERF) $(EXEFILE) 1
	$(PERF) $(EXEFILE) 2
	$(PERF) $(EXEFILE) 3
