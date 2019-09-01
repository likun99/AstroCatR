#!/bin/bash

catdir=$HOME/flcgs/HD117688 #HD
imdir=$HOME/flcgs/new1.txt #ssd3/HD

for catfile in $catdir/*.*.cat;do
  cat=`basename $catfile`

  time1=`grep ${cat%.*}.fit $imdir| awk '{print $2}'`
  time=${time1%.*}
  
  # echo $time
  
 # time=`gethead $imdir/${cat%.*}.fit AST3TIME`


  new="ai_1603-6241_"`date --date=@$time -u +%y%m%d.%H%M%S`"_1x1_1_1.cat"

  ln -s $catfile $HOME/flcgs/raw_HD117688/$new 

  $HOME/flcgs/xingbiao/xingbiao1 $HOME/flcgs/raw_HD117688/$new $new $HOME/flcgs/config/config_HD117688/startable_config > $HOME/flcgs/log/log_HD117688.txt
  runtime=`grep msec $HOME/flcgs/log/log_HD117688.txt| awk '{print $4}'`
  
  if [ $? != 0 ]; then
    exit
  fi
#  rm 1.sql
  echo $cat $new $runtime $num
  echo $cat $new $runtime $num >> run.HD117688.log
  
done

