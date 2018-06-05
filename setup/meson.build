ibus_setup_unikey_sources = [
  'setup.cpp',
  'dlg_main_setup.cpp',
  'dlg_macro_table.cpp',
]

ibus_setup_unikey = executable(
  'ibus_setup_unikey',
  ibus_setup_unikey_sources,
  cpp_args : [
    '-DPKGDATADIR="@0@"'.format(pkg_datadir),
    '-DLOCALEDIR="@0@"'.format(pkg_localedir),
    '-DUI_DATA_DIR="@0@/ui"'.format(pkg_datadir),
  ],
  link_with: [libunikey],
  include_directories: incdir,
  objects: ibus_engine_unikey.extract_objects('utils.cpp'),
  dependencies: [ibus_dep, gtk_dep],
  install: true,
  install_dir : pkg_libexecdir,
)

install_data(
  ['setup-main.ui', 'setup-macro.ui'],
  install_dir: '@0@/ui'.format(pkg_datadir),
)
