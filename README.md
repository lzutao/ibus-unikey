# IBus-Unikey IME
A future clone of [ibus-unikey][ibus-unikey] aims to fix for the present.

[![Build Status](https://travis-ci.com/lzutao/ibus-unikey.svg?branch=dev)](https://travis-ci.com/lzutao/ibus-unikey)

### Description
ibus-unikey is an [IBus](https://github.com/ibus/ibus) IME. It use Unikey-engine (a modified version) for progress key event.

### Installation

#### From source
**Dependencies** (On Debian 9):
```bash
sudo apt-get update
sudo apt-get install ibus build-essential libtool autoconf autopoint autotools-dev 
sudo apt-get install libibus-1.0-dev libx11-dev libgtk2.0-dev libgtk-3-dev
```

**How to build**:
```bash
git clone https://github.com/lzutao/ibus-unikey.git
cd ibus-unikey
mkdir build && cd build
../autogen.sh --with-gtk-version=2 # default gtk version is 2, you can omit this option
make # to build
sudo make install # to install
```

**Note**: Above commands will install `ibus-unikey` in default localtion `/usr/local/`. 
If you want to change that, use `--prefix=<dir_to_install>` option in `../autogen.sh` like `../autogen.sh --prefix=/home/user/local`

### TODO

Read more in [TODO.md](TODO.md)

### Documentations

- An [awesome overview](https://huytd.github.io/posts/go-tieng-viet-linux.html) of @huytd
- [Overview of Imput Method for CJK (and Vietnamese)](https://blogs.gnome.org/happyaron/2011/01/15/linux-input-method-brief-summary/)
- [BongNV ibus project](https://github.com/bongnv/ibus-unikey)
- [BongNV own introduction](http://bongnv.github.io/2016/03/another-ibus-unikey.html)
- And few alternative engines with unikey-based:
  + https://devhub.io/repos/wilonth-BoGoEngine
  + https://devhub.io/repos/haqduong-ibus-bogo
- Some intial step with [vala](https://github.com/lewtds/ibus-bogo-vala/blob/master/src/main.vala)
- An hard to find custom ibus input-method-engine [guide](http://www.studymongolian.net/technical/how-to-create-linux-input-method-editor/)


[ibus-unikey]: https://github.com/vn-input/ibus-unikey
