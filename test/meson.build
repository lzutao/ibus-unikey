## Producing a coverage report
## http://mesonbuild.com/howtox.html#producing-a-coverage-report

check_dep = dependency('check')

test_inputproc = executable(
    'inputproc',
    'test_inputproc.cc',
    link_with: [libunikey],
    include_directories: include_directories('../ukengine'),
    dependencies: check_dep,
)

test('test_inputproc', test_inputproc,
    is_parallel: false)

test_ukengine = executable(
    'ukengine',
    'test_ukengine.cc',
    link_with: [libunikey],
    include_directories: include_directories('../ukengine'),
    dependencies: check_dep,
)

test('test_ukengine', test_ukengine,
    is_parallel: false)
