import sys
from parse import load

def main():
    input_path   = sys.argv[1]
    data_path    = sys.argv[2]
    gnuplot_path = sys.argv[3]
    png_path     = sys.argv[4]
    cpu_name     = sys.argv[5]

    with open(input_path) as f:
        data = load(f)

    with open(data_path, 'wt') as f:
        gnuplot_data(f, data)

    with open(gnuplot_path, 'wt') as f:
        gnuplot_script(f, data_path, png_path, cpu_name)


def gnuplot_data(f, data):
    for cardinality in xrange(1, 64+1):
        scalar, avx512, avx512_travis, avx512_zach = data[cardinality]

        scalar_min  = min(scalar.values)
        scalar_max  = max(scalar.values)
        scalar_avg  = (scalar_min + scalar_max) / 2
        scalar_best = scalar.best

        avx512_min  = min(avx512.values)
        avx512_max  = max(avx512.values)
        avx512_avg  = (avx512_min + avx512_max) / 2
        avx512_best = avx512.best

        avx512_travis_min  = min(avx512_travis.values)
        avx512_travis_max  = max(avx512_travis.values)
        avx512_travis_avg  = (avx512_travis_min + avx512_travis_max) / 2
        avx512_travis_best = avx512_travis.best

        avx512_zach_min  = min(avx512_zach.values)
        avx512_zach_max  = max(avx512_zach.values)
        avx512_zach_avg  = (avx512_zach_min + avx512_zach_max) / 2
        avx512_zach_best = avx512_zach.best

        f.write('%d %0.3f %0.3f %0.3f %0.3f %0.3f %0.3f %0.3f %0.3f %0.3f %0.3f %0.3f %0.3f %0.3f %0.3f %0.3f %0.3f\n' % (
            cardinality,
            scalar_best,
            scalar_avg,
            scalar_min,
            scalar_max,
            avx512_best,
            avx512_avg,
            avx512_min,
            avx512_max,
            avx512_travis_best,
            avx512_travis_avg,
            avx512_travis_min,
            avx512_travis_max,
            avx512_zach_best,
            avx512_zach_avg,
            avx512_zach_min,
            avx512_zach_max,
        ))


def gnuplot_script(file, data_path, png_path, cpu_name):
    data = {
        'data': data_path,
        'png' : png_path,
        'cpu' : cpu_name,
    }

    file.write(GNUPLOT % data)


GNUPLOT="""
set terminal png size 800,600
set output "%(png)s"

set title "Microbenchmark on %(cpu)s"
set xlabel "number of whitespaces in a 64 byte block"
set ylabel "CPU cycles/input byte"
set yrange [0:]
set xrange [0:64]
set xtics 4 
set key left

plot "%(data)s" using 1:3:4:5            with yerrorbars title "scalar"     ls 1 lc "magenta", \
     "%(data)s" using ($1+0.2):7:8:9     with yerrorbars title "AVX512VBMI" ls 1 lc "blue", \
     "%(data)s" using ($1+0.4):11:12:13  with yerrorbars title "AVX512VBMI (Travis)" ls 1 lc "green", \
     "%(data)s" using ($1+0.4):15:16:17  with yerrorbars title "AVX512VBMI (Zach)" ls 1 lc "green", \
     "%(data)s" using 1:2  with lines title "scalar (best)", \
     "%(data)s" using 1:6  with lines title "AVX512VBMI (best)", \
     "%(data)s" using 1:10 with lines title "AVX512VBMI (Travis) (best)", \
     "%(data)s" using 1:14 with lines title "AVX512VBMI (Zach) (best)"
"""

if __name__ == '__main__':
    main()

