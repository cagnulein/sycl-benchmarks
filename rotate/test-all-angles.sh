TIMEFORMAT=%R
source ../export.sh
echo -e "Angle\tCPU 1 Core\tGPU\tWDH   HGT   Full Dimension"
for i in {0..359}
do
        echo
	exec 3>&1 4>&2
	CPU=$({ time ./rotate_classic $i 1>&3 2>&4; } 2>&1);
	exec 3>&- 4>&-
	exec 3>&1 4>&2
	GPU=$({ time ./rotate.gpu $i 1>&3 2>&4; } 2>&1);
	exec 3>&- 4>&-
	echo -en $i"\t"$CPU"\t\t"$GPU"\t"$(./rotate.gpu $i 1);
done
