$1 ./Test_Diffusion.xml

rm -rf *.vtk

python ./verify.py

if [ $? -eq 0 ]
then
    exit 0
else
    exit 1
fi
