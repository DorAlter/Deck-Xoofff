use criterion::{black_box, criterion_group, criterion_main, BatchSize, Criterion, Throughput};
use rand::{thread_rng, RngCore};
use xoofff::Xoofff;

#[cfg(not(any(target_arch = "x86_64", target_arch = "x86")))]
type Benchmarker = Criterion;
#[cfg(any(target_arch = "x86_64", target_arch = "x86"))]
type Benchmarker = Criterion<criterion_cycles_per_byte::CyclesPerByte>;

fn xoofff<
    const KLEN: usize,   // deck function key byte length
    const MLEN: usize,   // to be absorbed message byte length
    const DLEN: usize,   // to be squeezed output byte length
    const OFFSET: usize, // deck function offset byte length
>(
    c: &mut Benchmarker,
) {
    let mut rng = thread_rng();

    let mut group = c.benchmark_group("xoofff");
    group.throughput(Throughput::Bytes((MLEN + DLEN + OFFSET) as u64));

    let mut key = vec![0u8; KLEN];
    let mut msg = vec![0u8; MLEN];
    let mut dig = vec![0u8; DLEN];

    rng.fill_bytes(&mut key);
    rng.fill_bytes(&mut msg);
    rng.fill_bytes(&mut dig);

    group.bench_function(
        format!(
            "key = {} | in = {} | out = {} (cached)",
            KLEN, MLEN, DLEN
        ),
        |bench| {
            bench.iter(|| {
                let mut deck = Xoofff::new(black_box(&key));
                deck.absorb(black_box(&msg), black_box(0b0), black_box(0));
                deck.squeeze(black_box(&mut dig));
            });
        },
    );
    
    group.bench_function(
        format!(
            "key = {} | in = {} | out = {} (random)",
            KLEN, MLEN, DLEN
        ),
        |bench| {
            bench.iter_batched(
                || (key.clone(), msg.clone(), dig.clone()),
                |(key, msg, mut dig)| {
                    let mut deck = Xoofff::new(black_box(&key));
                    deck.absorb(black_box(&msg), black_box(0b0), black_box(OFFSET));
                    deck.squeeze(black_box(&mut dig));
                },
                BatchSize::SmallInput,
            );
        },
    );
}

#[cfg(not(any(target_arch = "x86_64", target_arch = "x86")))]
criterion_group!(
    name = deck_function;
    config = Criterion::default();
    targets = 
    xoofff::<32, 1024, 32, 0>,
    xoofff::<32, 64, 32, 16>,
    xoofff::<32, 128, 32, 16>,
    xoofff::<32, 256, 32, 16>,
    xoofff::<32, 512, 32, 16>,
    xoofff::<32, 1024, 32, 16>,
    xoofff::<32, 2048, 32, 16>,
    xoofff::<32, 4096, 32, 16>,
);

#[cfg(any(target_arch = "x86_64", target_arch = "x86"))]
criterion_group!(
    name = deck_function;
    config = Criterion::default().with_measurement(criterion_cycles_per_byte::CyclesPerByte);
    targets = 
    xoofff::<32, 1024, 32, 0>,
    xoofff::<32, 64, 32, 16>,
    xoofff::<32, 128, 32, 16>,
    xoofff::<32, 256, 32, 16>,
    xoofff::<32, 512, 32, 16>,
    xoofff::<32, 1024, 32, 16>,
    xoofff::<32, 2048, 32, 16>,
    xoofff::<32, 4096, 32, 16>,
);
criterion_main!(deck_function);