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
					action="store_true", default=False,
					dest='debug')

	opt.add_option('--prof',
					help=('Compile with profiler symbols.'),
					action="store_true", default=False,
					dest='prof')

	opt.add_option('--dist',
					help=('Compile for distribution.'),
					action="store_true", default=False,
					dest='dist')

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
	conf.env['STATICLIB'] += ['pantheios.1.be.file.gcc44']
	conf.env['STATICLIB'] += ['pantheios.1.bec.file.gcc44']
#	conf.env['STATICLIB'] += ['pantheios.1.be.fprintf.gcc44']
#	conf.env['STATICLIB'] += ['pantheios.1.bec.fprintf.gcc44']

	#Always use these
	conf.env['STATICLIB'] += ['pantheios.1.fe.all.gcc44']
	conf.env['STATICLIB'] += ['pantheios.1.util.gcc44']

	conf.env['CPPPATH'] += ['../lib/pantheios/include']
	conf.env['CPPPATH'] += ['../lib/stlsoft/include']
	conf.env['CPPPATH'] += ['../lib/gc/include']
	
	conf.env['LIB_RT'] = ['rt']
	
	#gotta turn CS off for 32-bit
	if Options.options.bit_32:
		conf.env['CXXFLAGS'] += ['--cs-off']
		conf.env['LINKFLAGS'] += ['--cs-off']
	else:	
		conf.env['CXXFLAGS'] += ['--cs-include-path=../headers']
		conf.env['LINKFLAGS'] += ['--cs-include-path=../headers']
	
		conf.env['CXXFLAGS'] += ['--cs-exclude-path=../test/unit-tests']
		conf.env['LINKFLAGS'] += ['--cs-exclude-path=../test/unit-tests']
	
	conf.env.PREFIX = ".."

	if Options.options.bit_32:
		print "Configuring for 32-bit compilation"
		conf.env['LIBPATH'] += ['../lib']
		conf.env['CXXFLAGS'] += [ '-m32']
		conf.env['LINKFLAGS'] += ['-m32']
		# we need to do the distribution flags as well for 32-bit
		Options.options.dist = True

	if Options.options.debug:
		print "Configuring with debugging symbols"
		conf.env['CXXFLAGS'] += ['-g']
		conf.env['CXXFLAGS'] += ['-O0']

	if Options.options.prof:
		print "Configuring for profiler!"
		# for use with gprof
		conf.env['CXXFLAGS'] += ['-pg']
		conf.env['LINKFLAGS'] += ['-pg']
		# Optimizations: -O, -O0 (off), -O1, -O2, -O3 (the higher the number, the more optimization it does, and the harder to debug it)
		conf.env['CXXFLAGS'] += ['-O0']
		# turn off coverage scanner
		conf.env['CXXFLAGS'] += ['--cs-off']
		conf.env['LINKFLAGS'] += ['--cs-off']

	if Options.options.dist:
		print "Configuring for distribution!"
		# Optimizations: -O, -O0 (off), -O1, -O2, -O3 (the higher the number, the more optimization it does, and the harder to debug it)
		conf.env['CXXFLAGS'] += ['-O3']
		# turn off coverage scanner
		conf.env['CXXFLAGS'] += ['--cs-off']
		conf.env['LINKFLAGS'] += ['--cs-off']
		# turn off asserts
		conf.env['CXXFLAGS'] += ['-DNDEBUG']


def post(ctx):
	import os
	val = 0
	count = 1
	for i in range(0, count):
		val = os.system("bin/wifu-transport-test")
	
	val = (val >> 8)

	# val now contains the number of tests which failed
	if val > 0:
		error = "%d error(s) encountered during wifu-transport tests." %(val)
		#raise Exception(error)

	val = os.system("bin/wifu-integration-test")
	val = (val >> 8)

	# val now contains the number of tests which failed
	if val > 0:
		error = "%d error(s) encountered during wifu-integration tests." %(val)
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
		includes='headers',
		libpath = ['../lib/gc/gc_32'],
		staticlib = ['gccpp','gc','cord'],
		uselib='PTHREAD RT',
			target='udp-blaster')
	else:
		udp_blaster = bld(features='cxx cprogram',
		source=udp_files,
		includes='headers',
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
		includes='applib headers headers/exceptions',
			ccflags="-c -fPIC",
			export_incdirs='applib',
			uselib='PTHREAD RT',
			libpath = ['../lib/gc/gc_32'],
			staticlib = ['gccpp','gc','cord'],
		target='wifu-transport-api')
	else:
		lib = bld(features='cxx cstaticlib',
		source=api_files,
		includes='applib headers headers/exceptions',
			ccflags="-c -fPIC",
			export_incdirs='applib',
			uselib='PTHREAD RT',
			libpath = ['../lib/gc/gc_64'],
			staticlib = ['gccpp','gc','cord'],
		target='wifu-transport-api')

def build_wifu(bld):
	src_files = bld.glob('src/*.cc')
	src_files += bld.glob('src/contexts/*.cc')
	src_files += bld.glob('src/events/*.cc')
	src_files += bld.glob('src/states/*.cc')
	src_files += bld.glob('src/states/tcp-ap/*.cc')
	src_files += bld.glob('src/states/atp/*.cc')
	src_files += bld.glob('src/observer/*.cc')
	src_files += bld.glob('src/packet/*.cc')
	src_files += bld.glob('src/exceptions/*.cc')
	src_files += bld.glob('src/visitors/*.cc')
	src_files += bld.glob('src/protocol/*.cc')
	src_files += bld.glob('src/events/protocol_events/*.cc')
	src_files += bld.glob('src/events/framework_events/*.cc')

	if Options.options.bit_32:
		exe = bld(features='cxx cprogram',
		source=src_files,
		includes='headers  headers/contexts headers/events headers/states headers/observer headers/packet headers/exceptions headers/visitors headers/states/tcp-ap headers/states/atp headers/events/protocol_events headers/events/framework_events',
		uselib='PTHREAD RT',
			libpath = ['../lib/gc/gc_32'],
			staticlib = ['gccpp','gc','cord'],
		target='wifu-transport')
	else:
		exe = bld(features='cxx cprogram',
		source=src_files,
		includes='headers headers/contexts headers/events headers/states headers/observer headers/packet headers/exceptions headers/visitors headers/states/tcp-ap headers/states/atp headers/events/protocol_events headers/events/framework_events',
		uselib='PTHREAD RT',
			libpath = ['../lib/gc/gc_64'],
			staticlib = ['gccpp','gc','cord'],
		target='wifu-transport')

def build_wifu_transport_unit_test(bld):
	test_files = bld.glob('test/unit-tests/*.cpp')
	
	project_files = bld.glob('src/*.cc')
	project_files.remove('src/main.cc')
	project_files += bld.glob('src/observer/*.cc')
	project_files += bld.glob('src/contexts/*.cc')
	project_files += bld.glob('src/events/*.cc')
	project_files += bld.glob('src/states/*.cc')
	project_files += bld.glob('src/packet/*.cc')
	project_files += bld.glob('src/exceptions/*.cc')
	project_files += bld.glob('src/visitors/*.cc')
	project_files += bld.glob('src/protocol/*.cc')
	project_files += bld.glob('src/events/protocol_events/*.cc')
	project_files += bld.glob('src/events/framework_events/*.cc')

	filesToUse = test_files + project_files

	if Options.options.bit_32:
		unit_test = bld(features='cxx cprogram',
		source=filesToUse,
		includes='preliminary headers lib/pantheios/include lib/stlsoft/include lib/gtest/include headers/contexts headers/events headers/states headers/observer headers/packet headers/exceptions headers/visitors test/unit-tests/headers headers/states/tcp-ap headers/states/atp headers/events/framework_events headers/events/protocol_events',
		uselib='PTHREAD RT',
			libpath = '../lib/gc/gc_32',
			staticlib = ['gccpp','gc','cord'],
			target='wifu-transport-unit-test')
	else:
		unit_test = bld(features='cxx cprogram',
		source=filesToUse,
		includes='preliminary headers lib/pantheios/include lib/stlsoft/include lib/gtest/include headers/contexts headers/events headers/states headers/observer headers/packet headers/exceptions headers/visitors test/unit-tests/headers headers/states/tcp-ap headers/states/atp headers/events/framework_events headers/events/protocol_events',
		uselib='PTHREAD RT',
			libpath = '../lib/gc/gc_64',
			staticlib = ['gccpp','gc','cord'],
			target='wifu-transport-unit-test')
	
def build_wifu_transport_integration_test(bld):
	integration_test_files = bld.glob('test/integration-tests/*.cpp')
	integration_test_files += bld.glob('preliminary/Timer.cc')
	integration_test_files += bld.glob('src/exceptions/*.cc')
	integration_test_files += bld.glob('src/packet/*.cc')
	integration_test_files += bld.glob('src/visitors/*.cc')
	integration_test_files += bld.glob('src/observer/*.cc')
	integration_test_files += bld.glob('src/events/framework_events/*.cc')
	integration_test_files += bld.glob('src/events/Event.cc')

	if Options.options.bit_32:
		integration_test = bld(features='cxx cprogram',
			source=integration_test_files,
			includes='preliminary headers headers/exceptions headers/packet headers/visitors headers/observer lib/gtest/include',
			libpath = '../lib/gc/gc_32',
			staticlib = ['gccpp', 'gc', 'cord'],
			uselib_local='wifu-transport-api',
			target='wifu-integration-test')
	else:
		integration_test = bld(features='cxx cprogram',
			source=integration_test_files,
			includes='preliminary headers headers/exceptions headers/packet headers/visitors headers/observer lib/gtest/include',
			libpath = '../lib/gc/gc_64',
			staticlib = ['gccpp', 'gc', 'cord'],
			uselib_local='wifu-transport-api',
			target='wifu-integration-test')

def build_tcp_tahoe_sender(bld):
	files = bld.glob('preliminary/TCPTahoeSender.cc')
	files += bld.glob('preliminary/WiFuSocketAPI.cc')
	files += bld.glob('preliminary/KernelSocketAPI.cc')
	files += bld.glob('applib/*.cc')
	files += bld.glob('src/AddressPort.cc')

	if Options.options.bit_32:
		sender = bld(features='cxx cprogram',
			source=files,
			includes='preliminary preliminary/headers headers headers/exceptions headers/packet headers/visitors headers/observer lib/gtest/include',
			libpath = '../lib/gc/gc_32',
			staticlib = ['gccpp', 'gc', 'cord'],
			uselib_local='wifu-transport-api',
			target='tcp-tahoe-sender')
	else:
		sender = bld(features='cxx cprogram',
			source=files,
			includes='preliminary preliminary/headers headers headers/exceptions headers/packet headers/visitors headers/observer lib/gtest/include',
			libpath = '../lib/gc/gc_64',
			staticlib = ['gccpp', 'gc', 'cord'],
			uselib_local='wifu-transport-api',
			target='tcp-tahoe-sender')

def build_tcp_tahoe_receiver(bld):
	files = bld.glob('preliminary/TCPTahoeReceiver.cc')
	files += bld.glob('preliminary/WiFuSocketAPI.cc')
	files += bld.glob('preliminary/KernelSocketAPI.cc')
	files += bld.glob('applib/*.cc')
	files += bld.glob('src/AddressPort.cc')
	files += bld.glob('src/Utils.cc')

	if Options.options.bit_32:
		receiver = bld(features='cxx cprogram',
			source=files,
			includes='preliminary preliminary/headers headers headers/exceptions headers/packet headers/visitors headers/observer lib/gtest/include',
			libpath = '../lib/gc/gc_32',
			staticlib = ['gccpp', 'gc', 'cord'],
			uselib_local='wifu-transport-api',
			target='tcp-tahoe-receiver')
	else:
		receiver = bld(features='cxx cprogram',
			source=files,
			includes='preliminary preliminary/headers headers headers/exceptions headers/packet headers/visitors headers/observer lib/gtest/include',
			libpath = '../lib/gc/gc_64',
			staticlib = ['gccpp', 'gc', 'cord'],
			uselib_local='wifu-transport-api',
			target='tcp-tahoe-receiver')

def build_echo_client(bld):
	files = bld.glob('preliminary/EchoClient.cc')
	files += bld.glob('preliminary/WiFuSocketAPI.cc')
	files += bld.glob('preliminary/KernelSocketAPI.cc')
	files += bld.glob('applib/*.cc')
	files += bld.glob('src/AddressPort.cc')

	if Options.options.bit_32:
		sender = bld(features='cxx cprogram',
			source=files,
			includes='preliminary preliminary/headers headers headers/exceptions headers/packet headers/visitors headers/observer lib/gtest/include',
			libpath = '../lib/gc/gc_32',
			staticlib = ['gccpp', 'gc', 'cord'],
			uselib_local='wifu-transport-api',
			target='echo-client')
	else:
		sender = bld(features='cxx cprogram',
			source=files,
			includes='preliminary preliminary/headers headers headers/exceptions headers/packet headers/visitors headers/observer lib/gtest/include',
			libpath = '../lib/gc/gc_64',
			staticlib = ['gccpp', 'gc', 'cord'],
			uselib_local='wifu-transport-api',
			target='echo-client')

def build_echo_server(bld):
	files = bld.glob('preliminary/EchoServer.cc')
	files += bld.glob('preliminary/WiFuSocketAPI.cc')
	files += bld.glob('preliminary/KernelSocketAPI.cc')
	files += bld.glob('applib/*.cc')
	files += bld.glob('src/AddressPort.cc')
	files += bld.glob('src/Utils.cc')

	if Options.options.bit_32:
		receiver = bld(features='cxx cprogram',
			source=files,
			includes='preliminary preliminary/headers headers headers/exceptions headers/packet headers/visitors headers/observer lib/gtest/include',
			libpath = '../lib/gc/gc_32',
			staticlib = ['gccpp', 'gc', 'cord'],
			uselib_local='wifu-transport-api',
			target='echo-server')
	else:
		receiver = bld(features='cxx cprogram',
			source=files,
			includes='preliminary preliminary/headers headers headers/exceptions headers/packet headers/visitors headers/observer lib/gtest/include',
			libpath = '../lib/gc/gc_64',
			staticlib = ['gccpp', 'gc', 'cord'],
			uselib_local='wifu-transport-api',
			target='echo-server')

def build_raw_socket_sender(bld):
	files = bld.glob('preliminary/RawSocketBlasterSender.cc')
	files += bld.glob('src/packet/*.cc')
	files += bld.glob('src/visitors/*.cc')
	files += bld.glob('src/Socket.cc')
	files += bld.glob('src/observer/*.cc')
	files += bld.glob('src/exceptions/*.cc')
	files += bld.glob('src/SocketOptions.cc')
	files += bld.glob('src/Semaphore.cc')
	files += bld.glob('src/PortManagerFactory.cc')
	files += bld.glob('src/AddressPort.cc')
	files += bld.glob('src/SocketManager.cc')
	files += bld.glob('src/Math.cc')
	files += bld.glob('src/Timer.cc')
	files += bld.glob('src/OptionParser.cc')

	if Options.options.bit_32:
		receiver = bld(features='cxx cprogram',
			source=files,
			includes='preliminary preliminary/headers headers headers/exceptions headers/packet headers/visitors headers/observer',
			uselib='PTHREAD RT',
			libpath = '../lib/gc/gc_32',
			staticlib = ['gccpp', 'gc', 'cord'],
			target='raw-socket-blaster-sender')
	else:
		receiver = bld(features='cxx cprogram',
			source=files,
			includes='preliminary preliminary/headers headers headers/exceptions headers/packet headers/visitors headers/observer',
			uselib='PTHREAD RT',
			libpath = '../lib/gc/gc_64',
			staticlib = ['gccpp', 'gc', 'cord'],
			target='raw-socket-blaster-sender')

def build_raw_socket_receiver(bld):
	files = bld.glob('preliminary/RawSocketBlasterReceiver.cc')
	files += bld.glob('src/packet/*.cc')
	files += bld.glob('src/visitors/*.cc')
	files += bld.glob('src/Socket.cc')
	files += bld.glob('src/observer/*.cc')
	files += bld.glob('src/exceptions/*.cc')
	files += bld.glob('src/SocketOptions.cc')
	files += bld.glob('src/Semaphore.cc')
	files += bld.glob('src/PortManagerFactory.cc')
	files += bld.glob('src/AddressPort.cc')
	files += bld.glob('src/SocketManager.cc')
	files += bld.glob('src/Math.cc')
	files += bld.glob('src/Timer.cc')
	files += bld.glob('src/OptionParser.cc')

	if Options.options.bit_32:
		receiver = bld(features='cxx cprogram',
			source=files,
			includes='preliminary preliminary/headers headers headers/exceptions headers/packet headers/visitors headers/observer',
			uselib='PTHREAD RT',
			libpath = '../lib/gc/gc_32',
			staticlib = ['gccpp', 'gc', 'cord'],
			target='raw-socket-blaster-receiver')
	else:
		receiver = bld(features='cxx cprogram',
			source=files,
			includes='preliminary preliminary/headers headers headers/exceptions headers/packet headers/visitors headers/observer',
			uselib='PTHREAD RT',
			libpath = '../lib/gc/gc_64',
			staticlib = ['gccpp', 'gc', 'cord'],
			target='raw-socket-blaster-receiver')


def build_udp_sender(bld):
	files = bld.glob('preliminary/UDPSender.cc')
	files += bld.glob('preliminary/WiFuSocketAPI.cc')
	files += bld.glob('preliminary/KernelSocketAPI.cc')
	files += bld.glob('applib/*.cc')
	files += bld.glob('src/AddressPort.cc')
	files += bld.glob('src/OptionParser.cc')
	files += bld.glob('src/Timer.cc')

	if Options.options.bit_32:
		receiver = bld(features='cxx cprogram',
			source=files,
			includes='preliminary preliminary/headers headers headers/exceptions headers/packet headers/visitors headers/observer',
			uselib='PTHREAD RT',
			libpath = '../lib/gc/gc_32',
			staticlib = ['gccpp', 'gc', 'cord'],
			uselib_local='wifu-transport-api',
			target='udp-sender')
	else:
		receiver = bld(features='cxx cprogram',
			source=files,
			includes='preliminary preliminary/headers headers headers/exceptions headers/packet headers/visitors headers/observer',
			uselib='PTHREAD RT',
			libpath = '../lib/gc/gc_64',
			staticlib = ['gccpp', 'gc', 'cord'],
			uselib_local='wifu-transport-api',
			target='udp-sender')

def build_udp_receiver(bld):
	files = bld.glob('preliminary/UDPReceiver.cc')
	files += bld.glob('preliminary/WiFuSocketAPI.cc')
	files += bld.glob('preliminary/KernelSocketAPI.cc')
	files += bld.glob('applib/*.cc')
	files += bld.glob('src/AddressPort.cc')
	files += bld.glob('src/OptionParser.cc')

	if Options.options.bit_32:
		receiver = bld(features='cxx cprogram',
			source=files,
			includes='preliminary preliminary/headers headers headers/exceptions headers/packet headers/visitors headers/observer',
			uselib='PTHREAD RT',
			libpath = '../lib/gc/gc_32',
			staticlib = ['gccpp', 'gc', 'cord'],
			uselib_local='wifu-transport-api',
			target='udp-receiver')
	else:
		receiver = bld(features='cxx cprogram',
			source=files,
			includes='preliminary preliminary/headers headers headers/exceptions headers/packet headers/visitors headers/observer',
			uselib='PTHREAD RT',
			libpath = '../lib/gc/gc_64',
			staticlib = ['gccpp', 'gc', 'cord'],
			uselib_local='wifu-transport-api',
			target='udp-receiver')

def build(bld):
#	build_blaster(bld)
#	build_sink(bld)

	build_staticlib(bld)
	build_wifu(bld)

	if not Options.options.bit_32:
		build_wifu_transport_unit_test(bld)
		build_wifu_transport_integration_test(bld)
	
	build_tcp_tahoe_sender(bld)
	build_tcp_tahoe_receiver(bld)

	build_echo_server(bld)
	build_echo_client(bld)

	build_raw_socket_receiver(bld)
	build_raw_socket_sender(bld)

	build_udp_receiver(bld)
	build_udp_sender(bld)

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
