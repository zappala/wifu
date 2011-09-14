
CXX= 	g++ -m32
BOBJS=	udpblast.o Tokenizer.o
LOBJS=	udplisten.o Tokenizer.o
OBJS =	$(BOBJS) $(LOBJS)

LIBS=

all: 	udpblast udplisten

udpblast: $(BOBJS)
	$(CXX) -o udpblast $(BOBJS) $(LIBS)

udplisten: $(LOBJS)
	$(CXX) -o udplisten $(LOBJS) $(LIBS)

clean:
	rm -f $(OBJS) $(OBJS:.o=.d)

realclean:
	rm -f $(OBJS) $(OBJS:.o=.d) udpblast udplisten


# These lines ensure that dependencies are handled automatically.
%.d:	%.cc
	$(SHELL) -ec '$(CC) -M $(CPPFLAGS) $< \
		| sed '\''s/\($*\)\.o[ :]*/\1.o $@ : /g'\'' > $@; \
		[ -s $@ ] || rm -f $@'

include	$(OBJS:.o=.d)
