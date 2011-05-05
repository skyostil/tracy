IN="$1"
OUT="$2"
SIZE=$(echo $IN | egrep -o "[0-9]+x[0-9]+")
gm convert -depth 8 -size $SIZE "rgba:$IN" -flip "$OUT"
