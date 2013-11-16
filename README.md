Minimal multi-threaded grep.

No bells, no whistles, no case insensitivity, not even regular expressions. Just plain text search. Works like:

> sed -n '/needle/p' haystack.txt

This will print all lines containing the string needle in the file haystack.txt. Because of the multi-threading the order of the printed lines will be somewhat random.


**BENCHMARK**

Searching a 9.3G ascii-text file named cred with search string ".se-" (406006 matching lines).

Computers:

* HP 8560w laptop (Core i7m, Linux), GNU sed 4.2.1, GNU grep 2.14, perl v5.12.4, Python 2.7.5
* Mac (Core i5, OS X), BSD sed (version?), BSD grep 2.5.1, perl v5.16.2, Python 2.7.5

Commands:

1   Sed:
    <pre><code>time sed -n '/\.se-/p' cred > se.txt</code></pre>

2   Perl:
    <pre><code>time perl -e 'while (<>) { /\.se-/ && print; }' cred > se.txt</code></pre>

3   Python:
    <pre><code>time python2 -c 'for line in open("cred"):
    if ".se-" in line:
        print line.rstrip()' > se.txt</code></pre>

4   Grep:
    <pre><code>time grep '\.se-' cred > se.txt</code></pre>

5   Fast-grep:
    <pre><code>time ./fast-grep '.se-' cred > se.txt</code></pre>

Results:

<table>
    <tr>
        <td>Computer</td> <td>1 sed</td> <td>2 perl</td> <td>3 python</td> <td>4 grep</td> <td>5 fast-grep</td>
    </tr>
    <tr>
        <td>HP laptop</td> <td>1m15s</td> <td>1m7s</td> <td>0m57s</td> <td>0m19s</td> <td>0m5s</td>
    </tr>
    <tr>
        <td>Mac OS X</td> <td>3m2s</td> <td>0m45s</td> <td>0m33s</td> <td>2m29s</td> <td>0m5s</td>
    </tr>
</table>

Note: fast-grep runs considerably slower the first time. After the file contents are cached in memory it performs good. This requires a computer with sufficient RAM, otherwise the filesystem becomes a bottleneck. None of the benchmarks in the table are from the first run. All programs had an equal chance to cache the file contents.

Note2: Sed and grep runs considerably faster on GNU/Linux while perl and python runs faster on OS X. To be fair the tests should run on the same hardware. I suspect the Mac has better hardware than the HP, which explains perl and python. Sed and grep can be explained with different implementations. The GNU implementations seems to handle memory more efficiently than the (ancient) BSD versions running on OS X. Another note is that the filesystem (or the disk) is faster on the Linux machine. It runs ext4 while the Mac runs HFS+.


**LICENSE**

    ~~=) All Rights Reversed - No Rights Reserved (=~~

Setting Orange, the 28th day of The Aftermath in the YOLD 3179

Albert Veli