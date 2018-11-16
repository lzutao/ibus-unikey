# read more in http://mesonbuild.com/Quick-guide.html#using-meson-as-a-distro-packager
project('ibus-unikey', ['cpp'],
    default_options: [
      'cpp_std=c++11',
      'buildtype=debug'
    ],
    version: '0.6.2',
    license: 'GPLv3',
    meson_version: '>= 0.46.0'
)

pkg_name = meson.project_name()
pkg_version = meson.project_version()
pkg_string = '@0@ @1@'.format(pkg_name, pkg_version)
pkg_bugreport = 'https://github.com/vn-input/ibus-unikey'

pkg_prefix = get_option('prefix')
pkg_bindir = join_paths(pkg_prefix, get_option('bindir'))
pkg_datadir = join_paths(pkg_prefix, get_option('datadir'))
pkg_includedir = join_paths(pkg_prefix, get_option('includedir'))
pkg_libdir = join_paths(pkg_prefix, get_option('libdir'))
pkg_libexecdir = join_paths(pkg_prefix, get_option('libexecdir'))
pkg_localedir = join_paths(pkg_prefix, get_option('localedir'))
pkg_mandir = join_paths(pkg_prefix, get_option('mandir'))

pkg_pkgdatadir = join_paths(pkg_datadir, pkg_name)
pkg_pkglibdir = join_paths(pkg_libdir, pkg_name)

pkg_rpath = pkg_pkglibdir

#pkg_mountdir = join_paths(pkg_pkgdatadir, 'mounts')
#pkg_schema_dir = join_paths(pkg_datadir, 'glib-2.0', 'schemas')

#pkg_namespace = 'org.gtk.vfs'

pkg_debug = get_option('buildtype').contains('debug')

cxx = meson.get_compiler('cpp')

## Auto headers ##
config_h = configuration_data()

# defines
set_defines = [
  # package
  ['PACKAGE', pkg_name],
  ['PACKAGE_BUGREPORT', pkg_bugreport],
  ['PACKAGE_NAME', pkg_name],
  ['PACKAGE_STRING', pkg_string],
  ['PACKAGE_VERSION', pkg_version],
  ['VERSION', pkg_version],
  # i18n
  ['GETTEXT_PACKAGE', pkg_name]
]

foreach define: set_defines
  config_h.set_quoted(define[0], define[1])
endforeach

# Globally define_GNU_SOURCE and therefore enable the GNU extensions
#config_h.set('_GNU_SOURCE', true)

# headers
check_headers = [
  ['HAVE_FCNTL_H', 'fcntl.h'],
  ['HAVE_LIBINTL_H', 'libintl.h'],
  ['HAVE_LOCALE_H', 'locale.h'],
  ['HAVE_MEMORY_H', 'memory.h'],
  ['HAVE_STDDEF_H', 'stddef.h'],
  ['HAVE_STDINT_H', 'stdint.h'],
  ['HAVE_STDLIB_H', 'stdlib.h'],
  ['HAVE_STRING_H', 'string.h'],
]

foreach header: check_headers
  has_header = cxx.has_header(header[1])
  config_h.set10(header[0], has_header)
endforeach

# functions
check_functions = [
  ['HAVE_MEMMOVE', 'memmove'],
  ['HAVE_MEMSET', 'memset'],
  ['HAVE_SETLOCALE', 'setlocale'],
  ['HAVE_STRCASECMP', 'strcasecmp'],
  ['HAVE_STRCHR', 'strchr'],
  ['HAVE_STRRCHR', 'strrchr'],
  ['HAVE_STRSTR', 'strstr'],
]

foreach func: check_functions
  config_h.set(func[0], cxx.has_function(func[1]))
endforeach

if cxx.get_id() == 'gcc' or cxx.get_id() == 'clang'
  common_flags = ['-DHAVE_CONFIG_H']
  if pkg_debug
      common_flags += [ '-DDEBUG' ] # just for debugging
  endif

  extra_args = [
    #'-pedantic-errors', # disable language extensions
    '-Werror',
  ]

  common_flags += cxx.get_supported_arguments(extra_args)
  add_project_arguments(common_flags, language : 'cpp')
endif

## Dependencies ##
if get_option('gtk3')
    gtk_dep = dependency('gtk+-3.0')
else
    gtk_dep = dependency('gtk+-2.0', version : '>= 2.24')
endif

ibus_dep = dependency('ibus-1.0', version : '>= 1.5.4')

i18n = import('i18n')

## Include ##
incdir = include_directories('.', 'ukengine', 'src')

## OUTPUT ##
configure_file(output : 'config.h',
               configuration : config_h)

output = '''
Build options:
    Version         @0@
    Install prefix  @1@
    GTK Version     @2@
'''.format(pkg_version, pkg_prefix, gtk_dep.version())

message(output)

## Subdirs ##
subdir('ukengine')
#subdir('src')
#subdir('setup')
#subdir('icons')
#subdir('po')
subdir('test')         # test is the last one to include
