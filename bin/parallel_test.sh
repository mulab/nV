#!/bin/bash
#use gnu parallel
#"echo {} >/dev/null;time=\$(echo \"scale=0;x=\$RANDOM % 10;scale=5;x/20+0.05\" |bc);sleep \$time;echo \$PARALLEL_SEQ done"
#"( echo {} |  env NV_BACKEND=1 NV_TIMEOUT=2 ~/nV/Release/nv ) 2>/dev/null ; echo \$PARALLEL_SEQ done "
#"echo {} |./nv/nv.sh 2>/dev/null; echo \$PARALLEL_SEQ done "
./parallel -a $1 -j 10 --joblog log \
    "echo {} |./nv/nv.sh 2>/dev/null; echo \$PARALLEL_SEQ done "
