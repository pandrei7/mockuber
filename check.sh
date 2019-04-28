#!/bin/bash

# Checher tema 2 SD 2019

#TODO
# timpi in ms, test timpi + valgrind flag, verif readme, coding style


NO_TASKS=5
NO_TESTS=10
EXEC=tema2
VALG_FLAG=0

#TODO
MAX_BONUS=10
#TEST_POINTS=(2 2 2 2 2 2 2 2 2)
TASK1_POINTS=(1 1 1 1 1 1 1 1 1 1)
TASK2_POINTS=(1 1 1 1 1 1 1 1 1 1)
TASK3_POINTS=(1 1 1 1 1 2 2 2 2 2)
TASK4_POINTS=(2 3 3 3 3 3 3 5 5 5)
TASK5_POINTS=(1 1 1 1 1 1 1 1 1 1)

#time is in ms
#TODO
TASK1_TIME=(1000 1000 1000 1000 1000 1000 1000 4000 4000 4000)
TASK2_TIME=(1000 1000 1000 1000 1000 1000 1000 4000 4000 4000)
TASK3_TIME=(1000 1000 1000 1000 1000 1000 1000 4000 4000 4000)
TASK4_TIME=(3000 3000 3000 3000 3000 3000 3000 50 100 180)
TASK5_TIME=(3000 3000 3000 3000 3000 3000 3000 10000 10000 10000)
#TIMEOUT_TIME=(1 1 1 1 1 1 12 1 3 5)

#TODO
VALGRIND_TIMEOUT_TIME=(14 14 14 14 14 14 14 50 50 50 150)
TOTAL=0

ERRORS_NUMBER=0
CODING_STYLE_BONUS=0
CURRENT_DIRECTORY=`pwd`
ONE=1
TWO=2
MESSAGE_SIZE=12
PASSED_TESTS_GRADE=0

make clean
make build

timeout_test()
{
	test_no=$1
    input_path=$2


    ./$EXEC $input_path

    timeout1=$((${TASK1_TIME[$(($test_no - 1))]}))
    timeout2=$((${TASK2_TIME[$(($test_no - 1))]}))
    timeout3=$((${TASK3_TIME[$(($test_no - 1))]}))
    timeout4=$((${TASK4_TIME[$(($test_no - 1))]}))
    timeout5=$((${TASK5_TIME[$(($test_no - 1))]}))

    #set TEST_RESULT$j (0 = no timeout; != 0 timeout)
    TEST_RESULT1=0
    TEST_RESULT2=0
    TEST_RESULT3=0
    TEST_RESULT4=0
    TEST_RESULT5=0



    t1=$(sed '1q;d' time.out)
		t2=$(sed '2q;d' time.out)
		t3=$(sed '3q;d' time.out)
		t4=$(sed '4q;d' time.out)
		t5=$(sed '5q;d' time.out)

		if echo $t1 | grep -q 'e'; then
			t1=0;
		else
			t1=$( echo ${t1:0:5} | sed -r 's/\.[0-9]*//' | sed -r 's/^[0]+//')
			if [ "$t1" == "" ]; then
				t1=0;
			fi
		fi

		if echo $t2 | grep -q 'e'; then
			t2=0;
		else
			t2=$( echo ${t2:0:5} | sed -r 's/\.[0-9]*//' | sed -r 's/^[0]+//')
			if [ "$t2" == "" ]; then
				t2=0;
			fi
		fi

		if echo $t3 | grep -q 'e'; then
			t3=0;
		else
			t3=$( echo ${t3:0:5} | sed -r 's/\.[0-9]*//' | sed -r 's/^[0]+//')
			if [ "$t3" == "" ]; then
				t3=0;
			fi
		fi

		if echo $t4 | grep -q 'e'; then
			t4=0;
		else
			t4=$( echo ${t4:0:5} | sed -r 's/\.[0-9]*//' | sed -r 's/^[0]+//')
			if [ "$t4" == "" ]; then
				t4=0;
			fi
		fi

		if echo $t5 | grep -q 'e'; then
			t5=0;
		else
			t5=$( echo ${t5:0:5} | sed -r 's/\.[0-9]*//' | sed -r 's/^[0]+//')
			if [ "$t5" == "" ]; then
				t5=0;
			fi
		fi



    if [ $t1 -gt $timeout1 ]; then
        TEST_RESULT1=1
        echo "TIMEOUT TASK 1 [$timeout1 s]"
    fi

    if [ $t2 -gt $timeout2 ]; then
        TEST_RESULT2=1
        echo "TIMEOUT TASK 2 [$timeout2 s]"
    fi

    if [ $t3 -gt $timeout3 ]; then
        TEST_RESULT3=1
        echo "TIMEOUT TASK 3 [$timeout3 s]"
    fi

    if [ $t4 -gt $timeout4 ]; then
        TEST_RESULT4=1
        echo "TIMEOUT TASK 4 [$timeout4 s]"
    fi

    if [ $t5 -gt $timeout5 ]; then
        TEST_RESULT5=1
        echo "TIMEOUT TASK 5 [$timeout5 s]"
    fi


}

valgrind_test()
{
    test_no=$1
    input_path=$2

	# Get the input and ref files for the current test
	timeout=${VALGRIND_TIMEOUT_TIME[$test_no]}

	(time timeout $timeout valgrind --leak-check=full --error-exitcode=1 -q ./$EXEC $input_path) 2>time.err
	TEST_RESULT_VALGRIND=$?
}


# Check a single test and add the result to the total score. This function
# should _always_ receive a test number as the first and only argument
check_test()
{
    test_no=$1
    input_path="in/test"$test_no".in"

    if [ "$test_no" -le "7" ]; then

        #check valgrind first
        check_valgrind_test $1

        if [ "$TEST_RESULT_VALGRIND" != "0" ]; then
            echo "Test: $test_no ...................... FAILED BECAUSE OF VALGRIND"
            VALG_FLAG=1
            rm -f result.out
            rm -f time.err
            return
        fi
    else
        if [ "$VALG_FLAG" -eq "1" ]; then
            echo "Test: $test_no ...................... FAILED BECAUSE OF VALGRIND ON PREVIOUS TESTS"
            rm -f result.out
            rm -f time.err
            return

        else
            echo -n "Test: $test_no ...................... "
            echo "NO MEMORY LEAKS"

        fi
    fi

	timeout_test $test_no $input_path


    if [ "$TEST_RESULT1" == "0" ]; then
        out_path="out/task_1/test"$test_no".out"
        ref_path="ref/task_1/test"$test_no".ref"
        echo -n "Test: $test_no task 1 ...................... "
        # Check the result
      	diff -bBZ -i $ref_path $out_path 2>&1 1> my_diff
      	if test $? -eq 0; then
            echo "PASS [${TASK1_POINTS[$(($test_no - 1))]}p]"
            TOTAL=$(echo $TOTAL + ${TASK1_POINTS[$(($test_no - 1))]} | bc)
      	else
          	echo "FAILED"
     #     	echo "Diff result:"
      #    	cat my_diff | tail -n 10
    	fi
    fi

    if [ "$TEST_RESULT2" == "0" ]; then
    	out_path="out/task_2/test"$test_no".out"
        ref_path="ref/task_2/test"$test_no".ref"
        echo -n "Test: $test_no task 2 ...................... "
        # Check the result
      	diff -bBZ -i $ref_path $out_path 2>&1 1> my_diff
      	if test $? -eq 0; then
            echo "PASS [${TASK2_POINTS[$(($test_no - 1))]}p]"
            TOTAL=$(echo $TOTAL + ${TASK2_POINTS[$(($test_no - 1))]} | bc)
      	else
          	echo "FAILED"
#          	echo "Diff result:"
 #         	cat my_diff | tail -n 10
    	fi
    fi

    if [ "$TEST_RESULT3" == "0" ]; then
    	out_path="out/task_3/test"$test_no".out"
        ref_path="ref/task_3/test"$test_no".ref"
        echo -n "Test: $test_no task 3 ...................... "
        # Check the result
      	diff -bBZ -i $ref_path $out_path 2>&1 1> my_diff
      	if test $? -eq 0; then
            echo "PASS [${TASK3_POINTS[$(($test_no - 1))]}p]"
            TOTAL=$(echo $TOTAL + ${TASK3_POINTS[$(($test_no - 1))]} | bc)
      	else
          	echo "FAILED"
  #        	echo "Diff result:"
   #       	cat my_diff | tail -n 10
    	fi
    fi

    if [ "$TEST_RESULT4" == "0" ]; then
    	out_path="out/task_4/test"$test_no".out"
        ref_path="ref/task_4/test"$test_no".ref"
        echo -n "Test: $test_no task 4 ...................... "
        # Check the result
      	diff -bBZ -i $ref_path $out_path 2>&1 1> my_diff
      	if test $? -eq 0; then
            echo "PASS [${TASK4_POINTS[$(($test_no - 1))]}p]"
            TOTAL=$(echo $TOTAL + ${TASK4_POINTS[$(($test_no - 1))]} | bc)
      	else
          	echo "FAILED"
    #      	echo "Diff result:"
     #     	cat my_diff | tail -n 10
    	fi
    fi

    if [ "$TEST_RESULT5" == "0" ]; then
        out_path="out/task_5/test"$test_no".out"
        ref_path="ref/task_5/test"$test_no".ref"
        echo -n "Test: $test_no task 5 ...................... "
        # Check the result
      	diff -bBZ -i $ref_path $out_path 2>&1 1> my_diff
      	if test $? -eq 0; then
            echo "PASS [${TASK5_POINTS[$(($test_no - 1))]}p]"
            TOTAL=$(echo $TOTAL + ${TASK5_POINTS[$(($test_no - 1))]} | bc)
      	else
          	echo "FAILED"
      #    	echo "Diff result:"
       #   	cat my_diff | tail -n 10
    	fi
    fi

    #echo "TOTAL: $TOTAL"
    # Clean up
    rm -f my_diff
    rm -f task*.out
    rm -f time.err
}


check_valgrind_test()
{
    test_no=$1
    input_path="in/test"$test_no".in"

    #echo
    echo -n "Test: $test_no ...................... "

    valgrind_test $test_no $input_path

    if [ "$TEST_RESULT_VALGRIND" != "0" ]; then
        echo "DETECTED MEMORY LEAKS"
        #tail -n 10 time.err
        rm -f result.out
        rm -f time.err
        return
    else
        echo "NO MEMORY LEAKS"
        rm -f result.out
        rm -f time.err
        return
    fi
}

function checkBonus
{
    printf '%*s\n' "${COLUMNS:-$(($(tput cols) - $ONE))}" '' | tr ' ' -
    echo "" > checkstyle.txt

    echo -ne "Coding style Errors:\n"
    for entry in *.{cpp,hpp,h};
    do
        if [[ -f $entry ]]; then
            ./cpplint.py "$entry" > checkstyle.txt
            YOUR_BONUS=`cat checkstyle.txt | grep "Total errors found: "| tr -dc '0-9'`
            ERRORS_NUMBER=$(( $ERRORS_NUMBER + $YOUR_BONUS ))
        fi
    done

    if [[ $ERRORS_NUMBER != "0" ]]; then
        printf '\nCoding style check: %*sFAIL' "${COLUMNS:-$(($(tput cols) - $MESSAGE_SIZE))}" '' | tr ' ' -
        echo -ne "\n\t\tYou have a total of $ERRORS_NUMBER coding style errors.\n"

    else
        echo -ne "\n\tNone?\n\t\tWow, next level!"
        printf '\n\nCoding style check: %*sOK' "${COLUMNS:-$(($(tput cols) - $MESSAGE_SIZE + $TWO ))}" '' | tr ' ' -

        CODING_STYLE_BONUS=$(echo "scale=1; $TOTAL / 8" | bc -l)

        # CODING_STYLE_BONUS=$(($CODING_STYLE_BONUS))
    fi
    rm -f checkstyle.txt
}

function printBonus
{
    if [[ $ERRORS_NUMBER -gt 0 ]]; then
    printf '\n%*s' "${COLUMNS:-$(tput cols)}" '' | tr ' ' -
        echo -ne  "\n\tAnd you almost did it!\n"
    else
        echo -ne "\n\tDamn!\n"
    fi
    echo -ne "\n\t\tYou get $CODING_STYLE_BONUS/$MAX_BONUS for coding style.\n\n"
}

mkdir out
for i in `seq 1 5`; do
    mkdir out/task_$i
done


# Check if the the exec exist
if test ! -x $EXEC; then
    echo "Execf file missing"
    exit 1
fi

for ((i=1;i<=$NO_TESTS;++i))
do
	check_test $i
done

# And the restul is ...
echo "TOTAL: $TOTAL/80"
echo ""

checkBonus
printBonus
make clean

#echo "TIMEOUT TASK $j"
#[${TASK$j_TIME[$test_no]} s]"

# mkdir resources
# cp archive.zip resources/archive.zip
# cd resources
# unzip archive.zip
# cd ..
# checkBonus resources
# printBonus
# rm -rf resources
