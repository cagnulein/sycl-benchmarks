for i in {0..359}
do
        echo
	echo "Angle: " $i;
	time ./rotate.gpu $i;
done
