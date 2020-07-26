# Shadowsocks Face

shadowsocks-libev client gui wrapper.

## Build and install

First, install qt5 development package:

```sh
sudo apt install qt5-default    # Ubuntu
sudo dnf install qt5-devel      # Fedora
```

Then clone and build project:

```sh
git clone https://github.com/df543/Shadowsocks-Face.git
cd Shadowsocks-Face; mkdir build; cd build
qmake-qt5 .. || qmake ..
make
```

Also available from [AUR](https://aur.archlinux.org/packages/ss-face/).

## Troubleshooting

1. `ERROR: bind: Address already in use`: Try to kill all ss-local processes.
2. (macOS) Failed to start ss-local process, but shadowsocks-libev has already installed using homebrew: Start program via terminal `open -a path/ss-face`.
