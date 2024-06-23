all: Xoofff-benchmark
Xoofff-benchmark: Xoofff-benchmark
Xoofff-benchmark.pack: Xoofff-benchmark.tar.gz

BINDIR = ben
$(BINDIR):
	mkdir -p $(BINDIR)

MAKE ?= gmake
CC ?= gcc
AR = ar

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
    ASMFLAGS :=
endif
ifeq ($(UNAME_S),Darwin)
    ASMFLAGS := -x assembler-with-cpp -Wa,-defsym,old_gas_syntax=1 -Wa,-defsym,no_plt=1
endif
ifneq (,$(findstring mingw32,$(CC)))
    ASMFLAGS := -x assembler-with-cpp -Wa,-defsym,old_gas_syntax=1 -Wa,-defsym,no_plt=1
endif
ifneq (,$(findstring MINGW,$(UNAME_S)))
    ASMFLAGS := -x assembler-with-cpp -Wa,-defsym,no_type=1 -Wa,-defsym,no_size=1 -Wa,-defsym,no_plt=1
endif
ifneq (,$(findstring MSYS_NT,$(UNAME_S)))
    ASMFLAGS := -x assembler-with-cpp -Wa,-defsym,no_type=1 -Wa,-defsym,no_size=1 -Wa,-defsym,no_plt=1
endif
UNAME_M := $(shell uname -m)

HEADERS := $(HEADERS) config.h
SOURCES := $(SOURCES) config.h
CFLAGS := $(CFLAGS) -fomit-frame-pointer
CFLAGS := $(CFLAGS) -O2
CFLAGS := $(CFLAGS) -g0
ifneq ($(UNAME_M),aarch64)
            ifneq ($(UNAME_S),Darwin)
            ifneq ($(UNAME_M),riscv64)
            ifneq ($(UNAME_M),riscv32)
CFLAGS := $(CFLAGS) -march=native
endif
            endif
            endif
            endif
ifneq ($(UNAME_M),aarch64)
            ifneq ($(UNAME_S),Darwin)
            ifneq ($(UNAME_M),riscv64)
            ifneq ($(UNAME_M),riscv32)
CFLAGS := $(CFLAGS) -mtune=native
endif
            endif
            endif
            endif
CFLAGS := $(CFLAGS) -lm
HEADERS := $(HEADERS) Xoofff.h
SOURCES := $(SOURCES) Xoofff.h
HEADERS := $(HEADERS) XoofffModes.h
SOURCES := $(SOURCES) XoofffModes.h
HEADERS := $(HEADERS) brg_endian.h
SOURCES := $(SOURCES) brg_endian.h
HEADERS := $(HEADERS) Xoodoo.h
SOURCES := $(SOURCES) Xoodoo.h
HEADERS := $(HEADERS) Xoodyak-parameters.h
SOURCES := $(SOURCES) Xoodyak-parameters.h
HEADERS := $(HEADERS) Xoodoo-SnP.h
SOURCES := $(SOURCES) Xoodoo-SnP.h
HEADERS := $(HEADERS) Xoodoo-times4-SnP.h
SOURCES := $(SOURCES) Xoodoo-times4-SnP.h
HEADERS := $(HEADERS) Xoodoo-times8-SnP.h
SOURCES := $(SOURCES) Xoodoo-times8-SnP.h
HEADERS := $(HEADERS) align.h
SOURCES := $(SOURCES) align.h
HEADERS := $(HEADERS) timing.h
SOURCES := $(SOURCES) timing.h
HEADERS := $(HEADERS) testPerformance.h
SOURCES := $(SOURCES) testPerformance.h
HEADERS := $(HEADERS) testXooPerformance.h
SOURCES := $(SOURCES) testXooPerformance.h
INCLUDES := $(INCLUDES) timingSnP.inc
SOURCES := $(SOURCES) timingSnP.inc
INCLUDES := $(INCLUDES) timingPlSnP.inc
SOURCES := $(SOURCES) timingPlSnP.inc
INCLUDES := $(INCLUDES) timingSponge.inc
SOURCES := $(SOURCES) timingSponge.inc
INCLUDES := $(INCLUDES) timingKeyak.inc
SOURCES := $(SOURCES) timingKeyak.inc
INCLUDES := $(INCLUDES) timingXooSnP.inc
SOURCES := $(SOURCES) timingXooSnP.inc
INCLUDES := $(INCLUDES) timingXooPlSnP.inc
SOURCES := $(SOURCES) timingXooPlSnP.inc

SOURCES := $(SOURCES) Xoofff.c
$(BINDIR)/Xoofff.o: Xoofff.c $(HEADERS) $(INCLUDES)
	$(CC) $(INCLUDEFLAGS) $(CFLAGS)  -c $< -o $@
OBJECTS := $(OBJECTS) $(BINDIR)/Xoofff.o

SOURCES := $(SOURCES) XoofffModes.c
$(BINDIR)/XoofffModes.o: XoofffModes.c $(HEADERS) $(INCLUDES)
	$(CC) $(INCLUDEFLAGS) $(CFLAGS)  -c $< -o $@
OBJECTS := $(OBJECTS) $(BINDIR)/XoofffModes.o

SOURCES := $(SOURCES) Xoodoo-SIMD128.c
$(BINDIR)/Xoodoo-SIMD128.o: Xoodoo-SIMD128.c $(HEADERS) $(INCLUDES)
	$(CC) $(INCLUDEFLAGS) $(CFLAGS)  -c $< -o $@
OBJECTS := $(OBJECTS) $(BINDIR)/Xoodoo-SIMD128.o

SOURCES := $(SOURCES) Xoodyak-full-block-SIMD128.c
$(BINDIR)/Xoodyak-full-block-SIMD128.o: Xoodyak-full-block-SIMD128.c $(HEADERS) $(INCLUDES)
	$(CC) $(INCLUDEFLAGS) $(CFLAGS)  -c $< -o $@
OBJECTS := $(OBJECTS) $(BINDIR)/Xoodyak-full-block-SIMD128.o

SOURCES := $(SOURCES) Xoodoo-times4-SIMD128.c
$(BINDIR)/Xoodoo-times4-SIMD128.o: Xoodoo-times4-SIMD128.c $(HEADERS) $(INCLUDES)
	$(CC) $(INCLUDEFLAGS) $(CFLAGS)  -c $< -o $@
OBJECTS := $(OBJECTS) $(BINDIR)/Xoodoo-times4-SIMD128.o

SOURCES := $(SOURCES) Xoodoo-times8-SIMD256.c
$(BINDIR)/Xoodoo-times8-SIMD256.o: Xoodoo-times8-SIMD256.c $(HEADERS) $(INCLUDES)
	$(CC) $(INCLUDEFLAGS) $(CFLAGS)  -c $< -o $@
OBJECTS := $(OBJECTS) $(BINDIR)/Xoodoo-times8-SIMD256.o

SOURCES := $(SOURCES) main.c
$(BINDIR)/main.o: main.c $(HEADERS) $(INCLUDES)
	$(CC) $(INCLUDEFLAGS) $(CFLAGS)  -c $< -o $@
OBJECTS := $(OBJECTS) $(BINDIR)/main.o

SOURCES := $(SOURCES) testPerformance.c
$(BINDIR)/testPerformance.o: testPerformance.c $(HEADERS) $(INCLUDES)
	$(CC) $(INCLUDEFLAGS) $(CFLAGS)  -c $< -o $@
OBJECTS := $(OBJECTS) $(BINDIR)/testPerformance.o

SOURCES := $(SOURCES) timing.c
$(BINDIR)/timing.o: timing.c $(HEADERS) $(INCLUDES)
	$(CC) $(INCLUDEFLAGS) $(CFLAGS)  -c $< -o $@
OBJECTS := $(OBJECTS) $(BINDIR)/timing.o

SOURCES := $(SOURCES) testXooPerformance.c
$(BINDIR)/testXooPerformance.o: testXooPerformance.c $(HEADERS) $(INCLUDES)
	$(CC) $(INCLUDEFLAGS) $(CFLAGS)  -c $< -o $@
OBJECTS := $(OBJECTS) $(BINDIR)/testXooPerformance.o

Xoofff-benchmark: $(BINDIR) $(OBJECTS)
	mkdir -p $(dir $@)
	$(CC) -o $@ $(OBJECTS) $(CFLAGS)

Xoofff-benchmark.tar.gz: $(SOURCES)
	mkdir -p .pack/Xoofff-benchmark
	rm -rf .pack/Xoofff-benchmark/*
	cp $(SOURCES) .pack/Xoofff-benchmark/
	cd .pack/ ; tar -czf ../../Xoofff-benchmark.tar.gz Xoofff-benchmark/*

