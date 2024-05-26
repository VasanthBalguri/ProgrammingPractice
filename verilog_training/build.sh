#!/bin/bash

verilator --cc counter.v
verilator -Wall --trace -cc counter.v --exe tb.cpp 
make -C obj_dir -f Vcounter.mk Vcounter
