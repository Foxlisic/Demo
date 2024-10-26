VLIB="/usr/share/verilator/include"

all: app
	./tb -L

app: syn
	g++ -Ofast -o tb -I$(VLIB) -I.. \
		tb.cc \
		$(VLIB)/verilated_threads.cpp \
		$(VLIB)/verilated.cpp \
		obj_dir/Vvideo__ALL.a \
		-lSDL2
syn:
	verilator --threads 1 -cc video.v > /dev/null
	cd obj_dir && make -f Vvideo.mk > /dev/null
clean:
	rm -r obj_dir tb
