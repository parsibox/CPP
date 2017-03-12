CPP_OUT=../ProjectLibs
echo 
echo "Generating source code ..."
protoc  --cpp_out $CPP_OUT *.proto

if [ $? != '0' ]
then
	echo "Failed to generate code please check the error message printed."
	exit 1
fi

echo 
echo " -->Successfully geneerated Protocol buffer source code!"
echo " -->Please check directory: $CPP_OUT for source code files."
echo " NOTE: If you are adding or changing an existing proto file"
echo " please make sure that its compiling successfully, this script"
echo " will suppress any error messages from protoc."
echo 
echo "Generating library..."
cd $CPP_OUT
make
if [ $? -ne 0 ]
then
	echo " -->Failed to generate lib"
	exit 1;
fi
echo "Successfully generated lib!"

