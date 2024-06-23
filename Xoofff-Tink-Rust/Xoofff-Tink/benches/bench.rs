use criterion::{black_box, criterion_group, criterion_main, BenchmarkId, Criterion, Throughput};
use rand::{thread_rng, Rng, RngCore};
use std::time::Duration;

use reck::Deck;

#[cfg(not(any(target_arch = "x86_64", target_arch = "x86")))]
type Benchmarker = Criterion;
#[cfg(any(target_arch = "x86_64", target_arch = "x86"))]
type Benchmarker = Criterion<criterion_cycles_per_byte::CyclesPerByte>;

const KB: usize = 1024;
const MB: usize = 1024*KB;

fn bench(c: &mut Benchmarker) {
    let mut group = c.benchmark_group("DeckTink");
    let mut rng = thread_rng();
    let key = rng.gen::<[u8; 32]>();
    let nonce = rng.gen::<[u8; 32]>();
    let mut tag;
    let mut counter;
    let mut tag2;
    let mut counter2;

    for size in [KB, 2 * KB, 4 * KB, 8 * KB, 16 * KB, 32 * KB, 64 * KB, MB, 2 * MB, 4 * MB, 8 * MB, 16 * MB, 32 * MB, 64 * MB] {
        tag = [0u8; 32];
        counter = [0u8; 8];
        tag2 = [0u8; 32];
        counter2 = [0u8; 8];
        let mut buf = vec![0u8; size];
        rng.fill_bytes(&mut buf);
        let mut buf2 = vec![0u8; size];
        rng.fill_bytes(&mut buf2);
        
        group.throughput(Throughput::Bytes(size as u64));
        group.sample_size(10000);

        group.bench_function(BenchmarkId::new("wrap", size), |b| {
            let mut deck = Deck::new(&key, &nonce);
            b.iter(|| deck.wrap_inplace_detached(black_box(&mut buf), black_box(&mut tag), black_box(&mut counter)))
        });

        let mut deck2 = Deck::new(&key, &nonce);
        deck2.wrap_inplace_detached(black_box(&mut buf2), black_box(&mut tag2), black_box(&mut counter2)).expect("error");
        group.bench_function(BenchmarkId::new("unwrap", size), |b| {
            let mut unwrap_deck = Deck::new(&key, &nonce);
            b.iter(|| unwrap_deck.unwrap_inplace_detached(black_box(&mut buf2.clone()), black_box(&mut tag2.clone()), black_box(&mut counter2.clone())))
        });
    }

    group.finish();
}

#[cfg(not(any(target_arch = "x86_64", target_arch = "x86")))]
criterion_group!(
    name = benches;
    config = Criterion::default();
    targets = bench
);

#[cfg(any(target_arch = "x86_64", target_arch = "x86"))]
criterion_group!(
    name = benches;
    config = Criterion::default().with_measurement(criterion_cycles_per_byte::CyclesPerByte);
    targets = bench
);

criterion_main!(benches);
