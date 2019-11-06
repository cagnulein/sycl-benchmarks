source ../export.sh
echo "CPU Classic - 1 Core"
time ./rotate_classic
echo
echo
echo "GPU SYCL"
time ./rotate.gpu
