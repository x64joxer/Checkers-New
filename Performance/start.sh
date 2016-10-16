#!/bin/bash

source commoneEnv.sh

for i in "$@"
do
case $i in
    (-a=*|--action=*)
    ACTION="${i#*=}"	
    shift # past argument=value
    ;;
    *)
            echo Unknown option ${i}
	    exit -1	
    ;;
esac
done

if [ -z "$ACTION" ];  then
	echo Lack of ACTION argument
	exit -1
fi

TMP_FOLDER='tmp'
WORKER_FOLDER='Worker'
WORKER_BIN_NAME='worker'
WORKER_GENERATOR_FOLDER='workerGenerators'

USER_TRAFFIC_GENERATOR='x64joxer'
TRAFFIC_GENERATOR_IP='192.168.0.18'
OUTPUT_DIRECTORY='/home/x64joxer'
INPUT_DIRECTORY='../build-Worker'
NUMBER_OF_WORKERS='10'
SERVER_PORT='6000'
DELAY_MESSAGES='20'
SERVER_IP='192.168.0.6'

echo USER_TRAFFIC_GENERATOR=${USER_TRAFFIC_GENERATOR}
echo TRAFFIC_GENERATOR_IP=${TRAFFIC_GENERATOR_IP}
echo OUTPUT_DIRECTORY=${OUTPUT_DIRECTORY}
echo IPNUT_DIRECTORY=${INPUT_DIRECTORY}
echo NUMBER_OF_WORKERS=${NUMBER_OF_WORKERS}
echo SERVER_PORT=${SERVER_PORT}
echo DELAY_MESSAGES=${DELAY_MESSAGES}
echo SERVER_IP=${SERVER_IP}

if [ "$ACTION" == "PREPARE" ];  then
	echo Preparing directory
	./prepare_worker_folders.sh -i=${TRAFFIC_GENERATOR_IP} -w=${NUMBER_OF_WORKERS} -d=${INPUT_DIRECTORY} -o=${OUTPUT_DIRECTORY} -u=${USER_TRAFFIC_GENERATOR}

elif [ "$ACTION" == "START" ]; then
	echo Starting workers
	./start_workers.sh -i=${TRAFFIC_GENERATOR_IP} -w=${NUMBER_OF_WORKERS}  -o=${OUTPUT_DIRECTORY} -u=${USER_TRAFFIC_GENERATOR} -s=${SERVER_IP} -p=${SERVER_PORT} -d=${DELAY_MESSAGES}

elif [ "$ACTION" == "STOP" ]; then
	echo Stopping workers
	ssh  ${USER_TRAFFIC_GENERATOR}@${TRAFFIC_GENERATOR_IP} "pkill ${WORKER_BIN_NAME}"

elif [ "$ACTION" == "COUNTER" ]; then
	echo Stopping workers
	./get_counters.sh -i=${TRAFFIC_GENERATOR_IP} -w=${NUMBER_OF_WORKERS}  -o=${OUTPUT_DIRECTORY} -u=${USER_TRAFFIC_GENERATOR} -c='numberOfBestResultMessagesSend'


else
	echo Wrong option
	exit -1
fi

