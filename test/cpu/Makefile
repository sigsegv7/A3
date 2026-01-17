.PHONY: all
all: obj vcd

.PHONY: obj
obj:
	verilator -Wall --trace -cc ../../hdl/cpu/alu.sv --exe test_alu.cpp
	verilator -Wall --trace -cc ../../hdl/cpu/regbank.sv --exe test_regbank.cpp

.PHONY: vcd
vcd:
	make -C obj_dir/ -f Valu.mk Valu
	./obj_dir/Valu
	make -C obj_dir/ -f Vregbank.mk Vregbank
	./obj_dir/Vregbank
