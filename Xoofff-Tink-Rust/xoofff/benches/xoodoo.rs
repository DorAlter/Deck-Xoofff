use criterion::{black_box, criterion_group, criterion_main, BatchSize, Criterion, Throughput};
use rand::{thread_rng, Rng};
use xoofff::xoodoo;

fn xoodoo<const ROUNDS: usize>(c: &mut Criterion<criterion_cycles_per_byte::CyclesPerByte>) {
    let mut rng = thread_rng();

    let mut group = c.benchmark_group("xoodoo");
    group.throughput(Throughput::Bytes(48)); // Xoodoo permutation works on 384 -bit wide state
    let mut state = [0u32; 12];
    rng.fill(&mut state);

    group.bench_function(format!("xoodoo[{}] (cached)", ROUNDS), |bench| {
        bench.iter(|| xoodoo::permute::<{ ROUNDS }>(black_box(&mut state)))
    });
    group.bench_function(format!("xoodoo[{}] (random)", ROUNDS), |bench| {
        let mut state = [0u32; 12];
        rng.fill(&mut state);

        bench.iter_batched(
            || state.clone(),
            |mut state| xoodoo::permute::<{ ROUNDS }>(black_box(&mut state)),
            BatchSize::SmallInput,
        )
    });

    group.finish();
}

criterion_group!(name = permutation; 
                 config = Criterion::default().with_measurement(criterion_cycles_per_byte::CyclesPerByte);
                 targets = xoodoo::<6>);
criterion_main!(permutation);
