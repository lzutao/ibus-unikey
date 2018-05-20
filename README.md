# IBus-Unikey IME

<p align="center">
  <img src="https://github.com/lzutao/ibus-unikey/raw/master/icons/ibus-unikey.png" width=128>
</p>

A future clone of [ibus-unikey][ibus-unikey] aims to fix for the present.

ibus-unikey is an [IBus](https://github.com/ibus/ibus) IME. It uses a modified version of Unikey-engine for progressing key event.

[![Build Status](https://travis-ci.com/lzutao/ibus-unikey.svg?branch=master)](https://travis-ci.com/lzutao/ibus-unikey)

## Installing

### Ubuntu / Debian
```bash
sudo apt-get install ibus-unikey
```

After the installation completes, follow [those instructions][after_installing] to activate ibus-unikey.

## Compiling on GNU/Linux

### Dependencies
ibus-unikey currently needs following libraries and binaries:
- GNU gcc, g++ compiler or Clang
- GNU autotools with autoconf >= 2.69, autopoint >= 0.19.7, libtool >= 2.4
- IBus development library >= 1.5.4
- Gtk development library with libgtk2.0-dev >= 2.24.0

On Ubuntu 16.04 you can easily install them like this:
```bash
sudo apt-get update
sudo apt-get install build-essential autoconf autopoint autotools-dev
sudo apt-get install libibus-1.0-dev libgtk2.0-dev libgtk-3-dev
```

After installing required libraries and binaries, you are ready to build and install this software from source.

### Build and install

#### From a release tarball

Download a [tarball][release] and extract it, change current directory to extracted directory and use:
```bash
mkdir build && cd build
./configure --prefix=/usr --libexec=/usr/lib/ibus
make -j4
sudo make install
```

#### Latest version from GitHub

[Clone][how-to-clone] this repository and change the directory to the cloned folder and issue the commands:
```bash
mkdir build && cd build
../autogen.sh --prefix=/usr --libexec=/usr/lib/ibus
make -j4          # to build with 4 threads
sudo make install # to install
```

## Debian package build for maintainer

**Note**: Only for upstream version from GitHub

```bash
mkdir build && cd build
../autogen.sh --prefix=/usr --libexec=/usr/lib/ibus
make dist
```

Change the name of release tarball from `ibus-unikey-VERSION.tar.gz` to `ibus-unikey_VERSION.orig.tar.gz`, issue these commands:
```bash
tar xf ibus-unikey_VERSION.orig.tar.gz
cd ibus-unikey
cp -a ../../debian .
debuild -us -uc # build both source and binary package
```

To build only the binary package without signing the `.changes` file (or the non-existent `.dsc` file):
```bash
debuild -i -us -uc -b
```

Change the `-b` to `-S` to build only a source package.

[Read more about debuild](http://manpages.ubuntu.com/manpages/xenial/en/man1/debuild.1.html)

## Acknowledgements
- Copyright
```
Files: ukengine/*
Copyright: 2000-2005 Pham Kim Long <unikey@gmail.com>
License: GPL-3.0+

2009 - 2012 Lê Quốc Tuấn <mr.lequoctuan AT gmail DOT com>
       2016 Bong Nguyen <vanbong AT gmail DOT com>
       2018 Lzu Tao <1stpangu AT gmail DOT com>
```

## License
Licensed under [GNU GPLv3](COPYING).

## TODO

Read more in [TODO](https://github.com/lzutao/ibus-unikey/projects) at GitHub's Projects.

## Useful links

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
[after_installing]: https://github.com/Ubuntu-VN/wiki/wiki/G%C3%B5-ti%E1%BA%BFng-Vi%E1%BB%87t
