Minimal multi-threaded grep.

No bells, no whistles, no case insensitivity, no regular expressions. Just plain text search. Works similar to:

> sed -n '/needle/p' haystack.txt

This will print all lines containing the string *needle* in the file *haystack.txt*. Because of the multi-threading the order of the printed lines will be somewhat random.


**EXAMPLE**

Search for lines containing the string *'.se-'* in the file *cred*, but skip lines containing the string *'-|-|--'*:

> ./fast-grep -v '-|-|--' '.se-' cred


**LIMITATIONS**

* Only tested with GNU/Linux and OS X.
* Will fail for large files (> 2Gb) on 32-bit OS (possible to fix by using LFS and repeatedly calling mmap64 with 1 or 2 Gb in each iteration, this is currently not implemented).


**BENCHMARK**

Searching a 9.3G ascii-text file named *cred* with search string ".se-" (406006 matching lines).

Computers:

* **HP** 8560w laptop (Core i7m, Linux), GNU sed 4.2.1, GNU grep 2.14, perl v5.12.4, Python 2.7.5
* **Mac** (Core i5, OS X), BSD sed (version?), BSD grep 2.5.1, perl v5.16.2, Python 2.7.5
* **Ubuntu** (Xeon W3550), GNU sed 4.2.1, GNU grep 2.10, perl v5.14.2, Python 2.7.3

Commands:

1.  **Sed**
    <pre><code>time sed -n '/\.se-/p' cred > se.txt</code></pre>

2.  **Perl**
    <pre><code>time perl -e 'while (&lt;&gt;) { /\.se-/ && print; }' cred > se.txt</code></pre>

3.  **Python**
    <pre><code>time python2 -c 'for line in open("cred"):
    if ".se-" in line:
        print line.rstrip()' > se.txt</code></pre>

4.  **Grep**
    <pre><code>time grep '\.se-' cred > se.txt</code></pre>

5.  **Fast-grep**
    <pre><code>time ./fast-grep '.se-' cred > se.txt</code></pre>

Results:

<table>
    <tr>
        <td>Computer</td> <td>1 <b>sed</b></td> <td>2 <b>perl</b></td> <td>3 <b>python</b></td> <td>4 <b>grep</b></td> <td>5 <b>fast-grep</b></td>
    </tr>
    <tr>
        <td><b>HP laptop</b></td> <td>1m15s</td> <td>1m7s</td> <td>0m57s</td> <td>0m19s</td> <td>0m5s</td>
    </tr>
    <tr>
        <td><b>Mac OS X</b></td> <td>3m2s</td> <td>0m45s</td> <td>0m33s</td> <td>2m29s</td> <td>0m5s</td>
    </tr>
    <tr>
        <td><b>Ubuntu</b></td> <td>1m13s</td> <td>1m14s</td> <td>1m13s</td> <td>1m11s</td> <td>1m17s<br />(1m10s w/o threading)</td>
    </tr>
</table>

Note: fast-grep runs considerably slower the first time. After the file contents are cached in memory it performs good. This requires a computer with sufficient RAM, otherwise the filesystem becomes a bottleneck. None of the benchmarks in the table are from the first run. All programs had an equal chance to cache the file contents.

Note2: Sed and grep runs faster on Linux while perl and python runs faster on OS X. To be completely fair the tests should be re-run on the same hardware. Sed and grep can be explained with different implementations (BSD/GNU) while the file reading seems to be faster on the Linux machine (ext4/HFS+).

Note3: The Ubuntu machine didn't have enough RAM to cache the whole file in memory, so all programs had to do a lot of disk accesses. In this case it really measures the speed of the disk rather than the speed of the program.


**LICENSE**

    ~~=) All Rights Reversed - No Rights Reserved (=~~

Setting Orange, the 28th day of The Aftermath in the YOLD 3179

Albert Veli