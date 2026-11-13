# Unix Tools in C: A Map

Rewrite the core Unix/Linux tools from scratch, easiest first. Each tool lists functions to look up (read `man 2 <name>` for syscalls, `man 3 <name>` for libc), a deliberately vague algorithm, and resources.

## Ground rules

- Read the real tool's man page (`man 1 <tool>`), then **implement only the basics first**. Add flags one at a time.
- Compare your output against the real tool, byte for byte (`diff <(mytool x) <(realtool x)`).
- Always check return values. Print errors with `perror`/`strerror`.
- Compile strict: `gcc -Wall -Wextra -Wpedantic -g -fsanitize=address,undefined`.
- Stuck? Read the man page again, then the source of a small implementation (toybox, busybox, suckless sbase) **after** you've tried.

## General resources

| Resource | Why |
|---|---|
| `man 2 intro`, `man 3 intro`, `man 7 <topic>` | The primary source for everything |
| *The C Programming Language* (K&R) | Ch. 7-8 cover I/O and the Unix interface |
| *The Linux Programming Interface* (Kerrisk) | The best reference for the syscall side |
| *Advanced Programming in the UNIX Environment* (Stevens/Rago) | Classic, deeper |
| *Operating Systems: Three Easy Pieces* (OSTEP, free online) | Processes, files, concepts |
| POSIX spec (pubs.opengroup.org) | Defines what each tool must do |
| toybox, busybox, suckless sbase | Small readable reference implementations |
| GNU coreutils source | Full-featured, harder to read |

---

## Tier 0: Warm-up (args, stdout, exit codes)

| Tool | Functions to look up | Algorithm | Resources |
|---|---|---|---|
| `true` / `false` | `exit` | Return 0 or 1 | `man 1 true` |
| `echo` | `puts`, `printf`, `argv` | Loop over args, print with spaces, handle `-n` | `man 1 echo`, K&R ch. 5 |
| `yes` | `puts`, `fputs` | Print forever | `man 1 yes` |
| `pwd` | `getcwd` | Ask the OS, print | `man 3 getcwd` |
| `whoami` | `geteuid`, `getpwuid` | uid to name lookup | `man 3 getpwuid` |
| `hostname` | `gethostname` | Ask the OS, print | `man 2 gethostname` |
| `basename` / `dirname` | `strrchr` | Find last slash, split | `man 1 basename` |
| `sleep` | `nanosleep`, `strtod` | Parse arg, wait | `man 2 nanosleep` |
| `seq` | `strtol`, `printf` | Parse range, loop | `man 1 seq` |

## Tier 1: Reading and writing files

| Tool | Functions to look up | Vague algorithm | Resources |
|---|---|---|---|
| `cat` | `open`, `read`, `write`, `close` | Read a chunk, write a chunk, repeat until EOF; no args means stdin | K&R 8.2-8.3, `man 2 read` |
| `wc` | `read`, `isspace` | Walk bytes, track state for words, count newlines | `man 1 wc` |
| `head` | `read`, `fgets` | Count newlines, stop after N | `man 1 head` |
| `tail` | `lseek`, `fstat` | Seek near end, scan backward for N newlines, print forward | `man 2 lseek` |
| `tee` | `read`, `write` | Write each chunk to stdout and to the files | `man 1 tee` |
| `tac` / `rev` | `getline`, `realloc` | Read all lines, emit in reverse order (or reverse each line) | `man 3 getline` |
| `nl` | `getline` | Prefix each line with a counter | `man 1 nl` |
| `cmp` | `read`, `memcmp` | Read both files in parallel, report first difference | `man 1 cmp` |
| `od` / `hexdump` | `read`, `printf` | Read 16 bytes at a time, print offset, hex, and ASCII | `man 1 od` |

## Tier 2: The filesystem

| Tool | Functions to look up | Vague algorithm | Resources |
|---|---|---|---|
| `mkdir` | `mkdir`, `chmod` | Call it; `-p` means walk path components | `man 2 mkdir` |
| `rmdir` / `rm` | `rmdir`, `unlink`, `stat` | Remove entries; `-r` means recurse through directories first | `man 2 unlink` |
| `touch` | `utimensat`, `open` | Create if missing, update timestamps | `man 2 utimensat` |
| `ln` / `readlink` | `link`, `symlink`, `readlink` | One syscall each | `man 7 symlink` |
| `mv` | `rename`, `stat` | Try rename; if it crosses filesystems, copy then delete | `man 2 rename` |
| `cp` | `open`, `read`, `write`, `fstat`, `fchmod` | Copy bytes, preserve mode; `-r` recurses | `man 1 cp` |
| `chmod` / `chown` | `chmod`, `chown`, `strtol` | Parse mode (octal first, symbolic later), apply | `man 7 inode` |
| `stat` | `stat`, `lstat`, `localtime`, `strftime` | Fill struct, print fields | `man 2 stat` |
| `ls` | `opendir`, `readdir`, `stat`, `qsort` | List entries, sort, then add `-l`, `-a`, `-R`, columns | `man 3 readdir` |
| `du` / `df` | `stat`, `statvfs` | Recursively sum block counts / ask filesystem for totals | `man 2 statvfs` |
| `find` | `opendir`, `readdir`, `lstat`, `fnmatch` | Recursive walk; evaluate tests on each entry | `man 3 nftw`, `man 3 fnmatch` |
| `tree` | `opendir`, `readdir` | Recursive walk with an indent/prefix string | |

## Tier 3: Text processing

| Tool | Functions to look up | Vague algorithm | Resources |
|---|---|---|---|
| `uniq` | `getline`, `strcmp` | Compare each line to previous, count runs | `man 1 uniq` |
| `cut` | `getline`, `strchr`, `strtok_r` | Split by delimiter, pick fields or byte ranges | `man 1 cut` |
| `paste` / `comm` / `join` | `getline`, `strcmp` | Walk several files in lockstep | |
| `tr` | lookup table (`unsigned char[256]`) | Build a mapping table, then translate each byte | `man 1 tr` |
| `fold` / `fmt` | `getline` | Track column, break lines at width | |
| `sort` | `qsort`, `strcmp`, `getline` | Load lines, sort with comparator, print; add `-n`, `-r`, `-k`, `-u` | `man 3 qsort` |
| `grep` | `getline`, `strstr`, then `regcomp`/`regexec` | Per line, test for a match, print if matched; add `-v`, `-i`, `-n`, `-r` | `man 3 regex` |
| `diff` | `getline`, arrays | Longest common subsequence of lines, then print the gaps | Search: "Myers diff algorithm" |
| `sed` (subset) | `regexec`, `getline` | Parse a tiny script (`s///`, `d`, `p`), apply per line | `man 1 sed` |
| `awk` (tiny) | parser, `regexec` | Split into fields; match pattern; run action | The AWK Book |

## Tier 4: Processes and the system

| Tool | Functions to look up | Vague algorithm | Resources |
|---|---|---|---|
| `env` | `environ`, `execvp`, `setenv` | Modify environment, then exec the command | `man 3 exec` |
| `time` | `fork`, `execvp`, `wait4`, `clock_gettime` | Fork, exec, wait, report durations | `man 2 wait4` |
| `kill` | `kill`, `strtol` | Parse signal and pid, send | `man 7 signal` |
| `nice` | `setpriority`, `execvp` | Set priority, exec | `man 2 setpriority` |
| `xargs` | `fork`, `execvp`, `getline` | Read tokens from stdin, build argv, run | `man 1 xargs` |
| `ps` | `opendir`, `readdir`, `fopen` on `/proc` | Walk `/proc/<pid>/`, parse `stat` and `cmdline` | `man 5 proc` |
| `uptime` / `free` | `fopen`, `sscanf` | Parse `/proc/uptime`, `/proc/meminfo` | `man 5 proc` |
| `watch` | `fork`, `execvp`, `sleep` | Loop: clear, run, wait | |
| `top` | `/proc`, `termios`/`ncurses` | `ps` in a loop with sorting and a redraw | `man 3 termios` |

## Tier 5: Advanced

| Tool | Functions to look up | Vague algorithm | Resources |
|---|---|---|---|
| `dd` | `read`, `write`, `lseek` | Parse `key=value` args, copy blocks | `man 1 dd` |
| `less` / `more` | `termios`, `ioctl(TIOCGWINSZ)`, `read` | Raw terminal mode, page-at-a-time display, key handling | `man 3 termios` |
| **A shell** | `fork`, `execvp`, `waitpid`, `pipe`, `dup2`, `chdir`, `signal` | Read line, parse, fork/exec, wire pipes and redirects, builtins (`cd`, `exit`) | OSTEP shell project, "Write a shell in C" tutorials |
| `tar` | `read`, `write`, `stat`, headers | Fixed 512-byte headers followed by padded data blocks | `man 5 tar`, POSIX ustar spec |
| `gzip` | DEFLATE (via zlib first, from scratch later) | Huffman plus LZ77 | RFC 1951, RFC 1952 |
| `make` | parser, `stat`, `fork`, `execvp` | Parse rules into a dependency graph, rebuild what's out of date | POSIX make spec |
| `cron` | `fork`, `time`, `sleep` | Parse schedule lines, wake every minute, run due jobs | `man 5 crontab` |
| `nc` | `socket`, `connect`, `bind`, `listen`, `accept`, `poll` | Open socket, shuttle stdin/stdout to and from it | Beej's Guide to Network Programming |
| `ping` | raw `socket`, `sendto`, `recvfrom` | Build ICMP echo packet, send, time the reply | `man 7 raw`, `man 7 icmp` |
| tiny HTTP client/server | `getaddrinfo`, sockets | Speak plain-text HTTP/1.0 over TCP | Beej's Guide, RFC 9112 |
| `strace` (tiny) | `ptrace`, `waitpid` | Trace child, stop at each syscall, print number and args | `man 2 ptrace` |
| `mount` | `mount`, `/proc/mounts` | Thin wrapper over the syscall | `man 2 mount` |
| A tiny `init` | `fork`, `exec`, `waitpid`, `reboot` | Run as PID 1, spawn services, reap zombies | `man 7 pid_namespaces` |
| A small text editor | `termios`, escape sequences | Raw mode, line buffer, redraw loop | "Build Your Own Text Editor" (kilo) |

---

## Suggested order

1. Tier 0 and 1 (learn argv, file descriptors, buffers)
2. `ls`, `cp`, `mv`, `rm` (learn the filesystem API)
3. `grep`, `sort`, `uniq` (learn dynamic memory and string handling)
4. `ps`, `time`, `xargs` (learn fork/exec/wait)
5. **A shell**, which ties processes, pipes, and signals together
6. Everything else by interest

## Progress

- [ ] Tier 0
- [ ] Tier 1
- [ ] Tier 2
- [ ] Tier 3
- [ ] Tier 4
- [ ] Tier 5
