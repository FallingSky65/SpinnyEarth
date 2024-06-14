rm ./frames/*.png
./bin/SpinnyEarth 720 720 30 10
ffmpeg -hide_banner -loglevel error -framerate 30 -pattern_type glob -i 'frames/*.png' -c:v libx264 -pix_fmt yuv420p renders/result.mp4
