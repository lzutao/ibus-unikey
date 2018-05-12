# IBus-Unikey IME

<p align="center">
  <img src="https://github.com/lzutao/ibus-unikey/raw/master/icons/ibus-unikey.png" width=128>
</p>

A future clone of [ibus-unikey][ibus-unikey] aims to fix for the present.

ibus-unikey is an [IBus](https://github.com/ibus/ibus) IME. It uses a modified version of Unikey-engine for progressing key event.

[![Build Status](https://travis-ci.com/lzutao/ibus-unikey.svg?branch=dev)](https://travis-ci.com/lzutao/ibus-unikey)

## Installing

### Ubuntu / Debian
```bash
sudo apt-get install ibus-unikey
```

After the installation completes, follow [those instructions](https://www.sitecuatui.com/ibus-unikey-go-tieng-viet-ubuntu/) to activate ibus-unikey.

## Compiling on GNU/Linux

### Dependencies
ibus-unikey currently needs following libraries and binaries:
- GNU gcc, g++ compiler or Clang
- GNU autotools with autoconf >= 2.69, autopoint >= 0.19.7, libtool >= 2.4
- IBus development library >= 1.4.0
- Gtk development library with libgtk2.0-dev >= 2.24.0

On Ubuntu 16.04 you can easily install them like this:
```bash
sudo apt-get update
sudo apt-get install build-essential autoconf autopoint autotools-dev
sudo apt-get install libibus-1.0-dev libx11-dev libgtk2.0-dev libgtk-3-dev
```

After installing required libraries and binaries, you are ready to build and install this software from source.

### Build and install

#### From a release tarball

Download a [tarball][release] and extract it, change current directory to extracted directory and use:
```bash
./configure && make
sudo make install
```

#### Latest version from Github

[Clone][how-to-clone] this repository and change the directory to the cloned folder and issue the commands:
```bash
mkdir build && cd build
../autogen.sh --with-gtk-version=2 # default gtk version is 2, you can omit this option
make -j4          # to build with 4 threads
sudo make install # to install
```

**Note**: Above commands will install `ibus-unikey` in default localtion `/usr/local/`. If you want to change that, use `--prefix=<dir_to_install>` option. Example: `../autogen.sh --prefix=/home/user/local`

## Acknowledgements
- Author: Lê Quốc Tuấn \<mr.lequoctuan@gmail.com\>

## License
Licensed under [GNU GPLv3](COPYING).

## TODO

Read more in [TODO](https://github.com/lzutao/ibus-unikey/projects) at GitHub's Projects.

## Documentations

- An [awesome overview](https://huytd.github.io/posts/go-tieng-viet-linux.html) of @huytd
- [Overview of Imput Method for CJK (and Vietnamese)](https://blogs.gnome.org/happyaron/2011/01/15/linux-input-method-brief-summary/)
- [BongNV ibus project](https://github.com/bongnv/ibus-unikey)
- [BongNV own introduction](http://bongnv.github.io/2016/03/another-ibus-unikey.html)
- And few alternative engines with unikey-based:
  + https://devhub.io/repos/wilonth-BoGoEngine
  + https://devhub.io/repos/haqduong-ibus-bogo
- Some intial step with [vala](https://github.com/lewtds/ibus-bogo-vala/blob/master/src/main.vala)
- An hard to find custom ibus input-method-engine [guide](http://www.studymongolian.net/technical/how-to-create-linux-input-method-editor/)

[release]: https://github.com/lzutao/ibus-unikey/releases
[how-to-clone]: https://help.github.com/articles/cloning-a-repository/
[ibus-unikey]: https://github.com/vn-input/ibus-unikey
