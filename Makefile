all:
	./waf configure -j4
	./waf build -j4
	./waf install -j4

clean:
	./waf uninstall -j4
	./waf clean -j4
	./waf distclean -j4

debug:
	./waf configure -j4
	./waf build -j4 --debug
	./waf install -j4 --debug