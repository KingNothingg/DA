cp solution tests/solution
cd tests
for (( t = 1; t <= 99; t++ ));
do
	a=`cat $t.in`
	if [[ ("$a" = "") ]];
	then
		continue
	fi
	touch out.txt
	valgrind -q --tool=memcheck --leak-check=full ./solution < "$t.in" > out.txt
	a=`diff "$t.out" -q out.txt`
	if [[ ("$a" = "") ]];
	then
		echo "Test $t OK"
	else
		echo "Test $t WA"
	fi
	rm -rf out.txt
done
 
