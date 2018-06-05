ibus_engine_unikey_sources = [
  'main.cpp',
  'engine.cpp',
  'utils.cpp',
]

ibus_engine_unikey = executable(
  'ibus-engine-unikey',
  ibus_engine_unikey_sources,
  cpp_args : [
    '-DPKGDATADIR="@0@"'.format(pkg_datadir),
    '-DLOCALEDIR="@0@"'.format(pkg_localedir),
    '-DLIBEXECDIR="@0@"'.format(pkg_libexecdir),
  ],
  link_with: [libunikey],
  include_directories: incdir,
  dependencies: [ibus_dep],
  install: true,
  install_dir : pkg_libexecdir,
)

src_unikey_xml = configuration_data()
set_directories = [
  ['libexecdir', pkg_libexecdir],
  ['pkg_version', pkg_version],
  ['pkg_bugreport', pkg_bugreport],
  ['pkgdatadir', pkg_datadir],
]
foreach define: set_directories
  src_unikey_xml.set(define[0], define[1])
endforeach

configure_file(input : 'unikey.xml.in',
               output : 'unikey.xml',
               configuration : src_unikey_xml,
               install_dir: '@0@/ibus/component'.format(pkg_datadir),)
