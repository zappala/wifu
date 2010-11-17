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
	conf.env['LIBPATH'] += ['../lib']
	conf.env['LIB_PTHREAD'] = ['pthread']
	conf.env['STATICLIB'] += ['UnitTest++']
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

def post(ctx):
	import os
	val = 0
	val = os.system("bin/wifu-end-test")
	val = (val >> 8)

	# val now contains the number of tests which failed
	if val > 0:
		error = "%d error(s) encountered during tests." %(val)
		#raise Exception(error)

def build(bld):

	src_files = bld.glob('src/*.cc')
	test_files = bld.glob('test/*.cc')

	all_files = src_files
	all_files += test_files
	all_files.remove("src/main.cc")

	bld(features='cxx cprogram',
        source=bld.glob('src/*.cc'),
        includes="headers",
        uselib='PTHREAD RT',
        target='wifu-end')

	bld(features='cxx cprogram',
        source=all_files,
        includes='headers test/headers',
        uselib='PTHREAD RT',
		target='wifu-end-test')

	bld.add_post_fun(post)
