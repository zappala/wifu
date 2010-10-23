all:
	./waf configure
	./waf build
	./waf install

clean:
	./waf uninstall
	./waf clean
	./waf distclean
