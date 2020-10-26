#!/bin/sh

#############################################################
# claenup & build all
#############################################################

echo "Build all models ..."
make clean
make
cd specc_jpeg/
cd jpeg_spec
make clean
make
cd ..
cd jpeg_arch
make clean
make
cd ..
cd jpeg_comm
make clean
make
cd ..
cd ..
echo "Building all models... done"

#############################################################
# OSSS BEHAVIOUR runs
#############################################################

#
# spec
#

echo OSSS_BEHAVIOUR: spec, 116x96
time ./jpeg_spec/jpeg_spec.x images/lsh.bmp jpeg_spec/lsh_out.jpg
cmp jpeg_spec/lsh_out.jpg images/reference/lsh.jpg

echo OSSS_BEHAVIOUR: spec, 256x256
time ./jpeg_spec/jpeg_spec.x images/lena_256.bmp jpeg_spec/lena_256_out.jpg
cmp jpeg_spec/lena_256_out.jpg images/reference/lena_256.jpg

echo OSSS_BEHAVIOUR: spec, 461x346
time ./jpeg_spec/jpeg_spec.x images/butterfly.bmp jpeg_spec/butterfly_out.jpg
cmp jpeg_spec/butterfly_out.jpg images/reference/butterfly.jpg

echo OSSS_BEHAVIOUR: spec, 512x512
time ./jpeg_spec/jpeg_spec.x images/lena_512.bmp jpeg_spec/lena_512_out.jpg
cmp jpeg_spec/lena_512_out.jpg images/reference/lena_512.jpg

#
# arch
#

echo OSSS_BEHAVIOUR: arch, 116x96
time ./jpeg_arch/jpeg_arch.x images/lsh.bmp jpeg_arch/lsh_out.jpg
cmp jpeg_arch/lsh_out.jpg images/reference/lsh.jpg

echo OSSS_BEHAVIOUR: arch, 256x256
time ./jpeg_arch/jpeg_arch.x images/lena_256.bmp jpeg_arch/lena_256_out.jpg
cmp jpeg_arch/lena_256_out.jpg images/reference/lena_256.jpg

echo OSSS_BEHAVIOUR: arch, 461x346
time ./jpeg_arch/jpeg_arch.x images/butterfly.bmp jpeg_arch/butterfly_out.jpg
cmp jpeg_arch/butterfly_out.jpg images/reference/butterfly.jpg

echo OSSS_BEHAVIOUR: arch, 512x512
time ./jpeg_arch/jpeg_arch.x images/lena_512.bmp jpeg_arch/lena_512_out.jpg
cmp jpeg_arch/lena_512_out.jpg images/reference/lena_512.jpg

#
# comm
#

echo OSSS_BEHAVIOUR: comm, 116x96
time ./jpeg_comm/jpeg_comm.x images/lsh.bmp jpeg_comm/lsh_out.jpg
cmp jpeg_comm/lsh_out.jpg images/reference/lsh.jpg

echo OSSS_BEHAVIOUR: comm, 256x256
time ./jpeg_comm/jpeg_comm.x images/lena_256.bmp jpeg_comm/lena_256_out.jpg
cmp jpeg_comm/lena_256_out.jpg images/reference/lena_256.jpg

echo OSSS_BEHAVIOUR: comm, 461x346
time ./jpeg_comm/jpeg_comm.x images/butterfly.bmp jpeg_comm/butterfly_out.jpg
cmp jpeg_comm/butterfly_out.jpg images/reference/butterfly.jpg

echo OSSS_BEHAVIOUR: comm, 512x512
time ./jpeg_comm/jpeg_comm.x images/lena_512.bmp jpeg_comm/lena_512_out.jpg
cmp jpeg_comm/lena_512_out.jpg images/reference/lena_512.jpg

#############################################################
# SpecC runs
#############################################################

#
# spec
#

echo  
echo SpecC: spec, 116x96
time ./specc_jpeg/jpeg_spec/tb images/lsh.bmp specc_jpeg/jpeg_spec/lsh_out.jpg
cmp specc_jpeg/jpeg_spec/lsh_out.jpg images/reference/lsh.jpg

echo  
echo SpecC: spec, 256x256
time ./specc_jpeg/jpeg_spec/tb images/lena_256.bmp specc_jpeg/jpeg_spec/lena_256_out.jpg
cmp specc_jpeg/jpeg_spec/lena_256_out.jpg images/reference/lena_256.jpg

echo  
echo SpecC: spec, 461x346
time ./specc_jpeg/jpeg_spec/tb images/butterfly.bmp specc_jpeg/jpeg_spec/butterfly_out.jpg
cmp specc_jpeg/jpeg_spec/butterfly_out.jpg images/reference/butterfly.jpg

echo  
echo SpecC: spec, 512x512
time ./specc_jpeg/jpeg_spec/tb images/lena_512.bmp specc_jpeg/jpeg_spec/lena_512_out.jpg
cmp specc_jpeg/jpeg_spec/lena_512_out.jpg images/reference/lena_512.jpg

#
# arch
#

echo  
echo SpecC: arch, 116x96
time ./specc_jpeg/jpeg_arch/tb images/lsh.bmp specc_jpeg/jpeg_arch/lsh_out.jpg
cmp specc_jpeg/jpeg_arch/lsh_out.jpg images/reference/lsh.jpg

echo  
echo SpecC: arch, 256x256
time ./specc_jpeg/jpeg_arch/tb images/lena_256.bmp specc_jpeg/jpeg_arch/lena_256_out.jpg
cmp specc_jpeg/jpeg_arch/lena_256_out.jpg images/reference/lena_256.jpg

echo  
echo SpecC: arch, 461x346
time ./specc_jpeg/jpeg_arch/tb images/butterfly.bmp specc_jpeg/jpeg_arch/butterfly_out.jpg
cmp specc_jpeg/jpeg_arch/butterfly_out.jpg images/reference/butterfly.jpg

echo  
echo SpecC: arch, 512x512
time ./specc_jpeg/jpeg_arch/tb images/lena_512.bmp specc_jpeg/jpeg_arch/lena_512_out.jpg
cmp specc_jpeg/jpeg_arch/lena_512_out.jpg images/reference/lena_512.jpg

#
# comm
#

echo  
echo SpecC: comm, 116x96
time ./specc_jpeg/jpeg_comm/tb images/lsh.bmp specc_jpeg/jpeg_comm/lsh_out.jpg
cmp specc_jpeg/jpeg_comm/lsh_out.jpg images/reference/lsh.jpg

echo  
echo SpecC: comm, 256x256
time ./specc_jpeg/jpeg_comm/tb images/lena_256.bmp specc_jpeg/jpeg_comm/lena_256_out.jpg
cmp specc_jpeg/jpeg_comm/lena_256_out.jpg images/reference/lena_256.jpg

echo  
echo SpecC: comm, 461x346
time ./specc_jpeg/jpeg_comm/tb images/butterfly.bmp specc_jpeg/jpeg_comm/butterfly_out.jpg
cmp specc_jpeg/jpeg_comm/butterfly_out.jpg images/reference/butterfly.jpg

echo  
echo SpecC: comm, 512x512
time ./specc_jpeg/jpeg_comm/tb images/lena_512.bmp specc_jpeg/jpeg_comm/lena_512_out.jpg
cmp specc_jpeg/jpeg_comm/lena_512_out.jpg images/reference/lena_512.jpg
