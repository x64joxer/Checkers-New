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
    (-w=*|--workers=*)
    NUMBER_OF_WORKERS="${i#*=}"
    shift # past argument=value
    ;;
    (-s=*|--server=*)
    SERVER_IP="${i#*=}"
    shift # past argument=value
    ;;
    (-p=*|--port=*)
    SERVER_PORT="${i#*=}"
    shift # past argument=value
    ;;
    (-d=*|--delay=*)
    DELAY_MESSAGES="${i#*=}"
    shift # past argument=value
    ;;
    *)
            echo Unknown option ${i}
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

if [ -z "$SERVER_IP" ];  then
	echo Lack of SERVER_IP argument
	exit -1
fi

if [ -z "$SERVER_PORT" ];  then
	echo Lack of SERVER_POR argument
	exit -1
fi

if [ -z "$DELAY_MESSAGES" ];  then
	echo Lack of DELAY_MESSAGES argument
	exit -1
fi

WORKER_FOLDER='Worker'
WORKER_BIN_NAME='worker'
WORKER_GENERATOR_FOLDER='workerGenerators'

echo USER_TRAFFIC_GENERATOR=${USER_TRAFFIC_GENERATOR}
echo TRAFFIC_GENERATOR_IP=${TRAFFIC_GENERATOR_IP}
echo OUTPUT_DIRECTORY=${OUTPUT_DIRECTORY}
echo NUMBER_OF_WORKERS=${NUMBER_OF_WORKERS}
echo SERVER_PORT=${SERVER_PORT}
echo DELAY_MESSAGES=${DELAY_MESSAGES}
echo SERVER_IP=${SERVER_IP}

for(( i=1; i<=${NUMBER_OF_WORKERS}; i++ ))
do
	ssh -f ${USER_TRAFFIC_GENERATOR}@${TRAFFIC_GENERATOR_IP} "/${OUTPUT_DIRECTORY}/${WORKER_GENERATOR_FOLDER}/${WORKER_FOLDER}${i}/${WORKER_BIN_NAME} -t -i ${SERVER_IP} -p ${SERVER_PORT} -d ${DELAY_MESSAGES} -l /${OUTPUT_DIRECTORY}/${WORKER_GENERATOR_FOLDER}/${WORKER_FOLDER}${i}"
done

echo Result...
ssh -f ${USER_TRAFFIC_GENERATOR}@${TRAFFIC_GENERATOR_IP} "ps ax | grep ${WORKER_BIN_NAME}"
