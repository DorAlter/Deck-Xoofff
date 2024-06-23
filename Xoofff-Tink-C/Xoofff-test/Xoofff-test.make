all: Xoofff-test
Xoofff-test: Xoofff-test
Xoofff-test.pack: Xoofff-test.tar.gz

BINDIR = test
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
HEADERS := $(HEADERS) KeccakSponge.h
SOURCES := $(SOURCES) KeccakSponge.h
HEADERS := $(HEADERS) Xoofff.h
SOURCES := $(SOURCES) Xoofff.h
HEADERS := $(HEADERS) XoofffModes.h
SOURCES := $(SOURCES) XoofffModes.h
HEADERS := $(HEADERS) brg_endian.h
SOURCES := $(SOURCES) brg_endian.h
HEADERS := $(HEADERS) KeccakP-1600-SnP.h
SOURCES := $(SOURCES) KeccakP-1600-SnP.h
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
HEADERS := $(HEADERS) UT.h
SOURCES := $(SOURCES) UT.h
HEADERS := $(HEADERS) testPermutations.h
SOURCES := $(SOURCES) testPermutations.h
HEADERS := $(HEADERS) testSponge.h
SOURCES := $(SOURCES) testSponge.h
HEADERS := $(HEADERS) testDuplex.h
SOURCES := $(SOURCES) testDuplex.h
HEADERS := $(HEADERS) testMotorist.h
SOURCES := $(SOURCES) testMotorist.h
HEADERS := $(HEADERS) testKeyakv2.h
SOURCES := $(SOURCES) testKeyakv2.h
HEADERS := $(HEADERS) testKetjev2.h
SOURCES := $(SOURCES) testKetjev2.h
HEADERS := $(HEADERS) genKAT.h
SOURCES := $(SOURCES) genKAT.h
HEADERS := $(HEADERS) testPRG.h
SOURCES := $(SOURCES) testPRG.h
HEADERS := $(HEADERS) testKangarooTwelve.h
SOURCES := $(SOURCES) testKangarooTwelve.h
HEADERS := $(HEADERS) testKravatte.h
SOURCES := $(SOURCES) testKravatte.h
HEADERS := $(HEADERS) testKravatteModes.h
SOURCES := $(SOURCES) testKravatteModes.h
HEADERS := $(HEADERS) testSP800-185.h
SOURCES := $(SOURCES) testSP800-185.h
HEADERS := $(HEADERS) testXoofff.h
SOURCES := $(SOURCES) testXoofff.h
HEADERS := $(HEADERS) testXoofffModes.h
SOURCES := $(SOURCES) testXoofffModes.h
HEADERS := $(HEADERS) testXoodyak.h
SOURCES := $(SOURCES) testXoodyak.h
INCLUDES := $(INCLUDES) KeccakSponge.inc
SOURCES := $(SOURCES) KeccakSponge.inc
INCLUDES := $(INCLUDES) testSnP.inc
SOURCES := $(SOURCES) testSnP.inc
INCLUDES := $(INCLUDES) testSnPnominal.inc
SOURCES := $(SOURCES) testSnPnominal.inc
INCLUDES := $(INCLUDES) testPlSnP.inc
SOURCES := $(SOURCES) testPlSnP.inc
INCLUDES := $(INCLUDES) testSponge.inc
SOURCES := $(SOURCES) testSponge.inc
INCLUDES := $(INCLUDES) testDuplex.inc
SOURCES := $(SOURCES) testDuplex.inc
INCLUDES := $(INCLUDES) testMotorist.inc
SOURCES := $(SOURCES) testMotorist.inc
INCLUDES := $(INCLUDES) testKeyakv2.inc
SOURCES := $(SOURCES) testKeyakv2.inc
INCLUDES := $(INCLUDES) testKetjev2.inc
SOURCES := $(SOURCES) testKetjev2.inc
INCLUDES := $(INCLUDES) testPRG.inc
SOURCES := $(SOURCES) testPRG.inc
INCLUDES := $(INCLUDES) testXoodyakHash.inc
SOURCES := $(SOURCES) testXoodyakHash.inc
INCLUDES := $(INCLUDES) testXoodyakKeyed.inc
SOURCES := $(SOURCES) testXoodyakKeyed.inc

SOURCES := $(SOURCES) KeccakSponge.c
$(BINDIR)/KeccakSponge.o: KeccakSponge.c $(HEADERS) $(INCLUDES)
	$(CC)  $(CFLAGS)  -c $< -o $@
OBJECTS := $(OBJECTS) $(BINDIR)/KeccakSponge.o

SOURCES := $(SOURCES) Xoofff.c
$(BINDIR)/Xoofff.o: Xoofff.c $(HEADERS) $(INCLUDES)
	$(CC)  $(CFLAGS)  -c $< -o $@
OBJECTS := $(OBJECTS) $(BINDIR)/Xoofff.o

SOURCES := $(SOURCES) XoofffModes.c
$(BINDIR)/XoofffModes.o: XoofffModes.c $(HEADERS) $(INCLUDES)
	$(CC)  $(CFLAGS)  -c $< -o $@
OBJECTS := $(OBJECTS) $(BINDIR)/XoofffModes.o

SOURCES := $(SOURCES) KeccakP-1600-AVX2.s
$(BINDIR)/KeccakP-1600-AVX2.o: KeccakP-1600-AVX2.s $(HEADERS) $(INCLUDES)
	$(CC)  $(ASMFLAGS)  -c $< -o $@
OBJECTS := $(OBJECTS) $(BINDIR)/KeccakP-1600-AVX2.o

SOURCES := $(SOURCES) Xoodoo-SIMD128.c
$(BINDIR)/Xoodoo-SIMD128.o: Xoodoo-SIMD128.c $(HEADERS) $(INCLUDES)
	$(CC)  $(CFLAGS)  -c $< -o $@
OBJECTS := $(OBJECTS) $(BINDIR)/Xoodoo-SIMD128.o

SOURCES := $(SOURCES) Xoodyak-full-block-SIMD128.c
$(BINDIR)/Xoodyak-full-block-SIMD128.o: Xoodyak-full-block-SIMD128.c $(HEADERS) $(INCLUDES)
	$(CC)  $(CFLAGS)  -c $< -o $@
OBJECTS := $(OBJECTS) $(BINDIR)/Xoodyak-full-block-SIMD128.o

SOURCES := $(SOURCES) Xoodoo-times4-SIMD128.c
$(BINDIR)/Xoodoo-times4-SIMD128.o: Xoodoo-times4-SIMD128.c $(HEADERS) $(INCLUDES)
	$(CC)  $(CFLAGS)  -c $< -o $@
OBJECTS := $(OBJECTS) $(BINDIR)/Xoodoo-times4-SIMD128.o

SOURCES := $(SOURCES) Xoodoo-times8-SIMD256.c
$(BINDIR)/Xoodoo-times8-SIMD256.o: Xoodoo-times8-SIMD256.c $(HEADERS) $(INCLUDES)
	$(CC)  $(CFLAGS)  -c $< -o $@
OBJECTS := $(OBJECTS) $(BINDIR)/Xoodoo-times8-SIMD256.o

SOURCES := $(SOURCES) main.c
$(BINDIR)/main.o: main.c $(HEADERS) $(INCLUDES)
	$(CC)  $(CFLAGS)  -c $< -o $@
OBJECTS := $(OBJECTS) $(BINDIR)/main.o

SOURCES := $(SOURCES) UT.c
$(BINDIR)/UT.o: UT.c $(HEADERS) $(INCLUDES)
	$(CC)  $(CFLAGS)  -c $< -o $@
OBJECTS := $(OBJECTS) $(BINDIR)/UT.o

SOURCES := $(SOURCES) testPermutations.c
$(BINDIR)/testPermutations.o: testPermutations.c $(HEADERS) $(INCLUDES)
	$(CC)  $(CFLAGS)  -c $< -o $@
OBJECTS := $(OBJECTS) $(BINDIR)/testPermutations.o

SOURCES := $(SOURCES) testSponge.c
$(BINDIR)/testSponge.o: testSponge.c $(HEADERS) $(INCLUDES)
	$(CC)  $(CFLAGS)  -c $< -o $@
OBJECTS := $(OBJECTS) $(BINDIR)/testSponge.o

SOURCES := $(SOURCES) testDuplex.c
$(BINDIR)/testDuplex.o: testDuplex.c $(HEADERS) $(INCLUDES)
	$(CC)  $(CFLAGS)  -c $< -o $@
OBJECTS := $(OBJECTS) $(BINDIR)/testDuplex.o

SOURCES := $(SOURCES) testMotorist.c
$(BINDIR)/testMotorist.o: testMotorist.c $(HEADERS) $(INCLUDES)
	$(CC)  $(CFLAGS)  -c $< -o $@
OBJECTS := $(OBJECTS) $(BINDIR)/testMotorist.o

SOURCES := $(SOURCES) testKeyakv2.c
$(BINDIR)/testKeyakv2.o: testKeyakv2.c $(HEADERS) $(INCLUDES)
	$(CC)  $(CFLAGS)  -c $< -o $@
OBJECTS := $(OBJECTS) $(BINDIR)/testKeyakv2.o

SOURCES := $(SOURCES) testKetjev2.c
$(BINDIR)/testKetjev2.o: testKetjev2.c $(HEADERS) $(INCLUDES)
	$(CC)  $(CFLAGS)  -c $< -o $@
OBJECTS := $(OBJECTS) $(BINDIR)/testKetjev2.o

SOURCES := $(SOURCES) genKAT.c
$(BINDIR)/genKAT.o: genKAT.c $(HEADERS) $(INCLUDES)
	$(CC)  $(CFLAGS)  -c $< -o $@
OBJECTS := $(OBJECTS) $(BINDIR)/genKAT.o

SOURCES := $(SOURCES) testPRG.c
$(BINDIR)/testPRG.o: testPRG.c $(HEADERS) $(INCLUDES)
	$(CC)  $(CFLAGS)  -c $< -o $@
OBJECTS := $(OBJECTS) $(BINDIR)/testPRG.o

SOURCES := $(SOURCES) testKangarooTwelve.c
$(BINDIR)/testKangarooTwelve.o: testKangarooTwelve.c $(HEADERS) $(INCLUDES)
	$(CC)  $(CFLAGS)  -c $< -o $@
OBJECTS := $(OBJECTS) $(BINDIR)/testKangarooTwelve.o

SOURCES := $(SOURCES) testKravatte.c
$(BINDIR)/testKravatte.o: testKravatte.c $(HEADERS) $(INCLUDES)
	$(CC)  $(CFLAGS)  -c $< -o $@
OBJECTS := $(OBJECTS) $(BINDIR)/testKravatte.o

SOURCES := $(SOURCES) testKravatteModes.c
$(BINDIR)/testKravatteModes.o: testKravatteModes.c $(HEADERS) $(INCLUDES)
	$(CC)  $(CFLAGS)  -c $< -o $@
OBJECTS := $(OBJECTS) $(BINDIR)/testKravatteModes.o

SOURCES := $(SOURCES) testSP800-185.c
$(BINDIR)/testSP800-185.o: testSP800-185.c $(HEADERS) $(INCLUDES)
	$(CC)  $(CFLAGS)  -c $< -o $@
OBJECTS := $(OBJECTS) $(BINDIR)/testSP800-185.o

SOURCES := $(SOURCES) testXoofff.c
$(BINDIR)/testXoofff.o: testXoofff.c $(HEADERS) $(INCLUDES)
	$(CC)  $(CFLAGS)  -c $< -o $@
OBJECTS := $(OBJECTS) $(BINDIR)/testXoofff.o

SOURCES := $(SOURCES) testXoofffModes.c
$(BINDIR)/testXoofffModes.o: testXoofffModes.c $(HEADERS) $(INCLUDES)
	$(CC)  $(CFLAGS)  -c $< -o $@
OBJECTS := $(OBJECTS) $(BINDIR)/testXoofffModes.o

SOURCES := $(SOURCES) testXoodyak.c
$(BINDIR)/testXoodyak.o: testXoodyak.c $(HEADERS) $(INCLUDES)
	$(CC)  $(CFLAGS)  -c $< -o $@
OBJECTS := $(OBJECTS) $(BINDIR)/testXoodyak.o

Xoofff-test: $(BINDIR) $(OBJECTS)
	mkdir -p $(dir $@)
	$(CC) -o $@ $(OBJECTS) $(CFLAGS)

Xoofff-test.tar.gz: $(SOURCES)
	mkdir -p .pack/Xoofff-test
	rm -rf .pack/*
	cp $(SOURCES) .pack/
	cd .pack/ ; tar -czf ../../Xoofff-test.tar.gz *

