#! /bin/bash
cd ..
make clean
cd ./gtest_lcov
make clean
sudo rm -rf ./lcov_out/*
make
./appMain

##  参数-d 后面跟的目录是.gcon .gcda 所在的根目录
/home/fanchenxin/tools/lcov-1.11/bin/lcov -d ../ -t 'appMain' -o 'appMain.info' -b . -c
#/home/fanchenxin/tools/lcov-1.11/bin/lcov --directory . --output-file test_case.info --test-name  test_case

#../lcov-1.11/bin/genhtml -o . test_case.info
/home/fanchenxin/tools/lcov-1.11/bin/genhtml appMain.info --quiet --output-directory lcov_out --title "appMain" 

firefox ./lcov_out/index.html

#--show-details

# --capture --legend
