Please run: source ../export.sh && test-around.sh to see the issue
<pre>
./test-around.sh
Index           Time 1st try    Time 2nd try    Time 3rd try

1761303161      1.548           1.561           1.563  
<b>1761303162      3.177           3.173           3.194</b>
<b>1761303163      16.447          16.431          16.459</b>  
1761303164      1.249           1.235           1.247  
1761303165      1.953           1.967           1.955  
1761303166      2.052           2.039           2.038  
1761303167      2.165           2.172           2.156  
1761303168      1.261           1.271           1.290  
1761303169      1.392           1.411           1.407  
1761303170      1.299           1.305           1.307
<b>1761303171      5.829           5.842           5.838</b>
<b>1761303172      4.509           4.500           4.518</b>
1761303173      1.288           1.291           1.308
1761303174      1.232           1.225           1.242  
1761303175      1.813           1.838           1.796  
<b>1761303176      2.507           2.528           2.511</b>
1761303177      1.706           1.716           1.699  
<b>1761303178      8.490           8.477           8.506</b>
<b>1761303179      16.436          16.447          16.432</b>
1761303180      1.407           1.411           1.429  
1761303181      2.465           2.465           2.451  
<b>1761303182      8.492           8.502           8.479</b>
1761303183      1.263           1.246           1.239  
1761303184      1.847           1.887           1.845  
</pre>

<pre>
./test-around-opencl.sh
Index           Time 1st try    Time 2nd try    Time 3rd try

1761303161      0.950           0.944           0.939
<b>1761303162      3.109           3.071           3.080
1761303163      16.302          16.329          16.351</b>
1761303164      0.649           0.660           0.663
1761303165      1.479           1.512           1.480
1761303166      1.593           1.578           1.591
1761303167      1.662           1.642           1.668
1761303168      0.698           0.687           0.696
1761303169      0.836           0.835           0.831
1761303170      0.714           0.679           0.676
<b>1761303171      5.755           5.717           5.745
1761303172      4.424           4.413           4.409</b>
1761303173      0.710           0.710           0.722
1761303174      0.670           0.669           0.653
1761303175      1.088           1.077           1.078
<b>1761303176      2.418           2.422           2.430</b>
1761303177      1.024           1.022           1.029
<b>1761303178      8.391           8.397           8.354
1761303179      16.353          16.373          16.331</b>
1761303180      0.805           0.786           0.776
1761303181      1.901           1.878           1.895
<b>1761303182      8.394           8.385           8.386</b>
1761303183      0.654           0.663           0.664
1761303184      1.435           1.414           1.443
</pre>

### Releated issues:
https://github.com/intel/compute-runtime/issues/235  
https://github.com/intel/llvm/issues/813