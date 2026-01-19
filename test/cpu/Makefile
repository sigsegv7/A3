SV_FILES =						\
	../../hdl/soc/soc.sv		\
	../../hdl/cpu/domain.sv		\
	../../hdl/cpu/alu.sv 		\
	../../hdl/cpu/regbank.sv	\
	../../hdl/cpu/ctl.sv        \

TB_FILES =				\
	test_core.cpp

.PHONY: all
all: obj vcd

.PHONY: obj
obj:
	verilator -Wall --trace -cc $(SV_FILES) --exe $(TB_FILES)

.PHONY: vcd
vcd:
	make -C obj_dir/ -f Vsoc.mk Vsoc
	./obj_dir/Vsoc
