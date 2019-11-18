TIMEFORMAT=%R
echo -e "Index\tTime 1st try\tTime 2nd try\tTime 3rd try"
for i in {20000..22607}
do
        echo
	exec 3>&1 4>&2
	GPU1=$({ time ./rotate.gpu $i 40000 1>&3 2>&4; } 2>&1);
	exec 3>&- 4>&-
	exec 3>&1 4>&2
	GPU2=$({ time ./rotate.gpu $i 40000 1>&3 2>&4; } 2>&1);
	exec 3>&- 4>&-
	exec 3>&1 4>&2
	GPU3=$({ time ./rotate.gpu $i 40000 1>&3 2>&4; } 2>&1);
	exec 3>&- 4>&-
	echo -en $i"\t"$GPU1"\t\t"$GPU2"\t\t"$GPU3;
done
