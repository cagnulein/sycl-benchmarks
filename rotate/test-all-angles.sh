TIMEFORMAT=%R
source ../export.sh
echo -e "Angle\tCPU 1 Core\tOpenMP\tGPU\tUSM\tWDH   HGT   Full Dimension"
for i in {0..359}
do
        echo
	exec 3>&1 4>&2
	CPU=$({ time ./rotate_classic $i 1>&3 2>&4; } 2>&1);
	exec 3>&- 4>&-
	exec 3>&1 4>&2
	OMP=$({ time ./rotate_classic_omp $i 1>&3 2>&4; } 2>&1);
	exec 3>&- 4>&-
	exec 3>&1 4>&2
	GPU=$({ time ./rotate.gpu $i 1>&3 2>&4; } 2>&1);
	exec 3>&- 4>&-
	exec 3>&1 4>&2
	USM=$({ time ./rotate_usm.gpu $i 1>&3 2>&4; } 2>&1);
	exec 3>&- 4>&-
	echo -en $i"\t"$CPU"\t\t"$OMP"\t"$GPU"\t"$USM"\t"$(./rotate.gpu $i 1);
done
