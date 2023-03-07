# 2023q1 Homework2 (quiz2)

## Question 1

```
static inline uint64_t pow2(uint8_t e)
{
    return ((uint64_t) 1) << e;
}
```

Consider or operation and the example, we can conclude that
next_pow2 will try to turn every bit below the highest 1 bit
into 1.

```
 or  0   1
 0   0   1
 1   1   1

 ex x=6, 110 | 10 | 1 -> 111

uint64_t next_pow2(uint64_t x)
{
    /*Here we deals with 8 bits together*/
    x |= x >> 1;
    x |= x >> 1;
    x |= x >> 1;
    x |= x >> 1;
    x |= x >> 1;
    x |= x >> 1;
    x |= x >> 1;

    /* since we have 64 bits, we don't need to repeat 64 times.
     * just shift 8 bits to copy the group from 8th to 15th bits
     * and so on.
     */
    x |= x >> 8;
    x |= x >> 16;
    x |= x >> 32;

    /*Now all the bits will be 1, add 1 will get our result!*/
    return x + 1;
}

uint64_t next_pow2_built_in(uint64_t x)
{
    int pow = 63 - __builtin_clzl(x) + 1;
    return ((uint64_t) 1) << pow;
}
```

## Question 2

When Concat 2 binary using or operation, we have to consider
how many space we have to keep for next number. For example,
to concat 2 afer 1, 2 is 10 in binary form, so we have to shift
2 bits for 1 to make a room for 10.

```
int concatenatedBinary(int n)
{
    const int M = 1e9 + 7;
    int len = 0; /* the bit length to be shifted */
    /* use long here as it potentially could overflow for int */
    long ans = 0;
    for (int i = 1; i <= n; i++) {
        /* removing the rightmost set bit
         * e.g. 100100 -> 100000
         *      000001 -> 000000
         *      000000 -> 000000
         * after removal, if it is 0, then it means it is power of 2
         * as all power of 2 only contains 1 set bit
         * if it is power of 2, we increase the bit length
         */
        if (!(i & i - 1))
            len++;
        ans = (i | (ans << len)) % M;
    }
    return ans;
}

int concatenated_binary_built_in(int n)
{
    const int M = 1e9 + 7;
    int len = 0; /* the bit length to be shifted */
    /* use long here as it potentially could overflow for int */
    long ans = 0;
    for (int i = 1; i <= n; i++) {
        /* removing the rightmost set bit
         * e.g. 100100 -> 100000
         *      000001 -> 000000
         *      000000 -> 000000
         * after removal, if it is 0, then it means it is power of 2
         * as all power of 2 only contains 1 set bit
         * if it is power of 2, we increase the bit length
         */
        ans = (i | (ans << (64 - __builtin_clzl(i)))) % M;
    }
    return ans;
}
```

## Question 3 

We can see, swar_count_utf8 is way faster than count_utf8

```
Samples: 2K of event 'cycles'
Event count (approx.): 81152180673
Overhead  Command  Shared Object      Symbol
........  .......  .................  ..................................
    53.28%  me       libc.so.6          [.] __random
    23.89%  me       me                 [.] count_utf8
     9.89%  me       libc.so.6          [.] __random_r
     5.02%  me       me                 [.] main
     2.40%  me       libc.so.6          [.] rand
     1.85%  me       me                 [.] 0x00000000000010b4
     1.79%  me       me                 [.] __popcountdi2
     1.53%  me       me                 [.] swar_count_utf8
     0.18%  me       [kernel.kallsyms]  [k] mas_wr_store_entry.isra.0
     0.05%  me       [kernel.kallsyms]  [k] _raw_spin_lock_irqsave
     0.04%  me       [kernel.kallsyms]  [k] rcu_core
     0.04%  me       [kernel.kallsyms]  [k] update_sd_lb_stats.constprop.0
     0.04%  me       [kernel.kallsyms]  [k] llist_add_batch
     0.00%  me       [kernel.kallsyms]  [k] native_sched_clock
     0.00%  me       [kernel.kallsyms]  [k] native_write_msr
```

```
size_t count_utf8(const char *buf, size_t len)
{
    const int8_t *p = (const int8_t *) buf;
    size_t counter = 0;
    for (size_t i = 0; i < len; i++) {
        /* -65 is 0b10111111, anything larger in two-complement's should start
         * new code point.
         */
        if (p[i] > -65)
            counter++;
    }
    return counter;
}

size_t swar_count_utf8(const char *buf, size_t len)
{
    const uint64_t *qword = (const uint64_t *) buf;
    const uint64_t *end = qword + (len >> 3);
    size_t count = 0;
    for (; qword != end; qword++) {
        const uint64_t t0 = *qword;
        const uint64_t t1 = ~t0;
        const uint64_t t2 = t1 & 0x04040404040404040llu;
        const uint64_t t3 = t2 + t2;
        const uint64_t t4 = t0 & t3;
        count += __builtin_popcountll(t4);
    }
    /* According to utf-8 rule, all the chars not continuation bytes are
     * utf-8 bytes
     */
    count = (1 << 3) * (len / 8) - count;
    count += (len & 7) ? count_utf8((const char *) end, len & 7) : 0;

    return count;
}
```

## Question 4

```
bool is_pattern(uint16_t x)
{
    const uint16_t n = ~x + 1;
    return (n ^ x) < x;
}
```