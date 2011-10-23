#!/bin/bash
run_path=$(readlink -f .)
(cat<<EOF
#!/bin/bash
PREFIX="\$(dirname \$0)"
cd \$PREFIX
env NV_BACKEND=1 NV_TIMEOUT=2 ./cde-package/cde-exec $run_path/nv
exit 0
EOF
)> nv.sh

echo 'Integrate[x,x]' | cde ./nv

chmod +x nv.sh
mkdir -p tmp/nv
mv cde.options cde-package nv.sh tmp/nv/
cd tmp
tar czf ../nv.tar.gz nv/
cd ..
rm -r tmp
