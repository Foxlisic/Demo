VLIB="/usr/share/verilator/include"
VC=ffmpeg -framerate 60 -r 60 -i - -vf "scale=w=1600:h=1000,pad=width=1920:height=1080:x=160:y=40:color=black" -sws_flags neighbor -sws_dither none -f mp4 -q:v 0 -vcodec mpeg4 -y record.mp4

all: app
	cp video.v ../video.v
	./tb -L
	# ./tb -L -V | $(VC)
	# ./tb -H -v > tb.log

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
