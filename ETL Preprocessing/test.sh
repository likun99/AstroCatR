#!/bin/bash

catdir=$HOME/flcgs/HD88500 #HD
imdir=$HOME/flcgs/new1.txt #ssd3/HD

for catfile in $catdir/*.*.cat;do
  cat=`basename $catfile`

  time1=`grep ${cat%.*}.fit $imdir| awk '{print $2}'`
  time=${time1%.*}
  
  #echo $time
  
  new="ai_1603-6241_"`date --date=@$time -u +%y%m%d.%H%M%S`"_1x1_1_1.cat"

  ln -s $catfile $HOME/flcgs/raw_HD88500/$new 

  $HOME/flcgs/xingbiao/xingbiao1 $HOME/flcgs/raw_HD88500/$new $new $HOME/flcgs/config/config_HD88500/startable_config > $HOME/flcgs/log/log_HD88500.txt
  runtime=`grep msec $HOME/flcgs/log/log_HD88500.txt| awk '{print $4}'`
  
  if [ $? != 0 ]; then
    exit
  fi

  echo $cat $new $runtime $num
  echo $cat $new $runtime $num >> $HOME/flcgs/run.HD88500.log
  
done

