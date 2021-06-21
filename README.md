# Shadowsocks Face

A simple, cross-platform GUI for shadowsocks client.

It works on all shadowsocks implementations that supports JSON configuration, including [shadowsocks-rust](https://github.com/shadowsocks/shadowsocks-rust), [shadowsocks-libev](https://github.com/shadowsocks/shadowsocks-libev), [shadowsocks(python)](https://github.com/shadowsocks/shadowsocks), etc., but not [go-shadowsocks2](https://github.com/shadowsocks/go-shadowsocks2) which only supports command line URI argument. Set client program in settings.

## Build

Build requires CMake and Qt 5/6.

```sh
mkdir build; cd build
cmake ..
make
make install
```

Also, available in the [AUR](https://aur.archlinux.org/packages/ss-face/).
