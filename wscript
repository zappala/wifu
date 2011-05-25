import Options
import subprocess
import os
import os.path
import re

top = '.'
out = 'build'

def set_options(opt):
	opt.tool_options('compiler_cxx')

	opt.add_option('--32',
					help=('Compile 32-bit version.'),
					action="store_true", default=False,
					dest='bit_32')

	opt.add_option('--debug',
					help=('Compile with debugging symbols.'),
					action="store_true", default=True,
					dest='debug')


def configure(conf):
	print('→ configuring the project')
	conf.check_tool('compiler_cxx')
	conf.check_tool('gcc g++')
	if Options.options.bit_32:
		conf.env['LIBPATH'] += ['../lib/gtest/gtest_32']
		conf.env['LIBPATH'] += ['../lib/pantheios/pantheios_32']
	else:
		conf.env['LIBPATH'] += ['../lib/gtest/gtest_64']
		conf.env['LIBPATH'] += ['../lib/pantheios/pantheios_64']
	conf.env['LIB_PTHREAD'] = ['pthread']
	
	conf.env['STATICLIB'] += ['gtest']
	conf.env['STATICLIB'] += ['pantheios.1.core.gcc44']
	
	#Use the top two for logging to a file, the bottom two for logging to console 
#	conf.env['STATICLIB'] += ['pantheios.1.be.file.gcc44']
#	conf.env['STATICLIB'] += ['pantheios.1.bec.file.gcc44']
	conf.env['STATICLIB'] += ['pantheios.1.be.fprintf.gcc44']
	conf.env['STATICLIB'] += ['pantheios.1.bec.fprintf.gcc44']

	#Always use these
	conf.env['STATICLIB'] += ['pantheios.1.fe.all.gcc44']
	conf.env['STATICLIB'] += ['pantheios.1.util.gcc44']
	
	conf.env['LIB_RT'] = ['rt']
	
	#gotta turn CS off for 32-bit
	if Options.options.bit_32:
		conf.env['CXXFLAGS'] += ['--cs-off']
		conf.env['LINKFLAGS'] += ['--cs-off']
	else:	
		conf.env['CXXFLAGS'] += ['--cs-include-path=../headers']
		conf.env['LINKFLAGS'] += ['--cs-include-path=../headers']
	
		conf.env['CXXFLAGS'] += ['--cs-exclude-path=../test/end']
		conf.env['LINKFLAGS'] += ['--cs-exclude-path=../test/end']
	
	conf.env.PREFIX = ".."

	if Options.options.bit_32:
		print "Configuring for 32-bit compilation"
		conf.env['LIBPATH'] += ['../lib']
		conf.env['CXXFLAGS'] += [ '-m32']
		conf.env['LINKFLAGS'] += ['-m32']

	if Options.options.debug:
		print "Configuring with debugging symbols"
		conf.env['CXXFLAGS'] += ['-g']
		conf.env['CXXFLAGS'] += ['-O0']

def post(ctx):
	import os
	val = 0
	count = 1
	for i in range(0, count):
		val = os.system("bin/wifu-end-test")
	
	val = (val >> 8)

	# val now contains the number of tests which failed
	if val > 0:
		error = "%d error(s) encountered during wifu-end tests." %(val)
		#raise Exception(error)

	val = os.system("bin/wifu-frontend-test")
	val = (val >> 8)

	# val now contains the number of tests which failed
	if val > 0:
		error = "%d error(s) encountered during wifu-frontend tests." %(val)
		#raise Exception(error)

def build_blaster(bld):
    # udp blaster
	udp_files = bld.glob('preliminary/Timer.cc')
	udp_files += bld.glob('preliminary/UDPBlaster.cc')
	udp_files += bld.glob('src/UDPSocket.cc')
	udp_files += bld.glob('src/Semaphore.cc')

	if Options.options.bit_32:
		udp_blaster = bld(features='cxx cprogram',
		source=udp_files,
		includes='headers lib/gc/include',
		libpath = ['../lib/gc/gc_32'],
		staticlib = ['gccpp','gc','cord'],
		uselib='PTHREAD RT',
			target='udp-blaster')
	else:
		udp_blaster = bld(features='cxx cprogram',
		source=udp_files,
		includes='headers lib/gc/include',
		libpath = ['../lib/gc/gc_64'],
		staticlib = ['gccpp','gc','cord'],
		uselib='PTHREAD RT',
			target='udp-blaster')

def build_sink(bld):
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


def build_staticlib(bld):
    # shared files
	src_files = bld.glob('src/*.cc')
	src_files.remove('src/main.cc')
	src_files += bld.glob('src/exceptions/*.cc')

	# shared library
	api_files = bld.glob('applib/*.cc')
	api_files += src_files

	if Options.options.bit_32:
		lib = bld(features='cxx cstaticlib',
		source=api_files,
		includes='applib headers headers/exceptions lib/gc/include',
			ccflags="-c -fPIC",
			export_incdirs='applib lib/gc/include',
			uselib='PTHREAD RT',
			libpath = ['../lib/gc/gc_32'],
			staticlib = ['gccpp','gc','cord'],
		target='wifu-end-api')
	else:
		lib = bld(features='cxx cstaticlib',
		source=api_files,
		includes='applib headers headers/exceptions lib/gc/include',
			ccflags="-c -fPIC",
			export_incdirs='applib lib/gc/include',
			uselib='PTHREAD RT',
			libpath = ['../lib/gc/gc_64'],
			staticlib = ['gccpp','gc','cord'],
		target='wifu-end-api')

def build_wifu(bld):
	src_files = bld.glob('src/*.cc')
	src_files += bld.glob('src/contexts/*.cc')
	src_files += bld.glob('src/events/*.cc')
	src_files += bld.glob('src/states/*.cc')
	src_files += bld.glob('src/states/tcp-ap/*.cc')
	src_files += bld.glob('src/observer/*.cc')
	src_files += bld.glob('src/packet/*.cc')
	src_files += bld.glob('src/exceptions/*.cc')
	src_files += bld.glob('src/visitors/*.cc')

	if Options.options.bit_32:
		exe = bld(features='cxx cprogram',
		source=src_files,
		includes='headers lib/gc/include lib/pantheios/include lib/stlsoft/include headers/contexts headers/events headers/states headers/observer headers/packet headers/exceptions headers/visitors headers/states/tcp-ap',
		uselib='PTHREAD RT',
			libpath = ['../lib/gc/gc_32'],
			staticlib = ['gccpp','gc','cord'],
		target='wifu-end')
	else:
		exe = bld(features='cxx cprogram',
		source=src_files,
		includes='headers lib/gc/include lib/pantheios/include lib/stlsoft/include headers/contexts headers/events headers/states headers/observer headers/packet headers/exceptions headers/visitors headers/states/tcp-ap',
		uselib='PTHREAD RT',
			libpath = ['../lib/gc/gc_64'],
			staticlib = ['gccpp','gc','cord'],
		target='wifu-end')

def build_wifu_end_test(bld):
	test_files = bld.glob('test/end/*.cpp')
	
	project_files = bld.glob('src/*.cc')
	project_files.remove('src/main.cc')
	project_files += bld.glob('src/observer/*.cc')
	project_files += bld.glob('src/contexts/*.cc')
	project_files += bld.glob('src/events/*.cc')
	project_files += bld.glob('src/states/*.cc')
	project_files += bld.glob('src/states/tcp-ap/*.cc')
	project_files += bld.glob('src/packet/*.cc')
	project_files += bld.glob('src/exceptions/*.cc')
	project_files += bld.glob('src/visitors/*.cc')
	
	filesToUse = test_files + project_files

	if Options.options.bit_32:
		test_end = bld(features='cxx cprogram',
		source=filesToUse,
		includes='preliminary headers lib/gc/include lib/pantheios/include lib/stlsoft/include lib/gtest/include headers/contexts headers/events headers/states headers/observer headers/packet headers/exceptions headers/visitors test/end/headers headers/states/tcp-ap',
		uselib='PTHREAD RT',
			libpath = '../lib/gc/gc_32',
			staticlib = ['gccpp','gc','cord'],
			target='wifu-end-test')
	else:
		test_end = bld(features='cxx cprogram',
		source=filesToUse,
		includes='preliminary headers lib/gc/include lib/pantheios/include lib/stlsoft/include lib/gtest/include headers/contexts headers/events headers/states headers/observer headers/packet headers/exceptions headers/visitors test/end/headers headers/states/tcp-ap',
		uselib='PTHREAD RT',
			libpath = '../lib/gc/gc_64',
			staticlib = ['gccpp','gc','cord'],
			target='wifu-end-test')
	
def build_wifu_frontend_test(bld):
	test_frontend_files = bld.glob('test/frontend/*.cpp')
	test_frontend_files += bld.glob('preliminary/Timer.cc')
	test_frontend_files += bld.glob('src/exceptions/*.cc')
	test_frontend_files += bld.glob('src/packet/*.cc')
	test_frontend_files += bld.glob('src/visitors/*.cc')
	test_frontend_files += bld.glob('src/observer/*.cc')

	if Options.options.bit_32:
		test_frontend = bld(features='cxx cprogram',
			source=test_frontend_files,
			includes='preliminary headers headers/exceptions headers/packet headers/visitors headers/observer lib/gc/include lib/gtest/include',
			libpath = '../lib/gc/gc_32',
			staticlib = ['gccpp', 'gc', 'cord'],
			uselib_local='wifu-end-api',
			target='wifu-frontend-test')
	else:
		test_frontend = bld(features='cxx cprogram',
			source=test_frontend_files,
			includes='preliminary headers headers/exceptions headers/packet headers/visitors headers/observer lib/gc/include lib/gtest/include',
			libpath = '../lib/gc/gc_64',
			staticlib = ['gccpp', 'gc', 'cord'],
			uselib_local='wifu-end-api',
			target='wifu-frontend-test')

def build_simple_tcp_sender(bld):
	files = bld.glob('preliminary/SimpleTCPSender.cc')
	files += bld.glob('applib/*.cc')
	files += bld.glob('src/AddressPort.cc')

	if Options.options.bit_32:
		sender = bld(features='cxx cprogram',
			source=files,
			includes='preliminary headers headers/exceptions headers/packet headers/visitors headers/observer lib/gc/include lib/gtest/include',
			libpath = '../lib/gc/gc_32',
			staticlib = ['gccpp', 'gc', 'cord'],
			uselib_local='wifu-end-api',
			target='simple-tcp-sender')
	else:
		sender = bld(features='cxx cprogram',
			source=files,
			includes='preliminary headers headers/exceptions headers/packet headers/visitors headers/observer lib/gc/include lib/gtest/include',
			libpath = '../lib/gc/gc_64',
			staticlib = ['gccpp', 'gc', 'cord'],
			uselib_local='wifu-end-api',
			target='simple-tcp-sender')

def build_simple_tcp_receiver(bld):
	files = bld.glob('preliminary/SimpleTCPReceiver.cc')
	files += bld.glob('applib/*.cc')
	files += bld.glob('src/AddressPort.cc')

	if Options.options.bit_32:
		receiver = bld(features='cxx cprogram',
			source=files,
			includes='preliminary headers headers/exceptions headers/packet headers/visitors headers/observer lib/gc/include lib/gtest/include',
			libpath = '../lib/gc/gc_32',
			staticlib = ['gccpp', 'gc', 'cord'],
			uselib_local='wifu-end-api',
			target='simple-tcp-receiver')
	else:
		receiver = bld(features='cxx cprogram',
			source=files,
			includes='preliminary headers headers/exceptions headers/packet headers/visitors headers/observer lib/gc/include lib/gtest/include',
			libpath = '../lib/gc/gc_64',
			staticlib = ['gccpp', 'gc', 'cord'],
			uselib_local='wifu-end-api',
			target='simple-tcp-receiver')

def build(bld):
#	build_blaster(bld)
#	build_sink(bld)

	build_staticlib(bld)
	build_wifu(bld)

	if not Options.options.bit_32:
		build_wifu_end_test(bld)
		build_wifu_frontend_test(bld)
	
	build_simple_tcp_sender(bld)
	build_simple_tcp_receiver(bld)

#	bld.add_post_fun(post)

def get_files(dir, regex):
    list = []
    for filename in os.listdir (dir):
        # Ignore subfolders
        path = os.path.join (dir, filename)
        if os.path.isdir (path):
            continue

        # Ignore mismatched regex
        if not re.match(regex, filename):
            continue

        list.append(path)

    list.sort()
    return list
