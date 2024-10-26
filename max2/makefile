all: syn
pgm:
	quartus_pgm -m jtag -o "p;output_files/max2.pof"
syn:
	quartus_map --read_settings_files=on  --write_settings_files=off max2 -c max2
	quartus_fit --read_settings_files=off --write_settings_files=off max2 -c max2
	quartus_asm --read_settings_files=on  --write_settings_files=off max2 -c max2
	quartus_sta max2 -c max2
	cat output_files/max2.fit.summary | grep elements
clean:
	rm -rf db incremental_db simulation output_files *.bak *.qdf
