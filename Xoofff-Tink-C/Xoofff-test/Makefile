all: Xoofff-test
Xoofff-test: bin/Xoofff-test
Xoofff-test.pack: bin/Xoofff-test.tar.gz

BINDIR = bin/.build/Xoofff-test
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

HEADERS := $(HEADERS) bin/.build/Xoofff-test/config.h
SOURCES := $(SOURCES) bin/.build/Xoofff-test/config.h
INCLUDEFLAGS := $(INCLUDEFLAGS) -Ibin/.build/Xoofff-test/
INCLUDEFLAGS := $(INCLUDEFLAGS) -Ilib/high/Keccak/
INCLUDEFLAGS := $(INCLUDEFLAGS) -Ilib/high/Xoofff/
INCLUDEFLAGS := $(INCLUDEFLAGS) -Ilib/common/
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
INCLUDEFLAGS := $(INCLUDEFLAGS) -Ilib/low/KeccakP-1600/AVX2/
INCLUDEFLAGS := $(INCLUDEFLAGS) -Ilib/low/Xoodoo/
INCLUDEFLAGS := $(INCLUDEFLAGS) -Ilib/high/Xoodyak/
INCLUDEFLAGS := $(INCLUDEFLAGS) -Ilib/low/Xoodoo/SSE2/
INCLUDEFLAGS := $(INCLUDEFLAGS) -Ilib/low/Xoodoo-times4/SSSE3/
INCLUDEFLAGS := $(INCLUDEFLAGS) -Ilib/low/Xoodoo-times8/AVX2/
INCLUDEFLAGS := $(INCLUDEFLAGS) -Itests/UnitTests/
HEADERS := $(HEADERS) lib/high/Keccak/KeccakSponge.h
SOURCES := $(SOURCES) lib/high/Keccak/KeccakSponge.h
HEADERS := $(HEADERS) lib/high/Xoofff/Xoofff.h
SOURCES := $(SOURCES) lib/high/Xoofff/Xoofff.h
HEADERS := $(HEADERS) lib/high/Xoofff/XoofffModes.h
SOURCES := $(SOURCES) lib/high/Xoofff/XoofffModes.h
HEADERS := $(HEADERS) lib/common/brg_endian.h
SOURCES := $(SOURCES) lib/common/brg_endian.h
HEADERS := $(HEADERS) lib/low/KeccakP-1600/AVX2/KeccakP-1600-SnP.h
SOURCES := $(SOURCES) lib/low/KeccakP-1600/AVX2/KeccakP-1600-SnP.h
HEADERS := $(HEADERS) lib/low/Xoodoo/Xoodoo.h
SOURCES := $(SOURCES) lib/low/Xoodoo/Xoodoo.h
HEADERS := $(HEADERS) lib/high/Xoodyak/Xoodyak-parameters.h
SOURCES := $(SOURCES) lib/high/Xoodyak/Xoodyak-parameters.h
HEADERS := $(HEADERS) lib/low/Xoodoo/SSE2/Xoodoo-SnP.h
SOURCES := $(SOURCES) lib/low/Xoodoo/SSE2/Xoodoo-SnP.h
HEADERS := $(HEADERS) lib/low/Xoodoo-times4/SSSE3/Xoodoo-times4-SnP.h
SOURCES := $(SOURCES) lib/low/Xoodoo-times4/SSSE3/Xoodoo-times4-SnP.h
HEADERS := $(HEADERS) lib/low/Xoodoo-times8/AVX2/Xoodoo-times8-SnP.h
SOURCES := $(SOURCES) lib/low/Xoodoo-times8/AVX2/Xoodoo-times8-SnP.h
HEADERS := $(HEADERS) lib/common/align.h
SOURCES := $(SOURCES) lib/common/align.h
HEADERS := $(HEADERS) tests/UnitTests/UT.h
SOURCES := $(SOURCES) tests/UnitTests/UT.h
HEADERS := $(HEADERS) tests/UnitTests/testPermutations.h
SOURCES := $(SOURCES) tests/UnitTests/testPermutations.h
HEADERS := $(HEADERS) tests/UnitTests/testSponge.h
SOURCES := $(SOURCES) tests/UnitTests/testSponge.h
HEADERS := $(HEADERS) tests/UnitTests/testDuplex.h
SOURCES := $(SOURCES) tests/UnitTests/testDuplex.h
HEADERS := $(HEADERS) tests/UnitTests/testMotorist.h
SOURCES := $(SOURCES) tests/UnitTests/testMotorist.h
HEADERS := $(HEADERS) tests/UnitTests/testKeyakv2.h
SOURCES := $(SOURCES) tests/UnitTests/testKeyakv2.h
HEADERS := $(HEADERS) tests/UnitTests/testKetjev2.h
SOURCES := $(SOURCES) tests/UnitTests/testKetjev2.h
HEADERS := $(HEADERS) tests/UnitTests/genKAT.h
SOURCES := $(SOURCES) tests/UnitTests/genKAT.h
HEADERS := $(HEADERS) tests/UnitTests/testPRG.h
SOURCES := $(SOURCES) tests/UnitTests/testPRG.h
HEADERS := $(HEADERS) tests/UnitTests/testKangarooTwelve.h
SOURCES := $(SOURCES) tests/UnitTests/testKangarooTwelve.h
HEADERS := $(HEADERS) tests/UnitTests/testKravatte.h
SOURCES := $(SOURCES) tests/UnitTests/testKravatte.h
HEADERS := $(HEADERS) tests/UnitTests/testKravatteModes.h
SOURCES := $(SOURCES) tests/UnitTests/testKravatteModes.h
HEADERS := $(HEADERS) tests/UnitTests/testSP800-185.h
SOURCES := $(SOURCES) tests/UnitTests/testSP800-185.h
HEADERS := $(HEADERS) tests/UnitTests/testXoofff.h
SOURCES := $(SOURCES) tests/UnitTests/testXoofff.h
HEADERS := $(HEADERS) tests/UnitTests/testXoofffModes.h
SOURCES := $(SOURCES) tests/UnitTests/testXoofffModes.h
HEADERS := $(HEADERS) tests/UnitTests/testXoodyak.h
SOURCES := $(SOURCES) tests/UnitTests/testXoodyak.h
INCLUDES := $(INCLUDES) lib/high/Keccak/KeccakSponge.inc
SOURCES := $(SOURCES) lib/high/Keccak/KeccakSponge.inc
INCLUDES := $(INCLUDES) tests/UnitTests/testSnP.inc
SOURCES := $(SOURCES) tests/UnitTests/testSnP.inc
INCLUDES := $(INCLUDES) tests/UnitTests/testSnPnominal.inc
SOURCES := $(SOURCES) tests/UnitTests/testSnPnominal.inc
INCLUDES := $(INCLUDES) tests/UnitTests/testPlSnP.inc
SOURCES := $(SOURCES) tests/UnitTests/testPlSnP.inc
INCLUDES := $(INCLUDES) tests/UnitTests/testSponge.inc
SOURCES := $(SOURCES) tests/UnitTests/testSponge.inc
INCLUDES := $(INCLUDES) tests/UnitTests/testDuplex.inc
SOURCES := $(SOURCES) tests/UnitTests/testDuplex.inc
INCLUDES := $(INCLUDES) tests/UnitTests/testMotorist.inc
SOURCES := $(SOURCES) tests/UnitTests/testMotorist.inc
INCLUDES := $(INCLUDES) tests/UnitTests/testKeyakv2.inc
SOURCES := $(SOURCES) tests/UnitTests/testKeyakv2.inc
INCLUDES := $(INCLUDES) tests/UnitTests/testKetjev2.inc
SOURCES := $(SOURCES) tests/UnitTests/testKetjev2.inc
INCLUDES := $(INCLUDES) tests/UnitTests/testPRG.inc
SOURCES := $(SOURCES) tests/UnitTests/testPRG.inc
INCLUDES := $(INCLUDES) tests/UnitTests/testXoodyakHash.inc
SOURCES := $(SOURCES) tests/UnitTests/testXoodyakHash.inc
INCLUDES := $(INCLUDES) tests/UnitTests/testXoodyakKeyed.inc
SOURCES := $(SOURCES) tests/UnitTests/testXoodyakKeyed.inc

SOURCES := $(SOURCES) lib/high/Keccak/KeccakSponge.c
$(BINDIR)/KeccakSponge.o: lib/high/Keccak/KeccakSponge.c $(HEADERS) $(INCLUDES)
	$(CC) $(INCLUDEFLAGS) $(CFLAGS)  -c $< -o $@
OBJECTS := $(OBJECTS) $(BINDIR)/KeccakSponge.o

SOURCES := $(SOURCES) lib/high/Xoofff/Xoofff.c
$(BINDIR)/Xoofff.o: lib/high/Xoofff/Xoofff.c $(HEADERS) $(INCLUDES)
	$(CC) $(INCLUDEFLAGS) $(CFLAGS)  -c $< -o $@
OBJECTS := $(OBJECTS) $(BINDIR)/Xoofff.o

SOURCES := $(SOURCES) lib/high/Xoofff/XoofffModes.c
$(BINDIR)/XoofffModes.o: lib/high/Xoofff/XoofffModes.c $(HEADERS) $(INCLUDES)
	$(CC) $(INCLUDEFLAGS) $(CFLAGS)  -c $< -o $@
OBJECTS := $(OBJECTS) $(BINDIR)/XoofffModes.o

SOURCES := $(SOURCES) lib/low/KeccakP-1600/AVX2/KeccakP-1600-AVX2.s
$(BINDIR)/KeccakP-1600-AVX2.o: lib/low/KeccakP-1600/AVX2/KeccakP-1600-AVX2.s $(HEADERS) $(INCLUDES)
	$(CC) $(INCLUDEFLAGS) $(ASMFLAGS)  -c $< -o $@
OBJECTS := $(OBJECTS) $(BINDIR)/KeccakP-1600-AVX2.o

SOURCES := $(SOURCES) lib/low/Xoodoo/SSE2/Xoodoo-SIMD128.c
$(BINDIR)/Xoodoo-SIMD128.o: lib/low/Xoodoo/SSE2/Xoodoo-SIMD128.c $(HEADERS) $(INCLUDES)
	$(CC) $(INCLUDEFLAGS) $(CFLAGS)  -c $< -o $@
OBJECTS := $(OBJECTS) $(BINDIR)/Xoodoo-SIMD128.o

SOURCES := $(SOURCES) lib/low/Xoodoo/SSE2/Xoodyak-full-block-SIMD128.c
$(BINDIR)/Xoodyak-full-block-SIMD128.o: lib/low/Xoodoo/SSE2/Xoodyak-full-block-SIMD128.c $(HEADERS) $(INCLUDES)
	$(CC) $(INCLUDEFLAGS) $(CFLAGS)  -c $< -o $@
OBJECTS := $(OBJECTS) $(BINDIR)/Xoodyak-full-block-SIMD128.o

SOURCES := $(SOURCES) lib/low/Xoodoo-times4/SSSE3/Xoodoo-times4-SIMD128.c
$(BINDIR)/Xoodoo-times4-SIMD128.o: lib/low/Xoodoo-times4/SSSE3/Xoodoo-times4-SIMD128.c $(HEADERS) $(INCLUDES)
	$(CC) $(INCLUDEFLAGS) $(CFLAGS)  -c $< -o $@
OBJECTS := $(OBJECTS) $(BINDIR)/Xoodoo-times4-SIMD128.o

SOURCES := $(SOURCES) lib/low/Xoodoo-times8/AVX2/Xoodoo-times8-SIMD256.c
$(BINDIR)/Xoodoo-times8-SIMD256.o: lib/low/Xoodoo-times8/AVX2/Xoodoo-times8-SIMD256.c $(HEADERS) $(INCLUDES)
	$(CC) $(INCLUDEFLAGS) $(CFLAGS)  -c $< -o $@
OBJECTS := $(OBJECTS) $(BINDIR)/Xoodoo-times8-SIMD256.o

SOURCES := $(SOURCES) tests/UnitTests/main.c
$(BINDIR)/main.o: tests/UnitTests/main.c $(HEADERS) $(INCLUDES)
	$(CC) $(INCLUDEFLAGS) $(CFLAGS)  -c $< -o $@
OBJECTS := $(OBJECTS) $(BINDIR)/main.o

SOURCES := $(SOURCES) tests/UnitTests/UT.c
$(BINDIR)/UT.o: tests/UnitTests/UT.c $(HEADERS) $(INCLUDES)
	$(CC) $(INCLUDEFLAGS) $(CFLAGS)  -c $< -o $@
OBJECTS := $(OBJECTS) $(BINDIR)/UT.o

SOURCES := $(SOURCES) tests/UnitTests/testPermutations.c
$(BINDIR)/testPermutations.o: tests/UnitTests/testPermutations.c $(HEADERS) $(INCLUDES)
	$(CC) $(INCLUDEFLAGS) $(CFLAGS)  -c $< -o $@
OBJECTS := $(OBJECTS) $(BINDIR)/testPermutations.o

SOURCES := $(SOURCES) tests/UnitTests/testSponge.c
$(BINDIR)/testSponge.o: tests/UnitTests/testSponge.c $(HEADERS) $(INCLUDES)
	$(CC) $(INCLUDEFLAGS) $(CFLAGS)  -c $< -o $@
OBJECTS := $(OBJECTS) $(BINDIR)/testSponge.o

SOURCES := $(SOURCES) tests/UnitTests/testDuplex.c
$(BINDIR)/testDuplex.o: tests/UnitTests/testDuplex.c $(HEADERS) $(INCLUDES)
	$(CC) $(INCLUDEFLAGS) $(CFLAGS)  -c $< -o $@
OBJECTS := $(OBJECTS) $(BINDIR)/testDuplex.o

SOURCES := $(SOURCES) tests/UnitTests/testMotorist.c
$(BINDIR)/testMotorist.o: tests/UnitTests/testMotorist.c $(HEADERS) $(INCLUDES)
	$(CC) $(INCLUDEFLAGS) $(CFLAGS)  -c $< -o $@
OBJECTS := $(OBJECTS) $(BINDIR)/testMotorist.o

SOURCES := $(SOURCES) tests/UnitTests/testKeyakv2.c
$(BINDIR)/testKeyakv2.o: tests/UnitTests/testKeyakv2.c $(HEADERS) $(INCLUDES)
	$(CC) $(INCLUDEFLAGS) $(CFLAGS)  -c $< -o $@
OBJECTS := $(OBJECTS) $(BINDIR)/testKeyakv2.o

SOURCES := $(SOURCES) tests/UnitTests/testKetjev2.c
$(BINDIR)/testKetjev2.o: tests/UnitTests/testKetjev2.c $(HEADERS) $(INCLUDES)
	$(CC) $(INCLUDEFLAGS) $(CFLAGS)  -c $< -o $@
OBJECTS := $(OBJECTS) $(BINDIR)/testKetjev2.o

SOURCES := $(SOURCES) tests/UnitTests/genKAT.c
$(BINDIR)/genKAT.o: tests/UnitTests/genKAT.c $(HEADERS) $(INCLUDES)
	$(CC) $(INCLUDEFLAGS) $(CFLAGS)  -c $< -o $@
OBJECTS := $(OBJECTS) $(BINDIR)/genKAT.o

SOURCES := $(SOURCES) tests/UnitTests/testPRG.c
$(BINDIR)/testPRG.o: tests/UnitTests/testPRG.c $(HEADERS) $(INCLUDES)
	$(CC) $(INCLUDEFLAGS) $(CFLAGS)  -c $< -o $@
OBJECTS := $(OBJECTS) $(BINDIR)/testPRG.o

SOURCES := $(SOURCES) tests/UnitTests/testKangarooTwelve.c
$(BINDIR)/testKangarooTwelve.o: tests/UnitTests/testKangarooTwelve.c $(HEADERS) $(INCLUDES)
	$(CC) $(INCLUDEFLAGS) $(CFLAGS)  -c $< -o $@
OBJECTS := $(OBJECTS) $(BINDIR)/testKangarooTwelve.o

SOURCES := $(SOURCES) tests/UnitTests/testKravatte.c
$(BINDIR)/testKravatte.o: tests/UnitTests/testKravatte.c $(HEADERS) $(INCLUDES)
	$(CC) $(INCLUDEFLAGS) $(CFLAGS)  -c $< -o $@
OBJECTS := $(OBJECTS) $(BINDIR)/testKravatte.o

SOURCES := $(SOURCES) tests/UnitTests/testKravatteModes.c
$(BINDIR)/testKravatteModes.o: tests/UnitTests/testKravatteModes.c $(HEADERS) $(INCLUDES)
	$(CC) $(INCLUDEFLAGS) $(CFLAGS)  -c $< -o $@
OBJECTS := $(OBJECTS) $(BINDIR)/testKravatteModes.o

SOURCES := $(SOURCES) tests/UnitTests/testSP800-185.c
$(BINDIR)/testSP800-185.o: tests/UnitTests/testSP800-185.c $(HEADERS) $(INCLUDES)
	$(CC) $(INCLUDEFLAGS) $(CFLAGS)  -c $< -o $@
OBJECTS := $(OBJECTS) $(BINDIR)/testSP800-185.o

SOURCES := $(SOURCES) tests/UnitTests/testXoofff.c
$(BINDIR)/testXoofff.o: tests/UnitTests/testXoofff.c $(HEADERS) $(INCLUDES)
	$(CC) $(INCLUDEFLAGS) $(CFLAGS)  -c $< -o $@
OBJECTS := $(OBJECTS) $(BINDIR)/testXoofff.o

SOURCES := $(SOURCES) tests/UnitTests/testXoofffModes.c
$(BINDIR)/testXoofffModes.o: tests/UnitTests/testXoofffModes.c $(HEADERS) $(INCLUDES)
	$(CC) $(INCLUDEFLAGS) $(CFLAGS)  -c $< -o $@
OBJECTS := $(OBJECTS) $(BINDIR)/testXoofffModes.o

SOURCES := $(SOURCES) tests/UnitTests/testXoodyak.c
$(BINDIR)/testXoodyak.o: tests/UnitTests/testXoodyak.c $(HEADERS) $(INCLUDES)
	$(CC) $(INCLUDEFLAGS) $(CFLAGS)  -c $< -o $@
OBJECTS := $(OBJECTS) $(BINDIR)/testXoodyak.o

bin/Xoofff-test: $(BINDIR) $(OBJECTS)
	mkdir -p $(dir $@)
	$(CC) -o $@ $(OBJECTS) $(CFLAGS)

bin/Xoofff-test.tar.gz: $(SOURCES)
	mkdir -p bin/.pack/Xoofff-test
	rm -rf bin/.pack/Xoofff-test/*
	cp $(SOURCES) bin/.pack/Xoofff-test/
	cd bin/.pack/ ; tar -czf ../../bin/Xoofff-test.tar.gz Xoofff-test/*

