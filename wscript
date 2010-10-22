import Options

top = '.'
out = 'build'


def set_options(opt):
    # opt.tool_options('compiler_cxx')

    opt.add_option('--32',
                   help=('Compile 32-bit version.'),
                   action="store_true", default=False,
                   dest='bit_32')

    opt.add_option('--debug',
                   help=('Compile with debugging symbols.'),
                   action="store_true", default=False,
                   dest='debug')


def configure(conf):
    print('→ configuring the project')
    # conf.check_tool('compiler_cxx')
    conf.check_tool('gcc g++')
    conf.env['LIB_PTHREAD'] = ['pthread']
    conf.env['LIB_RT'] = ['rt']
    conf.env.PREFIX = ".."

    if Options.options.bit_32:
        print "Configuring for 32-bit compilation"
        conf.env['LIBPATH'] += ['../lib']
        conf.env['CXXFLAGS'] += [ '-m32']
        conf.env['LINKFLAGS'] += ['-m32']

    if Options.options.debug:
        print "Configuring with debugging symbols"
        conf.env['CXXFLAGS'] += ['-g']

def build(bld):
    bld.add_subdirs('src')
