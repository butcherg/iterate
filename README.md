iterate
=======

Simple command line program to iterate a command line with a single substitution 
from a list.

I wrote iterate to support multiple machines in a cluster, using Putty's plink 
in Windows.  iterate simply takes a command line with an inserted dollar sign, 
$, and subtitutes it with entries from a list specified either comma-delimited 
with the -l option or one-per-line in a file specified with the -f option.  The 
command must be contained within double quotes.  With DNS name resolution and 
pre-loaded public keys, this works rather well to shut down agroup of compute 
nodes in a cluster:

	iterate -l one,two,three,four "plink pi@$ sudo shutdown -h now"

The same in Ubuntu with ssh:

	iterate -l one,two,three,four "ssh pi@$ sudo shutdown -h now"

