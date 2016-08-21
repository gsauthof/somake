
.KEEP_STATE:

all:
	helloworld

TEMP += helloworld

hello: hello.o world.o
	$(CC) $(LDFLAGS) $^ -o $@

TEMP += hello hello.o world.o

# the `+` is Sun make target group syntax
foo.c + foo.h: gen_foo.sh
	./gen_foo.sh

TEMP += foo.c foo.h

main_foo.o: foo.h

main_foo: main_foo.o foo.o
	$(CC) $(LDFLAGS) $^ -o $@

TEMP += main_foo main_foo.o foo.o

# with pattern rules GNU make consinders both targets a group
# Sun make needs a `+` between both targets to establish the group
# (the + yields an error with GNU make)
# %.one + %.two: %.bar
%.one %.two: %.bar
	sed 's/x/y/' $< > $*.one
	sed 's/x/z/' $< > $*.two

foo.bar:
	echo x > foo.bar

TEMP += foo.bar

both: foo.one foo.two

TEMP += foo.one foo.two

# Sun make style conditional macro assignment
# conflicts with GNU make's immediate assignment syntax
warn-helloworld := CFLAGS += -Wall
warn-helloworld: helloworld

# GNU make style conditional macro assignment
# yields fatal error with Sun make
# gnu-warn-helloworld: CFLAGS += -Wall
# gnu-warn-helloworld: helloworld

clean:
	rm -f $(TEMP)
