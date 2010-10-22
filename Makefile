all:
	./waf configure --32
	./waf build
	./waf install

clean:
	./waf uninstall
	./waf clean
	./waf distclean
