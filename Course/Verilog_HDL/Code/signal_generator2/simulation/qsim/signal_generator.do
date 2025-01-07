onerror {exit -code 1}
vlib work
vlog -work work signal_generator.vo
vlog -work work Waveform1.vwf.vt
vsim -novopt -c -t 1ps -L cyclonev_ver -L altera_ver -L altera_mf_ver -L 220model_ver -L sgate work.signal_generator_vlg_vec_tst -voptargs="+acc"
vcd file -direction signal_generator.msim.vcd
vcd add -internal signal_generator_vlg_vec_tst/*
vcd add -internal signal_generator_vlg_vec_tst/i1/*
run -all
quit -f
