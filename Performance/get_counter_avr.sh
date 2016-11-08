#!/bin/bash

for i in "$@"
do
case $i in
    (-u=*|--user=*)
    USER_TRAFFIC_GENERATOR="${i#*=}"	
    shift # past argument=value
    ;;
    (-i=*|--ip=*)
    TRAFFIC_GENERATOR_IP="${i#*=}"
    shift # past argument=value
    ;;
    (-o=*|--outdir=*)
    OUTPUT_DIRECTORY="${i#*=}"
    shift # past argument=value
    ;;
    (-d=*|--inputdir=*)
    INPUT_DIRECTORY="${i#*=}"
    shift # past argument=value
    ;;
    (-w=*|--workers=*)
    NUMBER_OF_WORKERS="${i#*=}"
    shift # past argument=value
    ;;
    (-c=*|--counter=*)
    COUNTER="${i#*=}"
    shift # past argument=value
    ;;
    *)
            echo Unknown option
	    exit -1	
    ;;
esac
done

if [ -z "$USER_TRAFFIC_GENERATOR" ];  then
	echo Lack of USER_TRAFFIC_GENERATOR argument
	exit -1
fi

if [ -z "$TRAFFIC_GENERATOR_IP" ];  then
	echo Lack of TRAFFIC_GENERATOR_IP argument
	exit -1
fi

if [ -z "$OUTPUT_DIRECTORY" ];  then
	echo Lack of OUTPUT_DIRECTORY argument
	exit -1
fi

if [ -z "$NUMBER_OF_WORKERS" ];  then
	echo Lack of NUMBER_OF_WORKERS argument
	exit -1
fi

if [ -z "$COUNTER" ];  then
	echo Lack of COUNTER argument
	exit -1
fi

echo USER_TRAFFIC_GENERATOR=${USER_TRAFFIC_GENERATOR}
echo TRAFFIC_GENERATOR_IP=${TRAFFIC_GENERATOR_IP}
echo OUTPUT_DIRECTORY=${OUTPUT_DIRECTORY}
echo NUMBER_OF_WORKERS=${NUMBER_OF_WORKERS}
echo COUNTER=${COUNTER}

TMP_FOLDER='tmp'
WORKER_GENERATOR_FOLDER='workerGenerators'
WORKER_FOLDER='Worker'
TOTAL_SUM=0

for(( i=1; i<=${NUMBER_OF_WORKERS}; i++ ))
do
	echo ==================================
	echo Worker number ${i}
        RES=$(ssh ${USER_TRAFFIC_GENERATOR}@${TRAFFIC_GENERATOR_IP} "cat /${OUTPUT_DIRECTORY}/${WORKER_GENERATOR_FOLDER}/${WORKER_FOLDER}${i}/trace/* | grep -Eo '${COUNTER}: [0-9]{1,}' | grep -Eo '[0-9]{1,}'"  | awk '{ SUM += $1; COUNT+=1} END { print SUM/COUNT;}')
	echo ${RES} | awk '{print $1}'
	TOTAL_SUM=$(awk "BEGIN {print $RES+$TOTAL_SUM; exit}")
	echo ==================================
done

echo ""
echo Total average:
echo $TOTAL_SUM
echo ""
