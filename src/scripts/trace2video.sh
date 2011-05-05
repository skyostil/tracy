if [ -z $1 ]; then
    echo Usage: $0 trace.inl
    exit 1
fi

rm -f -- /tmp/trace_*{.raw,.png} gtkplayer
make CONFIG=-DMAKE_SCREENSHOTS\ -DTRACE_SOURCE_FILE=\\\"$1\\\" || exit
~/bin/run-vg ./gtkplayer
for f in /tmp/trace*.raw; do
    echo Converting $f
    convert -size 360x640 -depth 8 rgba:$f ${f%.raw}.png
    #convert -size 640x360 -depth 8 rgba:$f ${f%.raw}.png
done;
mencoder mf:///tmp/trace_\*.png -mf fps=1 -ovc lavc -lavcopts vcodec=mpeg4:mbd=2:trell -oac copy -o ${1%.inl}.avi
