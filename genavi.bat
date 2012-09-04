set WIDTH=196
set HEIGHT=170
set OUTFILE=evo.avi
rem ..\opt\mencoder.exe -ovc lavc -lavcopts vcodec=mpeg4:mbd=2:mv0:trell:v4mv:cbp:last_pred=3:predia=2:dia=2:vmax_b_frames=2:preme=2 -mf type=png:w=%WIDTH%:h=%HEIGHT%:fps=25 -nosound -o %OUTFILE% mf://*.png
rem ..\opt\mencoder.exe -ovc lavc -lavcopts vcodec=mpeg4:vbitrate=2160000:mbd=2:mv0:trell:v4mv:cbp:last_pred=3:predia=2:dia=2:vmax_b_frames=2:preme=2 -ofps 25 -mf type=png:w=%WIDTH%:h=%HEIGHT%:fps=25 -nosound -o %OUTFILE% mf://*.png
rem ..\opt\mencoder.exe "mf://*.png" -mf fps=25 -o %OUTFILE% -ovc lavc -lavcopts vcodec=mpeg4:vbitrate=50000000
rem ..\opt\ffmpeg -r 10 -b:v 1800 -i %03d.png %OUTFILE%
rem ..\opt\mencoder.exe mf://*.png -mf type=png:w=%WIDTH%:h=%HEIGHT%:fps=25 -ovc copy -nosound -o %OUTFILE% 