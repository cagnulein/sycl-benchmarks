TIMEFORMAT=%R
echo -e "Angle\tCPU 1 Core\tGPU"
for i in {0..359}
do
        echo
	exec 3>&1 4>&2
	CPU=$({ time ./rotate_classic $1 1>&3 2>&4; } 2>&1);
	exec 3>&- 4>&-
	exec 3>&1 4>&2
	GPU=$({ time ./rotate.gpu $i 1>&3 2>&4; } 2>&1);
	exec 3>&- 4>&-
	echo -en $i"\t"$CPU"\t\t"$GPU;
done
