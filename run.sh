# Shell file
# Run as: bash run.sh 6 input.txt 8 3

if [ $# -ne 4 ]
then
	echo "4 Arguments Expected";
	exit 1;
fi


if [ $3 -ne 1 ] && [ $3 -ne 2 ] && [ $3 -ne 4 ] && [ $3 -ne 8 ] && [ $3 -ne 16 ]
	then
	echo "The third argument should either 1, 2, 4, 8 or 16";
	exit 1;

elif [ $4 -ne 0 ] && [ $4 -ne 1 ] && [ $4 -ne 2 ] && [ $4 -ne 3 ] && [ $4 -ne 4 ]
	then 
	echo "The fourth argument should either 0, 1, 2, 3 or 4";
	exit 1;

else
	if [ -f $2 ]
		then
		if [[ $4 == "4" ]]
			then 
			mpiexec -n $3 ./mpi_exec $1 $2;
			
		else
			./exec $1 $2 $3 $4;
		fi

	else
		echo "The path $2 does not exist";
		exit 1;
	fi

fi


