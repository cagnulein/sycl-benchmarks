TIMEFORMAT=%R
source ../export.sh
date
for i in {0..359..3}; do time ./rotate.gpu $i; echo "gpu "$i; done&
for i in {1..359..3}; do time ./rotate.gpu $i; echo "gpu "$i; done&
for i in {2..359..3}; do time ./rotate.gpu $i; echo "gpu "$i; done&
