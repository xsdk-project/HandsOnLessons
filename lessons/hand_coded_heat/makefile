PROB = basic
REPORT = iops fops mem

help:
	./heat --help; exit 0

clean:
	@for x in *; do \
	    if [[ -d $$x ]]; then \
	        echo "Removing directory $$x"; \
		rm -rf $$x; \
	    fi; \
	done

#
# To get performance data, we actually run multiple instances
# using different valgrind tools
#
run:
	@rm -rf ${PROB}; mkdir ${PROB}
	@echo "./heat alpha=${ALPHA} dx=${DX} dt=${DT} bc0=${BC0} bc1=${BC1} ic=${IC} alg=${ALG} eps=${EPS} maxi=${MAXI} savi=${SAVI} save=${SAVE} outi=${OUTI}"
	@pushd ${PROB}; \
	if [[ -n $$(echo ${REPORT} | grep ops) ]]; then \
	    valgrind --log-file=valgrind_lackey.out --tool=lackey --detailed-counts=yes ../heat alpha=${ALPHA} dx=${DX} dt=${DT} bc0=${BC0} bc1=${BC1} ic="${IC}" alg=${ALG} eps=${EPS} maxi=${MAXI} savi=${SAVI} save=${SAVE} outi=${OUTI} noout=1 >& heat_lackey.out & \
	fi; \
	if [[ -n $$(echo ${REPORT} | grep mem) ]]; then \
	    valgrind --log-file=valgrind_memcheck.out --tool=memcheck ../heat alpha=${ALPHA} dx=${DX} dt=${DT} bc0=${BC0} bc1=${BC1} ic="${IC}" alg=${ALG} eps=${EPS} maxi=${MAXI} savi=${SAVI} save=${SAVE} outi=${OUTI} noout=1 >& heat_memcheck.out & \
	fi; \
	../heat alpha=${ALPHA} dx=${DX} dt=${DT} bc0=${BC0} bc1=${BC1} ic="${IC}" alg=${ALG} eps=${EPS} maxi=${MAXI} savi=${SAVI} save=${SAVE} outi=${OUTI} & \
	wait
	@if [[ -n $$(echo ${REPORT} | grep iops) ]]; then \
	    echo "Integer ops        = $$(cat ${PROB}/valgrind_lackey.out | grep I1\\\|I8\\\|I16\\\|I32\\\|I64 | tr -s ' ' | cut -d' ' -f5 | tr -d ',' | tr '\n' '+' | sed -e 's/$$/0\n/' | bc)"; \
	fi
	@if [[ -n $$(echo ${REPORT} | grep fops) ]]; then \
	    echo "Floating point ops = $$(cat ${PROB}/valgrind_lackey.out | grep F32\\\|F64\\\|F128\\\|V128\\\|V256 | tr -s ' ' | cut -d' ' -f5 | tr -d ',' | tr '\n' '+' | sed -e 's/$$/0\n/' | bc)"; \
	fi
	@if [[ -n $$(echo ${REPORT} | grep mem) ]]; then \
	    echo "Memory used        = $$(cat ${PROB}/valgrind_memcheck.out | grep 'total heap usage:' | tr -s ' ' | cut -d' ' -f9 | tr -d ',\n' | sed -e 's/$$/-748\n/' | bc) bytes"; \
	fi

#
# Short cuts
#
basic:
	${MAKE} PROB=$@ SAVI=100 run

basic_spikes:
	${MAKE} PROB=$@ BC1=0 IC="spikes(10,2,10,9)" SAVI=25 run

hr_spikes:
	${MAKE} PROB=$@ BC1=0 IC="spikes(10,20,10,90)" DX=0.01 SAVI=10 run

hr_spikes_smalldt:
	${MAKE} PROB=$@ BC1=0 IC="spikes(10,20,10,90)" DX=0.01 DT=0.0001 SAVI=500 run

hr_spikes_crankn:
	${MAKE} PROB=$@ BC1=0 IC="spikes(10,20,10,90)" DX=0.01 ALG=crankn SAVI=10 run

hr_spikes_crankn_largedt:
	${MAKE} PROB=$@ BC1=0 IC="spikes(10,20,10,90)" DX=0.01 DT=0.008 ALG=crankn SAVI=5 run

highres:
	${MAKE} PROB=$@ DX=0.01 SAVI=10 run

hr_smalldt_short:
	${MAKE} PROB=$@ DX=0.01 DT=0.001 SAVI=250 OUTI=250 run

hr_smalldt_long:
	${MAKE} PROB=$@ DX=0.01 DT=0.001 SAVI=250 OUTI=250 MAXI=20000 run

hr_crankn:
	${MAKE} PROB=$@ DX=0.01 DT=0.001 ALG=crankn SAVI=100 run

crankn_faster:
	${MAKE} PROB=$@ DX=0.01 DT=0.008 ALG=crankn SAVI=25 OUTI=50 run

view:
	@pushd ${PROB};\
        ${VISIT} -cli -s ../plot_heat.py


all: basic highres hr_smalldt_short hr_smalldt_long hr_crankn crankn_faster
