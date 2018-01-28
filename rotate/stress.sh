TIMEFORMAT=%R
source ../export.sh
for i in {0..359}; do time ./rotate_classic_omp $i; echo "omp "$i; done&
for i in {0..359..2}; do time ./rotate.gpu $i; echo "gpu "$i; done&
for i in {1..359..2}; do time ./rotate.gpu $i; echo "gpu "$i; done&
