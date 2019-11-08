cd fill_array
echo "### fill_array"
echo "#### cpu 1 core"
time ./fill_array_classic
echo "#### gpu"
time ./fill_array.gpu
echo
echo
cd ..
cd math
echo "### math"
echo "#### cpu 1 core"
time ./math_classic
echo "#### gpu"
time ./math.gpu
echo
echo
cd ..
cd math_x2
echo "### math_x2"
echo "#### cpu 1 core"
time ./math_classic
echo "#### gpu"
time ./math.gpu
echo
echo
cd ..
cd rotate
echo "### rotate"
echo "#### cpu 1 core"
time ./rotate_classic
echo "#### gpu"
time ./rotate.gpu
echo
./test-all-angles.sh
cd ..

