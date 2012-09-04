#!/bin/sh

fmt=png 

outfile=evo.avi

for i in *.$fmt; do
    img=$i
    break
done

width=`identify $img | awk '{print $3}' | awk -Fx '{print $1}'` 
height=`identify $img | awk '{print $3}' | awk -Fx '{print $2}'` 

obr=`expr $width \* $height \* 50 \* 25 / 256` 

opt="vbitrate=$obr:mbd=2:keyint=132:v4mv:vqmin=3:lumi_mask=0.07:dark_mask=0.2:scplx_mask=0.1:tcplx_mask=0.1:naq:trell" 
codec="mpeg4" 

rm -f divx2pass.log frameno.avi 

mencoder -ovc lavc -lavcopts vcodec=$codec:vpass=1:$opt -mf type=$fmt:w=$width:h=$height:fps=25 -nosound -o $outfile mf://\*.$fmt 

rm -f divx2pass.log
