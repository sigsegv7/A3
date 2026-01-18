SV_FILES =						\
	../../hdl/cpu/domain.sv		\
	../../hdl/cpu/alu.sv 		\
	../../hdl/cpu/regbank.sv

TB_FILES =				\
	test_core.cpp

.PHONY: all
all: obj vcd

.PHONY: obj
obj:
	verilator -Wall --trace -cc $(SV_FILES) --exe $(TB_FILES)

.PHONY: vcd
vcd:
	make -C obj_dir/ -f Vdomain.mk Vdomain
	./obj_dir/Vdomain
