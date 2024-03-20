#!/bin/bash

SUCCESS=0
FAIL=0
TEST_NUMBER=0
DIFF_RES=""
MAIN_OPTIONS="b e E n t T s v"
RED=`tput setaf 1`
GREEN=`tput setaf 2`
BASE=`tput setaf 7`

# расскомментировать по очереди по одной строке! иначе слишком много тестов и как следствие - долго и много времени.
declare -a files=(    
     "VAR tests/tests_files/char.txt"
     "VAR tests/tests_files/test_case_cat.txt"
     "VAR tests/tests_files/test_1_cat.txt"
     "VAR tests/tests_files/test_2_cat.txt"
     "VAR tests/tests_files/test_3_cat.txt"
     "VAR tests/tests_files/test_4_cat.txt"
     "VAR tests/tests_files/test_5_cat.txt"
     "VAR tests/tests_files/test_6_cat.txt"
     "VAR tests/tests_files/test_1_cat.txt tests/tests_files/test_2_cat.txt"
     "VAR tests/tests_files/test_1_cat.txt tests/tests_files/char.txt tests/tests_files/test_2_cat.txt"
     "VAR tests/tests_files/test_1_cat.txt tests/tests_files/test_case_cat.txt tests/tests_files/test_2_cat.txt"
     "VAR tests/tests/nofile.txt"
     "VAR tests/tests_files/test_1_cat.txt tests/tests_files/nofile.txt"
     "VAR tests/tests_files/test_1_cat.txt tests/tests_files/nofile.txt tests/tests_files/test_2_cat.txt"
)

testing() {
    t=$(echo $@ | sed "s/VAR/$var/") #echo "-n test_files/test_case_cat.txt test_files/test_1_cat.txt" | sed "s/VAR/$var/"
    ./s21_cat $t > tests/tests_log/${TEST_NUMBER}_test_s21_cat.log
    cat $t > tests/tests_log/${TEST_NUMBER}_test_sys_cat.log
    SHA1=`cat tests/tests_log/${TEST_NUMBER}_test_s21_cat.log | sha256sum`
    SHA2=`cat tests/tests_log/${TEST_NUMBER}_test_sys_cat.log | sha256sum`
    # echo $SHA1
    # echo $SHA2
    #DIFF_RES=`diff test_s21_cat.log test_sys_cat.log -q`

    # if [ -z "$DIFF_RES" ] && 
    if [[ $SHA1 == $SHA2 ]]
    then
      (( SUCCESS++ )) #&& ((TEST_NUMBER++))
        RESULT="SUCCESS"
        echo "[${GREEN}${SUCCESS}${BASE}/${RED}${FAIL}${BASE}] ${GREEN}${RESULT}${BASE} cat $t"

    else
      (( FAIL++ )) #&& ((TEST_NUMBER++))
        RESULT="FAIL"
        echo "[${GREEN}${SUCCESS}${BASE}/${RED}${FAIL}${BASE}] ${RED}${RESULT}${BASE} cat $t"

    fi
    # echo "[${GREEN}${SUCCESS}${BASE}/${RED}${FAIL}${BASE}] ${GREEN}${RESULT}${BASE} cat $t"
    # rm tests/tests_log/${TEST_NUMBER}_test_s21_cat.log tests/tests_log/${TEST_NUMBER}_test_sys_cat.log
}

# Тестирование по 1 опции из MAIN_OPTIONS 
for var1 in $MAIN_OPTIONS
do
    for i in "${files[@]}"
    do
        var="-$var1"
        testing "$i"
    done
done

# Тестирование по 2 опциям из MAIN_OPTIONS
for var1 in $MAIN_OPTIONS
do
    for var2 in $MAIN_OPTIONS
    do
        if [ $var1 != $var2 ]
        then
            for i in "${files[@]}"
                do
                    var="-$var1 -$var2"
                    testing "$i"
                done
        fi                        
    done
done


# # Тестирование по 3 опциям из MAIN_OPTIONS
# for var1 in $MAIN_OPTIONS
# do
#     for var2 in $MAIN_OPTIONS
#     do
#         for var3 in $MAIN_OPTIONS
#         do
#             if [ $var1 != $var2 ] && [ $var1 != $var3 ] && [ $var2 != $var3 ]
#             then
#                 for i in "${files[@]}"
#                     do
#                         var="-$var1 -$var2 -$var3"
#                         testing "$i"
#                     done
#             fi
#         done
#     done
# done

# # Тестирование по 4 опциям из MAIN_OPTIONS
# for var1 in $MAIN_OPTIONS
# do
#     for var2 in $MAIN_OPTIONS
#     do
#         for var3 in $MAIN_OPTIONS
#         do
#             for var4 in $MAIN_OPTIONS
#             do
#                 if [ $var1 != $var2 ] && [ $var1 != $var3 ] && [ $var1 != $var4 ] \
#                 && [ $var2 != $var3 ] && [ $var2 != $var4 ] && [ $var3 != $var4 ]
#                 then
#                     for i in "${files[@]}"
#                         do
#                           var="-$var1 -$var2 -$var3 -$var4"
#                           testing "$i"
#                         done
#                 fi
#             done
#         done
#     done
# done

# # Тестирование по 5 опциям из MAIN_OPTIONS
# for var1 in $MAIN_OPTIONS
# do
#     for var2 in $MAIN_OPTIONS
#     do
#         for var3 in $MAIN_OPTIONS
#         do
#             for var4 in $MAIN_OPTIONS
#             do
#                 for var5 in $MAIN_OPTIONS
#                 do
#                     if [ $var1 != $var2 ] && [ $var1 != $var3 ] && [ $var1 != $var4 ] \
#                     && [ $var1 != $var5 ] && [ $var2 != $var3 ] && [ $var2 != $var4 ] \
#                     && [ $var2 != $var5 ] && [ $var3 != $var4 ] && [ $var3 != $var5 ] \
#                     && [ $var4 != $var5 ]
#                     then
#                         for i in "${files[@]}"
#                         do
#                             var="-$var1 -$var2 -$var3 -$var4 -$var5"
#                             testing "$i"
#                         done
#                     fi
#                 done
#             done
#         done
#     done
# done

# # Тестирование по 6 опциям из MAIN_OPTIONS
# for var1 in $MAIN_OPTIONS
# do
#     for var2 in $MAIN_OPTIONS
#     do
#         for var3 in $MAIN_OPTIONS
#         do
#             for var4 in $MAIN_OPTIONS
#             do
#                 for var5 in $MAIN_OPTIONS
#                 do
#                     for var6 in $MAIN_OPTIONS
#                     do
#                         if [ $var1 != $var2 ] && [ $var1 != $var3 ] && [ $var1 != $var4 ] \
#                         && [ $var1 != $var5 ] && [ $var1 != $var6 ] && [ $var2 != $var3 ] \
#                         && [ $var2 != $var4 ] && [ $var2 != $var5 ] && [ $var2 != $var6 ] \
#                         && [ $var3 != $var4 ] && [ $var3 != $var5 ] && [ $var3 != $var6 ] \
#                         && [ $var4 != $var5 ] && [ $var4 != $var6 ] && [ $var5 != $var6 ]
#                         then
#                             for i in "${files[@]}"
#                             do
#                                 var="-$var1 -$var2 -$var3 -$var4 -$var5 -$var6"
#                                 testing "$i"
#                             done
#                         fi
#                     done
#                 done
#             done
#         done
#     done
# done

echo "FAIL: ${FAIL}"
echo "SUCCESS: ${SUCCESS}"
