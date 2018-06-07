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
