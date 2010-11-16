all:
	./waf configure -j2
	./waf build -j2
	./waf install -j2

clean:
	./waf uninstall -j2
	./waf clean -j2
	./waf distclean -j2
