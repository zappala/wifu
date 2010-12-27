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

	# udp blaster
	udp_files = bld.glob('preliminary/Timer.cc')
	udp_files += bld.glob('preliminary/UDPBlaster.cc')
	udp_files += bld.glob('src/UDPSocket.cc')
	udp_files += bld.glob('src/Semaphore.cc')

	udp_blaster = bld(features='cxx cprogram',
        source=udp_files,
        includes='headers',
        uselib='PTHREAD RT',
		target='udp-blaster')

	# udp sink
	udp_sink_files = bld.glob('preliminary/Timer.cc')
	udp_sink_files += bld.glob('preliminary/UDPSink.cc')
	udp_sink_files += bld.glob('src/UDPSocket.cc')
	udp_sink_files += bld.glob('src/Semaphore.cc')

	udp_sink = bld(features='cxx cprogram',
        source=udp_sink_files,
        includes='headers',
        uselib='PTHREAD RT',
		target='udp-sink')

	# SimpleTCP
	simple_tcp_files = bld.glob('preliminary/SimpleTCPServer.cc')
	simple_tcp_files += bld.glob('preliminary/SimpleTCP.cc')
	simple_tcp_files += bld.glob('src/UDPSocket.cc')
	simple_tcp_files += bld.glob('src/Semaphore.cc')
	simple_tcp_files += bld.glob('src/Identifiable.cc')
	simple_tcp_files += bld.glob('src/IDGenerator.cc')

	udp_sink = bld(features='cxx cprogram',
        source=simple_tcp_files,
        includes='headers',
        uselib='PTHREAD RT',
		target='simple-tcp-server')

	
#	simple_tcp_files = bld.glob('preliminary/SimpleTCPClient.cc')
#	simple_tcp_files += bld.glob('preliminary/SimpleTCP.cc')
#	simple_tcp_files += bld.glob('src/UDPSocket.cc')
#	simple_tcp_files += bld.glob('src/Semaphore.cc')
#	simple_tcp_files += bld.glob('src/Identifiable.cc')
#	simple_tcp_files += bld.glob('src/IDGenerator.cc')
#	udp_sink = bld(features='cxx cprogram',
#        source=simple_tcp_files,
#        includes='headers',
#        uselib='PTHREAD RT',
#		target='simple-tcp-client')



	# shared files
	src_files = bld.glob('src/*.cc')

	# shared library
	api_files = bld.glob('applib/*.cc')
	api_files += src_files

	lib = bld(features='cxx cstaticlib',
        source=api_files,
        includes="applib headers lib/gc/include",
		ccflags="-c -fPIC",
		export_incdirs="applib",
#		libpath = ['../lib/gc'],
#		staticlib = ['gccpp','gc','cord'],
        target='wifu-end-api')

	# exe
	test_files = bld.glob('test/*.cc')

	all_files = src_files
	all_files += test_files
	all_files.remove("src/main.cc")

	exe = bld(features='cxx cprogram',
        source=bld.glob('src/*.cc'),
        includes="headers lib/gc/include",
        uselib='PTHREAD RT',
		libpath = ['../lib/gc'],
		staticlib = ['gccpp','gc','cord'],
        target='wifu-end')

	# unit tests

	test = bld(features='cxx cprogram',
        source=all_files,
        includes='headers test/headers lib/gc/include',
        uselib='PTHREAD RT',
		libpath = ['../lib/gc'],
		staticlib = ['gccpp','gc','cord'],
		uselib_local='wifu-end-api',
		target='wifu-end-test')





	bld.add_post_fun(post)
