# Order Book Benchmark Results

These benchmark results compare the performance of the two order book implementations in Flox:

- `FullOrderBook`: a general-purpose order book based on `std::map`
- `WindowedOrderBook`: a performance-optimized order book with fixed-size price windows and ring buffers

All benchmarks were run on:

- CPU: 8 x 4200 MHz
- Cache: L1d 48 KiB, L1i 32 KiB (x4), L2 1280 KiB (x4), L3 8192 KiB (x1)
- Load average: ~1.6
- CPU scaling: enabled

> ⚠️ Note: CPU scaling may affect measurements. Real-time results may incur extra overhead.

---

## FullOrderBook

```
Benchmark                   Time             CPU   Iterations
-------------------------------------------------------------
BM_ApplyBookUpdate        286 us          286 us         3239
BM_BestBid               40.3 ns         40.3 ns     17439755
BM_BestAsk               39.8 ns         39.8 ns     17684283
```

## WindowedOrderBook

```
Benchmark                   Time             CPU   Iterations
-------------------------------------------------------------
BM_ApplyBookUpdate       11.6 us         11.6 us        60322
BM_BestBid               38.4 ns         38.4 ns     13120482
BM_BestAsk               38.5 ns         38.5 ns     18101878
```

---

## Summary

- `WindowedOrderBook` shows dramatically better performance for `BM_ApplyBookUpdate` — over **20x faster** than `FullOrderBook`.
- Both order books show comparable performance on `BM_BestBid` and `BM_BestAsk`.
- These results validate the design goal of `WindowedOrderBook` as a fast and cache-efficient structure for latency-sensitive trading systems.