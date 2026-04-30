# zim

Symlink utility that always expands both ends to full absolute paths, so
programs that cannot resolve `~` or relative paths see the real location.

## Usage

```
zim <target>       # create a symlink in CWD pointing to target
zim -r <linkpath>  # create a symlink at linkpath pointing back to CWD
```

**Normal mode** — run from the directory where you want the link to live:

```sh
cd ~/projects/myapp
zim ~/.ssh/id_ed25519.pub
# creates /home/user/projects/myapp/id_ed25519.pub -> /home/user/.ssh/id_ed25519.pub
```

**Reverse mode** (`-r`) — run from the directory that holds the real file:

```sh
cd /etc/nginx/sites-available
zim -r ../sites-enabled/zim.conf
# creates /etc/nginx/sites-enabled/zim.conf -> /etc/nginx/sites-available/zim.conf
```

The parent directory of the link must exist. The target file need not.

## Requirements

- Any POSIX.1-2001 system (Linux, macOS, BSDs)
- A C99 compiler: `gcc`, `clang`, or [tcc](https://bellard.org/tcc/)

## Get and build

```sh
git clone https://github.com/henyaw/zim.git
cd zim
make
```

To install to `/usr/local/bin`:

```sh
sudo install -m755 zim /usr/local/bin/
```

Or with a custom prefix:

```sh
sudo install -m755 zim /usr/bin/
```

### Alternate compilers

```sh
CC=clang make
CC=tcc make      # smallest toolchain, no extra dependencies
```

### Clean

```sh
make clean
```
