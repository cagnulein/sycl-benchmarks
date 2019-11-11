lspci | grep VGA | awk {'print $1'} | xargs lspci -v -s | grep " prefetchable" | grep size
