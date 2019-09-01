#!/bin/bash

if [ $# != 1 ]; then
	echo $0 NAME
	exit
fi

DIR=$1

rm -f /home/allen/flcgs/raw_$DIR/* 
rm -f /home/allen/flcgs/log/*
rm -f /home/allen/flcgs/run.$DIR.log
rm -f /home/allen/flcgs/txt/* log/*

#echo 0 > /home/ubuntu/flcgs/work/work_$DIR/out

