$1 ./Test_NavierStokesTurbVortex.xml

rm -rf *.vtk

python ./verify.py

if [ $? -eq 0 ]
then
    exit 0
else
    exit 1
fi
