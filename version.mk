MAJOR  		:= 1
MINOR   	:= 0
REVISION	:= 0

VERSION     :=${MAJOR}.${MINOR}.${REVISION}

CID 		:= $(shell git log --format=%h | head -n 1)
BUILD_TIME  := $(shell date +"%Y-%m-%d %H:%M:%S")