#!/bin/bash

source commoneEnv.sh

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

if [ -z "$INPUT_DIRECTORY" ];  then
	echo Lack of INPUT_DIRECTORY argument
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

echo USER_TRAFFIC_GENERATOR=${USER_TRAFFIC_GENERATOR}
echo TRAFFIC_GENERATOR_IP=${TRAFFIC_GENERATOR_IP}
echo INPUT_DIRECTORY=${INPUT_DIRECTORY}
echo OUTPUT_DIRECTORY=${OUTPUT_DIRECTORY}
echo NUMBER_OF_WORKERS=${NUMBER_OF_WORKERS}

TMP_FOLDER='tmp'
WORKER_GENERATOR_FOLDER='workerGenerators'
WORKER_FOLDER='Worker'

mkdir -p ${TMP_FOLDER}
rm -rf ${TMP_FOLDER}/*
mkdir -p ${TMP_FOLDER}/${WORKER_GENERATOR_FOLDER}

if [ ! -d "${INPUT_DIRECTORY}" ]; then
	  echo Input directory ${INPUT_DIRECTORY} not exist
	  exit -1
fi

for(( i=1; i<=${NUMBER_OF_WORKERS}; i++ ))
do
	mkdir -p ${TMP_FOLDER}/${WORKER_GENERATOR_FOLDER}/${WORKER_FOLDER}${i}
	cp -r ${INPUT_DIRECTORY}/* ${TMP_FOLDER}/${WORKER_GENERATOR_FOLDER}/${WORKER_FOLDER}${i}
	scp -r ${TMP_FOLDER}/${WORKER_GENERATOR_FOLDER} ${USER_TRAFFIC_GENERATOR}@${TRAFFIC_GENERATOR_IP}:/${OUTPUT_DIRECTORY}/
done

