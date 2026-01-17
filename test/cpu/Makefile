.PHONY: all
all:
	verilator -Wall --trace -cc ../../hdl/cpu/alu.sv --exe test_alu.cpp
	make -C obj_dir/ -f Valu.mk Valu
	./obj_dir/Valu
