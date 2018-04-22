# IBus-Unikey IME
A future clone of [ibus-unikey][ibus-unikey] aim to fix for the present.

### Description
ibus-unikey is an [IBus](https://github.com/ibus/ibus) IME. It use Unikey-engine (a modified version) for progress key event.

### Installation

**Dependencies** (On Debian 9):
```bash
sudo apt-get update
sudo apt-get install ibus git build-essential autoconf libtool autopoint libtool intltool libibus-1.0-dev libx11-dev libgtk2.0-dev
```

**How to build**:
```bash
git clone https://github.com/lzutao/ibus-unikey.git
ln -s README.md README
autoreconf --install
intltoolize
autoreconf
./configure
make
```

### TODO

Read more in [TODO.md](TODO.md)

### Documentations

- An [awesome overview](https://huytd.github.io/posts/go-tieng-viet-linux.html) of @huytd
- [Overview of Imput Method for CJK (and Vietnamese)](https://blogs.gnome.org/happyaron/2011/01/15/linux-input-method-brief-summary/)
- [BongNV ibus project](https://github.com/bongnv/ibus-unikey)
- [BongNV own introduction](http://bongnv.github.io/2016/03/another-ibus-unikey.html)
- And a alternative engine to unikey-based:
  + https://devhub.io/repos/wilonth-BoGoEngine
  + https://devhub.io/repos/haqduong-ibus-bogo
- Some intial step with [vala](https://github.com/lewtds/ibus-bogo-vala/blob/master/src/main.vala)
- An hard to find custom ibus input-method-engine [guide](http://www.studymongolian.net/technical/how-to-create-linux-input-method-editor/)


[ibus-unikey]: https://github.com/vn-input/ibus-unikey
