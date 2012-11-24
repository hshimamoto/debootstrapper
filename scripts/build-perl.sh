#!/bin/bash

. scripts/default

if [ ! -e $DL_DIR/$PERLTAR ]; then
	PushD $DL_DIR
	wget $PERLURL
	PopD
fi

if [ -e $WORK_DIR/$PERL/miniperl ]; then
	echo "Already exist"
	exit 0
fi

PushD $WORK_DIR

tar zxf $DL_DIR/$PERLTAR
cd $PERL

# make static miniperl
./Configure -de
cat >> config.sh <<'EOF'
ldflags=" -static $ldflags"
EOF
./Configure -de

Make

PopD
