function check(){
    NV=~/nV/bin/nv
    name=$1
    echo checking $name
    diff <(cat $name.n | env NV_BACKEND=0 $NV 2>/dev/null|tail -n +2) $name.result
	if [ $? -eq 0 ]
	then
		echo $name ':-)'
	else
		echo $name ':-('
	fi
}
function generate-result(){
    NV=~/nV/bin/nv
    name=$1
    echo generating result for $name
    (cat $name.n | env NV_BACKEND=0 $NV 2>/dev/null|tail -n +2) > $name.result
}
function check-all(){
    files=$(find . -mindepth 1 -type f -name '*.result')
    for file in $files
    do
	name=${file%.*}
	check $name
    done
}
function generate-result-all(){
    echo "Do you really wish to refresh all the test cases?"
    select yn in "Yes" "No"; do
        case $yn in
            Yes ) break;;
            No ) exit;;
        esac
    done
    files=$(find . -mindepth 1 -type f -name '*.result')
    for file in $files
    do
	name=${file%.*}
	generate-result $name
    done
}
